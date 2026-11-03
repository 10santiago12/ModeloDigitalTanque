# Conclusiones

- El gemelo digital cumple el ciclo completo: activo físico, activo virtual, conexión por Modbus TCP y gobierno desde una plataforma IoT.
- Separar la lógica (CODESYS), la interfaz de campo (ESP32) y la nube (Ubidots) permitió depurar cada capa de forma independiente.
- Limitaciones: la combinación OR impide apagar desde Ubidots un dip que está en ON, y el runtime opera en modo demostración. Una mejora natural es un selector Local/Remoto y la lectura de sensores reales de nivel.
- Mejoras futuras: seguridad (MQTT sobre TLS y credenciales fuera del código), registro histórico de eventos y sustitución del dipswitch por sensores reales.

---

Anterior: [Problemas y Soluciones](Problemas-y-Soluciones) | Siguiente: [Referencias](Referencias)
