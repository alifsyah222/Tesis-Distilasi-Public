#include<max6675.h>

#define MQ3pin A4
#define thermo A2
int refluks;
int bukaanrefluks;
int boiler = 255;
int count = 0;
int count2 = 0;
int count3 = 0;

int positioner;
int pompa1;
int pompa2;

int ktcSO = 7;    //termokopel
int ktcCS = 6;    //termokopel
int ktcCLK = 5;   //termokopel

long pt100;
float kolom_atas_temp = 0;
int randomNumber;
int randomTime;
unsigned long interval = 0;        // the time we need to wait
unsigned long previousMillis = 0;  // millis() returns an unsigned long
float mq3Value;
float sensorvalue;
unsigned long currentMillis;
unsigned long currentMillis2;
float suhuboiler;

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(9, OUTPUT);  //Boiler
  pinMode(10, OUTPUT);  //positioner

  pinMode(11, OUTPUT);  //reflux
  pinMode(12, OUTPUT);  //Pompa 1
  pinMode(13, OUTPUT);  //Pompa 2
  
  pinMode(A0, INPUT);  //PT100
  pinMode(MQ3pin, INPUT);  //sensor konsentrasi
  pinMode(thermo, INPUT);  //thermocouple
}

void loop() {

  //Coding boiler, boiler mati ketika suhu PT-100 mencapai 80 derajat celsius
  pt100 = analogRead(A0);

  //regresi suhu PT-100
  suhuboiler = 0.5077*pt100-108.77-30;

  // boiler = 0;
  // analogWrite(9, boiler);

  if(suhuboiler>80){
    count++;    
  }
  else{
    count = 0;
  }

  if(suhuboiler<75){
    count2++;    
  }
  else{
    count2 = 0;
  }  
  
  if(suhuboiler>80 && count>=20)
  {
    boiler = 0;
    analogWrite(9, boiler);
  }
  else if(suhuboiler<75 && count2>=20)
  {
    boiler = 255;
    analogWrite(9, boiler);
  }
  else
  {
    analogWrite(9, boiler);
  }

  //baca suhu kolom atas (termokopel) - sudah dalam celcius
  kolom_atas_temp = ktc.readCelsius();
  //kolom_atas_temp = analogRead(A2);
  
  //Baca MQ3
  mq3Value = analogRead(MQ3pin);                   
  //sensorvalue = 0.0006*exp(0.02*mq3Value);
  //sensorvalue = constrain (sensorvalue,0,100);

  currentMillis2=millis();

  if(mq3Value>550){
    count3++;    
  }
  else{
    count3 = 0;
  }  

  //if(sensorvalue>0)
  if(mq3Value>550 && count3>10)
  {
    //katup refluks   
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) >= interval) 
    {
      randomNumber = random(50, 255);
      analogWrite(11, randomNumber);
      refluks = randomNumber;
      previousMillis = millis(); 
      interval = random(240000, 420000); //4 menit - 7 menit
    }  
  }
  //else if(sensorvalue=0)
  else
  {
      analogWrite(11, 255);
      refluks = 255;   
  }  

  bukaanrefluks = map (refluks,0,255,0,100);
  
  Serial.print(currentMillis2);
  Serial.print("\t");
  Serial.print(boiler);
  Serial.print("\t");
  Serial.print(pt100);
  Serial.print("\t");
  Serial.print(suhuboiler);
  Serial.print("\t");
  Serial.print(kolom_atas_temp);
  Serial.print("\t");
  Serial.print(refluks);
  Serial.print("\t");
  Serial.print(bukaanrefluks);
  Serial.print("\t");
  Serial.println(mq3Value);
  delay(1000);
}



