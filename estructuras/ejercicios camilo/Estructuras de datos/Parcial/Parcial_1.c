/* 

Contexto:

En el procesamiento de flujos continuos de datos (data streams) —como telemetría de sensores o registros de transacciones— es habitual analizar bloques de los $K$ eventos más recientes. A esta estructura se le conoce como ventana deslizante (sliding window). Adicionalmente, en procesos de auditoría se requiere inspeccionar los eventos en orden inverso de llegada (desde el más reciente hasta el más antiguo).

Descripción del Requerimiento:

Escriba un programa en C que simule el procesamiento de un flujo de enteros positivos.

El programa debe leer primero un número entero K (K > 1), que representa el tamaño dinámico de la ventana.

A continuación, leerá una secuencia indeterminada de números enteros positivos que finaliza con el valor indicador -1.

El programa mantendrá una Cola dinámica con una capacidad máxima de K elementos.

Por cada entero leído del flujo, el programa debe:

Control de capacidad: Si la Cola alcanza el tamaño K, extraer el elemento más antiguo (dequeue: extraer el primer elemento de la cola).

Ingreso de dato: Insertar el nuevo valor en la Cola (enqueue).

Visualización invertida: Invocando la función (Que debe ser implementada por usted.):

void imprimirVentanaInvertida(cola c)

Imprimir por pantalla los elementos de la ventana actual ordenados desde el más reciente hacia el más antiguo.

Para lograr la impresión en orden inverso sin destruir ni alterar el estado original de la Cola, la función imprimirVentanaInvertida debe:

Transferir los datos de la Cola hacia una Pila auxiliar dinámica.

Recorrer e imprimir los elementos aprovechando la propiedad LIFO de la Pila.

Restaurar los elementos a la Cola original en su orden inicial antes de finalizar la función.

Ejemplo
Input:
3 10 20 30 40 50 -1
Output
10 -> 10
20 -> 20 10
30 -> 30 20 10
40 -> 40 30 20  
50 -> 50 40 30


Especificaciones y restricciones

Prohibición de arreglos: No está permitido el uso de arreglos (en ninguna de sus variantes) para almacenar los datos de la ventana o realizar la impresión. Toda la gestión debe hacerse con los TADs de Cola y Pila dinámicos.

Nivel de abstracción: Toda la manipulación de datos en el main y en imprimirVentanaInvertida debe realizarse exclusivamente a través de las funciones públicas de los TADs (enqueue, dequeue, push, pop, isEmpty, obtenerTamano). Está prohibido acceder a punteros de nodos por fuera de la implementación del TAD.

Gestión de memoria: Al finalizar el programa (o al destruir las pilas auxiliares de la función), toda la memoria asignada dinámicamente para los nodos debe ser liberada con free().

*/

#include <stdio.h>
#include "lista_simple.h" // librería simple

// Imprime invertido sin alterar la cola original
void imprimirVentanaInvertida(lista cola) {
    if (isEmpty(cola)) {
        printf("\n");
        return;
    }

    // Se necesitan 2 pilas para restaurar el orden original.
    // Pila1 invierte el orden de la cola. Pila2 lo vuelve a invertir, 
    // dejándolo en el orden original para devolverlo a la cola.
    lista pila1 = crearLista();
    lista pila2 = crearLista();
    
    int tam = size(cola);

    // Transferir Cola -> Pila1 (Invierte el orden)
    for (int i = 0; i < tam; i++) {
        short val = extract(cola); // Saca el más antiguo (frente de la cola)
        push(val, pila1);          // Lo apila (queda como el más reciente en la pila)
    }

    // Imprimir y transferir Pila1 -> Pila2 (Restaura el orden)
    for (int i = 0; i < tam; i++) {
        short val = pop(pila1);    // Saca el más reciente (LIFO)
        printf("%d ", val);        // imprime el valor
        push(val, pila2);          // se guarda en la pila2 para no perderlo
    }
    printf("\n");

    // Restaurar Cola original (Pila2 -> Cola)
    // Como pila2 tiene el orden original (el primero que salió está en el tope),
    // al hacer pop y enqueue, la cola recupera su estado exacto inicial.
    for (int i = 0; i < tam; i++) {
        short val = pop(pila2);
        enqueue(val, cola);
    }

    // Limpieza de memoria (rúbrica)
    destruirLista(pila1);
    destruirLista(pila2);
}

int main() {
    int K;
    
    // tamaño de la ventana
    if (scanf("%d", &K) != 1 || K <= 1) {
        printf("K entero mayor a 1.\n");
        return 1;
    }

    lista cola = crearLista();
    short val;

    //flujo de datos hasta encontrar -1
    while (scanf("%hd", &val) == 1 && val != -1) {
        
        //Si alcanza K, sacar el más antiguo
        if (size(cola) == K) {
            extract(cola); // = a dequeue()
        }

        // Ingreso del nuevo dato al final de la cola
        enqueue(val, cola);

        // Visualización: Imprime el nuevo valor y llama a la función
        printf("%d -> ", val);
        imprimirVentanaInvertida(cola);
    }

    //limpieza final de memoria
    destruirLista(cola);
    
    return 0;
}