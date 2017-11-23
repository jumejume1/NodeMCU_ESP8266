#include <ESP8266WiFi.h>
#include <time.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

const char* ssid = "<your wifi ssid>";
const char* password = "<your wifi password>";

int ledPin = 13;

int timezone = 7 * 3600;
int dst = 0;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif



void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  Serial.begin(115200);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.println("Wifi connecting to ");
  display.println( ssid );

  WiFi.begin(ssid,password);
 
  display.println("\nConnecting");

  display.display();

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      display.print("."); 
      display.display();       
  }

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  
  display.println("Wifi Connected!");
  display.print("IP:");
  display.println(WiFi.localIP() );

  display.display();

  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  display.println("\nWaiting for NTP...");

  while(!time(nullptr)){
     Serial.print("*");
     
     delay(1000);
  }
  display.println("\nTime response....OK"); 
  display.display();  
  delay(1000);

  display.clearDisplay();
  display.display();
}

void loop() {
  
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  
  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon + 1);
  Serial.print("/");
  Serial.print(p_tm->tm_year + 1900);
  
  Serial.print(" ");
  
  Serial.print(p_tm->tm_hour);
  Serial.print(":");
  Serial.print(p_tm->tm_min);
  Serial.print(":");
  Serial.println(p_tm->tm_sec);
  
  // Clear the buffer.
  display.clearDisplay();
 
  display.setTextSize(3);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.print(p_tm->tm_hour);
  display.print(":");
  if( p_tm->tm_min <10)
    display.print("0"); 
  display.print(p_tm->tm_min);
  
  display.setTextSize(2);
  display.setCursor(90,5);
  display.print(".");
  if( p_tm->tm_sec <10)
    display.print("0"); 
  display.print(p_tm->tm_sec);

  display.setTextSize(1);
  display.setCursor(0,40);
  display.print(p_tm->tm_mday);
  display.print("/");
  display.print(p_tm->tm_mon + 1);
  display.print("/");
  display.print(p_tm->tm_year + 1900);

  display.display();

  delay(1000); // update every 1 sec

}
