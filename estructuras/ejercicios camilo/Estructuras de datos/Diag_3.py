
def es_primo(num):
    if num < 2:
        return False
    for i in range(2, int(num**0.5) + 1):
        if num % i == 0:
            return False
    return True

n_casos = int(input())

for _ in range(n_casos):
    p = int(input())
    encontrado = False
    
    # (p1)
    p1 = 2
    while p1 <= p and not encontrado:
        if es_primo(p1):
            
            p2 = p1
            while p1 + p2 <= p and not encontrado:
                if es_primo(p2):
                    

                    p3 = p - p1 - p2
                    

                    if p3 >= p2 and es_primo(p3):
                        print(p1, p2, p3)
                        encontrado = True
                
                p2 += 1
        p1 += 1 