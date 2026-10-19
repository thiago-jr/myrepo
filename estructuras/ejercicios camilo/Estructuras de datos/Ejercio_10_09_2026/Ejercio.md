Título: Conversión y Evaluación de Expresiones: De Infijo a Posfijo

Descripción:

En la práctica cotidiana, las expresiones matemáticas se escriben utilizando notación infija, donde los operadores se ubican entre los operandos (por ejemplo, $5 + (3 \times 7)$). Sin embargo, la notación infija presenta ambigüedad operacional, lo que exige el uso de paréntesis y reglas explícitas de precedencia y asociatividad.

Considere la expresión: 3 + 4 * 2 / ( 1 - 5 ) ^ 2

Al analizarla, el orden estricto de evaluación es el siguiente:

Se evalúa la resta dentro del paréntesis: $(1 - 5) = -4$.

Se evalúa la potencia: $(-4)^2 = 16$, debido a su mayor prioridad sobre las demás operaciones.

Se evalúa la multiplicación: $4 \times 2 = 8$, al encontrarse más a la izquierda que la división (asociatividad por la izquierda).

Se evalúa la división: $8 / 16 = 0.5$, al tener mayor precedencia que la suma.

Se efectúa la suma final: $3 + 0.5 = 3.5$.

Por el contrario, la notación posfija (o Notación Polaca Inversa, RPN) elimina completamente la ambigüedad y la necesidad de paréntesis al posicionar el operador inmediatamente después de sus dos operandos ($5 \text{ } 3 \text{ } 7 \text{ } * \text{ } +$).

Escriba un programa en lenguaje C que, usando pilas, colas y arrays, reciba como entrada una cadena de caracteres de máximo 200 caracteres de longitud que representa una expresión matemática válida en notación infija. El programa debe transformar dicha expresión a notación posfija (respetando la precedencia y asociatividad de los operadores) e imprimir tanto la cadena en notación posfija como el resultado numérico de su evaluación.

Especificaciones y Restricciones:

Operadores soportados: Suma (+), Resta (-), Multiplicación (*), División (/), Potencia (^) y delimitadores ( y ).

Precedencia: $\text{Potencia } (\text{asoc. derecha}) > \text{Multiplicación/División } (\text{asoc. izquierda}) > \text{Suma/Resta } (\text{asoc. izquierda})$.

Entrada: Todos los elementos de la cadena (números enteros, operadores y paréntesis) están garantizados de estar separados por exactamente un espacio en blanco.

Salida: La primera línea debe contener la expresión posfija equivalente (con elementos separados por un espacio). La segunda línea debe mostrar el resultado numérico evaluado.

Ejemplo 1:
Input:
3 + 4 * 2 / ( 1 - 5 ) ^ 2
Output:
3 4 2 * 1 5 - 2 ^ / +
3.5

Ejemplo 2:
Input:
5 + ( 3 * 7 )
Output:
5 3 7 * +
26
