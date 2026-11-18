#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream> // Librería para guardar archivos

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

double radianes(double grados) {
    return grados * PI / 180.0;
}

Vector3 transformarVelocidad(Vector3 v, double yaw) {
    double c = std::cos(yaw);
    double s = std::sin(yaw);
    Vector3 resultado;
    resultado.x = c * v.x - s * v.y;
    resultado.y = s * v.x + c * v.y;
    resultado.z = v.z;
    return resultado;
}

void actualizarEstado(Estado& estado, double dt) {
    estado.velocidadWorld = transformarVelocidad(estado.velocidadBody, estado.yaw);
    estado.posicion.x += estado.velocidadWorld.x * dt;
    estado.posicion.y += estado.velocidadWorld.y * dt;
    estado.posicion.z += estado.velocidadWorld.z * dt;
    estado.yaw += estado.velocidadAngularZ * dt;
}

int main() {
    std::cout << std::fixed << std::setprecision(4);

    // Abrir archivo CSV para exportar datos
    std::ofstream archivo("trayectoria.csv");
    archivo << "tiempo,x,y,z,vx,vy,vz,yaw_deg\n";

    Estado vehiculo;
    vehiculo.posicion = {0.0, 0.0, 0.0};
    vehiculo.velocidadBody = {5.0, 2.0, 0.0};
    vehiculo.velocidadWorld = {0.0, 0.0, 0.0};
    vehiculo.yaw = radianes(0.0);
    vehiculo.velocidadAngularZ = 0.2;

    double dt = 0.1;
    double tiempoTotal = 10.0;
    int pasos = static_cast<int>(tiempoTotal / dt);

    // Guardar estado inicial (t = 0)
    archivo << 0.0 << "," << vehiculo.posicion.x << "," << vehiculo.posicion.y << ","
            << vehiculo.posicion.z << "," << vehiculo.velocidadWorld.x << ","
            << vehiculo.velocidadWorld.y << "," << vehiculo.velocidadWorld.z << ","
            << (vehiculo.yaw * 180.0 / PI) << "\n";

    std::cout << "SIMULACION CINEMATICA 2D/3D\n";
    std::cout << "---------------------------\n";

    for (int i = 0; i < pasos; i++) {
        double tiempo = (i + 1) * dt;
        actualizarEstado(vehiculo, dt);

        // Guardar cada paso en el CSV
        archivo << tiempo << "," << vehiculo.posicion.x << "," << vehiculo.posicion.y << ","
                << vehiculo.posicion.z << "," << vehiculo.velocidadWorld.x << ","
                << vehiculo.velocidadWorld.y << "," << vehiculo.velocidadWorld.z << ","
                << (vehiculo.yaw * 180.0 / PI) << "\n";

        if ((i + 1) % 10 == 0) {
            std::cout << "\nTiempo: " << tiempo << " s\n";
            std::cout << "Posicion World: (" << vehiculo.posicion.x << ", "
                      << vehiculo.posicion.y << ", " << vehiculo.posicion.z << ") m\n";
            std::cout << "Velocidad World: (" << vehiculo.velocidadWorld.x << ", "
                      << vehiculo.velocidadWorld.y << ", " << vehiculo.velocidadWorld.z << ") m/s\n";
            std::cout << "Yaw: " << vehiculo.yaw * 180.0 / PI << " grados\n";
        }
    }

    archivo.close();
    std::cout << "\nSIMULACION FINALIZADA. Datos guardados en trayectoria.csv\n";
    return 0;
}
