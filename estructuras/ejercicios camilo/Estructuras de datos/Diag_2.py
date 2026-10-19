#cantidad de casos de prueba
n_casos = int(input())

#Ciclo
for _ in range(n_casos):
    numero_actual = int(input())
    intentos = 0
    encontrado = False
    
    #El bucle del proces
    while intentos < 100:
        
        num_str = str(numero_actual)
    
        num_invertido = int(num_str[::-1])

        suma = numero_actual + num_invertido

        intentos += 1

        suma_str = str(suma)
        if suma_str == suma_str[::-1]:
            print(suma)
            encontrado = True
            break
        
        numero_actual = suma
        
    if not encontrado:
        print("imposible")