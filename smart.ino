#include <DHT.h>
#include <DHT_U.h>



#define INA1 8
#define INA2 13
#define INA3 10
#define INA4 11
#define ENA 9
#define ENB 12
#define front_TRIG 6 //TRIG 핀 설정 (초음파 보내는 핀)
#define front_ECHO 7 //ECHO 핀 설정 (초음파 받는 핀)
#define back_TRIG 4 //TRIG 핀 설정 (초음파 보내는 핀)
#define back_ECHO 5 //ECHO 핀 설정 (초음파 받는 핀)
#define green 3

int dis_front;
int dis_back;
int jodo=0;
int readValue;
int flag = 0;
int start = 0;
int jodo_s,jodo_b;
int PIN = A1; // A0에 센서연결
DHT dht(PIN, DHT11);
void setup()
{
  Serial.begin(9600);
  dht.begin(); //센서 준비
  pinMode(ENA, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(INA3, OUTPUT);
  pinMode(INA4, OUTPUT);
  
  pinMode(front_TRIG, OUTPUT);
  pinMode(front_ECHO, INPUT);
  pinMode(back_TRIG, OUTPUT);
  pinMode(back_ECHO, INPUT);

  pinMode(green,OUTPUT);

}
void humimi()
{
  float temp = dht.readTemperature(); //temp에 온도값 저장
  float humi = dht.readHumidity(); //humi에 습도값 저장
  Serial.print(temp);
  Serial.print(", ");
  Serial.println(humi);
}
int cho_front()
{
  long duration, distance;
  digitalWrite(front_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(front_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(front_TRIG, LOW);
  duration = pulseIn (front_ECHO, HIGH);
  distance = duration * 17 / 1000; 
  return distance;
}
int cho_back()
{
  long duration2, distance2;
  digitalWrite(back_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(back_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(back_TRIG, LOW);
  duration2 = pulseIn (back_ECHO, HIGH);
  distance2 = duration2 * 17 / 1000; 
  return distance2;
}
void straight()//전진
{
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  analogWrite(ENA, 128);
  digitalWrite(INA3, LOW);
  digitalWrite(INA4, HIGH);
  analogWrite(ENB, 128);
}
void back()
{
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  analogWrite(ENA, 128);
  digitalWrite(INA3, HIGH);
  digitalWrite(INA4, LOW);
  analogWrite(ENB, 128);
}
void stopp()
{
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  digitalWrite(INA3, LOW);
  digitalWrite(INA4, LOW);
}
void loop()
{
  humimi();
      dis_front = cho_front();       //거리 cm
    dis_back = cho_back();
    readValue = analogRead(A0); //조도 값
  if(flag == 0)               //베란다 순찰
  {
    if(jodo<readValue){jodo=readValue;}//이전값이 지금값보다 작으면 값 저장
    if(start == 0){straight();start = 1;digitalWrite(green,HIGH);}
    if((dis_front <= 20)&&(start==1))
    { stopp();
      start=2;
      delay(1500);
      back();
    }
    else if((dis_back<=20)&&(start==2))
    {
      stopp();flag = 1;start=3;
      jodo_s = jodo-50;
      jodo_b = jodo+50;    
      delay(1500);
      straight();
    }
  }
  else if(flag == 1)
  {
    readValue = analogRead(A0);
    if((readValue>=jodo_s)&&(readValue<jodo_b))
    {
      stopp();
      flag=2;
    }
    else if(dis_front <= 20)
    {
      stopp();
      flag=2;
      }
  }
  else if(flag==2)
  {
      digitalWrite(green,LOW);
      delay(1000);
      digitalWrite(green,HIGH);
      delay(1000);
  }
  else;
}
