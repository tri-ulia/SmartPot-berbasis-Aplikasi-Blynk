#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

char auth[] = "sHcf7bZAnqTo8NvW1PyoVa4KsSPb9jBz"; 
char ssid[] = "LABOR 8"; 
char pass[] = "poiuytrewq";  

float t;
int readD1;
int readD2;
int moisture_sensor1;
int moisture_sensor2;
int Pin_D3 = 0;  //Driver motor
int Pin_D4 = 2;  //Driver motor
int Pin_D5 = 14; //DHT 11
int Pin_D7 = 13; //Water Level
int Pin_D8 = 15; //Kelembaban

#define DHTTYPE DHT11 
const int dry = 600; // nilai sensor ketika kering 
const int wet = 200; // nilai sensor ketika basah
 
DHT dht(Pin_D5, DHTTYPE);
BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() 
{ 
  Serial.begin(9600);
  pinMode(Pin_D3,OUTPUT);
  pinMode(Pin_D4,OUTPUT);
  pinMode(Pin_D5,OUTPUT);
  pinMode(Pin_D7,OUTPUT);
  pinMode(Pin_D8,OUTPUT);
  pinMode(A0,INPUT);
  
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  Blynk.begin(auth, ssid, pass);

  lcd.init(); // inisialisasi lcd
  lcd.backlight();
  lcd.setCursor(0, 0);       //KOLOM 1 BARIS 1  
  lcd.print("Welcome to our");      
  lcd.setCursor(1, 1);       //KOLOM 2 BARIS 2
  lcd.print("Smartpot System");          
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);         
  lcd.print("Muhammad Fadhly");       
  lcd.setCursor(0, 1);        
  lcd.print("Tri Ulia Sari");
  delay (2000);
  lcd.clear();
}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  Blynk.virtualWrite(V7, t);
  Blynk.virtualWrite(V8 , h);
}

void loop() 
{
  Blynk.run();
  //Sensor water level
  digitalWrite(Pin_D7, HIGH);
  delay(100);                     
  readD1 = analogRead(0);     
  digitalWrite(Pin_D7, LOW);      
  delay(100);   
  //sensor kelembaban tanah
  digitalWrite(Pin_D8, HIGH);    
  delay(100);                     
  readD2 = analogRead(0);       
  digitalWrite(Pin_D8, LOW);  
  delay(100); 

  moisture_sensor1=map(readD1, wet, dry, 100, 0);
  moisture_sensor2=map(readD2, wet, dry, 100, 0);

    //serial monitor
    Serial.print("sensor 1 = ");   
    Serial.print(moisture_sensor1);
    Serial.print(" / sensor 2 = ");   
    Serial.println(moisture_sensor2);


  if (moisture_sensor2<50)
   {
    Serial.print("waterpump Aktif\n"); 
    digitalWrite(Pin_D3,HIGH);
    digitalWrite(Pin_D4,LOW);
    //digitalWrite(Pin_S2,LOW);
    //digitalWrite(Pin_S3,HIGH);
    delay(10);
    }
  if (moisture_sensor2>=50)
   {
    Serial.print("waterpump mati\n"); 
    digitalWrite(Pin_D3,LOW);
    digitalWrite(Pin_D4,LOW);
    //digitalWrite(Pin_S2,HIGH);
    //digitalWrite(Pin_S3,LOW);
    delay(10);
    }

  //Blynk server
  Blynk.virtualWrite(V5, moisture_sensor1); 
  Blynk.virtualWrite(V6, moisture_sensor2); 
  timer.run(); 

  float t=dht.readTemperature();
    
  lcd.setCursor(0,0); //Baris 1
  lcd.print("T : ");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0,1); //Baris 2
  lcd.print("H : ");
  lcd.print(moisture_sensor2);
  lcd.print(" RH");
  lcd.print(" W : ");
  lcd.print(moisture_sensor1);
  lcd.print(" %");  
}
