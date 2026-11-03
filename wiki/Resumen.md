# Resumen

## Síntesis

Se construyó un gemelo digital de un sistema de monitoreo de nivel de un tanque. El **activo físico** es un montaje con un dipswitch de tres posiciones (sensores simulados B1, B2, B3) y cinco LEDs (estados del nivel). El **activo virtual** es un programa en lenguaje Ladder con su HMI en CODESYS. Ambos se conectan mediante un **ESP32** que actúa como servidor **Modbus TCP**. El ESP32 además publica el estado del tanque a la plataforma IoT **Ubidots** por MQTT y recibe comandos desde ella para modificar el estado.

| Requisito del reto | Cómo se cumplió | Sección |
|---|---|---|
| Activo físico (montaje RA 2.3) | Dipswitch + 5 LEDs sobre ESP32 | [Hardware Real](Hardware-Real) |
| Activo virtual (HMI CODESYS RA 2.3) | Ladder y visualización en CODESYS | [Implementación en CODESYS](Implementacion-en-CODESYS) |
| Conexión por Modbus con ESP32 | CODESYS (cliente) a ESP32 (servidor), TCP 502 | [Comunicación Modbus](Comunicacion-Modbus) |
| Publicar estado a plataforma IoT | MQTT a Ubidots, 8 variables | [Integración con Ubidots](Integracion-con-Ubidots) |
| Modificar estado desde la plataforma | 3 variables de comando, combinadas con el dip (OR) | [Integración con Ubidots](Integracion-con-Ubidots) |

---

Anterior: [Home](Home) | Siguiente: [Diseño del Sistema](Diseno-del-Sistema)
