#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// -------------------------
// CONFIGURAÇÃO DE HARDWARE
// -------------------------

#define DHTPIN 15          // Pino do sensor DHT22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define POT_PIN 34         // Potenciômetro (simula batimentos cardíacos)
#define TRIGGER_PIN 5      // Trigger do sensor ultrassônico
#define ECHO_PIN 18        // Echo do sensor ultrassônico
#define ALERT_LED_PIN 2    // LED de alerta
#define BUZZER_PIN 4       // Buzzer para alertas sonoros

// -------------------------
// CONFIGURAÇÃO DE INTERNET
// -------------------------

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* apiKey = "RJHLLK8H7LZRZRBF";   // Chave Write API do ThingSpeak
const char* server = "http://api.thingspeak.com";

// -------------------------
// CONTROLE DE TEMPO (CLOUD)
// -------------------------

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;          // Envio para a nuvem a cada 15s

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Configuração dos pinos
  pinMode(POT_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ALERT_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Desliga atuadores no início
  digitalWrite(ALERT_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Conecta ao WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado com sucesso!");
}

void loop() {

  // ============================================================
  // PROCESSAMENTO LOCAL (EM TEMPO REAL)
  // ============================================================

  // Leitura do DHT22 (com validação de erro)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) { h = 0; t = 0; }

  // Conversão do potenciômetro para faixa de BPM (60–150)
  int potValue = analogRead(POT_PIN);
  int heartRate = map(potValue, 0, 4095, 60, 150);

  // Medição do sensor ultrassônico
  long duracao, distancia;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duracao = pulseIn(ECHO_PIN, HIGH);
  distancia = (duracao / 2) * 0.0344;   // Conversão para centímetros

  // -------------------------
  // LÓGICA DE ALERTAS LOCAIS
  // -------------------------

  if (heartRate > 110) {
    digitalWrite(ALERT_LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000, 100);         // Alerta de estresse
  }
  else if (distancia < 30) {
    digitalWrite(ALERT_LED_PIN, HIGH);
    tone(BUZZER_PIN, 500, 100);          // Alerta de ergonomia
  }
  else if (t > 35.0 || h < 30.0) {
    digitalWrite(ALERT_LED_PIN, HIGH);
    tone(BUZZER_PIN, 1500, 100);         // Alerta ambiental
  }
  else {
    digitalWrite(ALERT_LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  // ============================================================
  // ENVIO DE DADOS PARA A NUVEM (THINGSPEAK)
  // ============================================================

  if ((millis() - lastTime) > timerDelay) {

    Serial.println("\n--- ENVIANDO TELEMETRIA ---");
    Serial.printf("BPM: %d | Dist: %ldcm | Temp: %.1fC | Umid: %.1f%%\n",
                  heartRate, distancia, t, h);

    if (WiFi.status() == WL_CONNECTED) {

      HTTPClient http;

      // URL de envio GET
      String url = String(server) + "/update?api_key=" + apiKey +
                   "&field1=" + String(t) +
                   "&field2=" + String(h) +
                   "&field3=" + String(heartRate) +
                   "&field4=" + String(distancia);

      http.begin(url);
      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.printf("Sucesso! HTTP Code: %d\n", httpCode);
      } else {
        Serial.printf("Falha no envio. HTTP Code: %d\n", httpCode);
      }

      http.end();
    }
    else {
      Serial.println("Erro: WiFi desconectado.");
    }

    lastTime = millis();
  }

  delay(50);   // Pequeno delay para estabilidade no Wokwi
}
