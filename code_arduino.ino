#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11
#define pirPin 2
#define redLEDpin 4
#define ylwLEDpin 5
#define grnLEDpin 6
const int trigPin = 9;
const int echoPin = 10;
DHT dht(DHTPIN, DHTTYPE);

long duration;
int distance;
float temperature;
float humidity;
int pirState = LOW;  // Start with no motion detected

void setup() {
    // Serial.begin(9600);
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT);
    pinMode(redLEDpin, OUTPUT);
    pinMode(grnLEDpin, OUTPUT);
    pinMode(ylwLEDpin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(pirPin, INPUT);
    dht.begin();
}

void readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
}

void readDHT() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
}

void readPIR() {
  pirState = digitalRead(pirPin);
}

void loop() {
    readDistance();
    readDHT();
    readPIR();

    String motion = (pirState == HIGH) ? "YES" : "NO";
    if (isnan(temperature)) {temperature = 0; /* Serial.println("Failed to read from DHT sensor!");*/ }
    if (isnan(humidity)) humidity = 0;

    String payload = ">{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + ", \"distance\":" + String(distance) + ", \"motion\":\"" + motion + "\"}<";
    Serial.println(payload);


    if (pirState == HIGH) digitalWrite(grnLEDpin, HIGH);
    else digitalWrite(grnLEDpin, LOW);

    if(distance < 10) digitalWrite(ylwLEDpin, HIGH);
    else digitalWrite(ylwLEDpin, LOW);
    
    if(temperature > 25) digitalWrite(redLEDpin, HIGH);
    else digitalWrite(redLEDpin, LOW);
    
    delay(500);
}