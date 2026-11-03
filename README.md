# ModeloDigitalTanque

Gemelo digital de un sistema de monitoreo del nivel de un tanque. Un montaje físico con un ESP32 (dipswitch como sensores simulados y cinco LEDs como indicadores) se conecta por **Modbus TCP** con un programa en Ladder y su HMI en **CODESYS**, y publica y recibe el estado del tanque desde la plataforma IoT **Ubidots** por MQTT.

## Contenido del repositorio

| Elemento | Descripción |
|---|---|
| `Tanque_CodeSys/` | Proyecto CODESYS (Ladder, variables globales, visualización) y su exportación PLCopen XML |
| `conexion_modbus.ino` | Firmware del ESP32: servidor Modbus TCP y cliente MQTT para Ubidots |
| `wiki/` | Documentación: diseño, implementación y validación del gemelo digital |

## Documentación

La documentación completa está en la wiki: https://github.com/10santiago12/ModeloDigitalTanque/wiki

## Cómo reproducirlo

1. Cargar `conexion_modbus.ino` en un ESP32 con las bibliotecas `modbus-esp8266` y `PubSubClient`, configurando el WiFi y el token de Ubidots en las constantes del inicio del archivo.
2. Abrir `Tanque_CodeSys/Ejercicio_Tanque.project` en CODESYS, fuera del modo Simulation, y configurar la IP del ESP32 en el dispositivo Modbus TCP Server.
3. Crear en Ubidots el dispositivo `tanque` con las variables de estado y las variables de comando `b1_remoto`, `b2_remoto` y `b3_remoto`.

## Créditos

- Autor: Santiago Urrego Rodríguez
- Docente: Juan Manuel Aranda López King
- Curso: Internet Industrial de las Cosas (IIoT), Universidad de La Sabana
- Fecha: noviembre de 2026
