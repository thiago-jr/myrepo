n_casos = int(input("Ingrese el número de casos: "))

for _ in range(n_casos):
    cadena = input("Ingrese la cadena: ").replace(" ", "")

    max_longitud = -1

    for i in range(len(cadena)):
        char_actual = cadena[i]

        # Flecha a la izquierda
        if char_actual == '<':
            longitud_actual = 1
            j = i + 1

            if j < len(cadena) and cadena[j] == '-':
                while j < len(cadena) and cadena[j] == '-':
                    longitud_actual += 1
                    j += 1

            elif j < len(cadena) and cadena[j] == '=':
                while j < len(cadena) and cadena[j] == '=':
                    longitud_actual += 1
                    j += 1

            max_longitud = max(max_longitud, longitud_actual)

        # Flecha a la derecha
        elif char_actual == '>':
            longitud_actual = 1
            j = i - 1

            if j >= 0 and cadena[j] == '-':
                while j >= 0 and cadena[j] == '-':
                    longitud_actual += 1
                    j -= 1

            elif j >= 0 and cadena[j] == '=':
                while j >= 0 and cadena[j] == '=':
                    longitud_actual += 1
                    j -= 1

            max_longitud = max(max_longitud, longitud_actual)

    print(max_longitud)