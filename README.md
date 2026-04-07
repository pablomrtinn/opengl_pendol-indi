# Ejercicio Bloque 1 - Gráficos Interactivos (Qt + OpenGL)

Este proyecto es una aplicación desarrollada en **C++** utilizando el framework **Qt** y la biblioteca **OpenGL**. El objetivo principal es la creación de una escena interactiva con elementos gráficos (como una estrella, soportes y bolas) que pueden controlarse manualmente mediante el teclado o automáticamente mediante un temporizador.

## 🚀 Características

- **Visualización 3D/2D con OpenGL**: Renderizado de geometrías personalizadas mediante VAOs y VBOs.
- **Interfaz de Usuario (Qt)**: Formulario diseñado con Qt Designer (`MyForm.ui`) que permite interactuar con la simulación.
- **Modos de Funcionamiento**:
    - **Modo Manual**: El usuario controla la escena o el tiempo mediante el teclado
    - **Modo Automático**: La escena se actualiza dinámicamente utilizando un `QTimer` que lanza un evento cada 50ms.
- **Sincronización**: Uso de *Signals & Slots* de Qt para actualizar elementos de la interfaz (como un `doubleSpinBox`) en tiempo real según el tiempo de la simulación.

## Controles del Modo Manual
- Tecla A: Disminuye el ángulo del primer soporte (angleMan1) en 5 grados, con un límite mínimo de -40°.
- Tecla D: Aumenta el ángulo del primer soporte (angleMan1) en 5 grados, con un límite máximo de 0°.
- Tecla Q: Disminuye el ángulo del segundo soporte (angleMan2) en 5 grados, con un límite mínimo de 0°.
- Tecla E: Aumenta el ángulo del segundo soporte (angleMan2) en 5 grados, con un límite máximo de 40°.

## 📂 Estructura del Proyecto

- `main.cpp`: Punto de entrada de la aplicación. Configura el perfil de OpenGL (3.3 Core Profile).
- `MyForm.h/cpp/ui`: Define la ventana principal y la lógica de los controles de la interfaz (botones de Play/Pause, selección de modo).
- `MyGLWidget.h/cpp`: Contiene el núcleo gráfico. Gestión de shaders, creación de buffers para los objetos (Estrella, Terra, Suport, Bola) y la lógica de renderizado en `paintGL`.
- `shaders/`: (No incluidos en la lista pero referenciados) Contienen el `basicShader.vert` y `basicShader.frag`.

## 🛠️ Requisitos

- **Qt 5 / Qt 6** (con el módulo `widgets`)
- **OpenGL 3.3** o superior.
- **GLM**: Biblioteca de matemáticas para OpenGL.
- **Compilador C++**: Compatible con C++11 o superior.

## 🔧 Instalación y Compilación

1. Abre el archivo de proyecto `Exercici_bloc1.pro` en **Qt Creator**.
2. Configura el kit de compilación (Desktop).
3. Asegúrate de tener las librerías de OpenGL instaladas en tu sistema.
4. Compila y ejecuta (Ctrl + R).

## 🎮 Controles

- **Botón Play/Pause**: Activa o desactiva la animación en modo automático.
- **RadioButtons**: Cambian entre el control manual y el movimiento automático por tiempo.
- **Teclado**: (Configurado en `keyPressEvent` dentro de `MyGLWidget.cpp`) permite interactuar con la escena cuando el foco está en el widget de OpenGL.

## 📝 Notas del Desarrollador
El proyecto utiliza una estructura clásica de la asignatura:
- Se generan VAOs específicos para cada objeto (`VAO_ESTRELLA`, `VAO_TERRA`, etc.).
- La comunicación entre la lógica de tiempo y la interfaz se realiza mediante el slot `timeUpdate(double)`.

---

*Este proyecto ha sido desarrollado como parte de las prácticas de desarrollo de Interacción y diseño de interfícies (INDI) en la universidad UPC en el campus EPSEVG*
