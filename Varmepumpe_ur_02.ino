// For Sensor. Blue wire is ground, Green wire is 5 V and White(Green)  is signal
//  This Arduino sketch reads DS18B20 "1-Wire" digital temperature sensors.
//  Copyright (c) 2010 Mark McComb, hacktronics LLC License: http://www.opensource.org/licenses/mit-license.php (Go crazy)
//  Tutorial: http://www.hacktronics.com/Tutorials/arduino-1-wire-tutorial.html

#include <DS3231.h>    //Ur
#include <Wire.h>      //Ur

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature . 
DallasTemperature sensors(&oneWire);

// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html


const int VP_Pin=8; //Signal udgang til styring af Varmepumpe


DeviceAddress temp_3Thermometer = 
   { 0x10, 0x1D, 0x78, 0xDE, 0x02, 0x08, 0x00, 0x94 }; //Top of Tank
DeviceAddress temp_2Thermometer = 
   { 0x10, 0x02, 0x0A, 0xDE, 0x02, 0x08, 0x00, 0x05 }; // 5-10 cm from top of Tank
DeviceAddress temp_1Thermometer = 
   { 0x10, 0xB6, 0x7B, 0xDE, 0x02, 0x08, 0x00, 0x7E }; //Bottom or 50 cm from top of tank
DeviceAddress temp_4Thermometer = 
   { 0x10, 0x7C, 0x7D, 0xDE, 0x02, 0x08, 0x00, 0xB2 }; // Air temp
int VPLevel=LOW;             //JJG VP slukket
int check=5;
float temp_1=0.1;
float temp_2=0.2;
float temp_3=0.3;
float temp_4=0.4;
float TairStart=17; //Min air temp for start VP
float TairStop=14;  //Air temp where VP stop
float max_retur=45; //normal 40 Celcius

int wait=0; //Not waiting

//Ur definition//
DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
float TimeReset=0;
 

byte year, month, date, DoW, hour, minute, second;


//--------------------------------------------------------------------------------//
void setup(void)

{
	// Start the I2C interface  //* has to be used if "set clock
	 Wire.begin();
        //*  Clock.setSecond(00);//Set the second 
        //*  Clock.setMinute(29);//Set the minute 
        //*  Clock.setHour(21);  //Set the hour 
        //*  Clock.setDoW(2);    //Set the day of the week
        //*  Clock.setDate(6);  //Set the date of the month
        //*  Clock.setMonth(7);  //Set the month of the year
        //*  Clock.setYear(15);  //Set the year (Last two digits of the year)
	
 // Start the serial interface
 Serial.begin(9600);

  // Start up the library
  sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(temp_1Thermometer, 10);
  sensors.setResolution(temp_2Thermometer, 10);
  sensors.setResolution(temp_3Thermometer, 10);
  sensors.setResolution(temp_4Thermometer, 10);
}

//------------------------------------------------------------------------------------------------//
void loop(void)
{ 

     
  temp_1=(sensors.getTempC(temp_1Thermometer));//TEST2
    temp_2=(sensors.getTempC(temp_2Thermometer));//TEST2
    temp_3=(sensors.getTempC(temp_3Thermometer));//TEST2
    temp_4=(sensors.getTempC(temp_4Thermometer));//TEST2  
  
  
  
  
  
   
 if (temp_4 >TairStart) {VPLevel=HIGH;} //temp_4 is Air temp
 if (temp_4 <TairStop) {VPLevel=LOW;}
 if (TimeReset==9) {wait=0;}   //Reset if time 9 to 10
 if (temp_3 >max_retur) {wait=1;}  //Temp i "bund" af tank
 if (wait>0) {VPLevel=LOW;}
 
 
  int second,minute,hour,date,month,year,temperature; 
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  
  Serial.print("20");
  Serial.print(year,DEC);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print(' ');
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  Serial.print(' ');
  Serial.print(temp_1);
  Serial.print(' ');
  Serial.print(temp_2);
  Serial.print(' ');
  Serial.print(temp_3);
  Serial.print(' ');
  Serial.print(temp_4);
  Serial.print(' ');
  Serial.print(VPLevel);
  Serial.print('\n');
  
  TimeReset=minute;

 sensors.requestTemperatures();
       
 digitalWrite(VP_Pin,VPLevel);
   delay(60000);//1000 equel to 1 sec. 
}
 

