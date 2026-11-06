# Agregador resiliente de APIs

Sistemas Distribuidos · Evaluación-taller · Escuela de Ingeniería de Sistemas e Informática, UIS

Servicio en FastAPI que consume una API externa pública (Open-Meteo) y un servicio inestable que falla a propósito (sensores), y expone una API propia con reintentos selectivos, backoff con jitter, caché con TTL e idempotencia en escrituras.

## 1. Integrantes y roles

| Integrante                           | Rol                                                         |
| ------------------------------------ | ----------------------------------------------------------- |
| Piert Anggela Katherin Quekan Garcia | Operación del servicio inestable, Cliente de la API externa |
| Karen Angélica Sandoval Martínez     | Resiliencia frente al servicio inestable                    |
| Santiago Jimenez Rico                | API propia, modelos Pydantic, pruebas y README              |

## 2. API externa elegida: Open-Meteo

Se encadenan dos llamadas:

1. **Geocodificación:** `GET https://geocoding-api.open-meteo.com/v1/search?name={ciudad}&count=1&language=es` devuelve las coordenadas. Si la ciudad no existe, la respuesta no trae la clave `results`.
2. **Pronóstico:** `GET https://api.open-meteo.com/v1/forecast?latitude=..&longitude=..&current=temperature_2m,wind_speed_10m` devuelve el clima actual.

Las respuestas se validan con modelos Pydantic v2. Si no cumplen el modelo, se responde 502. Cada llamada saliente tiene timeout explícito (5 s en total, 3 s de conexión).

## 3. Estructura del proyecto

```
agregador/
├── requirements.txt
├── README.md
├── peticiones.http          # colección de peticiones de prueba
├── medir.py                 # medición de latencia con y sin caché
├── probar.py                # prueba rápida del cliente de Open-Meteo
├── servicio_inestable.py    # entregado por el docente
├── evidencias/              # logs y salidas de las pruebas
└── app/
    ├── __init__.py
    ├── main.py              # API propia (FastAPI)
    ├── cliente.py           # cliente de Open-Meteo
    ├── sensores.py          # cliente del servicio inestable (reintentos, paginación)
    ├── cache.py             # caché en memoria con TTL
    └── modelos.py           # modelos Pydantic
```

## 4. Cómo ejecutarlo

Requiere Python 3.10 o superior.

### 4.1 Instalación

```bash
python -m venv venv
source venv/bin/activate          # Windows: venv\Scripts\activate
pip install -r requirements.txt
```

### 4.2 Servicio inestable (terminal 1)

```bash
python servicio_inestable.py                                   # valores por defecto, puerto 9000
python servicio_inestable.py --tasa-fallos 0.4 --semilla 42    # fallos reproducibles
python servicio_inestable.py --host 0.0.0.0                    # accesible desde otros equipos
```

Comprobación: `curl http://127.0.0.1:9000/salud` debe responder `{"estado":"ok"}`.

### 4.3 Agregador (terminal 2)

Desde la carpeta raíz (la que contiene `app/`), no desde dentro de `app/`:

```bash
URL_INESTABLE=http://127.0.0.1:9000 CACHE_TTL=300 uvicorn app.main:app --port 8000
```

| Variable de entorno | Por defecto | Efecto |
|---|---|---|
| `URL_INESTABLE` | `http://127.0.0.1:9000` | URL base del servicio inestable |
| `CACHE_TTL` | `300` | TTL de la caché del clima en segundos (`0` la desactiva) |

La documentación OpenAPI queda en `http://127.0.0.1:8000/docs`.

## 5. API propia

| Método y ruta | Descripción | Códigos |
|---|---|---|
| `GET /salud` | Comprobación de vida | 200 |
| `GET /v1/ciudades/{nombre}/clima` | Combina geocodificación y pronóstico de Open-Meteo (con caché) | 200, 404, 422, 502, 504 |
| `GET /v1/sensores` | Lista completa de sensores, recorriendo toda la paginación del servicio inestable | 200, 502, 504 |
| `GET /v1/sensores/{sensor_id}/lectura` | Lectura actual de un sensor | 200, 404, 502, 504 |
| `POST /v1/alertas` | Crea una alerta; exige la cabecera `Idempotency-Key` | 201, 409, 422 |
| `GET /v1/alertas` | Lista las alertas creadas | 200 |

Criterio de códigos de estado:

