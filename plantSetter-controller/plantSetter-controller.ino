#include<DHT.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP32.h>

#define sensorM A0 // Soil Sensor input at Analog PIN A0
const String ssid="DEAD";
const String password ="M24680zb";
const String id= "a06e71b81f68d1258833b943b564257b1579bafe88322c107af7b9a70d7c3ad4" ;
int Rw =5;  // num of pin 
int Rn =4;
int Rk =14;
int Rp =12;
int N;
int P;
int K ;
int firebaseSize ;

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
  pinMode(Rw, OUTPUT);
  pinMode(Rn, OUTPUT);
  pinMode(Rp, OUTPUT);
  pinMode(Rk, OUTPUT);
  digitalWrite(Rw,HIGH);
  digitalWrite(Rn,HIGH);
  digitalWrite(Rp,HIGH);
  digitalWrite(Rk,HIGH);  

  
  delay(500);
}

void loop() {
  
  valueM= analogRead(sensorM);
  moisture = valueM;
 
  
   

     
  temp = dht.readTemperature();
  Hum=dht.readHumidity();

bool flag = Firebase.getString(firebaseData,"site-units/"+id+"/state" );

if (flag ){
  String state = firebaseData.stringData();
} 
  Firebase.setFloat(firebaseData,"site-units/"+id+"/TEMPERATURE", temp);
  Firebase.setFloat(firebaseData, "site-units/"+id+"/HUMIDITY", Hum);
  Firebase.setInt(firebaseData,"site-units/"+id+"/moisture",moisture);


   String firebaseState ="";
  if (Firebase.getString(firebaseData, "site-units/"+id+"/state")) {
   firebaseState = firebaseData.stringData();}

    bool SizeFlag = Firebase.getInt(firebaseData, "site-units/"+id+"/Size");
    if (SizeFlag){
      firebaseSize=firebaseData.intData();
    }

    bool Nflag = Firebase.getFloat(firebaseData,"site-units/"+id+"/N");
    if(Nflag){
       N=firebaseData.intData();
    }

        
    bool Pflag = Firebase.getInt(firebaseData,"site-units/"+id+"/P");
    if (Pflag) {
      P=firebaseData.intData();
    }
         
    bool Kflag = Firebase.getInt(firebaseData,"site-units/"+id+"/K");
    if(Kflag){
      K=firebaseData.intData();
    }
  if (firebaseState == "i") {
    Firebase.setInt(firebaseData,"site-units/"+id+"/Size", 500);
    Firebase.setString(firebaseData,"site-units/"+id+"/state","n");
      
  } else if (firebaseState == "n") {
  int delayTimeN  = N*0.624*firebaseSize;
  Serial.print("n   ");
  Serial.println(delayTimeN);
  digitalWrite(Rn,LOW);
  delay(delayTimeN);
  digitalWrite(Rn,HIGH);

  Firebase.setString(firebaseData,  "site-units/"+id+"/state","p");

  } else if (firebaseState == "p") {
  int delayTimeP  = P*firebaseSize*0.624;
  Serial.print("p   ");
  Serial.println(delayTimeP);
  digitalWrite(Rp,LOW);  
  delay(delayTimeP);
  digitalWrite(Rp,HIGH);
  Firebase.setString(firebaseData,  "site-units/"+id+"/state","k");

  } else if (firebaseState == "k") {
  int delayTimeK  = K*firebaseSize*0.624;
  Serial.print("k   ");
  Serial.println(delayTimeK);
  digitalWrite(Rk,LOW);
  delay(delayTimeK);
  digitalWrite(Rk,HIGH);
  Firebase.setString(firebaseData,"site-units/"+id+"/state", "w");

  } else if (firebaseState == "w") {
      
      
      int delayTimeW = (firebaseSize*0.97) * 14.55;
      digitalWrite(Rw,LOW);
      Serial.print("Water   ");
      Serial.println(delayTimeW);
      
      delay(delayTimeW);
      digitalWrite(Rw,HIGH);
     
      Firebase.setString(firebaseData,"site-units/"+id+"/state", "d");

    
  } else {
   /* int D = Firebase.getInt(firebaseData, "D");
      D = (D / 100) * (20 / firebaseSize);*/
      Firebase.setInt(firebaseData,"site-units/"+id+"/Size", 200);
  }
  delay(1000);
}
