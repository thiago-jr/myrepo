"""
benchmark_nuevo.py
==================

Benchmark de los algoritmos de ordenamientos.py, con mejoras respecto a
benchmark.py:

  1. Mide con time.perf_counter() (mucho más preciso que time.time()).
  2. Escritura INCREMENTAL de resultados.csv: cada fila se escribe con
     flush() a disco apenas se calcula su promedio. Si Colab se desconecta,
     lo ya medido queda guardado (y al re-ejecutar, reanuda donde iba).
  4. Progreso en pantalla: algoritmo, tamaño y corrida en curso.
  5. Antes de cada corrida se hace muestra.copy() (fuera del cronómetro).

Archivos necesarios (misma carpeta): ordenamientos.py y numeros.txt
Salida: resultados.csv  (columnas: tamaño_muestra, algoritmo, tiempo_promedio)

Colab ejecutarlo:
    !python -u benchmark_nuevo.py
o pegando todo este código en una celda y ejecutándola.
"""

import csv
import os
import random
import time

# Los cuatro algoritmos que se usan tal cual, importados de la librería.
from ordenamientos import insercion, burbuja, seleccion, mezcla, quicksort

#  CONFIGS

ARCHIVO_ENTRADA = "numeros.txt"
ARCHIVO_SALIDA  = "resultados.csv"

REPETICIONES = 5      # corridas por cada combinación (algoritmo, tamaño)
REANUDAR     = True   # True: si resultados.csv ya existe, continúa donde iba

# Tamaños por familia de complejidad:
TAMANOS_ON2   = [500, 1000, 5000, 10000, 20000, 30000]
TAMANOS_NLOGN = [500, 1000, 10000, 30000, 50000, 100000,
                 200000, 300000, 400000, 500000]

# Diccionario de configuración: cada algoritmo con su función y sus
# tamaños permitidos. 
# Para agregar/quitar algoritmos o cambiar tamaños, editar SOLO el diccionario.
# Los O(n log n) van primero aposta: terminan rápido y dejan asegurada
# la parte de tamaños grandes antes de arrancar los O(n²), que son lo más lento.
CONFIGURACION = {
    "mezcla":    {"funcion": mezcla,          "tamanos": TAMANOS_NLOGN},
    "quicksort": {"funcion": quicksort,       "tamanos": TAMANOS_NLOGN},  
    "insercion": {"funcion": insercion,       "tamanos": TAMANOS_ON2},
    "burbuja":   {"funcion": burbuja,         "tamanos": TAMANOS_ON2},
    "seleccion": {"funcion": seleccion,       "tamanos": TAMANOS_ON2},
}


#  FUNCIONES AUX
def cargar_numeros(ruta):
    """Lee el archivo UNA vez y devuelve la lista completa de enteros."""
    with open(ruta, "r", encoding="utf-8") as f:
        contenido = f.read()
    return [int(token) for token in contenido.replace(",", " ").split()]


def combinaciones_ya_hechas(ruta):
    """
    Devuelve el conjunto de parejas (algoritmo, tamaño) que ya figuran en el
    CSV. Sirve para reanudar SI Y SOLO SI el benchmark tras una desconexión sin repetir
    combinaciones ya medidas.
    """
    hechos = set()
    if not os.path.exists(ruta):
        return hechos
    with open(ruta, "r", newline="", encoding="utf-8-sig") as f:
        for fila in csv.reader(f):
            if len(fila) >= 3 and fila[0] != "tamaño_muestra":
                try:
                    hechos.add((fila[1], int(fila[0])))
                except ValueError:
                    pass    # línea malformada: se ignora y se vuelve a medir
    return hechos

#  BENCHMARK

def main():
    print("BENCHMARK  |  time.perf_counter()  |  CSV incremental")

    # Carga de datos UNA sola vez
    print(f"\nCargando '{ARCHIVO_ENTRADA}'", flush=True)
    try:
        numeros = cargar_numeros(ARCHIVO_ENTRADA)
    except FileNotFoundError:
        print(f"ERROR: no se encontró '{ARCHIVO_ENTRADA}' en la carpeta actual.")
        return
    print(f"Se cargaron {len(numeros):,} números en memoria.\n", flush=True)

    #CSV
    if not REANUDAR and os.path.exists(ARCHIVO_SALIDA):
        os.remove(ARCHIVO_SALIDA)                    # empezar de cero

    if not os.path.exists(ARCHIVO_SALIDA):           # encabezado si es nuevo
        with open(ARCHIVO_SALIDA, "w", newline="", encoding="utf-8-sig") as f:
            csv.writer(f).writerow(["tamaño_muestra", "algoritmo", "tiempo_promedio"])

    hechos = combinaciones_ya_hechas(ARCHIVO_SALIDA) if REANUDAR else set()
    if hechos:
        print(f"REANUDANDO: '{ARCHIVO_SALIDA}' ya contiene {len(hechos)} "
              f"combinaciones medidas; esas se omitirán.")
        print("(Para medir todo de nuevo: borrar resultados.csv o poner"
              "REANUDAR = False.)\n")

    #Benchmark con escritura incremental
    inicio_total = time.perf_counter()
    filas_escritas = 0

    # El archivo se mantiene abierto en modo "append" durante todo el
    # benchmark; cada fila se escribe con flush() inmediato, de modo que
    # un corte de Colab no pierde nada de lo ya calculado.
    with open(ARCHIVO_SALIDA, "a", newline="", encoding="utf-8-sig") as f_salida:
        escritor = csv.writer(f_salida)

        for nombre, config in CONFIGURACION.items():
            funcion = config["funcion"]

            for n in config["tamanos"]:
                if n > len(numeros):
                    print(f"[{nombre}] n = {n:,}: no hay datos suficientes, se omite.")
                    continue
                if (nombre, n) in hechos:
                    print(f"[{nombre}] n = {n:,}: ya estaba en el CSV, se omite.")
                    continue

                muestra = numeros[:n]           # primeros n números del archivo
                tiempos = []

                for corrida in range(1, REPETICIONES + 1):
                    datos = muestra.copy()      # SIEMPRE copia (fuera del cronómetro)

                    inicio = time.perf_counter()     # --- empieza a medir ---
                    _ = funcion(datos)
                    fin = time.perf_counter()        # --- deja de medir ---

                    duracion = fin - inicio
                    tiempos.append(duracion)
                    print(f"[{nombre:<9}] n = {n:>7,} | corrida {corrida}/{REPETICIONES}"
                          f" | {duracion:.6f} s", flush=True)

                promedio = sum(tiempos) / REPETICIONES

                # Escritura INCREMENTAL: la fila se guarda apenas se calcula.
                escritor.writerow([n, nombre, f"{promedio:.6f}"])
                f_salida.flush()
                filas_escritas += 1

                print(f"[{nombre:<9}] n = {n:>7,} | PROMEDIO {promedio:.6f} s"
                      f"  ->  fila guardada\n", flush=True)

    duracion_total = time.perf_counter() - inicio_total

    # Resumen
    with open(ARCHIVO_SALIDA, "r", encoding="utf-8-sig") as f:
        filas_totales = sum(1 for _ in f) - 1      # sin contar el encabezado

    print("RESUMEN DEL BENCHMARK")
    print(f"  Filas guardadas en esta sesión        : {filas_escritas}")
    print(f"  Filas totales en '{ARCHIVO_SALIDA}'  : {filas_totales}")
    print(f"  Tiempo total del benchmark            : {duracion_total:,.1f} s "
          f"({duracion_total / 60:.1f} minutos)")



if __name__ == "__main__":
    main()