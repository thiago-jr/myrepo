import grpc

import greeter_pb2
import greeter_pb2_grpc


def run():
    # 'with' cierra el canal automáticamente al terminar
    with grpc.insecure_channel("localhost:50051") as channel:
        stub = greeter_pb2_grpc.GreeterStub(channel)

        # --- Llamada unaria ---
        response = stub.SayHello(greeter_pb2.HelloRequest(name="Estudiante", age=20))
        print("Respuesta unaria:", response.message)

        response2 = stub.SayGoodBye(greeter_pb2.HelloRequest(name="Estudiante", age=20))
        print("Respuesta unaria:", response2.message)

        # --- Llamada con streaming del servidor ---
        print("Respuestas en streaming:")
        for response in stub.SayHelloRepeatedly(
            greeter_pb2.HelloRequest(name="Estudiante", age=20)
        ):
            print(" ->", response.message)

        print("Respuestas en streaming:")
        for response in stub.sayGoodbyeRepeatedly(
            greeter_pb2.HelloRequest(name="Estudiante", age=20)
        ):
            print(" ->", response.message)


if __name__ == "__main__":
    run()
