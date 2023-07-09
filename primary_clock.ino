#include "iarduino_RTC-2.0.0/src/iarduino_RTC.h"
#include "GyverTM1637.h"
#include "EEPROM.h"

/* 7, 8, 9 - пины RST, CLK, DAT*/
/* 2, 3 - пины CLK, DIO */

enum pin_map : uint8_t 
{
  EN1 = 4,
  IN1 = 6,
  IN2 = 5,
  RTC_rst = 7,
  RTC_clk = 8,
  RTC_dat = 9,
  TM1637_clk = 2,
  TM1637_dio = 3,
  KEY1 = 12,
  KEY2 = 11
};

int min = 0;
iarduino_RTC timer(RTC_DS1302, RTC_rst, RTC_clk, RTC_dat);
GyverTM1637 display_clock(TM1637_clk, TM1637_dio);

void set_EN(bool);
void change_hours();
void change_minutes();

bool polarity = true;
bool 
  value_key1 = false,
  value_key2 = false;

void setup() 
{
  Serial.begin(9600);

  pinMode(KEY1, INPUT_PULLUP); // перевод минут
  pinMode(KEY2, INPUT_PULLUP); // перевод часа
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN1, OUTPUT);

  digitalWrite(EN1, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  delay(20);
  
  timer.begin();
  timer.settime(0, 00, 12, 9, 7, 23, -1);

  display_clock.clear();        // очистить
  display_clock.brightness(7);  // яркость 0-7
  display_clock.point(1);
  //display_clock.displayClock(18, 33);  // вывести часы и минуты
  min = timer.minutes;
/*
  Serial.println( timer.seconds ); // выводим количество секунд
  Serial.println( timer.minutes ); // выводим количество минут
  Serial.println( timer.hours   ); // выводим количество часов в 12 часовом формате
  Serial.println( timer.Hours   ); // выводим количество часов в 24 часовом формате
  Serial.println( timer.midday  ); // выводим 0 (до полудня), или 1 (после полудня)
  Serial.println( timer.day     ); // выводим текущий день месяца
  Serial.println( timer.weekday ); // выводим текущий день недели: 0 (воскресение), 1 (понедельник), ... , 5 (пятница), 6 (суббота)
  Serial.println( timer.month   ); // выводим текущий месяц
  Serial.println( timer.year    ); // выводим текущий год
  Serial.println( min    ); // выводим количество секунд прошедших с начала эпохи Unix
  */
}

void loop() 
{
  timer.gettime();
  //change_minutes();
  //delay(1000);
  
  int 
    is_key_pressed1 = !digitalRead(12),
    is_key_pressed2 = !digitalRead(11);

  if(is_key_pressed1 && !value_key1)
  {
    delay(10);
    //Serial.println("Левая кнопка нажата");
    value_key1 = true; 

    //действие по переводу минуты
    if(timer.minutes == 59)
      timer.minutes = 0;
    else
      timer.minutes += 1;  
    timer.settime(-1, timer.minutes);
  }
  if(!is_key_pressed1 && value_key1)
  {
    delay(10);
    //Serial.println("Левая кнопка отпущена");
    value_key1 = false;
  }

  if(is_key_pressed2 && !value_key2)
  {
    delay(10);
    //Serial.println("Правая кнопка нажата");
    value_key2 = true;

    //действие по переводу часа 
    timer.settime(-1, -1, timer.hours + 1 );
  }
  if(!is_key_pressed2 && value_key2)
  {
    delay(10);

    //Serial.println("Правая кнопка отпущена");
    value_key2 = false;
  }
  if(timer.minutes > min)
  {
    change_minutes();
    min = timer.minutes;
  }
  display_clock.displayClock(timer.hours, timer.minutes);  // вывести часы и минуты
  
  //EEPROM.update(1, timer.minutes);
  //EEPROM.update(2, timer.Hours);
}
void change_minutes()
{
  polarity = !polarity;

  digitalWrite(IN1, polarity);
  digitalWrite(IN2, !polarity);
  delay(10);

  digitalWrite(EN1, HIGH);
  delay(500);
  digitalWrite(EN1, LOW);
}
void change_hours()
{
  for(int i = 0; i < 60; i++)
  {
    change_minutes();
    delay(500);
  }
}


