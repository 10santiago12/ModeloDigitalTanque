# Pruebas y Validación

## Pruebas funcionales

Se verificó el comportamiento con las ocho combinaciones del dipswitch. El resultado fue observado por el autor en el montaje, en CODESYS y en el dashboard.

| B1B2B3 | Estado esperado | LED físico | CODESYS | Ubidots |
|---|---|---|---|---|
| 000 | Vacío | OK | OK | OK |
| 100 | Bajo | OK | OK | OK |
| 110 | Correcto | OK | OK | OK |
| 111 | Alto | OK | OK | OK |
| 001, 010, 011, 101 | Error | OK | OK | OK |

Pruebas de la plataforma IoT:

| Prueba | Resultado |
|---|---|
| Publicación del estado en Ubidots al cambiar el dip | Los valores se actualizan en el dashboard |
| Cambio del estado desde un switch remoto de Ubidots | CODESYS y el montaje físico responden al cambio |

---

Anterior: [Integración con Ubidots](Integracion-con-Ubidots) | Siguiente: [Problemas y Soluciones](Problemas-y-Soluciones)
