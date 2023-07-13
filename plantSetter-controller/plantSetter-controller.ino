#include<DHT.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP32.h>

#define sensorM A0 // Soil Sensor input at Analog PIN A0
const String ssid="ROMANOZZI2.4";
const String password ="011145146Mo@";
const String id= "a06e71b81f68d1258833b943b564257b1579bafe88322c107af7b9a70d7c3ad4" ;
int Rw =5;  // num of pin 
int Rn =4;
int Rk =14;
int Rp =12;
int Re = 15;

int N;
int P;
int K ;
int Size =200;
int counter;

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
  pinMode(Re, OUTPUT);
  digitalWrite(Rw,HIGH);
  digitalWrite(Rn,HIGH);
  digitalWrite(Rp,HIGH);
  digitalWrite(Rk,HIGH);  
  digitalWrite(Re,HIGH);  


  
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

    bool counterFlag = Firebase.getInt(firebaseData, "site-units/"+id+"/counter");
    if (counterFlag){
      counter=firebaseData.intData();
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
  if (firebaseState == "n") {
    if(counter == 0){
      digitalWrite(Rn,HIGH);
      Firebase.setInt(firebaseData,"site-units/"+id+"/counter",ceil((0.7*P*Size)/1000));
      Firebase.setString(firebaseData,"site-units/"+id+"/state","p");


    }
    else{
      digitalWrite(Rn,LOW);
      delay(1000);
      Firebase.setInt(firebaseData,"site-units/"+id+"/counter",counter-1);
    }
      
  } else if (firebaseState == "p") {
  if(counter == 0){
      digitalWrite(Rp,HIGH);
      Firebase.setInt(firebaseData,"site-units/"+id+"/counter",ceil((0.7*K*Size)/1000));
      Firebase.setString(firebaseData,"site-units/"+id+"/state","k");


    }
    else{
      digitalWrite(Rp,LOW);
      delay(1000);
      Firebase.setInt(firebaseData,"site-units/"+id+"/counter",counter-1);
    }
  } else if (firebaseState == "k") {
     if(counter == 0){
      digitalWrite(Rk,HIGH);
      Firebase.setInt(firebaseData,"site-units/"+id+"/counter",ceil((Size*0.96*14.55)/1000));
      Firebase.setString(firebaseData,"site-units/"+id+"/state","w");


    }
    else{
      digitalWrite(Rk,LOW);
      delay(1000);
      Firebase.setInt(firebaseData,"site-units/"+id+"/counter",counter-1);
    }

  } else if (firebaseState == "w") {
  if(counter == 0){
      digitalWrite(Rw,HIGH);
      Firebase.setInt(firebaseData,"site-units/"+id+"/counter",ceil((Size*14.55)/1000));
      Firebase.setString(firebaseData,"site-units/"+id+"/state","e");


    }
    else{
      digitalWrite(Rw,LOW);
      delay(1000);
      Firebase.setInt(firebaseData,"site-units/"+id+"/counter",counter-1);
    }

  } else if (firebaseState == "e") {
      
if(counter == 0){
      digitalWrite(Re,HIGH);
  
      Firebase.setString(firebaseData,"site-units/"+id+"/state","d");


    }
    else{
      digitalWrite(Re,LOW);
      delay(1000);
      Firebase.setInt(firebaseData,"site-units/"+id+"/counter",counter-1);
    }
  }
    
  else {
        
        Firebase.setInt(firebaseData,"site-units/"+id+"/counter",ceil((0.7*N*Size)/1000));
  
        


    }
   

  
  
}
