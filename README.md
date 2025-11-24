# **README – Proyecto Final ESP32 Smart Car (HTTP API + MQTT + GUI Web)**

Nicolas Clavijo 0000314037

Juan Pablo Parrado 0000291023

Presentación sustentación:https://www.canva.com/design/DAG5kdYOyQo/iVhoR91BbWGHavvMLzh9mg/edit?utm_content=DAG5kdYOyQo&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton

## Proyecto: Carro 2WD controlado por ESP32

Incluye:

* Control por **API REST**
* Telemetría por **MQTT TLS**
* **Interfaz Web (GUI)** moderna
* Manejo de motores con **L298N**
* Sensor ultrasónico (o **mock** si no está conectado)
* Código modular (`settings.h`, `sensor.cpp`, `root_ca.h`)

---

# **1. Estructura del proyecto**

```
/src
 ├── main.cpp
 ├── settings.h
 ├── sensor.h
 ├── sensor.cpp
 ├── root_ca.h
/gui
 └── index.html
README.md
```

---

# **2. Requisitos**

## Hardware

* ESP32 DevKit V1
* Driver de motores **L298N**
* Carro 2WD con 2 motores DC
* Caja de **4 pilas AA en serie (6V)**
* Cables Dupont
* (Opcional) Sensor HC-SR04
  → El proyecto funciona también con modo *mock sensor*.

## 💻 Software

* Arduino IDE con ESP32 Boards
* Extensiones instaladas:

  * **Arduino ESP32 Core**
  * Biblioteca: PubSubClient
  * Biblioteca: WiFiClientSecure

---

# 🔌 **3. Conexiones**

## Motores — L298N → ESP32

### Motor A

```
ENA → GPIO14  
IN1 → GPIO26  
IN2 → GPIO27
```

### Motor B

```
ENB → GPIO32  
IN3 → GPIO25  
IN4 → GPIO33
```

## Alimentación

```
Caja 4 AA (+) → L298N +12V  
Caja 4 AA (–) → L298N GND  
ESP32 GND     → L298N GND   (OBLIGATORIO)
```

## Importante

* Tener puesto el jumper **5V-EN**
* NUNCA alimentar motores desde la ESP32
* 2 pilas AA NO funcionan → 4 AA mínimo (6V)

---

# **4. API REST disponible**

## Healthcheck

`GET /api/v1/healthcheck`

### Ejemplo de respuesta:

```json
{
  "status": "ok",
  "wifi_ip": "192.168.0.28",
  "mqtt_connected": true,
  "motion_active": false,
  "uptime_ms": 127382
}
```

---

## Mover el carro

`POST /api/v1/move`

### Parámetros:

| Parametro   | Tipo | Valores                              |
| ----------- | ---- | ------------------------------------ |
| direction   | str  | forward, backward, left, right, stop |
| speed       | int  | 0–100                                |
| duration_ms | int  | 0–5000                               |

### Ejemplo:

```bash
curl -X POST "http://<ip>/api/v1/move?direction=forward&speed=60&duration_ms=1500"
```

Respuesta:

```json
{
  "status": "accepted",
  "direction": "forward",
  "speed": 60,
  "duration_ms": 1500
}
```

---

# **5. MQTT Telemetría**

El ESP32 publica por MQTT TLS en:

### Movimiento aceptado:

`esp32car/commands`

### Telemetría del sensor:

`esp32car/telemetry/distance`

Ejemplo de payload de distancia:

```json
{
  "device": "esp32car",
  "type": "ultrasonic",
  "unit": "cm",
  "distance": 52.33,
  "ts": 1234567
}
```

---

# **6. Interfaz Web (GUI)**

La GUI `index.html` incluye:

* Botones de movimiento
* Lectura de telemetría MQTT
* Log en tiempo real
* Comunicación con API REST del ESP32
* Conexión automática vía WebSocket MQTT

### Configuración:

Editar en `index.html`:

```js
const ROBOT_IP = "192.168.0.28";
```

Luego abrir el archivo en el navegador.

---

# **7. Configuración del código**

En `settings.h`:

### Pines

```cpp
#define PIN_ENA 14
#define PIN_IN1 26
#define PIN_IN2 27

#define PIN_ENB 32
#define PIN_IN3 25
#define PIN_IN4 33
```

### Frecuencia PWM (IMPORTANTE)

```cpp
#define PWM_FREQ 1000   // 5kHz causaba pitidos en el L298N
#define PWM_RES  8
```

### WiFi / MQTT

```cpp
#define WIFI_SSID "NCOCLO25"
#define WIFI_PASSWORD "N1c0145#01"
#define MQTT_HOST "test.mosquitto.org"
#define MQTT_PORT 8883
```

### Sensor (mock)

```cpp
#define USE_MOCK_SENSOR 1
```

---

# **8. Cómo usar el sistema**

1. Alimentar el L298N con **4 AA** (6V).
2. Conectar ESP32 → PC.
3. Subir el código desde Arduino IDE.
4. Ver IP en el monitor serial.
5. Abrir `gui/index.html`.
6. Colocar la IP en el panel.
7. Probar botones de movimiento.

---

# 🔍 **9. Troubleshooting**

### El carro no se mueve, pero acepta comandos

* Revisa GND común entre ESP32 y L298N.
* Revisa conexión batería → +12V y GND.
* Verifica que el jumper 5V-EN esté puesto.

### El driver hace PITIDO

* Frecuencia PWM estaba demasiado alta → corregido a 1000Hz.

### Motores vibran pero no avanzan

* Las pilas AA están débiles.
* Usa pilas nuevas o recargables NiMH.

---
