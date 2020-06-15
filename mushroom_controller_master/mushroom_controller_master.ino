//Include the LCD Library.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

//pin anlog of AMT1001
int RHpin = A0;
int RTpin = A2;

//set default of sensor
float RH = 0;
float RT = 0;

//pin digital of Relay
int water = 12;
int fan = 13;

const int buttonPin1 = 2;     // Charachter change button
const int buttonPin2 = 3;     // Move button
const int buttonPin3 = 4;     // Reset screen button

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

//set T and H
float set1RH = 70;
float set2RH = 80;
float set1RT = 25;
float set2RT = 32;

//for display page
int Display = 0;

void setup() {
  Serial.begin(9600); //for monitoring in computer
  pinMode(water, OUTPUT);
  pinMode(fan, OUTPUT);
  //init LCD
  lcd.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power on");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Starting");
  lcd.setCursor(0, 1);
  lcd.print("System....");
  delay(3000);
  lcd.setCursor(0, 0);
  lcd.print("Press Key 1 Red");
  lcd.setCursor(0, 1);
  lcd.print("To Display   ");
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

}

void loop() {

  //read button
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  //read value of Hum and Temp
  RH = analogRead(RHpin);
  RT = analogRead(RTpin);
  RT = (((RT * 5) / 1023) * 80) / 0.8;
  RH = (((RH * 5) / 1023) * 100) / 3;
  Serial.print("humidity: ");
  Serial.print(RH);
  Serial.print(" %");
  Serial.print("\t\t");
  Serial.print("temperature: ");
  Serial.print(RT);
  Serial.println(" C");


  //one push to change page (Display)
  if (buttonState1 == 0) {
    delay(500);                        // delay to debounce switch
    Display ++;
    if (Display > 5) {
      lcd.clear();
      Display = 1;
    }
  }
  switch (Display) {
    //show realtime humidity and temperature
    case 1: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Humi: ");
        lcd.print((int)RH); //print value cast int
        lcd.print(" %");
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print((int)RT);
        lcd.print(" C");
        // condition for active pump and fan
        if (RT > set2RT) {
          digitalWrite(fan, 0);
        } else if (RT < set1RT) {
          digitalWrite(fan, 1);
        }
        if (RH < set1RH) {
          digitalWrite(water, 0);
        } else if (RH > set2RH) {
          digitalWrite(water, 1);
        }

        //if push key3 will show current setting Hum and Temp
        if (buttonState3 == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Limit H: ");
          lcd.print((int)set1RH);
          lcd.print("-");
          lcd.print((int)set2RH);
          lcd.print(" %");
          lcd.setCursor(0, 1);
          lcd.print("Limit T: ");
          lcd.print((int)set1RT);
          lcd.print("-");
          lcd.print((int)set2RT);
          lcd.print(" C");
          delay(4000); //show time 4 sec
        }
        break;
      }
    //page for setting min humidity
    case 2: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Set Humi: ");
        lcd.print((int)set1RH);
        lcd.setCursor(10, 1);
        lcd.print((int)set2RH);
        if (buttonState3 == 0) { //push key3 for up
          set1RH++;
        }
        if (buttonState2 == 0) { //push key3 for donw
          set1RH--;
        }
        break;
      }
    //page for setting max humidity
    case 3: {
        lcd.clear();
        lcd.setCursor(10, 0);
        lcd.print((int)set1RH);
        lcd.setCursor(0, 1);
        lcd.print("Set Humi: ");
        lcd.print((int)set2RH);
        if (buttonState3 == 0) {
          set2RH++;
        }
        if (buttonState2 == 0) {
          set2RH--;
        }
        break;
      }
    //page for setting min temperature
    case 4: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Set Temp: ");
        lcd.print((int)set1RT);
        lcd.setCursor(10, 1);
        lcd.print((int)set2RT);
        if (buttonState3 == 0) {
          set1RT++;
        }
        if (buttonState2 == 0) {
          set1RT--;
        }
        break;
      }
    //page for setting max temperature
    case 5: {
        lcd.clear();
        lcd.setCursor(10, 0);
        lcd.print((int)set1RT);
        lcd.setCursor(0, 1);
        lcd.print("Set Temp: ");
        lcd.print((int)set2RT);
        if (buttonState3 == 0) {
          set2RT++;
        }
        if (buttonState2 == 0) {
          set2RT--;
        }
        break;
      }
  }
  delay(150);//round to process
}
