"""
benchmark.py

Mide el tiempo promedio de ejecución de los algoritmos de ordenamientos.py
sobre muestras de distintos tamaños tomadas de "numeros.txt".

Requisitos:
    - ordenamientos.py y numeros.txt en la misma carpeta que este script.

Salida:
    - resultados.csv con columnas: tamaño_muestra, algoritmo, tiempo_promedio
"""

import csv
import time

from ordenamientos import insercion, burbuja, seleccion, mezcla, quicksort

# CONFIG
ARCHIVO_ENTRADA = "numeros.txt"
ARCHIVO_SALIDA  = "resultados.csv"

TAMANOS = [500, 1000, 10000, 30000, 50000,
           100000, 200000, 300000, 400000, 500000]

REPETICIONES = 5

ALGORITMOS = {
    "insercion": insercion,
    "burbuja":   burbuja,
    "seleccion": seleccion,
    "mezcla":    mezcla,
    "quicksort": quicksort,
}

def cargar_numeros(ruta):
    """
    Lee el archivo y devuelve una lista de enteros.
    Acepta un número por línea o separados por espacios / comas.
    """
    with open(ruta, "r", encoding="utf-8") as f:
        contenido = f.read()
    return [int(token) for token in contenido.replace(",", " ").split()]


def main():
    #Cargar los 500,000 enteros
    print(f"Cargando '{ARCHIVO_ENTRADA}' ...")
    try:
        numeros = cargar_numeros(ARCHIVO_ENTRADA)
    except FileNotFoundError:
        print(f"Error: no se encontró '{ARCHIVO_ENTRADA}' en la carpeta actual.")
        return
    print(f"Se cargaron {len(numeros):,} números.\n")

    if len(numeros) < max(TAMANOS):
        print(f"AVISO: el archivo tiene menos de {max(TAMANOS):,} números; "
              f"los tamaños que excedan los datos se omitirán.\n")

    filas = []   # resultados que irán al CSV

    #Recorrer los tamaños de muestra
    for n in TAMANOS:
        if n > len(numeros):
            print(f"n = {n:,}: no hay suficientes datos, se omite.\n")
            continue

        # Muestra: los primeros n números del archivo.
        muestra = numeros[:n]

        #Ejecutar cada algoritmo REPETICIONES veces, midiendo cada corrida
        for nombre, funcion in ALGORITMOS.items():
            tiempos = []
            for corrida in range(1, REPETICIONES + 1):
                datos = muestra.copy()      # copia de trabajo (fuera del cronómetro)
                inicio = time.time()        # ---- empieza a medir ----
                funcion(datos)
                fin = time.time()           # ---- deja de medir ----
                tiempos.append(fin - inicio)
                print(f"n = {n:>7,} | {nombre:<9} | corrida {corrida}: "
                      f"{fin - inicio:.6f} s")

            #Promedio de las 5 corridas
            promedio = sum(tiempos) / REPETICIONES
            filas.append([n, nombre, f"{promedio:.6f}"])
            print(f"n = {n:>7,} | {nombre:<9} | promedio   : "
                  f"{promedio:.6f} s")
        print("-" * 62)

    # Guardar resultados en el CSV
    #(utf-8-sig para que Excel muestre la "ñ" del encabezado)
    with open(ARCHIVO_SALIDA, "w", newline="", encoding="utf-8-sig") as f:
        escritor = csv.writer(f)
        escritor.writerow(["tamaño_muestra", "algoritmo", "tiempo_promedio"])
        escritor.writerows(filas)

    print(f"\nListo: {len(filas)} filas guardadas en '{ARCHIVO_SALIDA}'.")


if __name__ == "__main__":
    main()