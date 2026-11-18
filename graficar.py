import subprocess
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# 1. Compilar y ejecutar el codigo C++ automaticamente
print("Compilando generadorcsv.cpp...")
subprocess.run(["g++", "-std=c++17", "-O2", "generadorcsv.cpp", "-o", "simulacion"], check=True)

print("Ejecutando simulacion en C++...")
ejecutable = "simulacion.exe" if os.name == "nt" else "./simulacion"
subprocess.run([ejecutable], check=True)

# 2. Leer los datos exactos generados por C++
df = pd.read_csv("trayectoria.csv")

# 3. Configurar el grafico profesional
plt.style.use("dark_background")
fig, ax = plt.subplots(figsize=(10, 6), dpi=150)

# Trazar trayectoria completa
ax.plot(df["x"], df["y"], color="#00d4ff", linewidth=2.5, label="Trayectoria del vehículo")

# Marcar puntos y flechas de orientacion (Yaw) cada 1 segundo (cada 10 pasos)
puntos_segundo = df.iloc[::10]
ax.scatter(puntos_segundo["x"], puntos_segundo["y"], color="white", zorder=5, s=40)

# Dibujar vectores de orientacion (Yaw)
yaw_rad = np.radians(puntos_segundo["yaw_deg"])
u = np.cos(yaw_rad)
v = np.sin(yaw_rad)
ax.quiver(puntos_segundo["x"], puntos_segundo["y"], u, v, color="#ff9f1c", scale=22, width=0.004, zorder=6, label="Orientación (Yaw)")

# Etiquetar tiempos clave
# Etiquetar tiempos clave
for _, fila in puntos_segundo.iterrows():
    t = int(round(fila["tiempo"]))
    if t in (0, 2, 4, 6, 8):
        ax.annotate(f"t={t}s", (fila["x"], fila["y"]), textcoords="offset points", xytext=(-10, 10), fontsize=9, color="#e0e0e0")


# Punto final exacto
final = df.iloc[-1]
ax.annotate(f"Final ({final['x']:.2f}, {final['y']:.2f}) m",
            (final["x"], final["y"]), textcoords="offset points", xytext=(-45, -18),
            fontsize=9, fontweight="bold", color="#00d4ff")

# Estetica del grafico
ax.set_title("Simulación Cinemática 2D/3D (Cálculo en C++ / Gráfico en Python)", fontsize=13, pad=15)
ax.set_xlabel("Posición X World (m)", fontsize=11)
ax.set_ylabel("Posición Y World (m)", fontsize=11)
ax.grid(True, linestyle="--", alpha=0.25)
ax.axis("equal")
ax.legend(loc="upper left")

plt.tight_layout()
plt.savefig("trayectoria.png")
print("Imagen 'trayectoria.png' generada con exito.")
plt.show()
