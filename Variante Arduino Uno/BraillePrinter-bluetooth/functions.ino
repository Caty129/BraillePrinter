// Move motors --------------------------------------
void moveRightX(int numPulse){
  // move motor clockwise
  digitalWrite(dirPinX, HIGH);  //enables motor to move in direction, HIGH = forward
  // 400 Steps for one rotation (0.9°/360° = 400)
  for(int x = 0; x< numPulse; x++){
    digitalWrite(pulsPinX, HIGH);
    delayMicroseconds(500);
    digitalWrite(pulsPinX, LOW);
    delayMicroseconds(500);
  } 
};

void moveLeftX(int numPulse){
  // move motor counterclockwise
  digitalWrite(dirPinX, LOW);  //enables motor to move in direction, LOW = backward
  // 400 Steps for one rotation (0.9°/360° = 400)
  for(int x = 0; x< numPulse; x++){
    digitalWrite(pulsPinX, HIGH);
    delayMicroseconds(500);
    digitalWrite(pulsPinX, LOW);
    delayMicroseconds(500);
  }
};

void moveForwardY(int numPulse){
  // move motor clockwise
  digitalWrite(dirPinY, HIGH);  //enables motor to move in direction, HIGH = forward
  // 400 Steps for one rotation (0.9°/360° = 400)
  for(int x = 0; x< numPulse; x++){
    digitalWrite(pulsPinY, HIGH);
    delayMicroseconds(500);
    digitalWrite(pulsPinY, LOW);
    delayMicroseconds(500);
  }  
};

void moveBackwardY(int numPulse){
  // move motor clockwise
  digitalWrite(dirPinY, LOW);  //enables motor to move in direction, LOW = backward
  // 400 Steps for one rotation (0.9°/360° = 400)
  for(int x = 0; x< numPulse; x++){
    digitalWrite(pulsPinY, HIGH);
    delayMicroseconds(500);
    digitalWrite(pulsPinY, LOW);
    delayMicroseconds(500);
  }
};

void turnOnSteppers(){
  digitalWrite(enPinX, LOW);
  digitalWrite(enPinY, LOW);
}

void turnOffSteppers(){
  digitalWrite(enPinX, HIGH);
  digitalWrite(enPinY, HIGH);
}

void startPosition(){
  digitalWrite(enPinX,LOW);
  bool switchTempVar = digitalRead(switchPin) ; 
  while (!switchTempVar)
  {
    moveRightX(1); 
    switchTempVar = digitalRead(switchPin) ; 
    if (switchTempVar){
      digitalWrite(enPinX,HIGH); 
    }
  }
}

void firstCell()
{
  //digitalWrite(enPinX,LOW);
  moveLeftX(startX); 
  
}

//Punch Dot w/ Solenoid ------------------------------
void punchDot(){
  digitalWrite(solenPin, HIGH);       
  delay(20);
  digitalWrite(solenPin, LOW);
  delay(50);
  
}

//Safety Switch --------------------------------------
void testSwitch(){
  bool sw = digitalRead(switchPin);
  Serial.print("Safety Switch: ");
  Serial.print(sw);
  Serial.print("\n");
}

//Piezo Buzzer ---------------------------------------
 void startSignal(){ // before starting print
  for (int aa = 0; aa<2; aa++){
    tone(piezoPin, 100); // activate piezo buzzer on Pin with 100Hz 
    delay(1000); // pause - buzzer still active
    noTone(piezoPin); // turn off buzzer
    delay(100);
  }
 }

 void endSignal(){  //when print is done
  for (int aa = 0; aa<2; aa++){
    tone(piezoPin, 440); // activate piezo buzzer on Pin with 100Hz 
    delay(1000); // pause - buzzer still active
    noTone(piezoPin); // turn off buzzer
    delay(100);
  }
 }

 
//User input -----------------------------------------
int dataWithEndMarker(){
  static byte index = 0;
  int tempIndex = 0;
  char endMarker = '\n';
  char currentChar;
  
  while(SerialBT.available() && (newData == false)){
    delay(10);    //delay added to make it stable
    currentChar = SerialBT.read();
    if (c == '\n') {break;}
    voice += currentChar;   //to make a string of command

    if (currentChar != endMarker){
      inputCharArray[index] = currentChar;
      index++;

      if (index >= numChars){
        index = numChars - 1;
      }
     }

     else {
      inputCharArray[index] = '\0'; // terminate string
      tempIndex = index;
      index = 0;
      newData = true;
      return tempIndex;
     } 
  }
}

void takeUserInput(){
  while (!newData)
  {inputLength = dataWithEndMarker(); }
}

void showUserInput(){
  if(newData == true){
    Serial.print("\n Input Data:  ");
    Serial.println(inputCharArray);
    newData = false;
  }
}

