# --- Imágenes ---
docker build -t nombre:tag .                   # Construir imagen
docker images                                  # Listar imágenes
docker rmi nombre:tag                          # Eliminar imagen
docker pull ubuntu:22.04                       # Descargar imagen de Docker Hub
# --- Contenedores ---
docker run -d -p HOST:CT --name n imagen:tag   # Crear y correr
docker ps                                      # Contenedores activos
docker ps -a                                   # Todos (incluso detenidos)
docker stop nombre                             # Detener (SIGTERM)
docker kill nombre                             # Forzar detención (SIGKILL)
docker rm nombre                               # Eliminar contenedor
docker logs nombre                             # Ver logs
docker exec -it nombre bash                    # Shell interactivo
# --- Sistema ---
docker system df                               # Uso de disco
docker system prune -f                         # Limpiar recursos sin uso
