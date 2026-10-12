# app.py
from http.server import HTTPServer, BaseHTTPRequestHandler

MENSAJE = "Hola desde Docker — versión actualizada 2.0"


class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/plain; charset=utf-8")
        self.end_headers()
        self.wfile.write(MENSAJE.encode())

    def log_message(self, format, *args):
        print(f"[Solicitud] {args[0]} {args[1]}")


if __name__ == "__main__":
    print(f"Servidor iniciado en puerto 8080")
    print(f"Mensaje: {MENSAJE}")
    HTTPServer(("", 8080), Handler).serve_forever()