void  showLengthOfArray(){
    Serial.print("Length Of Input: "); 
    Serial.print(inputLength); 
    Serial.print("\n\n");
}




//print Braille Cell (8x8 Grid)

void punchBrailleDot(bool bitIsOne){
  if (bitIsOne){
    punchDot();
    Serial.println("punch");
  }
  else Serial.println("no punch");
}

void printBrailleCell(){  // for testing & dimensions
  int punchDelay = 50;
  punchDot();  //bit 1

  moveLeftX(nextDot);
  delay(punchDelay);
  punchDot();  //bit2

  moveForwardY(nextDot);
  delay(punchDelay);
  punchDot();  //bit3

  moveRightX(nextDot);
  delay(punchDelay);
  punchDot();  //bit4

  moveForwardY(nextDot);
  delay(punchDelay);
  punchDot();  //bit5

  moveLeftX(nextDot);
  delay(punchDelay);
  punchDot();  //bit6

  moveForwardY(nextDot);
  delay(punchDelay);
  punchDot();  //bit7

  moveRightX(nextDot);
  delay(punchDelay);
  punchDot();  //bit8
}

void printBrailleChar(){

  for (int ii=0; ii < inputLength; ii++){

    //ASCII array
    char tempChar = inputCharArray[ii];

    int* tempBrailleArray;
    tempBrailleArray = ASCII2Braille(tempChar);

    for (int jj=0; jj<8; jj++){

      bool tempBit = tempBrailleArray[jj];

      switch(jj){      //Braille dots from char

        case 0: 
        punchBrailleDot(tempBit);
        moveForwardY(nextDot);
        delay(50);
        break;

        case 1:
        punchBrailleDot(tempBit);
        moveForwardY(nextDot);
        delay(50);
        break;

        case 2:
        punchBrailleDot(tempBit);
        moveBackwardY(2*nextDot);
        moveLeftX(nextDot);
        delay(50);
        break;

        case 3:
        punchBrailleDot(tempBit);
        moveForwardY(nextDot);
        delay(50);
        break;
        
        case 4:
        punchBrailleDot(tempBit);
        moveForwardY(nextDot);
        delay(50);
        break;

        case 5:
        punchBrailleDot(tempBit);
        moveForwardY(nextDot);
        moveRightX(nextDot);
        delay(50);
        break;
        
        case 6:
        punchBrailleDot(tempBit);
        moveLeftX(nextDot);
        delay(50);
        break;

        case 7:
        punchBrailleDot(tempBit);
        moveBackwardY(3*nextDot);
        moveLeftX(nextCell);
        delay(50);
        Serial.println("Moving to next letter");
        break;

        default:
        break;
        
      }// end switch case


    }//end j loop
    
    if (ii == 25){
        moveRightX(25*nextCell);
        moveForwardY(nextLine);
      }

  }//end i loop

  Serial.println(" "); 
  Serial.println("Printing Complete ");

  // acoustic signal for printing complete
  endSignal();

  moveForwardY(3*nextLine);
  moveRightX(3*nextDot);
  turnOffSteppers();


  
}// end function


// ASCII to Braille Unicode conversion ---------------
/*
 * Braille Dot Numbering according to Braille Authority of North America (BANA) 
 * http://www.brailleauthority.org/eightdot/eightdot.html
 * 
 * 8-dot Braille (255 possible combinations)
 * [1] [4]
 * [2] [5]
 * [3] [6]
 * [7] [8]
 * 
 * 6-dot Braille (63 possible combinations)
 * [1] [4]
 * [2] [5]
 * [3] [6]
 * 
 */

