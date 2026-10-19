def calcular_sublistas(texto):
    sublistas = set() # Eliminamos duplicados usando un conjunto
    sublistas.add("") # Caso de cero elementos
    
    # Cortes consecutivos
    for i in range(len(texto)):
        for j in range(i + 1, len(texto) + 1): 
            sub = texto[i:j] 
            #no haya letras repetidas
            if len(set(sub)) == len(sub):
                sublistas.add(sub)
                
    return sorted(list(sublistas))

def calcular_subsecuencias(texto):
    # Si el texto está vacío, devuelve un conjunto con la cadena vacía
    if not texto:
        return {""}
    
    # Primer caracter y el resto del texto
    primer_char = texto[0]
    resto_texto = texto[1:]
    
    # Llamada recursiva para obtener las subsecuencias del resto del texto
    subsecuencias_restantes = calcular_subsecuencias(resto_texto)
    
    resultado = set()
    for sub in subsecuencias_restantes:
        #No incluimos el primer caracter
        resultado.add(sub)
        
        #Incluimos el primer caracter al inicio
        nueva_sub = primer_char + sub
        #No haya elementos repetidos
        if len(set(nueva_sub)) == len(nueva_sub):
            resultado.add(nueva_sub)
            
    return resultado

# --- PROGRAMA PRINCIPAL ---
cadena = input("string: ")

print("\nSUBLISTAS (Consecutivas)")
for sl in calcular_sublistas(cadena):
    print(f"['{sl}']" if sl != "" else "[]")

print("\nSUBSECUENCIAS (No necesariamente consecutivas)")
#Alfabéticamente el resultado de la recursión antes de imprimir
for ss in sorted(list(calcular_subsecuencias(cadena))):
    print(f"['{ss}']" if ss != "" else "[]")