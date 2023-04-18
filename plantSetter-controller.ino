#include<DHT.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP32.h>
# define sensorM A0 // Soil Sensor input at Analog PIN A0
const String ssid="DEAD";
const String password ="M24680zb";
int RELAY1 =5;  // num of pin 
int RELAY2 =4;
int RELAY3 =14;
int RELAY4 =12;
boolean state = false;
int count=0; 

  int valueM= 0, moisture=0;

WiFiClient  client;
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
#define Firebase_Host "planet-setter-default-rtdb.europe-west1.firebasedatabase.app"
#define ServiceAccount "firebase-adminsdk-5j9c0@planet-setter.iam.gserviceaccount.com"
#define SecretKey   "nE1O01o9eDC1LRclU3GLuwD7QiB16BhruvDKfcIL"
#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
 float temp;
float Hum;


void setup()
{
  Serial.begin(9600);
  dht.begin();
pinMode(sensorM, INPUT);
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
  pinMode(RELAY, OUTPUT);

  delay(500);
}

void loop() {Serial.begin(9600);
Serial.print("MOISTURE LEVEL : ");
   valueM= analogRead(sensorM);
   moisture = valueM
   Serial.print(moisture);
   Serial.println("%");
   
   if(moisture <750)
     {
      Serial.println("The Soil is Dry");
     }
   
  
   else if ( moisture <500)
   
     {
      Serial.println("THE SOIL IS WET.");
     }
   else{
    Serial.println("THE SOIL  SHOULD BE WATERED");
   }
     
  temp = dht.readTemperature();
Hum=dht.readHumidity();
Serial.print("Temprature=  ");
Serial.print(temp);
Serial.println(" c");
Serial.print("Humidity=  ");
Serial.print(Hum);
Serial.println(" %");
count ++;
if(count==50){
digitalWrite(RELAY,HIGH);
state = true;
Firebase.setBool(firebaseData,"state",state);
 Serial.println("pump is on");
delay(10111);
count = 0; 
}
else{
digitalWrite(RELAY,LOW);
state = false;
Serial.println("pump is off");
  Firebase.setBool(firebaseData,"state",state);
  Firebase.setInt(firebaseData,"count",count);
  Firebase.setFloat(firebaseData, "TEMPERATURE", temp);
  Firebase.setFloat(firebaseData, "HUMIDITY", Hum);
   delay(1000);}




}
