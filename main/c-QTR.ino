//Digital Sensor Values ------------------------------------------------------------------------

void qtrRead()
{
  position=qtr.readLineBlack(sensorValues);

  for(int i=0; i< NUM_SENSORS ; i++ )
  {
    if(sensorValues[i] > 500) dval[i] = 1;
    else dval[i] = 0;
  }
}

//Saving Calibration Values ---------------------------------------------------------------------

void qtrSave()
{
  beep(100,100,100,50);
  
  leftCount=0;
  rightCount=0;
  byte percentage = 0;
  lcd.setCursor(0,1);
  
  for(int i=0;i<3;i++)
  {
    leftTurn(150,150);
    while(leftCount <= 300 || rightCount <=300) qtr.calibrate();

    brake();
    percentage += 10;
    refresh(percentage);
    leftCount=0;
    rightCount=0;
    delay(100);
    
    rightTurn(150,150);
    while(leftCount <= 600 || rightCount <=600) qtr.calibrate();

    brake();
    percentage += 10;
    refresh(percentage);
    leftCount=0;
    rightCount=0;
    delay(100);
    
    leftTurn(150,150);
    while(leftCount <= 300 || rightCount <=300) qtr.calibrate();
    brake();
    percentage += 10;
    refresh(percentage);
    leftCount=0;
    rightCount=0;
    delay(100);
  }

  brake();
  //Saving to EEPROM
  
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    EEPROM.write(i,(qtr.calibrationOn.minimum[i]/2));
    EEPROM.write(i+10,(qtr.calibrationOn.maximum[i]/10));
  }

  refresh(100);
  beep(1000);

  cal:
  lcd.clear();
  lcd.print("Calib. Completed");
  lcd.setCursor(0,1);
  lcd.print("BACK        MAIN");

  checkButtonPress();
  if(centerButton) menu();
  else if(rightButton) loop(); //main code

  lcd.clear();
  
  for (int i = 0; i < 4; i++)
  {
    lcd.print(qtr.calibrationOn.minimum[i]);
    lcd.print(" ");
  }

  lcd.setCursor(0,1);
  for (int i = 4; i < 8; i++)
  {
    lcd.print(qtr.calibrationOn.minimum[i]);
    lcd.print(" ");
  }

  delay(5000);
  lcd.clear();

  for (int i = 0; i < 3; i++)
  {
    lcd.print(qtr.calibrationOn.maximum[i]);
    lcd.print(" ");
  }
  
  lcd.setCursor(0,1);
  for (int i = 3; i < 6; i++)
  {
    lcd.print(qtr.calibrationOn.maximum[i]);
    lcd.print(" ");
  }

  delay(3000);
  lcd.clear();

  for (int i = 6; i < 8; i++)
  {
    lcd.print(qtr.calibrationOn.maximum[i]);
    lcd.print(" ");
  }

  delay(3000);
  goto cal;
}

void qtrLoad()
{
  qtr.calibrate();
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    qtr.calibrationOn.minimum[i] = 2*(EEPROM.read(i));
    qtr.calibrationOn.maximum[i] = 10*(EEPROM.read(i+10));
  }
  beep(500);
}

//=====================================================================================================================================