| Situación | Código |
|---|---|
| Todo correcto | 200 (201 al crear una alerta) |
| La ciudad o el sensor no existe | 404 |
| Parámetro inválido, cuerpo inválido o falta `Idempotency-Key` | 422 |
| `Idempotency-Key` reutilizada con un cuerpo distinto | 409 |
| La dependencia respondió con error o con datos que no cumplen el modelo | 502 |
| La dependencia no respondió a tiempo (o agotó los reintentos por timeout) | 504 |

No se devuelve ningún 500 genérico: las excepciones de dominio (`CiudadNoEncontrada`, `ErrorDependencia`, `DependenciaLenta`, `RecursoNoEncontrado`, `ServicioNoDisponible`, `RespuestaInvalida`) se traducen a estos códigos.

## 6. Resiliencia frente al servicio inestable

Implementada en `app/sensores.py`, función `get_con_reintentos`.

| Decisión | Valor |
|---|---|
| Máximo de intentos | 5 |
| Timeout por intento | 2 s (menor que la latencia lenta de 4 s del servicio, para forzar el reintento) |
| Backoff | Exponencial con jitter completo: espera aleatoria entre 0 y `min(8, 0.5 · 2^(intento-1))` s |
| Se reintenta | 503, 429, timeouts y errores de red |
| No se reintenta | 404 (`RecursoNoEncontrado`), 400 y cualquier otro código (`RespuestaInvalida`) |
| `Retry-After` | Si llega en un 429, se espera ese valor más un jitter de 0 a 0,5 s |
| Paginación | Se sigue el campo `siguiente` hasta que es `null` (23 sensores en 5 páginas) |
| Agotados los intentos | 504 si el último fallo fue un timeout; 502 en los demás casos |

### 6.1 Evidencias de reintentos

Las evidencias provienen de una corrida con los parámetros por defecto del servicio inestable. Se hicieron 8 peticiones a `GET /v1/sensores`: 7 respondieron 200 tras reintentar y 1 respondió 504 al agotar los intentos. Los logs completos están en `evidencias/agregador.log` y `evidencias/servicio_inestable_normal.log`.

**Backoff exponencial con jitter** (una misma página, 20:57:08 a 20:57:13):

```
sensores /sensores -> HTTP 503 (intento 1/5), espero 0.09 s
sensores /sensores -> timeout (intento 2/5), espero 0.59 s
sensores /sensores -> HTTP 503 (intento 3/5), espero 1.52 s
sensores /sensores -> HTTP 503 (intento 4/5), espero 0.52 s
httpx ...?cursor=bzo1 "HTTP/1.1 200 OK"
```

Lo que crece con cada intento es el tope de la espera (0,5, 1, 2 y 4 s); la espera concreta es aleatoria dentro de ese tope, por eso 0,52 s puede seguir a 1,52 s.

**Respeto de `Retry-After`** (21:00:14):

```
Servicio:  [21:00:14.835] GET /sensores -> 429 (Retry-After: 2)
Agregador: 21:00:14,836 sensores /sensores -> HTTP 429 (intento 1/5), espero 2.25 s
Servicio:  [21:00:17.093] GET /sensores -> 200
```

Entre el 429 y la siguiente llamada pasaron 2,26 s. En las 6 respuestas 429 de la corrida, el agregador esperó 2,36, 2,09, 2,39, 2,25, 2,23 y 2,34 s, es decir, siempre entre 2,0 y 2,5 s (`Retry-After` más jitter). En el log del servicio no aparece ninguna línea `429 (no respetó Retry-After ...)`.

Nota: a las 20:57:13 y 20:57:15 hay dos 429 consecutivos. El segundo es un 429 nuevo sorteado al azar (el servicio lo registra como `Retry-After: 2`, no como incumplimiento) y por eso hubo una segunda espera de 2,09 s.

**Timeout menor que la respuesta lenta, con reintento exitoso** (20:59:19):

```
Servicio:  [20:59:19.167] GET /sensores -> lenta (4.0 s)
Agregador: 20:59:21,168 sensores /sensores -> timeout (intento 1/5), espero 0.03 s
Agregador: 20:59:21,206 ...?cursor=bzoyMA%3D%3D "HTTP/1.1 200 OK"
Servicio:  [20:59:23.168] GET /sensores -> 200     (la respuesta lenta termina cuando el cliente ya se había ido)
```

**Agotamiento de intentos que termina en 504** (20:59:48 a 20:59:56, última página):

