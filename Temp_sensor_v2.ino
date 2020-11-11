#include <Wire.h>

const int tmp_address = 0x48;
const int tmp_reg = 0x00;
const int tmp_12bit = 0x60;
const int tmp_config = 0x01;

unsigned int temp_raw;
double temp;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Wire.begin();  
  init_temp(); 
}

void loop() {
  // put your main code here, to run repeatedly:

  get_temp();
  delay(1000);
  Serial.println(temp);
}


void init_temp()
{
  Wire.beginTransmission(tmp_address);  
  Wire.write(tmp_config);
  Wire.write(tmp_12bit);
  Wire.endTransmission();
  Wire.beginTransmission(tmp_address); 
  Wire.write(tmp_reg);
  Wire.endTransmission();
}

void get_temp()
{ 
  Wire.requestFrom(tmp_address,2);
  
  while(Wire.available()){
    temp = temp_calculation(Wire.read()<<4 | Wire.read()>>4);
    
  }
}

double temp_calculation(unsigned int temp_raw)
{

  /*Serial.println(temp_raw);*/
  unsigned int temp_raw_new;
  double temperature;
  if (temp_raw&0x800)
  {

//    Serial.println("before conversion");
//    Serial.println(temp_raw);
    
    //temp_raw = 0xFFF-temp_raw;
//    temp_raw_new<<4;
//    temp_raw_new>>4;

    temp_raw_new = ~temp_raw+1;
    temp_raw_new = temp_raw_new&0x0FFF;

//    Serial.println("after conversion");
//    Serial.println(temp_raw_new);
    
    temperature = temp_raw_new*0.0625*-1;
  }
  else{ 
    temperature = (temp_raw&0x7FF)*0.0625;
  }    
  return temperature;
}
