# Gemelo Digital de un Tanque con CODESYS, ESP32, Modbus TCP y Ubidots

Curso: Internet Industrial de las Cosas (IIoT) y Nuevas Tendencias, Universidad de La Sabana.
Resultado de aprendizaje: RA 3.2. Sustentación: 3 de noviembre de 2026.

Esta wiki documenta el diseño, la implementación y la validación de un gemelo digital de un sistema de monitoreo de nivel de un tanque. El activo físico (dipswitch y LEDs sobre un ESP32) se conecta por Modbus TCP con un activo virtual en CODESYS, y el estado se publica en la plataforma IoT Ubidots por MQTT.

## Contenido

| Página | Temas |
|---|---|
| [Resumen](Resumen) | Síntesis del proyecto y requisitos del reto frente a su cumplimiento |
| [Diseño del Sistema](Diseno-del-Sistema) | Requisitos, lógica del tanque, arquitectura, decisiones de diseño, mapa de registros Modbus, diagrama UML de secuencia, viabilidad económica |
| [Implementación en CODESYS](Implementacion-en-CODESYS) | Activo virtual: programa Ladder, dispositivos y canales Modbus, I/O Mapping |
| [Hardware Real](Hardware-Real) | Esquema de interconexión y firmware del ESP32 |
| [Comunicación Modbus](Comunicacion-Modbus) | Enlace Modbus TCP entre CODESYS y el ESP32 |
| [Integración con Ubidots](Integracion-con-Ubidots) | Broker MQTT, publicación, comandos y dashboard |
| [Pruebas y Validación](Pruebas-y-Validacion) | Pruebas funcionales y de la plataforma IoT |
| [Problemas y Soluciones](Problemas-y-Soluciones) | Problemas encontrados durante el desarrollo y su resolución |
| [Conclusiones](Conclusiones) | Conclusiones, limitaciones y mejoras futuras |
| [Referencias](Referencias) | Referencias en formato IEEE |

## Vista general

```mermaid
flowchart LR
    subgraph Campo["Activo físico"]
        DIP["Dipswitch<br/>B1 GPIO4, B2 GPIO16, B3 GPIO17"]
        LED["5 LEDs<br/>GPIO 18, 19, 21, 22, 23"]
    end
    subgraph ESP["ESP32 (firmware Arduino)"]
        MB["Servidor Modbus TCP<br/>puerto 502, Unit-ID 1"]
        MQ["Cliente MQTT<br/>PubSubClient"]
    end
    subgraph PC["Activo virtual (PC)"]
        PLC["CODESYS Control Win V3 x64<br/>Ladder + cliente Modbus"]
        HMI["HMI / visualización"]
    end
    subgraph Nube["Plataforma IoT"]
        UB["Ubidots STEM<br/>dispositivo 'tanque'<br/>dashboard"]
    end
    DIP --> ESP
    ESP --> LED
    MB <-->|"Modbus TCP, WiFi"| PLC
    PLC --- HMI
    MQ <-->|"MQTT 1883, WiFi"| UB
```

Siguiente: [Resumen](Resumen)
