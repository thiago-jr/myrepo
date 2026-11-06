import asyncio
from app.cliente import obtener_clima_ciudad

print(asyncio.run(obtener_clima_ciudad("Bucaramanga")))
