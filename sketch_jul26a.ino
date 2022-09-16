#include<DHT.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP32.h>
const String ssid="ROMANOZZI 2.4";
const String password ="01201551009Mo2";


WiFiClient  client;
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
#define Firebase_Host "planet-setter-default-rtdb.europe-west1.firebasedatabase.app"
#define ServiceAccount "firebase-adminsdk-5j9c0@planet-setter.iam.gserviceaccount.com"
#define SecretKey   "nE1O01o9eDC1LRclU3GLuwD7QiB16BhruvDKfcIL"
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
 float temp;
float Hum;


void setup()
{
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  Firebase.begin(Firebase_Host, SecretKey);
  Firebase.reconnectWiFi(true);
   Serial.println("------------------------------------");
  Serial.println("Connected...");
  delay(500);
}

void loop() {Serial.begin(9600);
  temp = dht.readTemperature();
Hum=dht.readHumidity();
Serial.print("Temprature=  ");
Serial.print(temp);
Serial.println(" c");
Serial.print("Humidity=  ");
Serial.print(Hum);
Serial.println(" %");

  Firebase.setFloat(firebaseData, "TEMPERATURE", temp);
  Firebase.setFloat(firebaseData, "HUMIDITY", Hum);
   delay(200);





}
