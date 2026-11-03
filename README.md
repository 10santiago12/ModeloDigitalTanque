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

## Créditos

- Autor: Santiago Urrego Rodríguez
- Docente: Juan Manuel Aranda López King
- Curso: Internet Industrial de las Cosas (IIoT), Universidad de La Sabana
- Fecha: noviembre de 2026
