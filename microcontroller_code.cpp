#Arduino-side code
# C++
#include "Adafruit_LEDBackpack.h"

int to_sum = 0;
bool modifier = false;
#include "PinChangeInterrupt.h"

// Rotary Encoder Inputs
#define CLK1 A1
#define DT1 2
#define CLK2 3
#define DT2 5

int counter1 = 1;
int currentStateCLK1;
int lastStateCLK1;
String currentDir1 = "";

int counter2 = 1;
int currentStateCLK2;
int lastStateCLK2;
String currentDir2 = "";

/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define PAUSE 1000
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_7segment matrix = Adafruit_7segment();
int vars = 5000;
void setup() {
 /* display.setTextWrap(true);
#ifndef __AVR_ATtiny85__

  Serial.println("7 Segment Backpack Test");
#endif
  matrix.begin(0x70);

  delay(1000);
  */
    pinMode(9, INPUT);
    pinMode(12, OUTPUT);

    /*
  // Set encoder pins as inputs
  pinMode(CLK1, INPUT_PULLUP);
  pinMode(DT1, INPUT_PULLUP);
  pinMode(CLK2, INPUT_PULLUP);
  pinMode(DT2, INPUT_PULLUP);
*/
  // Setup Serial Monitor
  Serial.begin(115200);
  Serial.setTimeout(100);
 // Serial.println(vars);
/*
  // Read the initial state of CLK
  lastStateCLK1 = digitalRead(CLK1);
  lastStateCLK2 = digitalRead(CLK2);

  // Call updateEncoder() when any high/low changed seen
  // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachPCINT(digitalPinToPCINT(CLK1), updateEncoder1, CHANGE);
  attachPCINT(digitalPinToPCINT(DT1), updateEncoder1, CHANGE);
  attachPCINT(digitalPinToPCINT(CLK2), updateEncoder2, CHANGE);
  attachPCINT(digitalPinToPCINT(DT2), updateEncoder2, CHANGE);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.fillScreen(0);
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  */
}



int TimeBetweenPulses() {
  int first = 0;
  int second = 0;
  while (digitalRead(9) == LOW) {
  }
  while (digitalRead(9) == HIGH) {
  }
  while (digitalRead(9) == LOW) {
    first = millis();
  }
  while (digitalRead(9) == HIGH) {
  }
  while (digitalRead(9) == LOW) {
    second = millis();
  }
  while (digitalRead(9) == HIGH) {
    return second - first;
  }
}
bool Generate_Binary_Bit() {
  long first_difference = TimeBetweenPulses();
  long second_difference = TimeBetweenPulses();
  if (first_difference != second_difference) {
    return first_difference < second_difference;
  } else return (Generate_Binary_Bit());
}





int generate_dice_length(int max) {
  int i = 0;
  while (true) {
    // Serial.println(2 ^ i);
    // Serial.println(i);
    //  Serial.println(max);
    if (1 << i >= max) {
      return i;
    }
    i++;
  }
}
int RollDn(int size) {
  int present_bit = 0b0;
  int num_bits = 0;
  int max_bits = generate_dice_length(size);
  int x = 0b0;
  int y = 0b0;
  int z = 0b0;

  for (num_bits = 0; num_bits < max_bits; num_bits++) {
    x = (x << 1) | Generate_Binary_Bit();
   //Serial.print(x+1);
  }

        if (x < size) {
      return x+1;
    }
    else return (RollDn(size));
  }

float count = 0;
long rolls_total = 0;
float averange = 0.0000;
int roll = 0;






void display_print() {
  display.setCursor(0, 0);
  display.setTextColor(1);
  display.setTextSize(2);
  display.fillScreen(0);
  display.print("Hello World");
  display.display();
}
int rotation = 1;
int number_added = 0;
int number_of_dice = 1;
int dice = 4;
void check_rotation() {
  if (rotation == 1) {
    dice = 4;
  }
  if (rotation == 2) {
    dice = 6;
  }
  if (rotation == 3) {
    dice = 8;
  }
  if (rotation == 4) {
    dice = 10;
  }
  if (rotation == 5) {
    dice = 12;
  }
  if (rotation == 6) {
    dice = 12;
  }
  if (rotation == 7) {
    dice = 20;
  }
  if (rotation == 8) {
    dice = 100;
  }
}
void keep_track_of_dice_size() {
  if (modifier == false && adv_or_dis() == false) {
    if (currentDir2 == "CW") {
      rotation += 1;
    }
    if (currentDir2 == "CCW") {
      rotation -= 1;
    }
    if (rotation > 8) {
      rotation = 1;
    }
    if (rotation < 1) {
      rotation = 8;
    }
  }
}

void keep_track_of_modifier() {
  if (modifier == true) {
    if (currentDir2 == "CW") {
      number_added += 1;
    }
    if (currentDir2 == "CCW") {
      number_added -= 1;
    }
  }
}

