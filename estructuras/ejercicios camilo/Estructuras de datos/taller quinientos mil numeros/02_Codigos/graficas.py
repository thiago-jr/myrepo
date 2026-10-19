"""
graficas.py
===========

Genera las 3 gráficas del benchmark a partir de resultados.csv
(columnas: tamaño_muestra, algoritmo, tiempo_promedio).

Uso:
    python graficas.py                 # usa resultados.csv
    python graficas.py otro.csv        # usa otro archivo
    !python graficas.py                # desde Colab

Produce:
    grafica1_loglog.png   (5 algoritmos, escala log-log)
    grafica2_on2.png      (inserción, burbuja, selección; lineal, n <= 30,000)
    grafica3_nlogn.png    (mezcla, quicksort; lineal, n <= 500,000)
"""

import sys

import matplotlib
matplotlib.use("Agg")      # guarda PNGs sin abrir ventanas (modo script)
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import pandas as pd
import seaborn as sns

# --------------------------- Configuración ---------------------------
CSV_POR_DEFECTO = "resultados.csv"

LENTOS  = ["insercion", "burbuja", "seleccion"]
RAPIDOS = ["mezcla", "quicksort"]
ORDEN   = LENTOS + RAPIDOS

ETIQUETAS = {
    "insercion": "Inserción  O(n²)",
    "burbuja":   "Burbuja  O(n²)",
    "seleccion": "Selección  O(n²)",
    "mezcla":    "Mezcla (merge)  O(n·log n)",
    "quicksort": "Quicksort  O(n·log n)",
}
COLORES = {
    "insercion": "#d62728", "burbuja": "#ff7f0e", "seleccion": "#8c564b",
    "mezcla":    "#1f77b4", "quicksort": "#2ca02c",
}
MARCADORES = {"insercion": "o", "burbuja": "s", "seleccion": "^",
              "mezcla": "D", "quicksort": "v"}
# ----------------------------------------------------------------------


def cargar_datos(ruta):
    df = pd.read_csv(ruta, encoding="utf-8-sig")   # utf-8-sig quita el BOM
    faltan = {"tamaño_muestra", "algoritmo", "tiempo_promedio"} - set(df.columns)
    if faltan:
        raise ValueError(
            f"Faltan columnas {faltan}. Encontradas: {list(df.columns)}\n"
            "¿El CSV tiene otra codificación u otros nombres de columna?"
        )
    df["algoritmo"] = pd.Categorical(df["algoritmo"], categories=ORDEN, ordered=True)
    return df.sort_values(["algoritmo", "tamaño_muestra"]).reset_index(drop=True)


def _trazar(ax, datos, algoritmos):
    """Dibuja las líneas con seaborn y reetiqueta la leyenda."""
    sns.lineplot(
        data=datos, x="tamaño_muestra", y="tiempo_promedio",
        hue="algoritmo", hue_order=algoritmos,
        style="algoritmo", markers=MARCADORES, dashes=False,
        palette=COLORES, linewidth=2.2, markersize=8, ax=ax,
    )
    handles, labels = ax.get_legend_handles_labels()
    ax.legend(handles, [ETIQUETAS.get(l, l) for l in labels],
              title="Algoritmo", loc="upper left")


def grafica1_loglog(df, archivo="grafica1_loglog.png"):
    fig, ax = plt.subplots(figsize=(9, 6))
    _trazar(ax, df, ORDEN)
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.grid(True, which="both", linewidth=0.4, alpha=0.7)
    ax.set_title("Algoritmos de ordenamiento: tiempo promedio vs tamaño (log-log)",
                 fontsize=13)
    ax.set_xlabel("Tamaño de la muestra n (escala logarítmica)")
    ax.set_ylabel("Tiempo promedio (segundos, escala logarítmica)")
    fig.savefig(archivo, dpi=150, bbox_inches="tight")
    plt.close(fig)
    return archivo


def grafica2_on2(df, archivo="grafica2_on2.png"):
    fig, ax = plt.subplots(figsize=(9, 6))
    _trazar(ax, df[df["algoritmo"].isin(LENTOS)], LENTOS)
    ax.set_xlim(0, 30_000)
    ax.xaxis.set_major_formatter(mticker.FuncFormatter(lambda x, _: f"{int(x):,}"))
    ax.set_title("Algoritmos O(n²): inserción, burbuja y selección (escala lineal)",
                 fontsize=13)
    ax.set_xlabel("Tamaño de la muestra n")
    ax.set_ylabel("Tiempo promedio (segundos)")
    fig.savefig(archivo, dpi=150, bbox_inches="tight")
    plt.close(fig)
    return archivo


def grafica3_nlogn(df, archivo="grafica3_nlogn.png"):
    fig, ax = plt.subplots(figsize=(9, 6))
    _trazar(ax, df[df["algoritmo"].isin(RAPIDOS)], RAPIDOS)
    ax.set_xlim(0, 500_000)
    ax.xaxis.set_major_formatter(mticker.FuncFormatter(lambda x, _: f"{int(x):,}"))
    ax.set_title("Algoritmos O(n·log n): mezcla y quicksort (escala lineal)",
                 fontsize=13)
    ax.set_xlabel("Tamaño de la muestra n")
    ax.set_ylabel("Tiempo promedio (segundos)")
    fig.savefig(archivo, dpi=150, bbox_inches="tight")
    plt.close(fig)
    return archivo


def main():
    sns.set_theme(style="whitegrid", context="notebook")
    ruta = sys.argv[1] if len(sys.argv) > 1 else CSV_POR_DEFECTO
    df = cargar_datos(ruta)
    print(f"Datos: {ruta} ({len(df)} filas)")
    for generada in (grafica1_loglog(df), grafica2_on2(df), grafica3_nlogn(df)):
        print(f"Guardada: {generada}")


if __name__ == "__main__":
    main()