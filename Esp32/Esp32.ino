#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

const int inputPin = 34; // Chân số 34 của ESP32

WebServer server(80);

unsigned long previousMillis = 0; // Biến lưu giữ thời gian trước đó
const long interval = 1000; // Khoảng thời gian cập nhật (ms)

void handleRoot() {
  String content = "<!DOCTYPE html>\
  <html>\
  <head>\
    <title>Esp32</title>\
  </head>\
  <body>\
    <h1>Web Server</h1>\
    <label for=\"label1\">Label 1:</label>\
    <input type=\"text\" id=\"label1Entry\"><br><br>\
    <label for=\"label2\">Label 2:</label>\
    <input type=\"text\" id=\"label2Entry\"><br><br>\
    <label for=\"label3\">Label 3:</label>\
    <input type=\"range\" id=\"label3Entry\" min=\"0\" max=\"100\" value=\"0\" onchange=\"updateLabel3Value()\"><br><br>\
    <label for=\"label4\">Label 4:</label>\
    <input type=\"range\" id=\"label4Entry\" min=\"0\" max=\"100\" value=\"0\" onchange=\"updateLabel4Value()\"><br><br>\
    <button onclick=\"saveValues()\">Save</button>\
    <button onclick=\"resetValues()\">Reset</button><br><br>\
    <label for=\"parameter\">Parameter:</label><br>\
    <label id=\"valueLabel1\">Value label 1: 0</label><br>\
    <label id=\"valueLabel2\">Value label 2: 0</label><br>\
    <label id=\"valueLabel3\">Value label 3: 0</label><br>\
    <label id=\"valueLabel4\">Value label 4: 0</label><br>\
    <label id=\"valueLabel5\">Value label 5: </label>\
    <script>\
      function saveValues() {\
        var label1Value = document.getElementById(\"label1Entry\").value;\
        var label2Value = document.getElementById(\"label2Entry\").value;\
        var label3Value = document.getElementById(\"label3Entry\").value;\
        var label4Value = document.getElementById(\"label4Entry\").value;\
        document.getElementById(\"valueLabel1\").textContent = \"Value label 1: \" + label1Value;\
        document.getElementById(\"valueLabel2\").textContent = \"Value label 2: \" + label2Value;\
        document.getElementById(\"valueLabel3\").textContent = \"Value label 3: \" + label3Value;\
        document.getElementById(\"valueLabel4\").textContent = \"Value label 4: \" + label4Value;\
        fetch('/save?label1=' + label1Value + '&label2=' + label2Value + '&label3=' + label3Value + '&label4=' + label4Value);\
      }\
      function resetValues() {\
        document.getElementById(\"label1Entry\").value = \"\";\
        document.getElementById(\"label2Entry\").value = \"\";\
        document.getElementById(\"label3Entry\").value = 0;\
        document.getElementById(\"label4Entry\").value = 0;\
        document.getElementById(\"valueLabel1\").textContent = \"Value label 1: 0\";\
        document.getElementById(\"valueLabel2\").textContent = \"Value label 2: 0\";\
        document.getElementById(\"valueLabel3\").textContent = \"Value label 3: 0\";\
        document.getElementById(\"valueLabel4\").textContent = \"Value label 4: 0\";\
        fetch('/reset');\
      }\
      function updateLabel3Value() {\
        var label3Value = document.getElementById(\"label3Entry\").value;\
        document.getElementById(\"valueLabel3\").textContent = \"Value label 3: \" + label3Value;\
      }\
      function updateLabel4Value() {\
        var label4Value = document.getElementById(\"label4Entry\").value;\
        document.getElementById(\"valueLabel4\").textContent = \"Value label 4: \" + label4Value;\
      }\
      function updateLabel5Value(value) {\
        document.getElementById(\"valueLabel5\").textContent = \"Value label 5: \" + value;\
      }\
    </script>\
  </body>\
  </html>";

  server.send(200, "text/html", content);
}

void handleSave() {
  String label1 = server.arg("label1");
  String label2 = server.arg("label2");
  String label3 = server.arg("label3");
  String label4 = server.arg("label4");
  
  Serial.print("Label 1: ");
  Serial.println(label1);
  Serial.print("Label 2: ");
  Serial.println(label2);
  Serial.print("Label 3: ");
  Serial.println(label3);
  Serial.print("Label 4: ");
  Serial.println(label4);
  
  server.send(200, "text/plain", "Data saved successfully.");
}

void handleReset() {
  Serial.println("Reset!");
  handleSave(); // In lại các giá trị label để hiển thị chúng trên Serial Monitor
}

void updateLabel5Value() {
  int value = digitalRead(inputPin);
  server.sendContent("data: " + String(value) + "\n\n"); // Gửi giá trị về client
  Serial.print("Value label 5: ");
  Serial.println(value);
}

void setup() {
  Serial.begin(9600);

  // Set up Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_GET, handleSave);
  server.on("/reset", HTTP_GET, handleReset);
  server.begin();
}

void loop() {
  server.handleClient();

  // Kiểm tra thời gian để cập nhật giá trị label 5
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    updateLabel5Value();
    previousMillis = currentMillis;
  }
}