int * ASCII2Braille(char myChar){
  switch((unsigned int)myChar)
  {
    case 'a':
    static int a[8] = {1,0,0,0,0,0,0,0};
    return a;
    break;

    case 'b':
    static int b[8] = {1,1,0,0,0,0,0,0};
    return b;
    break;

    case 'c':
    static int c[8] = {1,0,0,1,0,0,0,0};
    return c;
    break;
    
    case 'd':
    static int d[8] = {1,0,0,1,1,0,0,0};
    return d;
    break;

    case 'e':
    static int e[8] = {1,0,0,0,1,0,0,0};
    return e;
    break;

    case 'f':
    static int f[8] = {1,1,0,1,0,0,0,0};
    return f;
    break;

    case 'g':
    static int g[8] = {1,1,0,1,1,0,0,0};
    return g;
    break;
 
    case 'h':
    static int h[8] = {1,1,0,0,1,0,0,0};
    return h;
    break;

    case 'i':
    static int i[8] = {0,1,0,1,0,0,0,0};
    return i;
    break;

    case 'j':
    static int j[8] = {0,1,0,1,1,0,0,0};
    return j;
    break;

    case 'k':
    static int k[8] = {1,0,1,0,0,0,0,0};
    return k;
    break;

    case 'l':
    static int l[8] = {1,1,1,0,0,0,0,0};
    return l;
    break;

    case 'm':
    static int m[8] = {1,0,1,1,0,0,0,0};
    return m;
    break;

    case 'n':
    static int n[8] = {1,0,1,1,1,0,0,0};
    return n;
    break;
    
    case 'o':
    static int o[8] = {1,0,1,0,1,0,0,0};
    return o;
    break;

    case 'p':
    static int p[8] = {1,1,1,1,0,0,0,0};
    return p;
    break;

    case 'q':
    static int q[8] = {1,1,1,1,1,0,0,0};
    return q;
    break;

    case 'r':
    static int r[8] = {1,1,1,0,1,0,0,0};
    return r;
    break;

    case 's':
    static int s[8] = {0,1,1,1,0,0,0,0};
    return s;
    break;

    case 't':
    static int t[8] = {0,1,1,1,1,0,0,0};
    return t;
    break;

    case 'u':
    static int u[8] = {1,0,1,0,0,1,0,0};
    return u;
    break;

    case 'v':
    static int v[8] = {1,1,1,0,0,1,0,0};
    return v;
    break;

    case 'w':
    static int w[8] = {0,1,0,1,1,1,0,0};
    return w;
    break;

    case 'x':
    static int x[8] = {1,0,1,1,0,1,0,0};
    return x;
    break;

    case 'y':
    static int y[8] = {1,0,1,1,1,1,0,0};
    return y;
    break;

    case 'z':
    static int z[8] = {1,0,1,0,1,1,0,0};
    return z;
    break;

    case 'ä'
    :
    static int ae[8] = {0,0,1,1,1,0,0,0};
    return ae;
    break;

    case 'ü':
    static int ue[8] = {1,1,0,0,1,1,0,1};
    return ue;
    break;

    case 'ö':
    static int oe[8] = {0,1,0,1,0,1,0,1};
    return oe;
    break;

    case 'ß':
    static int sz[8] = {0,0,1,1,1,1,0,1};
    return sz;
    break;

    case 'A':
    static int A[8] = {1,0,0,0,0,0,1,0};
    return A;
    break;

    case 'B':
    static int B[8] = {1,1,0,0,0,0,1,0};
    return B;
    break;

    case 'C':
    static int C[8] = {1,0,0,1,0,0,1,0};
    return C;
    break;
    
    case 'D':
    static int D[8] = {1,0,0,1,1,0,1,0};
    return D;
    break;

    case 'E':
    static int E[8] = {1,0,0,0,1,0,1,0};
    return E;
    break;

    case 'F':
    static int F[8] = {1,1,0,1,0,0,1,0};
    return F;
    break;

    case 'G':
    static int G[8] = {1,1,0,1,1,0,1,0};
    return G;
    break;
 
    case 'H':
    static int H[8] = {1,1,0,0,1,0,1,0};
    return H;
    break;

    case 'I':
    static int I[8] = {0,1,0,1,0,0,1,0};
    return I;
    break;

    case 'J':
    static int J[8] = {0,1,0,1,1,0,1,0};
    return J;
    break;

    case 'K':
    static int K[8] = {1,0,1,0,0,0,1,0};
    return K;
    break;

    case 'L':
    static int L[8] = {1,1,1,0,0,0,1,0};
    return L;
    break;

    case 'M':
    static int M[8] = {1,0,1,1,0,0,1,0};
    return M;
    break;

    case 'N':
    static int N[8] = {1,0,1,1,1,0,1,0};
    return N;
    break;
    
    case 'O':
    static int O[8] = {1,0,1,0,1,0,1,0};
    return O;
    break;

    case 'P':
    static int P[8] = {1,1,1,1,0,0,1,0};
    return P;
    break;

    case 'Q':
    static int Q[8] = {1,1,1,1,1,0,1,0};
    return Q;
    break;

    case 'R':
    static int R[8] = {1,1,1,0,1,0,1,0};
    return R;
    break;

    case 'S':
    static int S[8] = {0,1,1,1,0,0,1,0};
    return S;
    break;

    case 'T':
    static int T[8] = {0,1,1,1,1,0,1,0};
    return T;
    break;

    case 'U':
    static int U[8] = {1,0,1,0,0,1,1,0};
    return U;
    break;

    case 'V':
    static int V[8] = {1,1,1,0,0,1,1,0};
    return V;
    break;

    case 'W':
    static int W[8] = {0,1,0,1,1,1,1,0};
    return W;
    break;

    case 'X':
    static int X[8] = {1,0,1,1,0,1,1,0};
    return X;
    break;

    case 'Y':
    static int Y[8] = {1,0,1,1,1,1,1,0};
    return Y;
    break;

    case 'Z':
    static int Z[8] = {1,0,1,0,1,1,1,0};
    return Z;
    break;
  
    case 'Ä':
    static int AE[8] = {0,0,0,0,1,1,1,0};
    return AE;
    break;

    case 'Ü':
    static int UE[8] = {0,1,1,0,0,1,0,1};
    return UE;
    break;

    case 'Ö':
    static int OE[8] = {0,0,1,0,1,0,0,1};
    return OE;
    break;

    case '0':
    static int zero[8] = {0,0,1,1,0,1,0,0};
    return zero;
    break;
  
    case '1':
    static int one[8] = {1,0,0,0,0,1,0,0};
    return one;
    break;
  
    case '2':
    static int two[8] = {1,1,0,0,0,1,0,0};
    return two;
    break;
  
    case '3':
    static int three[8] = {1,0,0,1,0,1,0,0};
    return three;
    break;
  
    case '4':
    static int four[8] = {1,0,0,1,1,1,0,0};
    return four;
    break;
  
    case '5':
    static int five[8] = {1,0,0,0,1,1,0,0};
    return five;
    break;
  
    case '6':
    static int six[8] = {1,1,0,1,0,1,0,0};
    return six;
    break;
  
    case '7':
    static int seven[8] = {1,1,0,1,1,1,0,0};
    return seven;
    break;
  
    case '8':
    static int eight[8] = {1,1,0,0,1,1,0,0};
    return eight;
    break;
  
    case '9':
    static int nine[8] = {0,1,0,1,0,1,0,0};
    return nine;
    break;

    case ' ':
    static int space[8] = {0,0,0,0,0,0,0,0};
    return space;
    break;

    case ',':
    static int comma[8] = {0,1,0,0,0,0,0,0};
    return comma;
    break;

    case ';':
    static int semicolon[8] = {0,1,1,0,0,0,0,0};
    return semicolon;
    break;

    case '!':
    static int shouting[8] = {0,0,0,0,0,1,0,0};
    return shouting;
    break;

    case '?':
    static int question[8] = {0,1,0,0,0,1,0,0};
    return question;
    break;

    case ':':
    static int colon[8] = {0,1,0,0,1,0,0,0};
    return colon;
    break;

   /* case '*':
    static int star[8] = {0,0,1,0,1,0,0,0};
    return star;
    break;

    case '-':
    static int minus[8] = {0,0,1,0,0,1,0,0};
    return minus;
    break;

    case '_':
    static int underscore[8] = {0,0,0,1,1,1,1,0};
    return underscore;
    break;

    case '"':
    static int quotation[8] = {0,0,0,1,0,0,0,0};
    return quotation;
    break;
    

    case '|':
    static int vertLine[8] = {0,0,1,1,0,0,0,0};
    return vertLine;
    break;

    case '+':
    static int plus[8] = {0,1,1,0,1,0,0,0};
    return plus;
    break;

    case '>':
    static int bigger[8] = {0,0,0,1,1,0,0,0};
    return bigger;
    break;

    case '(':
    static int rBracketL[8] = {0,1,1,0,0,1,0,0};
    return rBracketL;
    break;

    case '&':
    static int kaufUnd[8] = {1,1,1,1,0,1,0,0};
    return kaufUnd;
    break;

    case '<':
    static int smaller[8] = {0,0,0,0,1,1,0,0};
    return smaller;
    break;

    case '/':
    static int slash[8] = {0,1,0,0,1,1,0,0};
    return slash;
    break;

    case ')':
    static int rBracketR[8] = {0,0,1,0,1,1,0,0};
    return rBracketR;
    break;

    case '=':
    static int equal[8] = {0,1,1,0,1,1,0,0};
    return equal;
    break;

    case '{':
    static int cBracketL[8] = {1,1,1,0,1,1,0,0};
    return cBracketL;
    break;

    case '#':
    static int hashtag[8] = {0,0,1,1,1,1,0,0};
    return hashtag;
    break;

    case '}':
    static int cBracketR[8] = {0,1,1,1,1,1,0,0};
    return cBracketR;
    break;

    case '%':
    static int percent[8] = {1,1,1,1,1,1,0,0};
    return percent;
    break;

    case '@':
    static int klammeraffe[8] = {0,0,1,1,1,0,1,0};
    return klammeraffe;
    break;

    case '[':
    static int bracketL[8] = {1,1,1,0,1,1,1,0};
    return bracketL;
    break;

    case ']':
    static int bracketR[8] = {0,1,1,1,1,1,1,0};
    return bracketR;
    break;

    case '~':
    static int wave[8] = {0,1,1,1,0,1,0,0};
    return wave;
    break;
    
*/    
   }
}
