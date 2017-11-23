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
  int r = 35;
  // Now draw the clock face 
  
  display.drawCircle(display.width()/2, display.height()/2, 2, WHITE);
  //
  //hour ticks
  for( int z=0; z < 360;z= z + 30 ){
  //Begin at 0° and stop at 360°
    float angle = z ;
    
    angle=(angle/57.29577951) ; //Convert degrees to radians
    int x2=(64+(sin(angle)*r));
    int y2=(32-(cos(angle)*r));
    int x3=(64+(sin(angle)*(r-5)));
    int y3=(32-(cos(angle)*(r-5)));
    display.drawLine(x2,y2,x3,y3,WHITE);
  }
  // display second hand
  float angle = p_tm->tm_sec*6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  int x3=(64+(sin(angle)*(r)));
  int y3=(32-(cos(angle)*(r)));
  display.drawLine(64,32,x3,y3,WHITE);
  //
  // display minute hand
  angle = p_tm->tm_min * 6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(64+(sin(angle)*(r-3)));
  y3=(32-(cos(angle)*(r-3)));
  display.drawLine(64,32,x3,y3,WHITE);
  //
  // display hour hand
  angle = p_tm->tm_hour * 30 + int((p_tm->tm_min / 12) * 6 );
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(64+(sin(angle)*(r-11)));
  y3=(32-(cos(angle)*(r-11)));
  display.drawLine(64,32,x3,y3,WHITE);

  display.setTextSize(1);
  display.setCursor((display.width()/2)+10,(display.height()/2) - 3);
  display.print(p_tm->tm_mday);
  
   // update display with all data
  display.display();
  delay(100);
  display.clearDisplay();

}
