# Hardware Real

## Esquema de interconexión

El dipswitch es activo en bajo: cada pin usa `INPUT_PULLUP` y el interruptor en ON lleva el pin a GND. El firmware invierte la lectura para que ON signifique verdadero.

| Elemento | Conexión |
|---|---|
| Dip B1, B2, B3 | Un terminal a GPIO 4, 16, 17; el otro a GND |
| LED Vacío, Bajo, Correcto, Alto, Error | Ánodo a GPIO 18, 19, 21, 22, 23 mediante resistencia (220 o 330 Ω); cátodo a GND |

> **Pendiente del autor:** insertar aquí la foto del montaje y el esquemático (por ejemplo, exportado de Fritzing). Imagen sugerida: `![Montaje físico](images/montaje.jpg)`.

## Firmware del ESP32

El firmware (`conexion_modbus/conexion_modbus.ino`) usa las bibliotecas `modbus-esp8266` (servidor Modbus TCP) y `PubSubClient` (MQTT). Su funcionamiento en cada ciclo de `loop()`:

```mermaid
flowchart TD
    A["mb.task(): atender Modbus"] --> B["Leer dip (activo en bajo) y combinar con comando remoto (OR)"]
    B --> C["Escribir entradas efectivas en discrete inputs"]
    C --> D["Leer coils escritos por CODESYS y mover LEDs"]
    D --> E{"WiFi conectado?"}
    E -- No --> A
    E -- Si --> F{"MQTT conectado?"}
    F -- No --> G["Reintentar cada 5 s (sin bloquear Modbus)"]
    G --> A
    F -- Si --> H["mqtt.loop(): recibir comandos"]
    H --> I{"Hubo cambio o pasaron 30 s?"}
    I -- Si --> J["Publicar 8 variables en JSON (mín. 250 ms entre envíos)"]
    I -- No --> A
    J --> A
```

Aspectos relevantes:

- Reconexión MQTT sin bloqueo, para no interrumpir el servicio Modbus.
- Publicación por cambio, con latido cada 30 s y separación mínima de 250 ms, para respetar los límites del plan gratuito.
- `WiFi.setSleep(false)` para mantener estable el enlace TCP.

---

Anterior: [Implementación en CODESYS](Implementacion-en-CODESYS) | Siguiente: [Comunicación Modbus](Comunicacion-Modbus)
