#include "iarduino_RTC-2.0.0/src/iarduino_RTC.h"
#include "GyverTM1637.h"
#include "EEPROM.h"

/* 7, 8, 9 - пины RST, CLK, DAT*/
iarduino_RTC timer(RTC_DS1302, 7, 8, 9);
GyverTM1637 display_clock(2, 3);

int min = 0;

enum pins : uint8_t 
{
  IN1 = 6,
  IN2 = 5,
};

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

  pinMode(12, INPUT_PULLUP); // перевод минут
  pinMode(11, INPUT_PULLUP); // перевод часа

  set_IN(LOW);
  
  
  delay(20);
  
  timer.begin();
  timer.settime(0, 21, 01, 30, 6, 23, 5);
  //change_minutes();

  display_clock.clear();        // очистить
  display_clock.brightness(7);  // яркость 0-7
  display_clock.point(1);
  //display_clock.displayClock(16, 37);  // вывести часы и минуты

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
/* Обработка лицевых кнопок*/
  int 
    is_key_pressed1 = !digitalRead(12),
    is_key_pressed2 = !digitalRead(11);

  if(is_key_pressed1 && !value_key1)
  {
    delay(10);
    Serial.println("Левая кнопка нажата");
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
    Serial.println("Правая кнопка нажата");
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

  //блок вывода на экран
  //change_minutes();
  //delay(3000);
  //change_hours();

  /*
  if(millis() % 5000 == 0)
  {
    change_minutes();
  }
  */
  if(timer.minutes > min)
  {
    change_minutes();
    min = timer.minutes;
  }
  display_clock.displayClock(timer.hours, timer.minutes);  // вывести часы и минуты

  //EEPROM.update(1, timer.minutes);
  //EEPROM.update(2, timer.Hours);
}
void set_IN(bool state)
{
  digitalWrite(IN1, state);
  digitalWrite(IN2, state);
}

void change_minutes()
{
  //set_EN(HIGH);
  //delay(100);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(100);

  digitalWrite(IN1, polarity);
  digitalWrite(IN2, !polarity);
  
  //delay(100);
  
  //digitalWrite(LPWM_pin, LOW);
  //digitalWrite(RPWM_pin, LOW);
  
  polarity = !polarity;
  
  //set_EN(LOW);
  //delay(100);
}
void change_hours()
{
  for(int i = 0; i < 60; i++)
  {
    change_minutes();
    delay(500);
  }
  delay(5000);
}


