//Coming to the juction and take a turn -------------------------------------------------------------------

void turn() //Implicit turnings
{
  setLineFollow(manner);
  left=0;
  right=0;
  
  while(1)
  {
    qtrRead();
    if(dval[0] || dval[7])
    {
      if(dval[7]) left=1;
      if(dval[0]) right=1;
       
      centerAtJunction();
      
      if(left && right)
      {
        Tjunction=1;
        brake();
        beep(100,100,100,50);
        break;
      }
      
      else if(left) turnAngle(-90);
        
      else turnAngle(90);
      
      ontoLine(100);
      brake();
      break;
    }
    else lineFollow();
  }  
}

void turn(char input) //Explicit turnings
{
  setLineFollow(manner);
  switch(input)
  {
    case 'L': //Go forward and Turn Left
    while(1)
    {
      qtrRead();
      if(dval[7])
      { 
        centerAtJunction();
        brake('B');
        turnAngle(-90);
        brake();
        break;
      }
      else lineFollow();
    }
    break;

    case 'R': //Go forward and turn right
    while(1)
    {
      qtrRead();
      if(dval[0])
      { 
        centerAtJunction();
        brake('B');
        turnAngle(90);
        brake();
        break;
      }
      else lineFollow();
    }
    break;

    case 'B': //turn back
    turnAngle(180);
    //ontoLine(100);
    brake();
    break;  
  }
}

//Coming to the T the juction

void ontoT()
{
  setLineFollow(manner);
  while(1)
  {
    qtrRead();
    if(dval[0] && dval[7])
    {
      brake('B');
      beep(100,100,100,50);
      break;
    }
    lineFollow();
  }
}

void ontoL()
{
  setLineFollow(manner);
  while(1)
  {
    qtrRead();
    if(dval[0] || dval[7])
    {
      brake('B');
      break;
    }
    lineFollow();
  }
}

//Getting onto the line using PID

void ontoLine(int val)
{
  setLineFollow(manner);
  leftCount=0;
  rightCount=0;
  while(leftCount <= val || rightCount <= val) lineFollow();
  leftCount=0;
  rightCount=0;
}

//Centering at the junction

void centerAtJunction()
{
  forward();
  setEncoderPID('S');
  while(leftCount < 350) 
  {
    qtrRead();
    if(dval[7]) left=1;
    if(dval[0]) right=1;
    encoderPID();
  }
  
  qtrRead();
  if(dval[0] || dval[1] || dval[2] || dval[3] || dval[4] || dval[5] || dval[6] || dval[7]) straight=1;
  
  //brake('B'); //instead of braking at every junction
  //--------------------------
  if(left)
  {
    brake('B');
    delay(100);
  }

  else if(straight);

  else if(right)//else
  {
    brake('B');
    delay(100);
  }
  //-------------------------
  
  leftCount=0;
  rightCount=0;
  
}

//=======================================================================

//Turnings at Degrees ---------------------------------------------------

void encoderTurnBack()
{
  //420 turns required for 90 degree angle
  leftTurn();
  
  setEncoderPID('X');
  while(leftCount < 200) encoderPID();

  setEncoderPID('F');
  while(leftCount < 400) encoderPID();
  
  setEncoderPID('M');
  while(leftCount < 600) encoderPID();
  
  setEncoderPID('S');
  while(leftCount < 840) encoderPID();

  brake('R');
}

void turnAngle(int angle)
{
  //420 turns required for 90 degree angle
  
  char Mode;
  //bool complete=0;
  
  switch(angle)
  {
    case -90: //anticlockwise 90 degree
    Mode='R';
    leftTurn();
    setEncoderPID('X');
    while(leftCount < 50) encoderPID();
    break;
  
    case 90: //clockwise 90 degree
    Mode='L';
    rightTurn();
    setEncoderPID('X');
    while(leftCount < 50) encoderPID();
    break;

    case 180: //clockwise 90 degree
    Mode='L';
    rightTurn();
    setEncoderPID('X');
    while(leftCount < 200) encoderPID();
    break;
  }
  
  setEncoderPID('F');
  while(leftCount < 100) encoderPID();
  
  setEncoderPID('M');
  while(leftCount < 200) encoderPID();
  
  setEncoderPID('S');
  
  //while(!complete)
  while(1)
  {
    qtrRead();
    if(dval[3] || dval[4])
    {
      brake(Mode);
      //complete=1;
      break;
    }
    encoderPID();
  }

//  //aligning the bot
//  qtrRead();
//  while(!(dval[3] || dval[4]))
//  {
//    if(position < 3000) leftTurn(90,90);
//    else if(position > 4000) rightTurn(90,90);
//    qtrRead();
//  }
//  brake();

  leftCount=0;
  rightCount=0;
}

//===================================================================

//Basic Motion Functions --------------------------------------------

void start()
{
  digitalWrite(rightMotorPWM, LOW);
  digitalWrite(leftMotorPWM, LOW);

  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

void brake(char input) //sharp brakes
{
  switch(input)
  {
    case 'B': //backward brake
    backward(255,255);
    delay(brakeTime);
    brake();
    break;
    
    case 'R': //right brake
    rightTurn(255,255);
    delay(brakeTime);
    brake();
    break;
    
    case 'L': //left brake
    leftTurn(255,255);
    delay(brakeTime);
    brake();
    break;
    
    case 'F': //forward brake
    forward(255,255);
    delay(brakeTime);
    brake();
    break;

    default: //active brake
    brake();
    break;
  }
}

void brake() //active brake
{
  digitalWrite(rightMotorPWM, LOW);
  digitalWrite(leftMotorPWM, LOW);

  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, HIGH);
}


void motorSpeed(bool state)
{
  digitalWrite(leftMotorPWM, state);
  digitalWrite(rightMotorPWM, state);
}

void motorSpeed(byte leftSpeed, byte rightSpeed)
{
  analogWrite(leftMotorPWM, leftSpeed);
  analogWrite(rightMotorPWM, rightSpeed);
}

void forward()
{
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorForward, HIGH);
}

void forward(int LSpeed, int RSpeed)
{
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorForward, HIGH);

  analogWrite(leftMotorPWM, LSpeed);
  analogWrite(rightMotorPWM, RSpeed);
}

void backward()
{
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorBackward, HIGH);
}

void backward(int LSpeed, int RSpeed)
{
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorBackward, HIGH);

  analogWrite(leftMotorPWM, LSpeed);
  analogWrite(rightMotorPWM, RSpeed);
}

void leftTurn()
{
  digitalWrite(rightMotorBackward, LOW);
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorForward, HIGH);
}

void leftTurn(int LSpeed, int RSpeed)
{
  digitalWrite(rightMotorBackward, LOW);
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorForward, HIGH);

  analogWrite(leftMotorPWM, LSpeed);
  analogWrite(rightMotorPWM, RSpeed);
}

void rightTurn()
{
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, HIGH);
  digitalWrite(leftMotorForward, HIGH);
}

void rightTurn(int LSpeed, int RSpeed)
{
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, HIGH);
  digitalWrite(leftMotorForward, HIGH);
  
  analogWrite(leftMotorPWM, LSpeed);
  analogWrite(rightMotorPWM, RSpeed);
}

//===========================================
