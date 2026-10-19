# Listas y conjuntos

## 1. Conjuntos

Un **conjunto** es una colección de elementos.

Características:

- El origen de los elementos no es relevante.
    
- **No permite elementos repetidos.**
    
- El orden de los elementos no es importante.
    

Por ejemplo:

$$  
A = {1,2,3} = {3,1,2}  
$$

pero:

$$  
{1,2,2,3} = {1,2,3}  
$$

porque los elementos repetidos no se consideran diferentes dentro de un conjunto.

---

## 2. Listas

Una **lista** es una **secuencia finita de cero o más elementos**.

A diferencia de un conjunto:

- El **orden** de los elementos importa.
    
- Puede haber **elementos repetidos**.
    
- Puede contener elementos del mismo tipo o de tipos diferentes.
    

### Tipos de listas

Una lista puede ser:

1. **Homogénea**: todos sus elementos son del mismo tipo.
    
2. **Heterogénea**: sus elementos no necesariamente son del mismo tipo.
    

Si $T$ es el tipo de datos de los elementos de una lista, decimos que es una **lista de tipo $T$**.

Por ejemplo:

$$  
L = [1,4,7,2]  
$$

es una lista de enteros.

Mientras que:

$$  
L = [1,\text{"hola"},3.14]  
$$

es una lista heterogénea.

---

## 3. Longitud de una lista

La **longitud** de una lista es la cantidad de elementos que contiene.

Si:

$$  
L=(a_1,a_2,\ldots,a_n)  
$$

entonces:

$$  
\operatorname{longitud}(L)=n  
$$

> **Importante:** en una lista se habla de **ocurrencias** de elementos, porque un mismo valor puede aparecer varias veces.

Por ejemplo:

$$  
L=[a,b,a,c]  
$$

tiene longitud:

$$  
\operatorname{longitud}(L)=4  
$$

aunque solo tenga tres valores distintos: $a,b,c$.

---

# 4. Producto cartesiano

El producto cartesiano entre dos conjuntos $H$ y $M$ se define como:

$$  

H \times M

{(a,b)\mid a\in H \land b\in M}  
$$

Es decir, es el conjunto de todos los pares ordenados $(a,b)$ donde:

$$  
a\in H  
$$

y

$$  
b\in M  
$$

---

# 5. Lista vacía

La **lista vacía** es una lista que no contiene elementos.

Se representa normalmente como:

$$  
[]  
$$

y su longitud es:

$$  
\operatorname{longitud}([])=0  
$$

---

# 6. Sublista

Sea una lista:

$$  
L=(a_1,a_2,\ldots,a_n)  
$$

Una **sublista** de $L$ es una lista formada por elementos consecutivos de $L$, desde una posición $i$ hasta una posición $j$, donde:

$$  
1\leq i\leq j\leq n  
$$

Por tanto:

$$  
(a_i,a_{i+1},\ldots,a_j)  
$$

es una sublista de $L$.

### Ejemplo

Para:

$$  
L=(a,b,a,b)  
$$

algunas sublistas son:

$$  
(a)  
$$

$$  
(b)  
$$

$$  
(a,b)  
$$

$$  
(b,a)  
$$

$$  
(a,b,a)  
$$

$$  
(b,a,b)  
$$

$$  
(a,b,a,b)  
$$

La lista vacía también puede considerarse sublista dependiendo de la definición adoptada por el curso.

> **Idea clave:** una sublista conserva el orden y, además, sus elementos deben ser **consecutivos** en la lista original.

---

# 7. La lista como estructura recursiva

Una lista es una **estructura recursiva** porque puede definirse utilizando una lista más pequeña.

Por ejemplo:

