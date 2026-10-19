
# Genera el archivo "numeros.txt" con 500.000 enteros aleatorios

import random

# Semilla fija: al volver a correr el código se genera el MISMO
random.seed(42)

CANTIDAD = 500_000        # números a generar
MINIMO   = -1_000_000      # rango: enteros entre -1 millón y +1 millón
MAXIMO   = 1_000_000

# Genera la lista de números aleatorios
numeros = [random.randint(MINIMO, MAXIMO) for _ in range(CANTIDAD)]

# Guarda un número por línea en texto plano
with open("numeros.txt", "w") as archivo:
    archivo.write("\n".join(map(str, numeros)))

# Verificación rápida
print("Archivo generado: numeros.txt")
print("Cantidad de números:", len(numeros))
print("Primeros 10:       ", numeros[:10])
print("Mínimo y máximo:   ", min(numeros), max(numeros))