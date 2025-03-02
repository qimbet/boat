#include <Arduino.h>
#include <WiFi.h>
#include <string>

const char* targetIPAddress = "10.96.112.179";
const char* ssid = "IBC-WiFi5.0-1";
const char* password = "IBC-wifi-VFC15-150";
const int serverPort = 80;

int startTime = 0;

//IPAddress fixedIP = 192.176.99.99;

WiFiServer server(serverPort);
WiFiClient client;
TaskHandle_t receiverTask;//, senderTask;

void isIP() {
  //empty for now. Will be used to check if incoming data is an IP address, and will enter this into the send-address
}

// void connectToServer() {
//   // Attempt to connect to the server
//   while (!client.connect(targetIPAddress, serverPort)) {
//     Serial.println("Failed to connect to server. Retrying...");
//     delay(1000);
//   }
//   Serial.println("Connected to server!");
// }

void receiverTaskFunction(void* parameter) {
  while (true) {
    client = server.available();
    if (client) {
      Serial.println("Receiver task - Server available");
      while (client.connected()) {
        if (client.available()) {
          String dataReceived = client.readStringUntil('\n');
          Serial.println("Received: " + dataReceived);

          //while (true) {
              // Check if the client is connected
              // while (!client.connected()) {
              //   Serial.println("Client not connected. Reconnecting...");
              //   connectToServer();
              // }
              Serial.println("Preparing to send data...");
              String dataToSend = "Hello from ESP32!";
              client.println(dataToSend);
              Serial.println("Sent: " + dataToSend);
              vTaskDelay(50); // Adjust the delay based on your requirements
        }
      }

    }
    // If the client is not available, reconnect to the server
    else {
      Serial.println("Server not available. Reconnecting...");
      WiFi.disconnect();
      WiFi.reconnect();
      //connectToServer();
    }
    vTaskDelay(50); // Wait for a moment before checking the connection again
  }
}


void setup() {
  startTime = millis();
  Serial.begin(115200);

  // Connect to WiFi
  //WiFi.mode(WIFI_STA);  
  //WiFi.config(fixedIP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  Serial.print("IP:                     ");
  Serial.println(WiFi.localIP());

  int t = millis();
  while(true)
  {
    if ((millis()-t)>2000){
      t = millis();
      Serial.println("Waiting for server to become available");
    }
    client = server.available(); 
    if(client.connected())
    { 
      Serial.println("Connected!");
      break;
    }
  }
  


 // Create receiver task on core 0
  Serial.println("Starting receiver task");
  xTaskCreatePinnedToCore(receiverTaskFunction, "receiverTask", 10000, NULL, 1, &receiverTask, 0);

  // Create sender task on core 1
  // Serial.println("Starting sender task");
  // xTaskCreatePinnedToCore(senderTaskFunction, "senderTask", 10000, NULL, 1, &senderTask, 1);

  Serial.println("Tasks started");

}

void loop() {
  // int t = 0;
  // if ((millis()-startTime)>3000){
  //   startTime = millis();
  //   Serial.println("3 seconds passed");
  // }
}