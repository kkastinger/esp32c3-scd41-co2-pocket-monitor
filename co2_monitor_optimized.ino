#include <Wire.h>
#include <U8g2lib.h>
#include <bb_scd41.h>

SCD41 mySensor;
#define SDA_PIN 8 //sensor pins 8,9
#define SCL_PIN 9
//here is swi2c screen cuz its safe mode it actually worked with this
// it was sth like pins taking same cpu resources and it was locking up electrically and this helped
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 6, /* data=*/ 5, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  if (mySensor.init(SDA_PIN, SCL_PIN, false, 100000) == SCD41_SUCCESS) {
    mySensor.start(); 
    u8g2.begin();
    u8g2.setContrast(255);
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.setCursor(30, 27);
    u8g2.printf("Starting...");
    u8g2.sendBuffer();
    delay(1000);
  }  
}

void loop() {
  // ask sensor every 5 sec on serial and print on display
  if (mySensor.getSample()) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB10_tf);  
    char buf[32]; // fixed buffer size to prevent crashes
    sprintf(buf, "CO2: %d", mySensor.co2());
    u8g2.drawStr(30, 24, buf); 
    u8g2.setFont(u8g2_font_helvB08_tf);
    sprintf(buf, "Temp: %ldC", mySensor.temperature()/10);
    u8g2.drawStr(30, 38, buf); 
    sprintf(buf, "Humid: %d%%", mySensor.humidity());
    u8g2.drawStr(30, 50, buf);     
    u8g2.sendBuffer();  } 
  delay(2000); 
}