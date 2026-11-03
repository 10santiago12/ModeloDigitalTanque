# Problemas y Soluciones

## Problemas encontrados y soluciones

| Síntoma | Causa | Solución |
|---|---|---|
| `Connection aborted: socket_recv() failed` repetido en CODESYS | La biblioteca del ESP32 acepta una sola conexión por IP y cierra la anterior; había dos clientes activos desde el mismo PC | Una sola instancia del runtime y desactivar `MODBUSIP_UNIQUE_CLIENTS` en la biblioteca |
| `No driver found`, sin login, Scan Network deshabilitado | El proyecto estaba en modo **Simulation** | Desmarcar Simulation y conectar al runtime CODESYS Control Win vía Gateway |
| Los dips no cambiaban los valores | El dipswitch es activo en bajo y el sketch leía `INPUT` sin inversión | `INPUT_PULLUP` y lectura invertida |
| Dispositivo Modbus no se podía configurar con IP | Se había agregado el `ModbusTCP Server Device` (CODESYS como servidor) en lugar del `Modbus TCP Server` hijo del Client | Usar el servidor genérico como hijo del Client |
| MQTT `rc=5` | Token de Ubidots incorrecto | Usar el *Default token* de API Credentials |
| Los switches remotos no actuaban | API label de las variables distinto del esperado por el firmware | Crear las variables con label exacto `b1_remoto`, `b2_remoto`, `b3_remoto` |

---

Anterior: [Pruebas y Validación](Pruebas-y-Validacion) | Siguiente: [Conclusiones](Conclusiones)
