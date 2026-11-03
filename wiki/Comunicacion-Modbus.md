# Comunicación Modbus

## Comunicación Modbus TCP

CODESYS (cliente) consulta al ESP32 (servidor) por el puerto 502 de la red WiFi del hotspot. Se usan las funciones FC2 (lectura de entradas discretas) y FC15 (escritura de múltiples coils). Se verificó la comunicación de forma independiente con una petición FC2 manual desde Python, que devolvió el byte con el estado de los tres dips.

El mapa completo de registros se encuentra en [Diseño del Sistema](Diseno-del-Sistema) y la configuración de dispositivos y canales en [Implementación en CODESYS](Implementacion-en-CODESYS).

---

Anterior: [Hardware Real](Hardware-Real) | Siguiente: [Integración con Ubidots](Integracion-con-Ubidots)
