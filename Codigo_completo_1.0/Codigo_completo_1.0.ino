#include <AccelStepper.h>
#include <TimerOne.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT22

AccelStepper motorPasso (AccelStepper::FULL4WIRE, 8, 10, 9, 11);   // Passo completo 4 fios
DHT dht(DHTPIN,DHTTYPE);
OneWire  oneWire(6);  //Pino do sensor de temperatura
DallasTemperature sensors(&oneWire);
DeviceAddress ds18b20 = { 0x28, 0xFF, 0xA6, 0x0A, 0xB3, 0x17, 0x01, 0x78 };

//Variaveis PID temperatura
float kPt = 10.44; 
float kIt = 64/kPt;
float kDt = 16*kPt;

float dift = 0;

float Pt = 0;
float It = 0;
float Dt = 0;

float last_t = 50;

float actt = 0;

float spt = 30;

//Variaveis PID umidade
int dim = 128;  //variavel que controla a potencia
int y = 0;  //variavel de contagem
boolean zero_cross = false; //variavel para verificar se o zero cross aconteceu
int freqStep = 65;  //Frequencia do step

const int zerocross = 2;  //Porta do zero cross; Não alterar
const int activate = 5;  //Porta de controle do dimmer

float kPh = 31;
float kIh = 32/31;
float kDh = 8*31;

float Ph = 0;
float Ih = 0;
float Dh = 0;

float difh = 0;

float last_h = 30;

float acth = 0;


float mm = spt;
float media[5]={spt,spt,spt,spt,spt};

int posi = 0;
float last_pid = 0;

int passo = 50;

float V=0;

float last_time = 0;

float sph = 90;
  
int tempo = 0;
int j=0;

void setup() {

Serial.begin(9600);  
sensors.setResolution(ds18b20,11);
pinMode(zerocross, INPUT_PULLUP);
pinMode(activate, OUTPUT);
attachInterrupt(0, zero_cross_detect, CHANGE);
dht.begin(); 
Timer1.initialize(freqStep); //Note that this breaks analogWrite() for digital pins 9 and 10 on Arduino
Timer1.attachInterrupt(dim_check, freqStep);
  
  
Serial.println("CLEARSHEET");
Serial.println("LABEL,Tempo Arduino, Tempo PC, Temperatura, tatual, DIM,SP, P, I, D, PID");

motorPasso.setMaxSpeed(500);

while(analogRead(A0)>0){
  
motorPasso.moveTo(-2200);
motorPasso.setSpeed(-50);
motorPasso.runSpeedToPosition();
}

motorPasso.setCurrentPosition(0);
}

void loop() {

byte i;
float celsius;

if (millis() - last_pid > 2000){

Serial.println("CELL,GET,FROMSHEET,Controle,B,1"); // ==> request value from sheet
spt = Serial.readStringUntil(10).toInt(); // get response. Note: the '10' is important! Always use but never change ;-)

celsius = sensors.getTempC(ds18b20); 

media[j]=celsius;
mm = (media[0]+media[1]+media[2]+media[3]+media[4])/5;
j++;
if (j==5){j=0;}

dift = spt - mm;
Pt = dift;
It = It + dift;
Dt = (mm - last_t)/(millis()/1000-last_time);
last_t = mm;
actt = kPt * Pt + 1/kIt * It - kDt*Dt;


if (actt < 0){
  actt = 0;
}
if (actt > 100){
  actt = 100;
}

dim = 128 - potencia(actt);

if (dim<0){
  dim = 0;
}


Serial.println("CELL,GET,FROMSHEET,Controle,B,2"); // ==> request value from sheet
sph = Serial.readStringUntil(10).toInt();

  
float h = dht.readHumidity();
float t = dht.readTemperature();

difh = sph - h;

Ph = difh;
Ih = Ih + difh;
Dh = (h - last_h)/(millis()/1000-last_time);
last_h = h;
last_time = millis()/1000;

posi = 1650 - (kPh * Ph + 1/kIh * Ih - kDh*Dh);

if (posi < 0){
  posi = 0;
}
if (posi > 1650){
  posi = 1650;
}


last_pid = millis();

sensors.requestTemperatures();

//Serial.println( (String) "DATA," + millis() + ",TIMER," + mm + ","+celsius+"," + dim + "," + sp + "," + kP * P + "," +  I/kI + ","+ kD*D +"," + act);
}

motorPasso.moveTo(posi);                                  
motorPasso.setSpeed(100);                          
motorPasso.runSpeedToPosition();
  

  
}
