//Line Following: Speed Settings --------------------------------------------------------------

void setLineFollow(char manner)
{
  forward();
  
  lastError = 0;
  integral=0;

  switch(manner)
  {
    case 'S'://Slow
    Kp = 1;
    Kd = 10;
    Ki = 0.01;
    minSpeed = 0;
    baseSpeed = 100;
    maxSpeed = 120;
    break;

    case 'M'://Moderate
    default:
    Kp = 2;
    Kd = 50;
    Ki = 0;
    minSpeed = 0;
    baseSpeed = 150;
    maxSpeed = 200;
    break;

    case 'F'://Fast
    Kp = 1.5;
    Kd = 50;
    Ki = 0;
    minSpeed = 0;
    baseSpeed = 200;
    maxSpeed = 255;
    break;
  }
}

//Line Following : PID Algorithm -------------------------------------------------------------------------------------------------------------------------------------------------------------------

void lineFollow()
{
  position = qtr.readLineBlack(sensorValues); // get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.
  int error = position - 3500;
  
  integral = integral + error;
  
  int controlSpeed = Kp * error + Kd * (error - lastError) + Ki*integral;
  
  lastError = error;
  
  byte rightMotorSpeed = constrain((baseSpeed + controlSpeed), minSpeed, maxSpeed);
  byte leftMotorSpeed = constrain((baseSpeed - controlSpeed), minSpeed, maxSpeed);

  motorSpeed(leftMotorSpeed,rightMotorSpeed);
}

//=====================================================================================================================================================================================

//Encoder Turnings : Speed Settings ------------------------------------------------------------------------

void setEncoderPID(char manner)
{
  leftCount=0;
  rightCount=0;
  
  lastError=0;
  integral=0;

  switch(manner)
  {
    case 'S'://Slow
    Kp = 50;
    Kd = 100;
    //Ki = 0;
    minSpeed = 80;
    baseSpeed = 100;
    maxSpeed = 120;
    break;

    case 'M'://Moderate
    default:
    Kp = 50;
    Kd = 100;
    //Ki = 0;
    minSpeed = 130;
    baseSpeed = 150;
    maxSpeed = 170;
    break;

    case 'F'://Fast
    Kp = 50;
    Kd = 100;
    //Ki = 0;
    minSpeed = 180;
    baseSpeed = 200;
    maxSpeed = 220;
    break;

    case 'X'://Xtremely Fast
    Kp = 50;
    Kd = 100;
    //Ki = 0;
    minSpeed = 240;
    baseSpeed = 250;
    maxSpeed = 255;
  }

}

//Encoder Turnings : PID Algorithm ------------------------------------------------------------------------

void encoderPID()
{
  int error = leftCount - rightCount;
  
  int controlSpeed = Kp * error + Kd * (error - lastError); // + Ki*integral;
  lastError = error;
  
  int rightMotorSpeed = constrain((baseSpeed + controlSpeed),minSpeed, maxSpeed);
  int leftMotorSpeed = constrain((baseSpeed - controlSpeed),minSpeed, maxSpeed);
  
  motorSpeed(leftMotorSpeed,rightMotorSpeed);
}

//=========================================================================================================
