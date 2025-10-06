# 🧭 ACM_microondas_keypad_LCD_Kevin_Gonzalez  
### Arquitectura de Computadoras y Microcontroladores — HT Microondas  

---

## 📘 Descripción General

Este proyecto implementa el **control completo de un microondas digital**, utilizando un **microcontrolador STM32 Nucleo-64 (modelo L053R8)**.  
El sistema incluye una interfaz **LCD 16x2**, un **keypad matricial 4x4**, un **display de 7 segmentos de 4 dígitos**, y un **motor paso a paso** controlado mediante un **driver ULN2003**.

---

## ⚙️ Funcionalidad del Sistema

El sistema simula un microondas funcional con las siguientes características:

- **Selección de platillos predefinidos** mediante el keypad (`A`, `B`, `C`, `D`):
  - 🍿 `A` → Poporopos 2:30  
  - 🍕 `B` → Pizza 1:30  
  - 🥩 `C` → Carne 10:00  
  - 🥦 `D` → Verduras 3:00  

- **Definición manual del tiempo** ingresando los números (`0–9`).

- **Inicio del ciclo de cocción** con `*`  
  (inicia el conteo regresivo y activa el motor del plato).

- **Cancelación del proceso** con `#`.

- **Display de 7 segmentos** muestra el tiempo restante o la hora del reloj interno.

- **Motor stepper** simula el movimiento del plato giratorio durante la cocción.

- **LCD 16x2** muestra el estado del microondas:
  - “Poporopos 2:30”, “Calentando...”, “Listo!”, etc.

---

## 🧩 Componentes Utilizados

| Componente | Función |
|-------------|----------|
| 🧠 **STM32 Nucleo-64 L053R8** | Controlador principal (MMIO, GPIO directos) |
| ⚙️ **Driver ULN2003** | Control de bobinas del motor stepper |
| 🔁 **Motor paso a paso 28BYJ-48** | Simula el giro del plato del microondas |
| 🔢 **Display 7 segmentos 4 dígitos (cátodo común)** | Muestra tiempo y reloj |
| 💡 **Transistores 2N2222A NPN** | Controlan los 4 dígitos del display |
| 📟 **LCD 16x2** | Interfaz de texto para usuario |
| 🎛️ **Keypad 4x4** | Entrada del usuario (tiempo, opciones, control) |
| 🔧 **Potenciómetro de 10 kΩ** | Ajuste del contraste de la LCD |
| ⚫ **Resistencias de 200 Ω** | Limitadoras para segmentos del display |

---

## 🔌 Distribución de Pines

### 🔷 STM32 Nucleo L053R8

#### ➤ Display de 7 Segmentos
| Segmento | Pin MCU | Descripción |
|-----------|----------|-------------|
| A | PA0 | Segmento A |
| B | PA1 | Segmento B |
| C | PA11 | Segmento C |
| D | PA12 | Segmento D |
| E | PA4 | Segmento E |
| F | PA5 | Segmento F |
| G | PA6 | Segmento G |
| Dígitos | PA7–PA10 | Control de dígitos 1–4 (a través de 2N2222A) |

#### ➤ LCD 16x2 (modo 4 bits)
| Señal | Pin MCU |
|--------|----------|
| D4–D7 | PB0–PB3 |
| E (Enable) | PB4 |
| RS (Register Select) | PB5 |
| RW | GND |
| VCC | +5V |
| VO (Contraste) | Potenciómetro 10 kΩ |
| A/K (Backlight) | +5V / GND |

#### ➤ Motor Stepper (Driver ULN2003)
| Bobina | Pin MCU | Descripción |
|---------|----------|-------------|
| IN1 | PB6 | Bobina 1 |
| IN2 | PB7 | Bobina 2 |
| IN3 | PB8 | Bobina 3 |
| IN4 | PB9 | Bobina 4 |
| +12V | Alimentación del motor |
| GND común | Compartido con el STM32 |

#### ➤ Keypad 4x4
| Línea | Pin MCU | Tipo |
|--------|----------|------|
| C0–C3 | PC0–PC3 | Columnas (salida) |
| R0–R3 | PC4–PC7 | Filas (entrada con pull-up) |

---

## 🕹️ Flujo del Programa

flowchart TD
A[Inicio] --> B[Mostrar reloj en display y "00:00" en LCD]
B --> C[Lectura del keypad]
C -->|A-D| D[Platillo predefinido]
C -->|0–9| E[Entrada manual de tiempo]
C -->|*| F[Iniciar cocción]
C -->|#| G[Cancelar / Resetear]
F --> H[Activar motor y cuenta regresiva]
H --> I[Mostrar "Calentando..." en LCD]
I --> J[Tiempo llega a 0]
J --> K[Mostrar "Listo!" y detener motor]
G --> B
K --> B

---

### 🔬 Consideraciones Adicionales

Los displays de 7 segmentos son de cátodo común, controlados por transistores 2N2222A.

El motor paso a paso requiere fuente de 5V (la directa del nucleo funciona).

---

## 🎥 Videos de Demostración

🔹 Demostración del circuito físico:
👉 https://youtu.be/uB3EOVek4Uc

🔹 Explicación completa del código:
👉 https://youtu.be/YYYYYYYYYYY