from pydantic import BaseModel, Field


class Lugar(BaseModel):
    name: str
    latitude: float
    longitude: float
    country: str | None = None


class RespuestaGeocodificacion(BaseModel):
    results: list[Lugar] = Field(default_factory=list)


class ClimaActual(BaseModel):
    time: str
    temperature_2m: float
    wind_speed_10m: float


class RespuestaPronostico(BaseModel):
    current: ClimaActual


class ClimaCiudad(BaseModel):
    ciudad: str
    pais: str | None = None
    latitud: float
    longitud: float
    temperatura_c: float
    viento_kmh: float
    hora_medicion: str


class ErrorRespuesta(BaseModel):
    detalle: str


from typing import Literal


class AlertaCrear(BaseModel):
    sensor_id: str = Field(min_length=1, max_length=20)
    mensaje: str = Field(min_length=1, max_length=200)
    severidad: Literal["baja", "media", "alta"] = "media"


class Alerta(AlertaCrear):
    id: str
    creada_en: str
