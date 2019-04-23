#include <PMW3360.h>
/* 
Frame capture (=Camera) mode:
This mode disables navigation and overwrites any downloaded firmware.
A hardware reset is required to restore navigation, and the firmware must be reloaded.

# PIN CONNECTION
  * MI = MISO
  * MO = MOSI
  * SS = Slave Select / Chip Select
  * SC = SPI Clock
  * MT = Motion (active low interrupt line)
  * RS = Reset
  * GD = Ground
  * VI = Voltage in up to +5.5V 

Module   Arduino
  RS --- (NONE)
  GD --- GND
  MT --- (NONE)
  SS --- Pin_10   (use this pin to initialize a PMW3360 instance)
  SC --- SCK 
  MO --- MOSI
  MI --- MISO
  VI --- 5V
 */

#define SS  10   // Slave Select pin. Connect this to SS on the module.

PMW3360 sensor;

void setup() {
  Serial.begin(9600);  
  while(!Serial);
  
  if(sensor.begin(SS))  // 10 is the pin connected to SS of the module.
    Serial.println("Sensor initialization successed");
  else
    Serial.println("Sensor initialization failed");

  // wait for 250 ms for frame capture.
  delay(250);
}

void loop() {
  // The following routine shoud be alwyas performed together.
  // BEGIN -------------------------------------------------
  sensor.prepareImage();
  for(int i=0;i<36*36;i++)
  {
    byte pixel = sensor.readImagePixel();
    Serial.print(pixel, DEC);
    Serial.print(' ');
  }
  sensor.endImage();
  // END ----------------------------------------------------

  delay(50);

  // optional: Surface quality report
  int squal = sensor.readReg(REG_SQUAL);
  Serial.println(squal);

  delay(10);
}

/* Processing code for visualization (http://processing.org to downlaod)

import processing.serial.*;

Serial myPort;
int[] val = null;
boolean sync = false;
int lf = 10;

void setup()
{
  size(360, 380);
  String[] sList = Serial.list();
  String portName = sList[sList.length - 1];
  myPort = new Serial(this, portName, 9600);
}

void draw()
{
  if(myPort.available() > 0)
  {
    String s = myPort.readStringUntil(lf);
    
    if(s != null)
    {
      int[] nums = int(split(trim(s), ' '));
      
      if(nums.length == 36*36+1)
      {
        val = nums;
      }
    }
  }
  
  if(val == null)
  {
    background(0);
    return;
  }
  
  background(0);
 
  noStroke();
  for(int i=0;i<36;i++)
  {
    for(int j=0;j<36;j++)
    {
      int pixel = val[i*36+j];
      fill(pixel);
      rect(i*10, (35-j)*10, 10, 10);
    }
  }
  
  int squal = val[36*36];
  
  fill(255);
  text("SQUAL = "+squal, 10, 375);
}

void keyPressed()
{
  if(key == 's')
    save("frame.png");
}
*/