void keep_track_of_dice_number() {
  if (adv_or_dis() == false) {
    if (currentDir1 == "CW") {
      number_of_dice += 1;
    }
    if (currentDir1 == "CCW") {
      number_of_dice -= 1;
    }
    if (number_of_dice < 1) {
      number_of_dice = 1;
    }
  }
}

void updateEncoder1() {
  // Read the current state of CLK1
  currentStateCLK1 = digitalRead(CLK1);

  // If last and current state of CLK1 are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK1 != lastStateCLK1 && currentStateCLK1 == 1) {

    // If the DT1 state is different than the CLK1 state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT1) != currentStateCLK1) {
      counter1--;
      currentDir1 = "CCW";
    } else {
      // Encoder is rotating CW so increment
      counter1++;
      currentDir1 = "CW";
    }

    /*Serial.print("Direction1: ");
    Serial.print(currentDir1);
    Serial.print(" | Counter1: ");
    Serial.println(counter1);
    */
    keep_track_of_dice_number();
  }

  // Remember last CLK1 state
  lastStateCLK1 = currentStateCLK1;
}


void updateEncoder2() {
  // Read the current state of CLK1
  currentStateCLK2 = digitalRead(CLK2);

  // If last and current state of CLK1 are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK2 != lastStateCLK2 && currentStateCLK2 == 1) {

    // If the DT1 state is different than the CLK1 state then
    // the encoder is rotating CCW so decrement
    Serial.println(digitalRead(DT2));
    Serial.println(currentStateCLK2);
    if ((digitalRead(DT2) != currentStateCLK2)) {
      counter2--;
      currentDir2 = "CCW";
    } else {
      // Encoder is rotating CW so increment
      counter2++;
      currentDir2 = "CW";
    }


    keep_track_of_dice_size();
    check_rotation();
    keep_track_of_modifier();
  }

  // Remember last CLK1 state
  lastStateCLK2 = currentStateCLK2;
}

class Button {
public:
  Button(int pin)
    : pin_(pin) {
    pinMode(pin_, INPUT_PULLUP);
  }

  bool CheckIfSwitchPressed() {
    bool new_state = !digitalRead(pin_);
    //Serial.print(new_state);
    /*Serial.println(" ");
    Serial.println(last_state_);
    Serial.println(new_state);
    */
    if ((last_state_ == 0) && (new_state == 1)) {
      last_state_ = new_state;
      // Serial.println("true");
      return true;
    }
    last_state_ = new_state;

    return false;
  }
  bool CheckIfSwitchDePressed() {
    bool new_state = !digitalRead(pin_);
   // Serial.print(last_state_);
   // Serial.print(new_state);
    //Serial.print(new_state);
    /*Serial.println(" ");
    Serial.println(last_state_);
    Serial.println(new_state);
    */
    if ((last_state2_ == 1) && (new_state == 0)) {
      last_state2_ = new_state;
      // Serial.println("true");
      return true;
    }
    last_state2_ = new_state;

    return false;
  }
private:
  int pin_;
  bool last_state_ = false;
  bool last_state2_ = false;
};



//d
Button Advantage(6);
Button Disadvantage(7);
Button Sum(8);
Button Clear(10);
Button Modifier(12);
Button Roll(11);
bool roll_pressed = false;
bool advantage = false;
bool disadvantage = false;
unsigned long sum_total = 0;
bool need_to_roll = false;
void print_switch_state() {




  String State = "";
  bool changed = false;

  if (Roll.CheckIfSwitchPressed() == true) {
    Serial.println("Roll");
    need_to_roll = true;
  }
  if (Advantage.CheckIfSwitchPressed() == true) {
    Serial.println("Advantage");
    advantage = true;
  }
  if (Advantage.CheckIfSwitchDePressed() == true) {
    Serial.println("Advantage");
    advantage = false;
  }
  if (Disadvantage.CheckIfSwitchPressed() == true) {
    Serial.println("Disadvantage");
    disadvantage = true;
  }
  if (Disadvantage.CheckIfSwitchDePressed() == true) {
    Serial.println("Disadvantage");
    disadvantage = false;
  }
  if (Sum.CheckIfSwitchPressed() == true) {
    Serial.println("Sum");
    sum_total += to_sum;
  }
  if (Clear.CheckIfSwitchPressed() == true) {
    Serial.println("Clear");
    sum_total = 0;
    number_added = 0;
  }
  if (Modifier.CheckIfSwitchPressed() == true) {
    Serial.println("Modifier");
    modifier = true;
  }
  if (Modifier.CheckIfSwitchDePressed() == true) {
    Serial.println("Depressed");
    modifier = false;
  }
}
bool adv_or_dis() {
  if ((advantage == true) or (disadvantage == true)) {
    return true;
  }
  return false;
}
void update_display() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.setTextSize(2);

  if (adv_or_dis() == true) {
    if (advantage == true) {
      display.print("ADV");
    }
    if (disadvantage == true) {
      display.print("DIS");
    }
  }

  else {
    display.print(number_of_dice);
    display.print("d");
    display.print(dice);
  }
  if (number_added >= 0) {
    display.print("+");
  }
  display.print(number_added);
  display_sum();
  //display.display();
}

