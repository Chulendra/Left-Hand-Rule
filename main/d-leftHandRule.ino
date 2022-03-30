//Left Hand Rule -----------------------------------------------------------------------------

//Mesh Solving -------------------------------------------------------------------------------

void leaveSquare()
{
  //Leaving starting zone
  setEncoderPID('S');
  forward();
  qtrRead();
  while(dval[0] || dval[7])
  {
    encoderPID();
    qtrRead();
  }
  ontoLine(100);
}

void enterSquare()
{
  setEncoderPID('S');
  leftCount=0;
  while(left < 300) encoderPID();
  brake('B');
}

void solveMesh()
{
  leaveSquare();
  //Start solving
  while(!End) //loop runs until it detects end zone
  {
    //if detect, save it
    if(junctionDetect())  saveTurns();

    //turning on L junctions
    else
    {
      if(left) turnAngle(-90);
      if(right) turnAngle(90);
    }
  }
}

//Saving junctions into memory -----------------------------------------------------------------

void saveTurns() //saving in memory
{
  if(left)
  {
    turns=turns + 'L';

    light('B');
    beep(1);
    turnAngle(-90);
    light(' ');
    beep(0);
  }

  else if(straight)
  {
    turns=turns + 'S';
    
    light('B');
    beep(1);
    ontoLine(200);
    light(' ');
    beep(0);
  }

  else if(right)
  {
    turns=turns + 'R';
    
    light('B');
    beep(1);
    turnAngle(90);
    light(' ');
    beep(0);
  }

  else if(dead)
  {
    turns=turns + 'U';
    
    light('R');
    beep(1);
    turn('B');
    light(' ');
    beep(1);
  }

  else if(End)//Go to stopping square
  {
    turns=turns + 'E';
    
    light('G');
    beep(1);

    //entering square
    enterSquare();

    //turning back
    encoderTurnBack();
    
    
    beep(0);
    light(' ');
    delay(500);
  }

  correctTurns();
}

//Correcting incorrect decisions -----------------------------------------------------------------

void correctTurns()
{
  index=turns.length();
  if(index >= 3)
  {
    index--;
    if(turns.charAt(index-1)=='U') //check
    {
      switch(turns.charAt(index-2))
      {
        //============LU'X'==============
        case 'L':
        switch(turns.charAt(index))
        {
          case 'L':
          turns.remove(index-2);
          turns=turns + 'S';
          break;

          case 'S':
          turns.remove(index-2);
          turns=turns + 'R';
          break;

          case 'R':
          turns.remove(index-2);
          turns=turns + 'U';
          break;
        }
        break;
        
        //============SU'X'==============
        case 'S':
        switch(turns.charAt(index))
        {
          case 'L':
          turns.remove(index-2);
          turns=turns + 'R';
          break;

          case 'S':
          turns.remove(index-2);
          turns=turns + 'U';
          break;

          case 'R':
          turns.remove(index-2);
          turns=turns + 'L';
          break;
        }
        break;

        //============RU'X'==============
        case 'R':
        switch(turns.charAt(index))
        {
          case 'L':
          turns.remove(index-2);
          turns=turns + 'U';
          break;

          case 'S':
          turns.remove(index-2);
          turns=turns + 'L';
          break;

          case 'R':
          turns.remove(index-2);
          turns=turns + 'S';
          break;
        }
        break;
        
      }
    }
  }
}

//Detecting a junction -------------------------------------------------------------

byte turnCount=0;

bool junctionDetect()
{
  setLineFollow(manner);

  //reseting variables
  detect=0;
  left=0;
  right=0;
  straight=0;
  dead=0;
  turnCount=0;
  
  while(1)
  {
    qtrRead();

    //Checking dead ends
    if(!(dval[0] || dval[1] || dval[2] || dval[3] || dval[4] || dval[5] || dval[6] || dval[7])) //NOR (only true when all sensors off)
    {
      dead=1;
      detect=1;
      brake('B');

      break;
    }

    //Checking junction
    if(dval[0] || dval[7])
    {
      if(dval[7]) left=1;
      if(dval[0]) right=1;
       
      centerAtJunction(); //Mapping the junction
      //delay(500);

      //End of mesh : stopping square
      if( dval[0] && dval[1] && dval[2] && dval[3] && dval[4] && dval[5] && dval[6] && dval[7] )
      {
        detect=1;
        //brake();
        
        End=1;
        left=0;
        right=0;
        straight=0;
        
        break;
      }

      //Counting number of turns
      if(left) turnCount++;
      if(straight) turnCount++;
      if(right) turnCount++;

      //checking for junction
      if(turnCount>1) detect=1;
      else detect=0;

      break;
    }
    else lineFollow();
  }
  
  if(detect) 
  {
    /*light('R');
    beep(500);
    light(' ');*/
    return 1;
  }
  else return 0;
}

//=========================================================================================================

//Returning to Home ---------------------------------------------------------------------------------------

void goHome()
{
  setHomeTurns();
  leaveSquare();
  runHomeMesh();
}

//Reversing decisions to go home --------------------------------------------------------------------------

void setHomeTurns()
{
  for(int i=(turns.length()-1); i>=0; i--)
  {
    switch(turns.charAt(i))
    {
      case 'L':
      back_turns += 'R';
      break;

      case 'R':
      back_turns += 'L';
      break;

      case 'S':
      back_turns += 'S';
      break;

      default:
      break;
    }
  }

  back_turns += 'E';
}

//Runing for home -------------------------------------------------------------------------------------

void runHomeMesh()
{
  index=0;
  End = 0;
  setLineFollow('M');

  turn('B');
  ontoLine(100);

  while(!End)
  {
    if(junctionDetect())
    {
      loadHomeTurns();
    }
    else
    {
      if(left) turnAngle(-90);
      if(right) turnAngle(90);
    }
  }
}

void loadHomeTurns()
{
  switch(back_turns.charAt(index))
  {
    case 'L':
    turnAngle(-90);
    break;

    case 'R':
    turnAngle(90);
    break;
    
    case 'S'://default:
    break;

    case 'E':
    End=1;
    
    light('G');
    beep(1);
    enterSquare();
    beep(0);
    light(' ');
    break;
  }
  index++;
}


//Going to Destination from Home -------------------------------------------------------------------------------

void runMesh()
{
  index=0;
  End = 0;
  setLineFollow('M');
  ontoLine(100);

  while(!End)
  {
    if(junctionDetect())
    {
      loadTurns();
    }
    else
    {
      if(left) turnAngle(-90);
      if(right) turnAngle(90);
    }
  }
}

void loadTurns()
{
  switch(turns.charAt(index))
  {
    case 'L':
    turnAngle(-90);
    break;

    case 'R':
    turnAngle(90);
    break;
    
    case 'S'://default:
    //ontoLine(100);
    break;

    case 'E': //go to the stopping square
    End=1;
    light('B');
    //go(200,150);
    break;
  }
  index++;
}