```
20:59:50,770 timeout (intento 1/5), espero 0.21 s
20:59:50,989 HTTP 503 (intento 2/5), espero 0.67 s
20:59:51,665 HTTP 429 (intento 3/5), espero 2.39 s
20:59:54,060 HTTP 503 (intento 4/5), espero 0.19 s
Servicio: [20:59:54.252] GET /sensores -> lenta (4.0 s)     <- el 5.º intento expira por timeout
INFO: 127.0.0.1:50326 - "GET /v1/sensores HTTP/1.1" 504 Gateway Timeout
```

El agregador no se cuelga ni devuelve un 500: tras el quinto intento responde 504 con un mensaje claro.

**404 sin reintentos.** `GET /v1/sensores/S-999/lectura` responde `404 {"detail":"El sensor no existe"}` (ver `evidencias/log.txt`). En el código, el 404 lanza `RecursoNoEncontrado` antes de entrar a la espera del backoff, por lo que no hay segundo intento.

## 7. Caché y medición de latencia

Caché en memoria con TTL (`app/cache.py`) aplicada a `GET /v1/ciudades/{nombre}/clima`. Solo se guardan respuestas exitosas, de modo que un fallo temporal no queda fijado en la caché. La respuesta incluye la cabecera `X-Cache: HIT` o `MISS`. No se cachean las lecturas de sensores, porque son datos en tiempo real.

Medición con `medir.py` (10 peticiones seguidas a `/v1/ciudades/Bucaramanga/clima`; servidor reiniciado entre escenarios):

| Escenario | Media | Mínimo | Máximo |
|---|---|---|---|
| Sin caché (`CACHE_TTL=0`), medición 1 | 2490 ms | 2151 ms | 3427 ms |
| Sin caché (`CACHE_TTL=0`), medición 2 | 1328 ms | 1231 ms | 1571 ms |
| Con caché (`CACHE_TTL=300`), medición 1: 1 MISS + 9 HIT | 541 ms | 1 ms | 5389 ms |
| Con caché (`CACHE_TTL=300`), medición 2: 1 MISS + 9 HIT | 131 ms | 2 ms | 1284 ms |

Lectura de los resultados:

- Un acierto de caché (HIT) tarda entre 1 y 3 ms; un fallo (MISS) o una petición sin caché tarda entre 1,2 y 5,4 s, porque encadena dos llamadas HTTPS a Open-Meteo.
- En la medición 2, la media bajó de 1328 ms a 131 ms (unas 10 veces menos). La media con caché está dominada por la primera petición (MISS); con más aciertos tiende al tiempo de un HIT.
- La carga sobre Open-Meteo pasó de 20 llamadas salientes (2 por petición) a 2 en total para las 10 peticiones.
- Las dos mediciones de cada escenario difieren bastante (por ejemplo, 2490 ms frente a 1328 ms sin caché) porque la red de la sala y el servicio externo son compartidos y su latencia varía. Por eso se reportan ambas.

## 8. Idempotencia en `POST /v1/alertas`

La cabecera `Idempotency-Key` es obligatoria (entre 8 y 100 caracteres). El servidor guarda, por cada clave, una huella SHA-256 del cuerpo y la alerta creada:

- Misma clave y mismo cuerpo: se devuelve la alerta ya creada (mismo `id` y mismo `creada_en`), con la cabecera `Idempotent-Replayed: true`, sin crear otra. El código de estado de la repetición sigue siendo 201.
- Misma clave y cuerpo distinto: 409, porque es un error del cliente.
- Sin la cabecera: 422.

### 8.1 Demostración

Salida real de las cinco peticiones (se muestran la línea de estado, la cabecera relevante y el cuerpo; la salida completa de `curl` está en `evidencias/idempotencia.txt`):

