"""2. Escriba un script en Python que reciba como entrada una cadena de 
caracteres y de como salida la frecuencia de los caracteres en la cadena 
en el orden de aparición en la cadena de entrada. Solo se permite recorrer 
la cadena una vez"""

cadena = input("Escriba una cadena: ")

letras = []   # las letras que ya vi, en orden
veces = []    # cuantas veces salio cada una

for letra in cadena:
    if letra in letras:
        # busco en que posicion esta y le sumo 1 a su contador
        i = letras.index(letra)
        veces[i] = veces[i] + 1
    else:
        letras.append(letra)   # letra nueva
        veces.append(1)

for i in range(len(letras)):
    print(letras[i], "->", veces[i])