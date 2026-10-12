import time
from concurrent import futures

import grpc

import greeter_pb2
import greeter_pb2_grpc


class GreeterServicer(greeter_pb2_grpc.GreeterServicer):
    def SayHello(self, request, context):
        # 'request' es una instancia de HelloRequest
        # Debemos devolver una instancia de HelloReply
        return greeter_pb2.HelloReply(
            message=f"Hola, {request.name}, tienes {request.age}!"
        )

    def SayHelloRepeatedly(self, request, context):
        # Método de streaming: usamos 'yield' en vez de 'return'
        for i in range(5):
            yield greeter_pb2.HelloReply(
                message=f"Hola #{i + 1}, {request.name}, tienes {request.age}!"
            )
            time.sleep(1)

    def SayGoodBye(self, request, context):
        return greeter_pb2.HelloReply(
            message=f"Adiós, {request.name}, con {request.age} años!"
        )

    def sayGoodbyeRepeatedly(self, request, context):
        for i in range(5):
            yield greeter_pb2.HelloReply(
                message=f"Adiós #{i + 1}, {request.name}, con {request.age}!"
            )
            time.sleep(1)


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    greeter_pb2_grpc.add_GreeterServicer_to_server(GreeterServicer(), server)
    server.add_insecure_port("[::]:50051")
    server.start()
    print("Servidor gRPC escuchando en el puerto 50051...")
    server.wait_for_termination()


if __name__ == "__main__":
    serve()
