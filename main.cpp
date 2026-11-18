#include <iostream>
#include <cmath>
#include <iomanip>

constexpr double PI = 3.14159265358979323846;

struct Vector3 {
    double x, y, z;
};

struct Estado {
    Vector3 posicion;
    Vector3 velocidadBody;
    Vector3 velocidadWorld;
    double yaw;
    double velocidadAngularZ;
};

// Convierte grados a radianes
double radianes(double grados) {
    return grados * PI / 180.0;
}

// Transforma velocidad Body a World
Vector3 transformarVelocidad(Vector3 v, double yaw) {
    double c = std::cos(yaw);
    double s = std::sin(yaw);

    Vector3 resultado;

    resultado.x = c * v.x - s * v.y;
    resultado.y = s * v.x + c * v.y;
    resultado.z = v.z;

    return resultado;
}

// Actualiza el estado del vehiculo
void actualizarEstado(Estado& estado, double dt) {

    // 1. Transformar velocidad al sistema global
    estado.velocidadWorld =
        transformarVelocidad(estado.velocidadBody, estado.yaw);

    // 2. Actualizar posicion global
    estado.posicion.x += estado.velocidadWorld.x * dt;
    estado.posicion.y += estado.velocidadWorld.y * dt;
    estado.posicion.z += estado.velocidadWorld.z * dt;

    // 3. Actualizar orientacion
    estado.yaw += estado.velocidadAngularZ * dt;
}

int main() {

    std::cout << std::fixed << std::setprecision(4);

    // Condiciones iniciales
    Estado vehiculo;

    vehiculo.posicion = {0.0, 0.0, 0.0};

    vehiculo.velocidadBody = {5.0, 2.0, 0.0};

    vehiculo.velocidadWorld = {0.0, 0.0, 0.0};

    vehiculo.yaw = radianes(0.0);

    vehiculo.velocidadAngularZ = 0.2;

    // Parametros temporales
    double dt = 0.1;
    double tiempoTotal = 10.0;

    int pasos = static_cast<int>(tiempoTotal / dt);

    std::cout << "SIMULACION CINEMATICA 2D/3D\n";
    std::cout << "---------------------------\n";

    for (int i = 0; i < pasos; i++) {

        double tiempo = i * dt;

        actualizarEstado(vehiculo, dt);

        // Mostrar cada segundo
        if ((i + 1) % 10 == 0) {

            std::cout << "\nTiempo: " << tiempo + dt << " s\n";

            std::cout << "Posicion World: ("
                      << vehiculo.posicion.x << ", "
                      << vehiculo.posicion.y << ", "
                      << vehiculo.posicion.z << ") m\n";

            std::cout << "Velocidad World: ("
                      << vehiculo.velocidadWorld.x << ", "
                      << vehiculo.velocidadWorld.y << ", "
                      << vehiculo.velocidadWorld.z << ") m/s\n";

            std::cout << "Yaw: "
                      << vehiculo.yaw * 180.0 / PI
                      << " grados\n";
        }
    }

    std::cout << "\nSIMULACION FINALIZADA\n";

    return 0;
}
