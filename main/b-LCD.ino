//LCD Main Menu --------------------------------------------------------

void menu()
{
  menu:
  lcd.clear();
  delay(50);
  lcd.print("** DOMINATOR **");
  lcd.setCursor(0,1);
  lcd.print("Cal         Main");

  checkButtonPress();
  lcd.clear();
  
  if(rightButton) loop(); //run main code
  
  else if(centerButton) //run calibration
  {
    lcd.print("Calibration...");
    lcd.setCursor(0,1);
    lcd.print("BACK       START");
    checkButtonPress();
    
    if(centerButton) goto menu;
    
    else if(rightButton)
    {
      delay(50);
      lcd.clear();
      lcd.print("Calibrating...");
      qtrSave();
    }
  }
}

//Menu Operations ----------------------------------------------------------------------------

void refresh(float var)
{
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(var);
}

void checkButtonPress()
{
  delay(100);
  centerButton=0, rightButton=0;

  //debouncing check is required
  while(1)
  {
//    if(!digitalRead(leftPin)) 
//    {
//      leftButton = 1;
//      break;
//    }
    if(!digitalRead(centerPin)) 
    {
      centerButton = 1;
      break;
    }
    else if(!digitalRead(rightPin))
    {
      rightButton = 1;
      break;
    }
  }
  delay(100);
}

//Reset function -------------------------------------------------------------------

void(* resetFunc)(void) = 00;

void reset()//ISR for reset
{
  start(); //turning off all the functions
  beep(0);
  light(' ');
  resetFunc();
}
