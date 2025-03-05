//                                $$$$$$$$\ $$\                                                   
//                                \__$$  __|$$ |                                                  
//  $$\   $$\  $$$$$$\   $$$$$$\     $$ |   $$$$$$$\   $$$$$$\   $$$$$$\  $$$$$$\$$$$\   $$$$$$\  
//  $$ |  $$ |$$  __$$\  \____$$\    $$ |   $$  __$$\ $$  __$$\ $$  __$$\ $$  _$$  _$$\ $$  __$$\ 
//  $$ |  $$ |$$$$$$$$ | $$$$$$$ |   $$ |   $$ |  $$ |$$$$$$$$ |$$ |  \__|$$ / $$ / $$ |$$ /  $$ |
//  $$ |  $$ |$$   ____|$$  __$$ |   $$ |   $$ |  $$ |$$   ____|$$ |      $$ | $$ | $$ |$$ |  $$ |
//  \$$$$$$$ |\$$$$$$$\ \$$$$$$$ |   $$ |   $$ |  $$ |\$$$$$$$\ $$ |      $$ | $$ | $$ |\$$$$$$  |
//   \____$$ | \_______| \_______|   \__|   \__|  \__| \_______|\__|      \__| \__| \__| \______/ 
//  $$\   $$ |                                                                                    
//  \$$$$$$  |                                                                                    
//   \______/                                                                                     


// -------------------- RED WIFI --------------------
const char* ssid = ""; 
// Nombre de la red wifi

const char* password = "";
// Contraseña de la red WiFi



// -------------------- TELEGRAM --------------------
#define BOT_TOKEN ""
// Token de la API de Telegram. Se obtiene mediante el bot @BotFather

const char* CHAT_ID = ""; 
// Usuario o chat autorizado. Se obtiene mediante el bot @userinfobot



// -------------------- PERIODICIDAD --------------------
const long notificationInterval = 3600000; // Por def: 1h
// Cada cuanto tiempo en milisegundos enviará el reporte de temperatura y humedad.

const long checkInterval = 60000; // Por def: 1min
// Cada cuanto tiempo en milisegundos comprobará la temperatura en busqueda de cambios bruscos.

const float temperatureThreshold = 5.0; // Por def: 5ºC
// Nº de grados que debe aumentar la temperatura desde la última comprobación para que considere el cambio brusco.





/* 

        _    ______     _______ ____ _____ _____ _   _  ____ ___    _    
       / \  |  _ \ \   / / ____|  _ \_   _| ____| \ | |/ ___|_ _|  / \   
      / _ \ | | | \ \ / /|  _| | |_) || | |  _| |  \| | |    | |  / _ \  
     / ___ \| |_| |\ V / | |___|  _ < | | | |___| |\  | |___ | | / ___ \ 
    /_/   \_\____/  \_/  |_____|_| \_\|_| |_____|_| \_|\____|___/_/   \_\
                                                                      
                                                                      

    A partir de este momento, por favor, no modifique nada.
    Si se modifica algo provocando errores en el código, es muy probable que deje de funcionar el termómetro.
    Haga ediciones con precaución.
*/


// -------------------------- Dependencias (NO TOCAR) --------------------------
#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>

// Inicializamos las variables
unsigned long lastSendTime = 0;
unsigned long lastCheckTime = 0;
float lastTemperature = -100.0;

// Definimos los pines por los que se conectarán los componentes
#define BUZZER_PIN 3
#define DHTPIN 4
#define DHTTYPE DHT11 // Tipo de sensor: DHT11/DHT22
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure client; // Iniciamos el cliente WiFi
UniversalTelegramBot bot(BOT_TOKEN, client); // Iniciamos la api de Telegram

// -------------------- CONEXIÓN WIFI --------------------
void connectToWiFi() {
  Serial.println("[INFO] Intentando conectar a WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n[INFO] Conexión WiFi exitosa.");
}

// -------------------- AVISO SONORO --------------------
void playBuzzerAlert() {
  for (int i = 0; i < 8; i++) {
    // Activa el buzzer durante 250ms
    digitalWrite(BUZZER_PIN, HIGH);
    delay(250);

    // Desactiva el buzzer durante 250ms
    digitalWrite(BUZZER_PIN, LOW);
    delay(250);
  }
}