$$  
L = [a,L']  
$$

donde $L'$ es otra lista.

De manera conceptual:

$$  
\text{Lista}=  
\begin{cases}  
[] & \text{lista vacía}\  
[\text{primer elemento}+\text{otra lista}] & \text{lista no vacía}  
\end{cases}  
$$

Esto permite trabajar con listas mediante operaciones recursivas.

---

# 8. Subsecuencia

Una **subsecuencia** de una lista $L$ es una lista formada por **cero o más elementos de $L$**, sin necesidad de que sean consecutivos.

Sin embargo, los elementos seleccionados deben **conservar el orden relativo** que tenían en la lista original.

### Ejemplo

Sea:

$$  
L=(a,b,a,b)  
$$

Algunas subsecuencias son:

$$  
(a,b)  
$$

$$  
(b,b)  
$$

$$  
(b)  
$$

$$  
(a,b,b)  
$$

También:

$$  
[]  
$$

es una subsecuencia.

### Diferencia entre sublista y subsecuencia

|Concepto|¿Mantiene el orden?|¿Los elementos deben ser consecutivos?|
|---|---|---|
|**Sublista**|Sí|Sí|
|**Subsecuencia**|Sí|No|

Por ejemplo, para:

$$  
L=(a,b,a,b)  
$$

$$  
(a,b)  
$$

es tanto sublista como subsecuencia.

En cambio:

$$  
(a,b,b)  
$$

puede ser subsecuencia, pero **no** es sublista porque los tres elementos no aparecen de forma consecutiva.

---

# 9. Prefijo

Toda sublista que **comienza con el primer elemento** de la lista es un **prefijo**.

Para:

$$  
L=(a,b,a,b)  
$$

sus prefijos son:

$$  
[]  
$$

$$  
(a)  
$$

$$  
(a,b)  
$$

$$  
(a,b,a)  
$$

$$  
(a,b,a,b)  
$$

En los apuntes aparece:

> "Toda sublista de una lista que inicia en ~~ead~~ es un prefijo"

La palabra que falta casi con seguridad es **head**, término utilizado en estructuras de listas para referirse al **primer elemento**.

Por tanto:

> Toda sublista que inicia en el **primer elemento (head)** de la lista es un prefijo.

---

# 10. Sufijo

Toda sublista que **termina en el último elemento** de la lista es un **sufijo**.

Para:

$$  
L=(a,b,a,b)  
$$

sus sufijos son:

$$  
[]  
$$

$$  
(b)  
$$

$$  
(a,b)  
$$

$$  
(b,a,b)  
$$

$$  
(a,b,a,b)  
$$

En tus apuntes aparece:

> "toda sublista de una lista que finaliza en el ultimo elemento de la lista o un sofijo de la lisya"

La corrección sería:

> Toda sublista que finaliza en el **último elemento** de la lista es un **sufijo**.

---

# 11. Pertenencia e inclusión

### Pertenencia

Para expresar que un elemento $x$ pertenece a un conjunto $A$:

$$  
x\in A  
$$

Para expresar que no pertenece:

$$  
x\notin A  
$$

---

### Inclusión entre conjuntos

Si todos los elementos de $A$ pertenecen también a $B$, entonces:

$$  
A\subseteq B  
$$

Formalmente:

$$  
A\subseteq B  
\iff  
\forall x,(x\in A\rightarrow x\in B)  
$$

Es decir:

> Para todo $x$, si $x$ pertenece a $A$, entonces $x$ pertenece a $B$.

Tus apuntes parecen decir:

> "A pertenece a B"

pero aquí **no corresponde usar $\in$**. Para conjuntos se utiliza:

$$  
A\subseteq B  
$$

porque $A$ es un **conjunto contenido en** $B$, no un elemento de $B$ necesariamente.

---

# 12. Ocurre y precede

En el contexto de listas, estos términos describen la posición relativa de los elementos.

### Ocurrencia

Un elemento **ocurre** en una lista cuando aparece dentro de ella.

Por ejemplo:

$$  
L=(a,b,a,c)  
$$

El elemento $a$ ocurre **dos veces** en $L$.

---

### Precede

Decimos que un elemento **precede** a otro cuando aparece antes que él en la lista.

En:

$$  
L=(a,b,c,d)  
$$

$a$ precede a $c$.

También:

$$  
b  
$$

precede a:

$$  
d  
$$

---

# 13. Operaciones sobre listas

Entre las operaciones básicas mencionadas en clase están:

### Longitud

Obtiene la cantidad de elementos de una lista.

$$  
\operatorname{longitud}(L)  
$$

---

### Buscar

Busca un elemento $x$ dentro de una lista $L$.

$$  
\operatorname{buscar}(x,L)  
$$

La especificación exacta depende del lenguaje o modelo utilizado en clase.

---

### Insertar

Permite insertar un elemento $x$ en una lista $L$.

$$  
\operatorname{insertar}(x,L)  
$$

Aquí aparece una pregunta importante de tus apuntes:

> **¿Dónde?**

Porque insertar un elemento requiere determinar su posición.

Por ejemplo:

$$  
L=[a,b,c]  
$$

insertar $x$ al comienzo:

$$  
[x,a,b,c]  
$$

insertarlo al final:

$$  
[a,b,c,x]  
$$

---

### Eliminar

Elimina una ocurrencia de un elemento $x$ de una lista $L$.

$$  
\operatorname{eliminar}(x,L)  
$$

La pregunta de los apuntes:

> **¿Cuál?**

es relevante porque, si el elemento aparece varias veces, hay que especificar **qué ocurrencia** se elimina.

Por ejemplo:

$$  
L=[a,b,a,c]  
$$

eliminar $a$ podría producir:

$$  
[b,a,c]  
$$

o:

$$  
[a,b,c]  
$$

dependiendo de cuál ocurrencia se elimine.

---

### `first(L)`

Obtiene el **primer elemento** de una lista no vacía.

Por ejemplo:

$$  
\operatorname{first}([a,b,c])=a  
$$

Para la lista vacía, esta operación normalmente no está definida.

---

### `Retrieve(i,L)`

`Retrieve(i,L)` parece referirse a una operación para **obtener/recuperar el elemento que ocupa la posición $i$ de la lista $L$**.

Por ejemplo:

$$  
L=[a,b,c,d]  
$$

entonces:

$$  
\operatorname{Retrieve}(2,L)=b  
$$

La forma exacta de la operación (`Retrieve`, `retrieve`, índice, etc.) depende de la notación utilizada por tu profesor.

---

# Resumen conceptual

```text
CONJUNTO
│
├── No permite repetidos
├── El orden no importa
└── Se representa con { }

LISTA
│
├── Secuencia finita de 0 o más elementos
├── Puede tener repetidos
├── El orden importa
├── Puede ser homogénea o heterogénea
└── Se representa normalmente con [ ]

LISTA
│
├── Sublista
│   └── Elementos consecutivos
│
├── Subsecuencia
│   └── Elementos no necesariamente consecutivos
│
├── Prefijo
│   └── Comienza en el primer elemento
│
└── Sufijo
    └── Termina en el último elemento
```

### Relaciones importantes

$$  
\boxed{\text{Toda sublista es una subsecuencia}}  
$$

pero:

$$  
\boxed{\text{No toda subsecuencia es una sublista}}  
$$

Y:

$$  
\boxed{A\subseteq B \neq A\in B}  
$$

Estas dos últimas distinciones son especialmente importantes para no mezclar **sublista/subsecuencia** y **pertenencia/inclusión**.

También corregiría una cosa de tus apuntes originales: decir que la longitud es "la cantidad de ocurrencias de elementos" puede ser confuso. La definición más limpia es **la cantidad total de posiciones/elementos de la lista**, contando repetidos.