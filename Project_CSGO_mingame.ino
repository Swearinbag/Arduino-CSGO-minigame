#include <Keypad.h>
#include <LiquidCrystal.h>

//keypad box ---------------------------------------------------------------------------

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {11,10,9,8}; 
byte colPins[COLS] = {5,4,3,2};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// LCD box ---------------------------------------------------------------------

LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);

// Buzzer ----------------------------------------------------------------------



//---------------------------------------------------------------------------------------

int i=5;
int Pos = 0;
int Activated = 0; //0 = Code input, 1 = Defusing, 2 = Endscreen, 3 = replay?, 4 = death
int time = 90;
int timedelay = 1200;
int teller = 0;
float getal = 0;


// legacy code: 7,3,5,5,6,2,0,8

char Code[] = {"********"};
char Gus[] = {"********"};

//---------------------------------------------------------------------------------------

/* ! all serial prints are for debugging ! */

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  
}

void loop(){
  
  char key = keypad.getKey();
  int valueInt = key - 48;   // char to int
   
  
  
 // Bomb setup zone -------------------------------------------------------------------------------------------    
  
 if(Activated == 0){         //input code for defusing
   
  lcd.setCursor(3,0);
  lcd.print("enter code");
   
   if (key != NO_KEY && valueInt != -6 && valueInt != -13){
        Code[Pos] = valueInt;     //add pressed key to Code array
        Pos++;
        if(Pos > 7)
        {             //prepare values for defusing game
          Pos = 0;
//            Serial.println("Start");
            Activated = 1;
            delay(100);
            lcd.clear();
          
        }
    }
    lcd.setCursor(4, 1);
    lcd.print(Code);

  }
  
// defusal zone -------------------------------------------------------------------------------------------  
 if(Activated == 1){
      
   //lcd.clear();
   
  if (key != NO_KEY&& valueInt != -6 && valueInt != -13){ // if key == pressed
//    Serial.println(Code[Pos]);  // check value of Code[Pos]
//    Serial.println(valueInt);  // check value of value
    if(Code[Pos] == valueInt){ 
       Gus[Pos] = key;  // replace * with a number
//    Serial.println("Correct"); // if program makes it in this loop print "Correct"
      Pos++; // next spot in Code array
    }
  }
    
    lcd.setCursor(4, 1);
    lcd.print(Gus);
//    Serial.println(Pos);
    if(Pos > 7){      //if bomb defused
      Activated = 2;
      Pos = 0;
    }
   
   teller = teller + 2 + getal;
   //Serial.println(teller);
  if(time > 0){
    if ( timedelay >= 60 && teller >= 2100 ){   //bomb timer 800 -> 60 with n-20 delay
      tone(12, 4240,120);
      teller = 0;
      getal = getal + 0.5;
      Serial.println(getal);
      time = time - 1;
      lcd.setCursor(7,0);
      lcd.print(time);
      Serial.print(time);
    }   
  }
  if(time <= 0){
    for(int I=0;I<4;I++){
      tone(12, 1000,120);
      delay(300);
      }

    lcd.clear();
    Activated = 4;
    Serial.print("GO");
    
  }
 }
// Succesfull defuse zone -------------------------------------------------------------------------------------------  
  if(Activated == 2){
   
    for(int I=0;I<4;I++){
      tone(12, 1000 +(I*1000),120);
      delay(300);
      }
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("BOMB DEFUSED");
    delay(4000);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("CT's WIN");
    delay(4000);
    Activated = 3;
    lcd.clear();
  }
  // Ending zone -------------------------------------------------------------------------------------------  
  if(Activated == 3){ //ending screen
    
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("PRESS *");
    lcd.setCursor(3, 1);
    lcd.print("TO REPLAY");
    delay(50);
//    Serial.println(valueInt);
    
   if (key != NO_KEY && valueInt != -13){
     if (valueInt == -6){
      Activated = 0;
      lcd.clear();

// RESET VALUES
  i=5;
  Pos = 0;
  Activated = 0;
  time = 90;
  timedelay = 1200;
  teller = 0;
  getal = 0;

  for(int I=0;I<8;I++){

  Code[I] = '*';
  Gus[I] = '*';   
  }
  }
    
 }
}
  
// Failed defusal zone -------------------------------------------------------------------------------------------  
  if(Activated == 4){
  
  lcd.setCursor(3, 0);
  lcd.print("GAME  OVER");
  lcd.setCursor(1, 1);
  lcd.print("Terrorists WIN");
  delay(4000);
  Activated = 3; 
 
  }
}

