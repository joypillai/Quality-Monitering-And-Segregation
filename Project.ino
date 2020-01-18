#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//For Solenoid
int const TANK_SOLENOID=51;
int const OUTLET_SOLENOID=53;

//For TURBIDITY



//For Water Level Sensor
int const WATER_LEVEL_PIN=47;
int LEVEL_OUTPUT=0;


//For Temperature sensor

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

 float Celcius=0;
 float Fahrenheit=0;

// Setup the LiquidCrystal library with the pin numbers we have
// physically connected the module to.
LiquidCrystal lcd(22, 11, 5, 44, 3, 24);

int lastUpdate = 0;
int currentIndex = 0;
char* lyrics[] = {
  " WATER QLTY CHK ",
  "Turbidity:",
  "Temperature     ",
  "Water Level:    "
  
};

void setup() {
  // Setup the number of columns and rows that are available on the LCD.
  lcd.begin(16, 2);

  //For Temperature Sensor
    Serial.begin(9600);
  sensors.begin();
  
  //For Solenoid valve
  pinMode(TANK_SOLENOID,OUTPUT);
  pinMode(OUTLET_SOLENOID,OUTPUT);
  
  //For Water level
  pinMode(WATER_LEVEL_PIN,INPUT);
}

void loop() {
  
 //For Water level 
  
 LEVEL_OUTPUT=digitalRead(WATER_LEVEL_PIN);
 
 delay(200);
 if (LEVEL_OUTPUT==LOW)
 {
  Serial.println("Water level high detected");
 }
 else
 {
  Serial.println("Water level low detected");
  }

  //For Temperature Sensor
  
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  delay(1000);

  //For Turbidity
  int sensorValue = analogRead(A1);
  float voltage = sensorValue * (5.0 / 1024.0);

  Serial.println ("Sensor Output (V):");
  Serial.println (voltage);
  Serial.println();
  delay(100);
  
  // Get the time in milliseconds since the last reset.
  int time = millis();

  // If 800 milliseconds have passed since we last updated
  // the text on the screen, print the next line of the
  // lyrics on the screen.
  if ((time  - lastUpdate) >= 1200)
  {
    // Move the cursor back to the first column of the first row.
    lcd.setCursor(0, 0);

    // If we are writing "Drink all the..." or "Hack all the..."
    // then clear the screen and print the appropriate line.
    if (currentIndex == 0 || currentIndex == 2)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lyrics[currentIndex]);
    }
    else
    {
      // If we are writing the second line, move the cursor there
      // and print the appropriate line.
      lcd.setCursor(0, 1);
      lcd.print(lyrics[currentIndex]);
      lcd.print(Celcius);
      lcd.print(" C");
    }

    // Increment or reset the current index.
    if (currentIndex == 3)
    {
      currentIndex = 0;
    }
    else
    {
      currentIndex += 1;
    }

    // Update the time that we last refreshed the screen to track
    // when to update it again.
    lastUpdate = time;
  }
}