```text
### 1) Primera petición
HTTP/1.1 201 Created
idempotent-replayed: true
{"sensor_id":"S-001","mensaje":"CO2 alto en Biblioteca","severidad":"alta","id":"b660e982-1676-44d6-87b9-ed3ff82745d2","creada_en":"2026-10-26T02:51:51+00:00"}

### 2) Misma clave, mismo cuerpo
HTTP/1.1 201 Created
idempotent-replayed: true
{"sensor_id":"S-001","mensaje":"CO2 alto en Biblioteca","severidad":"alta","id":"b660e982-1676-44d6-87b9-ed3ff82745d2","creada_en":"2026-10-26T02:51:51+00:00"}

### 3) Lista de alertas
HTTP/1.1 200 OK
[{"sensor_id":"S-001","mensaje":"CO2 alto en Biblioteca","severidad":"alta","id":"b660e982-1676-44d6-87b9-ed3ff82745d2","creada_en":"2026-10-26T02:51:51+00:00"}]

### 4) Misma clave, cuerpo distinto
HTTP/1.1 409 Conflict
{"detail":"Idempotency-Key ya usada con un cuerpo distinto"}

### 5) Sin cabecera
HTTP/1.1 422 Unprocessable Content
{"detail":[{"type":"missing","loc":["header","idempotency-key"],"msg":"Field required","input":null}]}
```

Qué demuestra:

- Las peticiones 1 y 2, con la misma clave y el mismo cuerpo, devuelven la misma alerta: mismo `id` (`b660e982-...`) y mismo `creada_en`.
- `GET /v1/alertas` lista una sola alerta, aunque se hicieron dos `POST`.
- Reutilizar la clave con otro cuerpo da 409, y omitir la cabecera da 422.
- La repetición también responde 201; lo que la distingue es la cabecera `Idempotent-Replayed: true`.
- En esta corrida la alerta ya existía antes de la primera petición (`creada_en` es de las 02:51:51 y las peticiones se enviaron a las 02:55:19), por lo que la petición 1 también aparece como repetición. Para ver la creación original con un servidor limpio, se reinicia el agregador (las claves se guardan en memoria) y se ejecutan de nuevo los comandos.

Comandos usados (también están en `peticiones.http`):

```bash
# 1) Primera petición: crea la alerta
curl -i -X POST http://127.0.0.1:8000/v1/alertas \
  -H "Content-Type: application/json" -H "Idempotency-Key: demo-0001-abcdef" \
  -d '{"sensor_id":"S-001","mensaje":"CO2 alto en Biblioteca","severidad":"alta"}'

# 2) Misma clave y mismo cuerpo: devuelve la misma alerta
curl -i -X POST http://127.0.0.1:8000/v1/alertas \
  -H "Content-Type: application/json" -H "Idempotency-Key: demo-0001-abcdef" \
  -d '{"sensor_id":"S-001","mensaje":"CO2 alto en Biblioteca","severidad":"alta"}'

# 3) Debe haber una sola alerta
curl -i http://127.0.0.1:8000/v1/alertas

# 4) Misma clave con otro cuerpo: 409
curl -i -X POST http://127.0.0.1:8000/v1/alertas \
  -H "Content-Type: application/json" -H "Idempotency-Key: demo-0001-abcdef" \
  -d '{"sensor_id":"S-002","mensaje":"Otro mensaje","severidad":"baja"}'

# 5) Sin cabecera: 422
curl -i -X POST http://127.0.0.1:8000/v1/alertas \
  -H "Content-Type: application/json" \
  -d '{"sensor_id":"S-001","mensaje":"x"}'
```

## 9. Preguntas de análisis

**a) ¿Por qué es peligroso reintentar un POST sin clave de idempotencia?**
Un POST crea algo nuevo cada vez que llega. Si el cliente no recibe la respuesta (timeout, corte de red, un 503 devuelto después de haber procesado la petición), no puede saber si la operación se ejecutó. Si reintenta sin clave, el servidor no distingue una repetición de una petición nueva y duplica el efecto: dos alertas, dos cobros, dos correos. Por eso los reintentos automáticos del agregador se aplican solo a peticiones GET, que son seguras de repetir. Con `Idempotency-Key`, repetir un POST pasa a ser seguro.

**b) ¿Qué ocurriría si 50 clientes reintentaran al mismo tiempo sin jitter?**
Todos fallarían a la vez y esperarían exactamente lo mismo (por ejemplo 0,5 s, luego 1 s, luego 2 s), de modo que volverían en oleadas sincronizadas de 50 peticiones. Es la tormenta de reintentos (thundering herd): el servicio, que intenta recuperarse, recibe picos de carga y vuelve a fallar, y el ciclo se repite. Con jitter cada cliente espera un tiempo aleatorio dentro del tope y las llegadas se reparten en el tiempo. En nuestros logs, tras un 429 el servicio indica `Retry-After: 2` a todos por igual; por eso sumamos de 0 a 0,5 s de jitter (esperas observadas de 2,09 a 2,39 s), para que los clientes no regresen todos en el mismo instante.

