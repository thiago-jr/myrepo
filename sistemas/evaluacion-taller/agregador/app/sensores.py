import asyncio
import logging
import os
import random

import httpx
from pydantic import BaseModel, ValidationError

log = logging.getLogger("sensores")

URL_INESTABLE = os.environ.get("URL_INESTABLE", "http://127.0.0.1:9000")
MAX_INTENTOS = 5
BASE = 0.5  # segundos
TOPE = 8.0  # espera máxima del backoff
TIMEOUT = httpx.Timeout(2.0)  # menor que la latencia lenta (4 s) para forzar timeouts
REINTENTABLES = {429, 503}


# --- Modelos de lo que devuelve el servicio inestable ---
class Sensor(BaseModel):
    id: str
    tipo: str
    ubicacion: str


class PaginaSensores(BaseModel):
    sensores: list[Sensor]
    siguiente: str | None


class Lectura(BaseModel):
    sensor_id: str
    tipo: str
    valor: float
    unidad: str
    marca_tiempo: str


# --- Excepciones propias ---
class RecursoNoEncontrado(Exception): ...


class RespuestaInvalida(Exception): ...


class ServicioNoDisponible(Exception):
    def __init__(self, motivo: str, por_timeout: bool):
        super().__init__(motivo)
        self.por_timeout = por_timeout


def _retry_after(r: httpx.Response) -> float | None:
    try:
        return float(r.headers["Retry-After"])
    except (KeyError, ValueError):
        return None


async def get_con_reintentos(
    cliente: httpx.AsyncClient, ruta: str, params=None
) -> dict:
    motivo, por_timeout = "", False
    for intento in range(1, MAX_INTENTOS + 1):
        espera = None
        try:
            r = await cliente.get(
                f"{URL_INESTABLE}{ruta}", params=params, timeout=TIMEOUT
            )
        except httpx.TimeoutException:
            motivo, por_timeout = "timeout", True
        except httpx.TransportError as e:
            motivo, por_timeout = f"error de red: {e}", False
        else:
            if r.status_code == 200:
                return r.json()
            if r.status_code == 404:  # NO reintentar
                raise RecursoNoEncontrado(ruta)
            if r.status_code not in REINTENTABLES:  # 400 y otros: NO reintentar
                raise RespuestaInvalida(f"HTTP {r.status_code} en {ruta}")
            motivo, por_timeout = f"HTTP {r.status_code}", False
            if r.status_code == 429:
                ra = _retry_after(r)
                if ra is not None:
                    espera = ra + random.uniform(0, 0.5)  # respetar Retry-After

        if intento == MAX_INTENTOS:
            break
        if espera is None:  # backoff exponencial con jitter completo
            espera = random.uniform(0, min(TOPE, BASE * 2 ** (intento - 1)))
        log.warning(
            "%s -> %s (intento %d/%d), espero %.2f s",
            ruta,
            motivo,
            intento,
            MAX_INTENTOS,
            espera,
        )
        await asyncio.sleep(espera)

    raise ServicioNoDisponible(
        f"{ruta}: {motivo} tras {MAX_INTENTOS} intentos", por_timeout
    )


async def listar_todos_sensores(cliente: httpx.AsyncClient) -> list[Sensor]:
    sensores: list[Sensor] = []
    cursor = None
    while True:
        datos = await get_con_reintentos(
            cliente, "/sensores", {"cursor": cursor} if cursor else None
        )
        try:
            pagina = PaginaSensores.model_validate(datos)
        except ValidationError as e:
            raise RespuestaInvalida(str(e)) from e
        sensores.extend(pagina.sensores)
        cursor = pagina.siguiente
        if cursor is None:  # última página
            return sensores


async def obtener_lectura(cliente: httpx.AsyncClient, sensor_id: str) -> Lectura:
    datos = await get_con_reintentos(cliente, f"/sensores/{sensor_id}/lectura")
    try:
        return Lectura.model_validate(datos)
    except ValidationError as e:
        raise RespuestaInvalida(str(e)) from e
