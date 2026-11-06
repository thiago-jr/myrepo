import os
from fastapi import Response
from .cache import CacheTTL

cache_clima = CacheTTL(float(os.environ.get("CACHE_TTL", "300")))

from fastapi import FastAPI, HTTPException, Path

from .cliente import (
    CiudadNoEncontrada,
    DependenciaLenta,
    ErrorDependencia,
    obtener_clima_ciudad,
)
from .modelos import ClimaCiudad, ErrorRespuesta

app = FastAPI(
    title="Agregador resiliente",
    description="Combina geocodificación y pronóstico de Open-Meteo.",
    version="1.0",
)


@app.get("/salud", tags=["sistema"])
async def salud():
    return {"estado": "ok"}


@app.get(
    "/v1/ciudades/{nombre}/clima",
    response_model=ClimaCiudad,
    tags=["clima"],
    summary="Clima actual de una ciudad",
    responses={
        404: {"model": ErrorRespuesta, "description": "La ciudad no existe"},
        422: {"description": "Parámetro inválido"},
        502: {"model": ErrorRespuesta, "description": "La API externa falló"},
        504: {"model": ErrorRespuesta, "description": "La API externa tardó demasiado"},
    },
)
async def clima_de_ciudad(
    response: Response, nombre: str = Path(min_length=2, max_length=60)
):
    clave = nombre.strip().lower()
    if (guardado := cache_clima.get(clave)) is not None:
        response.headers["X-Cache"] = "HIT"
        return guardado

    try:
        datos = await obtener_clima_ciudad(nombre)
    except CiudadNoEncontrada:
        raise HTTPException(
            status_code=404, detail=f"No se encontró la ciudad '{nombre}'"
        )
    except DependenciaLenta:
        raise HTTPException(
            status_code=504, detail="La API externa tardó demasiado en responder"
        )
    except ErrorDependencia:
        raise HTTPException(
            status_code=502, detail="La API externa falló o respondió algo inválido"
        )

    lugar, clima = datos["lugar"], datos["clima"]
    resultado = ClimaCiudad(
        ciudad=lugar.name,
        pais=lugar.country,
        latitud=lugar.latitude,
        longitud=lugar.longitude,
        temperatura_c=clima.temperature_2m,
        viento_kmh=clima.wind_speed_10m,
        hora_medicion=clima.time,
    )

    cache_clima.set(clave, resultado)
    response.headers["X-Cache"] = "MISS"
    return resultado


import logging
import httpx
from .sensores import (
    Lectura,
    RecursoNoEncontrado,
    RespuestaInvalida,
    Sensor,
    ServicioNoDisponible,
    listar_todos_sensores,
    obtener_lectura,
)

logging.basicConfig(level=logging.INFO, format="%(asctime)s %(name)s %(message)s")


def _traducir(e: Exception) -> HTTPException:
    if isinstance(e, RecursoNoEncontrado):
        return HTTPException(404, "El sensor no existe")
    if isinstance(e, ServicioNoDisponible):
        return HTTPException(
            504 if e.por_timeout else 502,
            "El servicio de sensores no responde; intente más tarde",
        )
    return HTTPException(502, "Respuesta inválida del servicio de sensores")


@app.get(
    "/v1/sensores",
    response_model=list[Sensor],
    tags=["sensores"],
    responses={502: {"model": ErrorRespuesta}, 504: {"model": ErrorRespuesta}},
)
async def sensores():
    try:
        async with httpx.AsyncClient() as c:
            return await listar_todos_sensores(c)
    except (RecursoNoEncontrado, ServicioNoDisponible, RespuestaInvalida) as e:
        raise _traducir(e)


@app.get(
    "/v1/sensores/{sensor_id}/lectura",
    response_model=Lectura,
    tags=["sensores"],
    responses={
        404: {"model": ErrorRespuesta},
        502: {"model": ErrorRespuesta},
        504: {"model": ErrorRespuesta},
    },
)
async def lectura(sensor_id: str):
    try:
        async with httpx.AsyncClient() as c:
            return await obtener_lectura(c, sensor_id)
    except (RecursoNoEncontrado, ServicioNoDisponible, RespuestaInvalida) as e:
        raise _traducir(e)


import hashlib
import uuid
from datetime import datetime, timezone

from fastapi import FastAPI, Header, HTTPException, Path, Response

from .modelos import Alerta, AlertaCrear, ClimaCiudad, ErrorRespuesta

ALERTAS: dict[str, Alerta] = {}
# clave de idempotencia -> (huella del cuerpo, alerta creada)
IDEMPOTENCIA: dict[str, tuple[str, Alerta]] = {}


@app.post(
    "/v1/alertas",
    response_model=Alerta,
    status_code=201,
    tags=["alertas"],
    summary="Crear una alerta (idempotente)",
    responses={
        409: {
            "model": ErrorRespuesta,
            "description": "Idempotency-Key ya usada con un cuerpo distinto",
        },
        422: {"description": "Cuerpo inválido o falta la cabecera Idempotency-Key"},
    },
)
async def crear_alerta(
    cuerpo: AlertaCrear,
    response: Response,
    idempotency_key: str = Header(min_length=8, max_length=100),
):
    huella = hashlib.sha256(cuerpo.model_dump_json().encode()).hexdigest()

    previo = IDEMPOTENCIA.get(idempotency_key)
    if previo is not None:
        huella_previa, alerta = previo
        if huella_previa != huella:
            raise HTTPException(409, "Idempotency-Key ya usada con un cuerpo distinto")
        response.headers["Idempotent-Replayed"] = "true"
        return alerta  # misma respuesta, sin crear nada

    alerta = Alerta(
        id=str(uuid.uuid4()),
        **cuerpo.model_dump(),
        creada_en=datetime.now(timezone.utc).isoformat(timespec="seconds"),
    )
    ALERTAS[alerta.id] = alerta
    IDEMPOTENCIA[idempotency_key] = (huella, alerta)
    return alerta


@app.get("/v1/alertas", response_model=list[Alerta], tags=["alertas"])
async def listar_alertas():
    return list(ALERTAS.values())
