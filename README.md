# 🌡️ Termómetro Inteligente con Arduino

¡Bienvenido a **yeaThermo**! Un proyecto basado en **Arduino** que mide la temperatura ambiente y la humedad relativa del aire, te envía periódicamente los valores mediante telegram a tu cuenta o a un chat grupal, además, emite avisos sonoros y por mensajes de cambios bruscos de la temperatura. Totalmente configurable.

## 🚀 Características

- 📡 **Sensor de temperatura**: Utiliza un **DHT11/DHT22** o **LM35** para medir la temperatura.
- 🔔 **Alertas**: Activa un buzzer y envia notificaciones.
- 🔌 **Bajo consumo**: Optimizado para ahorrar energía.
- 📲 **Conectividad**: Es necesario una conexión a internet vía WiFi o ethernet.

## 🛠️ Materiales Necesarios

| Componente    | Descripción            |
|--------------|------------------------|
| Arduino Wemos D1 R2 | Microcontrolador |
| Sensor DHT11/DHT22 | Medición de temperatura y humedad |
| Resistencia 10kΩ | Para el sensor |
| Buzzer | Indicador de alerta |

## ⚙️ Instalación y Configuración

1. **Clona este repositorio**
   ```sh
   git clone https://github.com/tu_usuario/termometro-inteligente.git
   ```
2. **Abre el código en Arduino IDE**.
3. **Instala los drivers de la placa**: http://arduino.esp8266.com/stable/package_esp8266com_index.json
4. **Carga el código en la placa Arduino**.
5. **Conecta los componentes según el diagrama de conexión**.
6. **Abre el monitor serie** para ver las lecturas por 9600 baudios.

## 🔌 Diagrama de Conexión
![esquema](https://github.com/user-attachments/assets/e17ffeb9-7710-48d3-ba1a-c77604802a6f)

## 📌 Futuras Mejoras

- Integración con sitio web para el registro de las temperaturas mediante solicitudes cURL y almacenamiento en base de datos para generación de gráficos e informes.
- Aplicación móvil para visualización de datos almacenados en la base de datos.
- Implementación de pantalla LCD.
