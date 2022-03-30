void beep(int t)
{
  
  if(t==1) digitalWrite(buzzer, HIGH);
  else if(t==0) digitalWrite(buzzer, LOW);

  else
  {
    digitalWrite(buzzer, HIGH);
    delay(t);
    digitalWrite(buzzer, LOW);
  }
  
}

void beep(int t1, int t2, int t3, int dt)
{
  
    digitalWrite(buzzer, HIGH);
    delay(t1);
    digitalWrite(buzzer, LOW);
    delay(dt);
    digitalWrite(buzzer, HIGH);
    delay(t2);
    digitalWrite(buzzer, LOW);
    delay(dt);
    digitalWrite(buzzer, HIGH);
    delay(t3);
    digitalWrite(buzzer, LOW);
  
}

//===================================================

//RGB Light ----------------------------------------
void light(char color)
{
  digitalWrite(33, LOW); //OFF
  digitalWrite(35, LOW);
  digitalWrite(37, LOW);
  digitalWrite(39, LOW);
  digitalWrite(41, LOW);
  digitalWrite(43, LOW);
  
  switch(color)
  {
    case 'R':
    digitalWrite(37, HIGH);
    digitalWrite(39, HIGH);
    break;
    
    case 'G':
    digitalWrite(33, HIGH);
    digitalWrite(41, HIGH);
    break;

    case 'B':
    digitalWrite(35, HIGH);
    digitalWrite(43, HIGH);
    break;

    default:
    break;
  }
}

//===================================================
