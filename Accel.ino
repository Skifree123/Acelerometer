/*
TODO LIst

Covert varibles to arrays
Change printing srtings
Change functon to have initial varibles 
 
 */
#include <SPI.h>
#include <SD.h>
#include <Wire.h> // Must include Wire library for I2C
#include <SFE_MMA8452Q.h> // Includes the SFE_MMA8452Q library

MMA8452Q accel;
const int chipSelect = 10;

File dataFile;

float AArrayPositionX[10];
float AArrayPositionY[10];
float AArrayPositionZ[10];

float A1X[8];
float A1Y[8];
float A1Z[8];

float A2X[8];
float A2Y[8];
float A2Z[8];

float AV1X[8];
float AV1Y[8];
float AV1Z[8];

float AV2X[8];
float AV2Y[8];
float AV2Z[8];

String dataStringX;
String dataStringY;
String dataStringZ;

float AArrayPrintX = 0;
float AArrayPrintY = 0;
float AArrayPrintZ = 0;
  
int ArrayNumber = 0;

float DelayTime = 100;

float PositionFaultRange = 1.9; //Will need to play with in the future

String letterSign = A; 

void setup() {
  Serial.begin(9600);
  Serial.println("MMA8452Q Test Code!");
  accel.init();


  // Open serial communications and wait for port to open:
  //Serial.begin(9600);

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");

  // Open up the file we're going to log to!
  dataFile = SD.open("datjalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }
}
void A() {

  if (accel.available()) {
    A1X = A2X;
    A1Y = A2Y;
    A1Z = A2Z;

    //A1X = A1X - 9.81;
    //A1Y = A1Y - 9.81;
    //A1Z = A1Z - 9.81;

    accel.read();
    A2X = accel.cx;
    A2Y = accel.cy;
    A2Z = accel.cz; 

    float ATemp1 = ((A2X * DelayTime + AV1X) * DelayTime); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!CHECK
    if ( ATemp1 < PositionFaultRange && ATemp1 > PositionFaultRange * -1) {
      AArrayPositionX[ArrayNumber] = AArrayPositionX[ArrayNumber - 1];
    } else AArrayPositionY[ArrayNumber] = ATemp1;

    float ATemp2 = ((A2Y * DelayTime + AV1Y) * DelayTime); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!CHECK
    if ( ATemp2 < PositionFaultRange && ATemp2 > PositionFaultRange * -1) {
      AArrayPositionY[ArrayNumber] = AArrayPositionY[ArrayNumber - 1];
    } else AArrayPositionY[ArrayNumber] = ATemp2;

    float ATemp3 = ((A2Z * DelayTime + AV1Z) * DelayTime); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!CHECK
    if ( ATemp3 < PositionFaultRange && ATemp3 > PositionFaultRange * -1) {
      AArrayPositionZ[ArrayNumber] = AArrayPositionZ[ArrayNumber - 1];
    } else AArrayPositionZ[ArrayNumber] = ATemp3;
  }
}

void loop() {
  A();
  
    dataStringX = AArrayPrintX;
    dataStringY = AArrayPrintY;
    dataStringZ = AArrayPrintZ;
    
    Serial.print("    AX ");
    Serial.print(AArrayPrintX);
    Serial.print(",    AY ");
    Serial.print(AArrayPrintY);
    Serial.print(",    AZ ");
    Serial.println(AArrayPrintZ);

    // make a string for assembling the data to log:

    dataFile.print("    AX ");
    dataFile.print(AArrayPrintX);
    dataFile.print(",    AY ");
    dataFile.print(AArrayPrintY);
    dataFile.print(",    AZ ");
    dataFile.println(AArrayPrintZ);
    
  // print to the serial port too:
  // The following line will 'save' the file to the SD card after every
  // line of data - this will use more power and slow down how much data
  // you can read but it's safer!
  // If you want to speed up the system, remove the call to flush() and it
  // will save the file only every 512 bytes - every time a sector on the
  // SD card is filled with data.

  dataFile.flush();

  delay(DelayTime);

  ArrayNumber = ArrayNumber + 1;
}
