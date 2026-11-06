import time


class CacheTTL:
    def __init__(self, ttl: float):
        self.ttl = ttl
        self._datos: dict = {}

    def get(self, clave):
        item = self._datos.get(clave)
        if item is None:
            return None
        expira, valor = item
        if time.monotonic() >= expira:
            del self._datos[clave]
            return None
        return valor

    def set(self, clave, valor):
        self._datos[clave] = (time.monotonic() + self.ttl, valor)
