#include <Wire.h>
#include <U8g2lib.h>
#include <bb_scd41.h>

SCD41 mySensor;
//sensor pins 8,9
#define SDA_PIN 8
#define SCL_PIN 9
//here is swi2c screen cuz its safe mode it actually worked with this
// it was sth like pins taking same cpu resources and it was locking up electrically  and this helped
U8G2_SSD1306_72X40_ER_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 6, /* data=*/ 5, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);
  //sec to wake up cuz at the very start its 0
  for(int i=0; i<6; i++) { delay(500); Serial.print("."); }
//turn on sensor and if found send start cmd
  if (mySensor.init(SDA_PIN, SCL_PIN, false, 100000) == SCD41_SUCCESS) {
    mySensor.start(); 
  }
// wake up screen brightness to max
  u8g2.begin();
  u8g2.setContrast(255);
}

void loop() {
  // ask sensor every 5 sec on serial
  if (mySensor.getSample()) {
    int iCO2 = mySensor.co2();
    int iTemp = mySensor.temperature() / 10; //data
    int iHum = mySensor.humidity();
    
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    
    char buf[32];

    // y 10 height , text was running away, here is co2
    sprintf(buf, "CO2: %d", iCO2);
    u8g2.drawStr(2, 10, buf); 

    //here side by side cuz it took too much space screen too small
    sprintf(buf, "T:%dC H:%d%%", iTemp, iHum);
    u8g2.drawStr(2, 22, buf); 
    
    u8g2.sendBuffer();
    //print on serial 
    Serial.printf("NEW READ: CO2: %d, T: %d, H: %d\n", iCO2, iTemp, iHum);
  }
  
  
  delay(1000); 
}