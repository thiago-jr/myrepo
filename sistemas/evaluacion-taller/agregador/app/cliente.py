import httpx
from pydantic import ValidationError

from .modelos import RespuestaGeocodificacion, RespuestaPronostico

URL_GEO = "https://geocoding-api.open-meteo.com/v1/search"
URL_CLIMA = "https://api.open-meteo.com/v1/forecast"
TIMEOUT = httpx.Timeout(5.0, connect=3.0)  # explícito en cada llamada


class CiudadNoEncontrada(Exception):
    pass


class ErrorDependencia(Exception):
    """La API externa falló o respondió algo inválido (→ 502)."""


class DependenciaLenta(Exception):
    """La API externa no respondió a tiempo (→ 504)."""


async def _get(cliente: httpx.AsyncClient, url: str, params: dict) -> dict:
    try:
        r = await cliente.get(url, params=params, timeout=TIMEOUT)
        r.raise_for_status()
        return r.json()
    except httpx.TimeoutException as e:
        raise DependenciaLenta(str(e)) from e
    except httpx.HTTPError as e:
        raise ErrorDependencia(str(e)) from e


async def obtener_clima_ciudad(nombre: str) -> dict:
    async with httpx.AsyncClient() as cliente:
        # Llamada 1: nombre -> coordenadas
        datos_geo = await _get(
            cliente, URL_GEO, {"name": nombre, "count": 1, "language": "es"}
        )
        try:
            geo = RespuestaGeocodificacion.model_validate(datos_geo)
        except ValidationError as e:
            raise ErrorDependencia(f"Respuesta de geocodificación inválida: {e}") from e

        if not geo.results:
            raise CiudadNoEncontrada(nombre)
        lugar = geo.results[0]

        # Llamada 2: coordenadas -> clima
        datos_clima = await _get(
            cliente,
            URL_CLIMA,
            {
                "latitude": lugar.latitude,
                "longitude": lugar.longitude,
                "current": "temperature_2m,wind_speed_10m",
            },
        )
        try:
            clima = RespuestaPronostico.model_validate(datos_clima)
        except ValidationError as e:
            raise ErrorDependencia(f"Respuesta de pronóstico inválida: {e}") from e

    return {"lugar": lugar, "clima": clima.current}
