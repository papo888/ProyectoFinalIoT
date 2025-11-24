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

Aquí tienes **dos párrafos bien escritos**, claros, profesionales y listos para poner en tu README, explicando:

✔ cómo usó el proyecto la memoria del ESP32
✔ qué librerías utilizaste y para qué

---

# Uso de la memoria en el ESP32

El sistema usa de manera eficiente la memoria del ESP32, distribuyendo la carga entre la **RAM**, la **Flash** y las estructuras dinámicas del programa. El uso intensivo de WiFi, MQTT con TLS y el servidor HTTP obliga a gestionar cuidadosamente la memoria dinámica, especialmente la del heap. Los certificados TLS (almacenados en `root_ca.h`) se cargan en **Flash**, lo cual evita saturar la RAM al mantenerlos permanentemente accesibles sin ocupar espacio durante la ejecución. Las estructuras relacionadas con el movimiento (como el estado del motor, telemetría y buffers MQTT) se mantienen en RAM en objetos pequeños y livianos. Además, se evita crear strings gigantes o procesamientos pesados en cada loop para no fragmentar la memoria y asegurar un funcionamiento estable incluso con telemetría continua cada segundo.

También se manejó la memoria del sensor mediante lectura directa y cálculo puntual —sin almacenar historiales extensos— y en la interfaz web se controló el tamaño máximo del historial de puntos del radar para evitar un crecimiento ilimitado que pudiera generar cuelgues o ralentizaciones. Gracias al uso de PWM por hardware, WiFiClientSecure y PubSubClient, la mayor parte del procesamiento crítico se delega a módulos muy optimizados del SDK del ESP32, permitiendo que el consumo de memoria se mantenga bajo, estable y sin sobrecargas incluso durante conexiones TLS cifradas o peticiones simultáneas por la API.

---

# Librerías utilizadas

Para el funcionamiento completo del sistema se utilizaron varias librerías clave del entorno Arduino para ESP32. **WiFi.h** permite gestionar la conexión a redes inalámbricas en modo estación. **WebServer.h** implementa el servidor REST que expone los endpoints `/api/v1/healthcheck` y `/api/v1/move`. Para la comunicación segura se usó **WiFiClientSecure**, responsable de manejar el cifrado TLS y validar certificados con el CA almacenado en `root_ca.h`. La comunicación MQTT se implementó con **PubSubClient**, encargada de publicar telemetría y recibir comandos, funcionando encima del cliente TLS para garantizar seguridad extremo a extremo. Para el control de motores se empleó la API nativa `ledcSetup()` y `ledcWrite()` del ESP32, la cual usa canales de PWM por hardware, más estables y precisos.

El sensor ultrasónico se controló con funciones propias (`sensor.h` y `sensor.cpp`), pero apoyándose en primitivas de **Arduino.h** para lectura digital y temporización. Finalmente, en la interfaz web se usaron tecnologías estándar: HTML, TailwindCSS, Canvas 2D para el radar y WebSockets MQTT sobre `wss://` desde el navegador para graficar telemetría en tiempo real. Todo el stack fue elegido para ser liviano, compatible y altamente eficiente en entornos embebidos y web.


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

# Problemas Encontrados

Durante el desarrollo e integración del sistema se presentaron varios retos técnicos que requirieron ajustes tanto en hardware como en software:

### **1. Motores no se movían aunque el backend aceptaba los comandos**

* La API REST retornaba `202 accepted`, pero el carro no avanzaba.
* Causa: los pines configurados no coincidían con el cableado real del L298N.
* Solución: corregir los pines en `settings.h` y verificar voltajes del driver.

---

### **2. Pitido del L298N y motores sin fuerza**

* El L298N emitía un *beep* sin girar los motores.
* Causa: insuficiente corriente → 2 pilas AA no entregan la corriente requerida.
* Solución: usar **4 pilas AA nuevas**, o preferiblemente un pack 18650.

---

### **3. Sensor ultrasónico devolvía valores incorrectos (≈ 20 cm siempre)**

* Incluso acercando la mano, no habían variaciones.
* Causas detectadas:

  * El ECHO del HC-SR04 estaba a 5V → ESP32 solo admite 3.3V.
  * El divisor resistivo estaba mal conectado o con valores incorrectos.
  * El sensor estaba inclinado o vibrando sobre el chasis.
* Solución: rehacer el divisor resistivo con valores correctos **10k + 15k**, asegurar conexiones y soldar cables flojos.
---

# Mejoras a Futuro

Varias mejoras pueden implementarse para aumentar robustez, escalabilidad y funcionalidad del proyecto:

### **1. Agregar comunicación segura en la API REST**

* Actualmente la API corre en HTTP sin cifrado.
* Se puede implementar:

  * mbedTLS con certificados locales.
  * o ESP32 + reverse proxy Nginx en otro dispositivo.

---

### **2. Implementar detección de obstáculos y frenado automático**

* El vehículo puede frenar automáticamente si:

  * `{distance < 20 cm}`
* Y enviar alertas por MQTT:

  * `"warning: collision imminent"`

---


