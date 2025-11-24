#ifndef CONFIG_H
#define CONFIG_H

//
// ==== MOTOR DRIVER PINS (REAL WIRING) ====
//

// Motores A (lado izquierdo o derecho, según tu montaje)
#define PIN_ENA  14   // D14 → ENA
#define PIN_IN1  26   // D26 → IN1
#define PIN_IN2  27   // D27 → IN2

// Motores B (el otro lado)
#define PIN_ENB  32   // D12 → ENB
#define PIN_IN3  25   // D25 → IN3
#define PIN_IN4  33   // D33 → IN4

// PWM channels
#define PWM_CH_A 0
#define PWM_CH_B 1
#define PWM_FREQ 1000
#define PWM_RES  8

//
// ==== SENSOR ====
//
#define USE_MOCK_SENSOR 1
// #define PIN_TRIG 5
// #define PIN_ECHO 34

//
// ==== WiFi ====
//
#define WIFI_SSID      "NCOCLO25"
#define WIFI_PASSWORD  "N1c0145#01"

//
// ==== MQTT ====
//
#define MQTT_HOST      "test.mosquitto.org"
#define MQTT_PORT      8883
#define MQTT_USER      nullptr
#define MQTT_PASS      nullptr
#define MQTT_CLIENTID_PREFIX "esp32-car-"

#define MQTT_TOPIC_MOVEMENT   "esp32car/commands"
#define MQTT_TOPIC_DISTANCE   "esp32car/telemetry/distance"

#define TELEMETRY_PERIOD_MS 1000
#define MQTT_RETRY_MS 2000
#define WIFI_RETRY_MS 500

//
// ==== API REST ROUTES ====
//
#define API_HEALTH_PATH "/api/v1/healthcheck"
#define API_MOVE_PATH   "/api/v1/move"

#endif