**c) ¿Qué tan desactualizados pueden estar los datos de la caché y cuándo sería inaceptable?**
Como máximo, el TTL (300 s por defecto) más la antigüedad que ya tuviera el dato en Open-Meteo. En una de nuestras pruebas la medición era de las 00:15 y se consultó a las 00:18. Es aceptable para mostrar el clima, que cambia lentamente. Sería inaceptable cuando una decisión depende del dato fresco: alertas de seguridad (tormenta, CO₂ alto), precios o saldos, inventarios. Por eso no se cachean las lecturas de sensores y solo se cachea la API externa. Además, solo se almacenan respuestas exitosas, para no conservar errores.

**d) ¿Qué cambiaría en el diseño con varias réplicas detrás de un balanceador?**

- **Caché:** cada réplica tiene su propia caché en memoria, con menos aciertos y datos distintos entre réplicas. Habría que usar una caché compartida (por ejemplo, Redis).
- **Idempotencia:** es el cambio crítico. Si el primer POST cae en la réplica A y el reintento en la B, B no conoce la clave y crea un duplicado. El almacén de claves debe ser compartido (base de datos o Redis) y el registro de la clave debe ser atómico (`SET NX` o una restricción `UNIQUE`), para que dos réplicas no procesen la misma clave a la vez.
- **Reintentos:** cada réplica reintenta por su cuenta, así que N réplicas multiplican la carga sobre el servicio inestable. Convendría un límite global de reintentos (retry budget) o un circuit breaker.
- **Tiempos:** el balanceador tiene su propio timeout. En nuestros logs una petición llegó a tardar cerca de 9,5 s acumulando reintentos (20:57:08 a 20:57:17), así que habría que acotar el tiempo total máximo por petición para que el balanceador no la corte.
- **Límite por IP:** el servicio inestable bloquea por IP tras un 429. Con un balanceador, todas las réplicas pueden verse como una sola IP y el bloqueo las afectaría a todas.

## 10. Colección de peticiones de prueba

El archivo `peticiones.http` reúne las peticiones para revisar cada endpoint. Se ejecuta con la extensión REST Client de VS Code (o con cualquier cliente compatible con archivos `.http`); los comandos `curl` de las secciones anteriores son equivalentes. Las URLs base están al inicio del archivo como variables (`@agregador`, `@inestable`).

| Grupo | Peticiones | Resultado esperado |
|---|---|---|
| Sistema | `/salud` del agregador y del servicio inestable, `/config` | 200 |
| Clima (Open-Meteo, con caché) | Ciudad válida, ciudad inexistente, nombre de 1 letra | 200 (`X-Cache: MISS`, luego `HIT`), 404, 422 |
| Sensores (servicio inestable) | Lista completa, lectura de `S-001`, lectura de `S-999` | 200 con 23 sensores, 200, 404 |
| Idempotencia | Primera petición, repetición, lista, cuerpo distinto, sin cabecera, cuerpo inválido | 201, 201 con `Idempotent-Replayed: true`, 200 con una alerta, 409, 422, 422 |

Los códigos 502 y 504 dependen del estado de las dependencias y no se pueden disparar solo con el archivo. Para reproducirlos:

- **502 en sensores:** detener el servicio inestable y repetir `GET /v1/sensores`; el agregador agota los 5 intentos por error de red.
- **504 en sensores:** arrancar el servicio con `python servicio_inestable.py --tasa-fallos 0 --tasa-429 0 --tasa-lentitud 1` y pedir `GET /v1/sensores/S-001/lectura`; todos los intentos superan el timeout de 2 s.
- **502 y 504 en clima:** se probaron cambiando temporalmente la URL de pronóstico por una inválida (502) y el timeout por `httpx.Timeout(0.001)` (504). Las salidas están en `evidencias/log.txt`.

## 11. Evidencias incluidas

| Archivo | Contenido |
|---|---|
| `evidencias/agregador.log` | Reintentos, esperas y 504 del agregador |
| `evidencias/servicio_inestable_normal.log` | Fallos inyectados por el servicio (503, 429 con `Retry-After`, respuestas lentas) |
| `evidencias/log.txt` | Salidas de `curl` de las partes B y C y mediciones de latencia |
| `evidencias/idempotencia.txt` | Demostración de idempotencia |
