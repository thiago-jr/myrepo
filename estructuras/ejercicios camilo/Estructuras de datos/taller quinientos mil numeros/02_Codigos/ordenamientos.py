"""
ordenamientos.py
Librería de algoritmos de ordenamiento.

Uso desde otro archivo:
    from ordenamientos import insercion, burbuja, seleccion, mezcla, quicksort

Todas las funciones RECIBEN una lista y DEVUELVEN una nueva lista ordenada.
La lista original no se modifica.
"""


def insercion(arr):
    """
    Ordenamiento por inserción.
    Explicacion: similitud a ordenar cartas en la mano. Se toma un elemento
    y se inserta en su lugar correcto dentro de la parte ya ordenada.

    inglés: Insertion Sort (insertion_sort)
    """
    lista = arr.copy()
    for i in range(1, len(lista)):
        valor = lista[i]          # elemento a insertar
        j = i - 1
        # Mueve hacia la derecha todo lo que sea mayor que el "valor"
        while j >= 0 and lista[j] > valor:
            lista[j + 1] = lista[j]
            j -= 1
        lista[j + 1] = valor      # inserta en su lugar
    return lista


def burbuja(arr):
    """
    Ordenamiento burbuja.
    Explicacion: compara vecinos e intercambia si están mal ordenados.
    En cada pasada el más grande "flota" hasta el final.

    inglés: Bubble Sort (bubble_sort)
    """
    lista = arr.copy()
    n = len(lista)
    for i in range(n - 1):
        for j in range(n - 1 - i):
            if lista[j] > lista[j + 1]:
                lista[j], lista[j + 1] = lista[j + 1], lista[j]
    return lista


def seleccion(arr):
    """
    Ordenamiento por selección.
    Explicacion: busca el elemento más pequeño de la parte desordenada
    y lo coloca al principio de esa parte. Repite hasta el final.

    inglés: Selection Sort (selection_sort)
    """
    lista = arr.copy()
    n = len(lista)
    for i in range(n - 1):
        min_idx = i
        # Busca el mínimo en el resto de la lista
        for j in range(i + 1, n):
            if lista[j] < lista[min_idx]:
                min_idx = j
        # Intercambia el mínimo con la posición actual
        lista[i], lista[min_idx] = lista[min_idx], lista[i]
    return lista


def mezcla(arr):
    """
    Ordenamiento por mezcla (Merge Sort).
    Explicacion: divide la lista en mitades hasta tener piezas de tamaño 1,
    ordena cada mitad y luego las mezcla.

    inglés: Merge Sort (merge_sort)
    """
    # Caso base: una lista de 0 o 1 elementos ya está ordenada
    if len(arr) <= 1:
        return arr.copy()

    # Divide en dos mitades y ordena cada una (recursión)
    medio = len(arr) // 2
    izquierda = mezcla(arr[:medio])
    derecha = mezcla(arr[medio:])

    # Mezcla las dos mitades ya ordenadas
    return _mezclar(izquierda, derecha)


def _mezclar(izq, der):
    """
    Función auxiliar de mezcla: combina dos listas ordenadas en una.

    inglés: merge
    """
    resultado = []
    i = j = 0
    # Compara el frente de cada lista y toma el menor
    while i < len(izq) and j < len(der):
        if izq[i] <= der[j]:
            resultado.append(izq[i])
            i += 1
        else:
            resultado.append(der[j])
            j += 1
    # Agrega lo que sobró (solo una de las dos tendrá elementos)
    resultado.extend(izq[i:])
    resultado.extend(der[j:])
    return resultado


def quicksort(arr):
    """
    Quicksort.
    Explicacion: elige un "pivote", separa los menores a la izquierda
    y los mayores a la derecha, y repite el proceso en cada lado.

    inglés: Quick Sort (quick_sort o quicksort)
    """
    # Caso base
    if len(arr) <= 1:
        return arr.copy()

    pivote = arr[0]  # usamos el primer elemento como pivote
    menores = [x for x in arr[1:] if x <= pivote]
    mayores = [x for x in arr[1:] if x > pivote]

    return quicksort(menores) + [pivote] + quicksort(mayores)

# Prueba personal, solo se ejecuta si corres ESTE archivo directo

if __name__ == "__main__":
    prueba = [5, 2, 9, 1, 7, 3]
    print("Original:            ", prueba)
    print("Inserción:           ", insercion(prueba))
    print("Burbuja:             ", burbuja(prueba))
    print("Selección:           ", seleccion(prueba))
    print("Mezcla (Merge Sort): ", mezcla(prueba))
    print("Quicksort:           ", quicksort(prueba))