// -------------------- ENVIO DATOS --------------------
void sendTemperatureHumidity() {
  Serial.println("[INFO] Leyendo datos del sensor DHT...");
  // Leemos la temperatura y humedad del sensor DHT
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    // Si hay un error al leer los datos, se muestra un mensaje de error.
    Serial.println("[ERROR] Error leyendo del sensor DHT!");
    return;
  }
  
  Serial.print("[INFO] Temperatura: ");
  Serial.print(temperature);
  Serial.print("°C, Humedad: ");
  Serial.print(humidity);
  Serial.println("%");
  
  // Construimos mensaje y lo enviamos al chat de Telegram
  String message = "🌡️ Temperatura: " + String(temperature) + "°C\n💧 Humedad: " + String(humidity) + "%";
  bot.sendMessage(CHAT_ID, message, "");
  
  // Actualizamos la última temperatura leída
  lastTemperature = temperature;
}

// -------------------- COMPROBAR CAMBIOS BRUSCOS --------------------
void checkTemperature() {
  Serial.println("[INFO] Comprobando cambios bruscos de temperatura...");
  
  float temperature = dht.readTemperature();
  
  if (isnan(temperature)) {
    Serial.println("[ERROR] Error leyendo la temperatura!");
    return;
  }
  
  if (lastTemperature != -100.0 && abs(temperature - lastTemperature) >= temperatureThreshold) {
    int minutos = checkInterval / (1000.0 * 60.0);
    Serial.println("[ALERTA] Cambio brusco de temperatura detectado!");
    
    String alertMessage = "⚠️ ALERTA: Cambio brusco de temperatura!\n"
                          "Temperatura (hace " + String(minutos) + " min): " + String(lastTemperature) + "ºC\n"
                          "Temperatura actual: " + String(temperature) + "°C";
    
    bot.sendMessage(CHAT_ID, alertMessage, "");
    playBuzzerAlert();
  }
  
  lastCheckTime = millis();
  lastTemperature = temperature;
}

// -------------------- Procesar mensajes recibidos --------------------
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "") {
      from_name = "Desconocido";
    }

    Serial.println("[INFO] Mensaje recibido:");
    Serial.print("  - Usuario: ");
    Serial.println(from_name);
    Serial.print("  - Chat ID: ");
    Serial.println(chat_id);
    Serial.print("  - Texto: ");
    Serial.println(text);

    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "❌ No estás autorizado para usar este bot.", "");
      continue;
    }

    if (text == "/start") {
      bot.sendMessage(chat_id, "🤖 ¡Hola! Envíame /temp para obtener la temperatura y humedad actuales.", "");
    } else if (text == "/temp") {
      // Envía la temperatura y humedad actuales
      sendTemperatureHumidity();
    } else if (text == "/test") {
      // Simulación de alerta por cambio brusco de temperatura.
      playBuzzerAlert();
    } else if (text == "/play") {
      // Activa el buzzer para debug.
      digitalWrite(BUZZER_PIN, HIGH);
    } else if (text == "/stop") {
      // Desactiva el buzzer tras debug.
      digitalWrite(BUZZER_PIN, LOW);
    } else {
      // Si el mensaje no coincide con ningún comando, envía un mensaje de error.
      bot.sendMessage(chat_id, "❓ Comando no reconocido. Usa /temp para obtener los datos.", "");
    }
  }
}

// -------------------- Inicializar --------------------
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(30);
  digitalWrite(BUZZER_PIN, LOW);
  connectToWiFi();
  client.setInsecure();
  Serial.println("[INFO] Sistema iniciado correctamente.");
  lastTemperature = dht.readTemperature();
  sendTemperatureHumidity();
}

void loop() {
  if (millis() - lastSendTime > notificationInterval) {
    lastSendTime = millis();
    // Enviamos el reporte de humedad y temperatura.
    sendTemperatureHumidity();
  }

  if (millis() - lastCheckTime > checkInterval) {
    // Comprobamos la temperatura en busqueda de cambios bruscos de temperatura.
    checkTemperature();
  }

  // Comprobamos si hay mensajes pendientes de leer (comandos)
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  handleNewMessages(numNewMessages);
}
