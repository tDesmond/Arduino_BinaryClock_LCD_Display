#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
boolean buttonActive = false;   //check if button being held for long press:
boolean alarmSwitch = false;    //Check if alarm ON or OFF:
boolean bSetMinutes = false;    //Check if minutes the same as actual time:
boolean alarmBeep = false;      //Turn on alarm:
boolean alarmMode = false;      //Check if in alarm mode:
boolean bSetHour = false;       //Detect the alarm hour is the same as the actual time:
boolean goBack = false;         //Go back to display mode from alarm mode:
boolean alarm = false;          //Checks if alarm is on:
long longPressTime = 1000;      //Time the button is held to activate the long press:
long buttonTimer = 0;           //Times how long the button for long press function:
int setMinutes = 0;             //Minutes the alarm is set at:
int setHour = 0;                //Hours the alarm is set at:

void setup() {
	
  // put your setup code here, to run once:
  Serial.begin(9600);           
  
  //Ativate Button Pins:
  pinMode(8, INPUT);            
  pinMode(7, INPUT);
  
  //setting up the lcd:
  lcd.begin(16, 2);             
}

void loop() {
                                                     
  //needed to be a double for the power math function:
  double mValue, hValue;           

  
  // read the input on analog pin 0~5:
  for (int i = 5; i >= 0; i--) {     
    //Values taken from the Analog pins to detect the binary code:                                                
    int sensorValue0 = analogRead(i);                                                 

    //If statements to calcuate the time:
    if (i == 5 && sensorValue0 > 50)                    mValue += pow(2, i);          
    else if (sensorValue0 > 50 && sensorValue0 < 100)   hValue += pow(2, i);
    else if (sensorValue0 > 100 && sensorValue0 < 150)  mValue += pow(2, i);
    else if (sensorValue0 > 150 ) {
      mValue += pow(2, i);
      hValue += pow(2, i);
    }
  }


  
  // Detect if the user does a long or short button press
  if (digitalRead(8) == HIGH) {
    if (buttonActive == false) {                                                      
      // Time the button press
      buttonActive = true;
      buttonTimer = millis();
    }

    // Check if the button was held for longer than the set longPressTime
    if (millis() - buttonTimer > longPressTime && alarmMode == false) {
      if (alarmMode == false) {
        alarmMode = true;
        alarmSwitch = true;
      }
      delay(250);
    }
  } else if (buttonActive == true)buttonActive = false;                               
  //Button not pressed:
  // Convert time to integer value
  String sHours = String(hValue, 0);                                                  
  String sMinutes = String(mValue, 0);                                                
  int hours = sHours.toInt();                                                         
  int minutes = sMinutes.toInt();
  
  // Display time if not in alarm mode
  if (!alarmMode) {

    //Ptints Time in binary on the lcd:
    //calls binary fromatting function to have leading zeroes:
    binaryFormatter(hours); lcd.print(hours, BIN); lcd.print(":");                    
    binaryFormatter(minutes); lcd.print(minutes, BIN);
    lcd.setCursor(15, 0); lcd.print("B");
    lcd.setCursor(0, 1);

    //Calls Funtion to format and print the decimal time:
    timePrint(hours, minutes);                                                        
    lcd.setCursor(15, 1); lcd.print("D");
  }

  // Print Alarm for a alarm mode
  if (alarmMode) {

    //Prints adjustable settings
    lcd.clear();
    lcd.print("Alarm");                                                                 

    lcd.setCursor(0, 1);
    timePrint(setHour, setMinutes);

    lcd.setCursor(10, 0);
    if (alarm)lcd.print("ON");
    else lcd.print("OFF");

    lcd.setCursor(10, 1);
    lcd.print("Back");

    // Moves the the curor to underline action:
    // The cursor underlines the users selection
    if (alarmSwitch)lcd.setCursor(10, 0);                                               
    else if (bSetHour)lcd.setCursor(1, 1);
    else if (bSetMinutes)lcd.setCursor(3, 1);
    else if (goBack)lcd.setCursor(10, 1);
    lcd.cursor();

    if (digitalRead(7) == HIGH) {
      //Calls function to switch bool values to move cursor:
      if (alarmSwitch) boolChange(alarmSwitch, bSetHour);                               
      else if (bSetHour) boolChange(bSetHour, bSetMinutes);
      else if (bSetMinutes)boolChange (bSetMinutes, goBack);
      else if (goBack) boolChange (goBack, alarmSwitch);
    }
    if (digitalRead(8) == HIGH) {
      if (alarmSwitch)alarm = !alarm;
      else if (bSetHour) {
        if (setHour < 23)setHour++;
        else setHour = 0;
      } else  if (bSetMinutes) {
        if (setMinutes < 59)setMinutes++;
        else setMinutes = 0;
      } else if (goBack)alarmMode = false;
    }
  }
  

  // Detect if the alarm goes off
  if (hours == setHour && minutes == setMinutes && alarm) alarmBeep = true;
  else alarmBeep = false;
  if (alarmBeep) {
    lcd.clear();
    lcd.print("Alarm!!!!!!!!!");
    delay(250);
  }
  

  // Turns the alarm off
  if (digitalRead(7) == HIGH && alarmBeep) {
    alarmMode = false;
    alarm = false;
  }
  
  delay(250);
  lcd.clear();
  Serial.println();
}

// This is used to switch the boolean values of two variables
// This is done to switch the cursor position to the next choice in the menu on the UI
void boolChange(boolean &b1, boolean &b2) {
  b1 = false;
  b2 = true;
}

// Formats the time output for digital time
void timePrint(int hour, int minutes) {
  if (hour < 10)lcd.print("0");
  lcd.print(hour); lcd.print(":");
  if (minutes < 10)lcd.print("0");
  lcd.print(minutes);
}

// Formats the time output in binary form
void binaryFormatter(int printDigit) {
  if (printDigit < 2) lcd.print(B0);
  if (printDigit < 4) lcd.print(B0);
  if (printDigit < 8) lcd.print(B0);
  if (printDigit < 16) lcd.print(B0);
  if (printDigit < 32) lcd.print(B0);
}