void display_sum() {
  int cursor_x = 0;
  int cursor_y = 0;
  if (sum_total != 0) {
    display.setTextSize(1);
    cursor_x = display.getCursorX();
    display.print(" SUM:");
    display.println("");
    cursor_y = display.getCursorY();
    display.setCursor(cursor_x, cursor_y);
    display.print(" ");
    display.print(sum_total);
  }
}
int rolls[100];
// break if zero
void clear_rolls() {
  for (int i = 0; i < 100; ++i) {
    rolls[i] = 0;
  }
}
int past_number_added = 0;
int first_roll = 0;
int second_roll = 0;
void print_rolls() {
  int total_roll = 0;

  if (adv_or_dis() == true) {
    if (advantage == true) {
  display.println("");
      display.setTextSize(1);
      first_roll = rolls[0];
      second_roll = rolls[1];

          if (first_roll > second_roll) {
            total_roll = first_roll;
          }
         else if (first_roll < second_roll) {
            total_roll = second_roll;
          } else {
            total_roll = second_roll;
          }
          matrix.println(total_roll + past_number_added);
          to_sum = total_roll + past_number_added;
      
        display.print(rolls[0]);
        display.print(" ");
          display.print(rolls[1]);
   //     Serial.print("2disadV");

    }
    if (disadvantage == true) {
      display.println("");
      display.setTextSize(1);
      first_roll = rolls[0];
      second_roll = rolls[1];
Serial.println(first_roll);
Serial.println(second_roll);
          if (first_roll < second_roll) {
            total_roll = first_roll;
          }
          else if (first_roll > second_roll) {
            total_roll = second_roll;
          } else {
            total_roll = second_roll;
          }
          matrix.println(total_roll + past_number_added);
          to_sum = total_roll + past_number_added;
      
        display.print(rolls[0]);
        display.print(" ");
          display.print(rolls[1]);
   //     Serial.print("2disadV");

    }
  } else {
    display.println("");
    display.setTextSize(1);
    for (int i = 0; i < 100; ++i) {
      if (rolls[i] == 0) {
        matrix.println(total_roll + past_number_added);
        to_sum = total_roll + past_number_added;
        break;
      }
      total_roll += rolls[i];
      display.print(rolls[i]);
      display.print(" ");
    }
  }
}
void roll_dice() {
  if ((adv_or_dis() == true) && (need_to_roll == true)) {
    if (advantage == true) {
      Serial.print("adV");
      clear_rolls();
      for (int i = 0; i < 2; ++i) {
        rolls[i] += RollDn(20);
      }
      past_number_added = number_added;
      need_to_roll = false;
      print_rolls();
    }
    if (disadvantage == true) {
      Serial.print("DisadV");
      clear_rolls();
      for (int i = 0; i < 2; ++i) {
        rolls[i] += RollDn(20);
      }
     
      past_number_added = number_added;
      need_to_roll = false;
      need_to_roll = false;
      print_rolls();
    }
  }

  else {
    if (need_to_roll == true) {
      clear_rolls();
      for (int i = 0; i < number_of_dice; ++i) {
        rolls[i] += RollDn(dice);
      }
      past_number_added = number_added;
    }
    print_rolls();
    need_to_roll = false;
  }
}
bool Run = false;
String Binary = "";
int countr = 0;
String x = "";

void loop() {
/*print_switch_state();
  update_display();
  roll_dice();
  display.display();
  matrix.writeDisplay();
  delay(6);
  */

  /*
  Binary += (Generate_Binary_Bit());
  countr += 1;
  if (countr == vars) {
    Serial.println(Binary);
  }
  if (countr <= vars) {
    Serial.println(countr);
  }
  */
  /*
  int Now = millis();
  Serial.println(Generate_Binary_Bit());
  Now = millis() - Now;
  Serial.println(Now);
  */

 //     digitalWrite(12, HIGH);

  if (Serial.available() > 0) {
    x = Serial.readStringUntil('\n');
    x.trim();
     if (x.length() > 0) {  // Only process non-empty strings
      digitalWrite(12, HIGH);
      int index = x.indexOf("d");
      int length = x.length();
      
      int dice_size = (x.substring(index+1, length).toInt());
      int dice_number = (x.substring(0, index).toInt());
      
      String Rolls = "";
    
      for (int i = 0; i < dice_number; ++i) {
   Rolls += (String(RollDn(dice_size)));
   //Rolls += 3;
   if (i != dice_number-1) {
  Rolls += ",";
   }
        //  roll += random(1, dice_size);
      }
    
    //  Serial.println(dice_number);
     // Serial.println(dice_size);
  Serial.println(Rolls);
  digitalWrite(12, LOW);
  }
  }
  


  
}
// built in
