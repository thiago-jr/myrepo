import statistics
import time

import httpx

URL = "http://127.0.0.1:8000/v1/ciudades/Bucaramanga/clima"

tiempos = []
with httpx.Client(timeout=30) as c:
    for _ in range(10):
        t = time.perf_counter()
        r = c.get(URL)
        tiempos.append((time.perf_counter() - t) * 1000)
        print(r.status_code, r.headers.get("x-cache"), f"{tiempos[-1]:.0f} ms")

print(
    f"media {statistics.mean(tiempos):.0f} ms | mín {min(tiempos):.0f} | máx {max(tiempos):.0f}"
)
