#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

const char* ssid     = "ssid"; // подставить название своей сети WiFi
const char* password = "password"; //подставить пароль свой сети WiFi

const char* remote_host_ya = "www.ya.ru"; // первый адрес для пинга
const char* remote_host_go = "www.google.com"; // второй адрес для пинга
// const IPAddress remote_ip(87, 250, 250, 242); // адрес для пинга
unsigned long previousMillis = 0;
const long interval = 300;     //интервал между пингами в сек
const long T_ROUTER = 200;     // время ожидания загрузки роутера в сек
const long T_RELAY = 10;       // задержка от выключения реле до включения в сек

int PIN_RELAY = 0;
int PIN_LED = 2;
int COUNT_PING = 0;         //количество попыток при отсутствия пинга (не задается)
int COUNT_PING_MAX = 2;     //максимальное количество отсутствия пинга до перезагрузки роутера
int COUNT_WIFI = 0;         //счетчик попыток подключения к WiFi
int COUNT_WIFI_MAX = 120; //300;   // Таймаут до перезагрузки роутера при отсутствии WiFi в сек

void setup()
{ 
  pinMode(PIN_RELAY, OUTPUT);  // Назначаем первый пин выходом
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH);  // Для управления используем высокий уровень, используем режим реле "нормально открытое"
  // Зажигаем светодиод до тех пор, пока не будет достигнуто успешное подключение по Wi-Fi
  digitalWrite(PIN_LED, LOW);
  Serial.begin(115200);
  delay(3000);
}

void ConnectWIFI()
// Пока вай-фай не подключен
//   Если количество попыток подключений меньше или равно максимальному количеству
//     Увеличиваем количество попыток на единицу
//   Иначе
//     Перезагружаем роутер
{
  Serial.println("Подключаюсь к WiFi"); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 
   while (WiFi.status() != WL_CONNECTED) 
   {
    if (COUNT_WIFI <= COUNT_WIFI_MAX)
     {
      COUNT_WIFI++;  
     }
    else
     {
      Serial.println();
      Relay();  //перезагружаемся так как нет подключения к WiFi
      Serial.println("Подключаюсь к WiFi"); 
     }
    delay(1000);
    Serial.print(".");
    }
  COUNT_WIFI = 0; // сбрасываем счетчик попыток подключения к WiFi  
  Serial.println();
  Serial.print("WiFi подключен, ip : ");  
  Serial.println(WiFi.localIP());
  digitalWrite(PIN_LED, HIGH); // Тушим светодиод
  COUNT_PING = 0;   // сбрасываем счетчик неуспешных пингов
}

void Relay()
// Если на первом пине высокий уровень (режим реле нормально открытое)
//   Разрываем цепь NO
{
  if (digitalRead(PIN_RELAY) == HIGH)
  {
    digitalWrite(PIN_RELAY, LOW);  //отключаем питание на розетке
    Serial.println("Реле Выключено");
  }
  delay(T_RELAY*1000);
  digitalWrite(PIN_RELAY, HIGH); //включаем питание на розетке
  Serial.println("Реле Включено, ждем загрузки роутера");
  delay(T_ROUTER*1000); // ждем загрузки роутера
  COUNT_PING = 0;   // сбрасываем счетчик неуспешных пингов
  COUNT_WIFI = 0; // сбрасываем счетчик попыток подключения к WiFi
  previousMillis = 0;
}

void loop() 
{ 
  if (WiFi.status() != WL_CONNECTED)  // нет подключения к WiFi
  {
    digitalWrite(PIN_LED, LOW);
    ConnectWIFI(); //подключаемся к WiFi
  }
  unsigned long currentMillis = millis();
  const long X = currentMillis - previousMillis;
  // Условие: Интервал между пингами равен заданному И реле нормально открытое И подключен Wi-Fi
  if (((currentMillis - previousMillis) >= interval*1000) && (digitalRead(PIN_RELAY) == HIGH) && (WiFi.status() == WL_CONNECTED))
  {
    previousMillis = currentMillis;  
    Serial.println("Попытка пинга: ");
    Serial.println(remote_host_ya);
    Serial.println(remote_host_go);
    // Serial.println(remote_ip);
  // Если есть пинг на яндекс ИЛИ гугл
  if((Ping.ping(remote_host_ya)) || (Ping.ping(remote_host_go)))
  // if (Ping.ping(remote_ip))  
   { //пинг есть - моргнули светодиодом
    Serial.println("Пинг есть!!");
    digitalWrite(PIN_LED, LOW);
    delay(2000);
    digitalWrite(PIN_LED, HIGH);
    COUNT_PING = 0;// сбрасываем счетчик неуспешных пингов
   } else 
   {  // пинга нет
    Serial.print("Пинг отсутствует :( ");
    COUNT_PING ++;
    Serial.println(COUNT_PING);
   }
  }
  if (COUNT_PING >= COUNT_PING_MAX) // если превышено количество попыток пинга перезагружаем роутер
  {
     Relay();
  }
  }