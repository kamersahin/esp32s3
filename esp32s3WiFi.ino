#include <WiFi.h>
#include <FastLED.h>

// WiFi bilgileri
const char* ssid = "FiberHGW_ZTAE43_2.4GHz";
const char* password = "NtFACkDafF";

// RGB LED pin ve sayısı
#define LED_PIN     38
#define NUM_LEDS    1

// RGB LED için renkler
CRGB leds[NUM_LEDS];

// Web sunucusu
WiFiServer server(80);

// LED durumu
bool ledStateRed = false;
bool ledStateGreen = false;
bool ledStateBlue = false;

// HTTP header
String header;

void setup() {
  Serial.begin(115200);
  
  // RGB LED ayarları
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  // WiFi bağlantısı
  Serial.println("WiFi'ye bağlanılıyor...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi bağlantısı yapıldı.");
  Serial.print("IP adresi: ");
  Serial.println(WiFi.localIP());

  // Web sunucusu başlatma
  server.begin();
}

void loop() {
  // WiFi ağı var mı kontrol etme
  WiFiClient client = server.available();   
  if (client) {
    // WiFi varsa işlemleri gerçekleştir
    handleClient(client);
  }

  // RGB LED kontrolü
  if (ledStateRed) {
    // LED'i kırmızı olarak ayarla
    leds[0] = CRGB::Red;
  } else if (ledStateGreen) {
    // LED'i yeşil olarak ayarla
    leds[0] = CRGB::Green;
  } else if (ledStateBlue) {
    // LED'i mavi olarak ayarla
    leds[0] = CRGB::Blue;
  } else {
    // LED'i kapat
    leds[0] = CRGB::Black;
  }
  FastLED.show();
}

// Wifi Ağ isteklerini işleme fonksiyonu
void handleClient(WiFiClient client) {
  // İstek zamanı
  unsigned long currentTime = millis();
  unsigned long previousTime = currentTime;
  const long timeoutTime = 2000;
  
  // İstekler alınıncaya kadar veya zaman aşımına kadar döngü
  while (client.connected() && currentTime - previousTime <= timeoutTime) {  
    currentTime = millis();
    if (client.available()) {             
      char c = client.read();            
      Serial.write(c);                   
      // İsteklerde HTTP header bilgilerini al
      if (c == '\n') {
        if (header.indexOf("GET /red") != -1) {
          // Kırmızı LED butonuna basıldı
          ledStateRed = !ledStateRed;
        } else if (header.indexOf("GET /green") != -1) {
          // Yeşil LED butonuna basıldı
          ledStateGreen = !ledStateGreen;
        } else if (header.indexOf("GET /blue") != -1) {
          // Mavi LED butonuna basıldı
          ledStateBlue = !ledStateBlue;
        }
        break;
      } else if (c != '\r') {  
        header += c;
      }
    }
  }
  
  // Ağ üzerinden yanıt gönder
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 { background-color: #4CAF50; } .button3 { background-color: #FF0000; }</style></head>");
  client.println("<body><h1 style=\"color:blue;\">ESP32S3 RGB LED Kontrolü</h1>");
  client.println("<body><h3 style=\"color:blue;\">Kamer ŞAHİN</h3>");
  client.println("<p>Kırmızı LED Durumu: " + String(ledStateRed ? "Açık" : "Kapalı") + "</p>");
  if (ledStateRed) {
    client.println("<p><a href=\"/red\"><button class=\"button button3\">Kapat</button></a></p>");
  } else {
    client.println("<p><a href=\"/red\"><button class=\"button button2\">Aç</button></a></p>");
  }
  client.println("<p>Yeşil LED Durumu: " + String(ledStateGreen ? "Açık" : "Kapalı") + "</p>");
  if (ledStateGreen) {
    client.println("<p><a href=\"/green\"><button class=\"button button3\">Kapat</button></a></p>");
  } else {
    client.println("<p><a href=\"/green\"><button class=\"button button2\">Aç</button></a></p>");
  }
  client.println("<p>Mavi LED Durumu: " + String(ledStateBlue ? "Açık" : "Kapalı") + "</p>");
  if (ledStateBlue) {
    client.println("<p><a href=\"/blue\"><button class=\"button button3\">Kapat</button></a></p>");
  } else {
    client.println("<p><a href=\"/blue\"><button class=\"button button2\">Aç</button></a></p>");
  }
  client.println("</body></html>");
  
  // Temizlik ve bağlantıyı kapat
  header = "";
  client.stop();
}
