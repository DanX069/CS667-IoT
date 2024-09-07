#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi
// const char *ssid = "Malware";
// const char *password = "dqou6728";
// const char* ssid = "Paras iPhone 12";
// const char* password = "12345678910";

// MQTT Broker
const char *mqtt_broker = "test.mosquitto.org";
const char *mqtt_topic = "esp8266/cs667paras432";
// const char *mqtt_username = "emqx";
// const char *mqtt_password = "public";
const int mqtt_port = 1883;

unsigned long ptm = 0;
WiFiClient espClient;
PubSubClient client(espClient);

void connect_wifi(){
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("..");
    }
    Serial.println("Connected to the WiFi network");
}

void connect_mqtt(){
    while (!client.connected()) {
        String client_id = "aleatoryfreak-esp8266";
        Serial.printf("client %s connecting...\n", client_id.c_str());
        // if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        if (client.connect(client_id.c_str())) {
            Serial.println("MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            Serial.print(" retrying...\n");
            if(WiFi.status() != WL_CONNECTED) connect_wifi();
            delay(2000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    connect_wifi();
    client.setServer(mqtt_broker, mqtt_port);
    connect_mqtt();
}

void loop() {
    client.loop();
    unsigned long ctm = millis();

    if (ctm - ptm >= 500) {
        ptm = ctm;

        if (Serial.available()) {
            if (Serial.find('>')) {
                String dataReceived = Serial.readStringUntil('<');
                dataReceived.trim();  

                if (dataReceived.length() > 0) {
                    Serial.print("Received data: ");
                    Serial.println(dataReceived);

                    // client.publish(mqtt_topic, dataReceived.c_str(), false);
                    if (client.publish(mqtt_topic, dataReceived.c_str(), false)) Serial.println("Data sent to MQTT");
                    else {
                        Serial.println("Failed to send data to MQTT broker, retrying...");
                        if (WiFi.status() != WL_CONNECTED) {
                            connect_wifi();
                            connect_mqtt();
                        }
                        if (!client.connected()) connect_mqtt();
                    }
                }
            }
        }
    }
}