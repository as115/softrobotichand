//Declare Variables
//I don't know if theres a way to make this look nicer, each motor needs its own variables so theres a lot
//Pin Numbers
const int MotorPos_0 = 12;
int MotorNeg_0 = 11;
int MotorPos_1 = 10;
int MotorNeg_1 = 9;
int MotorPos_2 = 8;
int MotorNeg_2 = 7;
int MotorPos_3 = 6;
int MotorNeg_3 = 5;
int MotorPos_4 = 4;
int MotorNeg_4 = 3;
const int FLEX_PIN_0 = A1;
const int FLEX_PIN_1 = A2;
const int FLEX_PIN_2 = A3;
const int FLEX_PIN_3 = A4;
const int FLEX_PIN_4 = A6;
const int interval = 2000;

//State Variables
boolean actuated_0 = false;
boolean actuated_1 = false;
boolean actuated_2 = false;
boolean actuated_3 = false;
boolean actuated_4 = false;

//Calibration variables (will change once Calibrate() is run, had to initialize to 0 for code to run
int cal_0 = 0;
int cal_1 = 0;
int cal_2 = 0;
int cal_3 = 0;
int cal_4 = 0;

//Time that the motor was turned on and state
//t = time
//s = state (high or low)
//p = positive, n = negative
//Number is which motor
int tp0 = 0;
int tn0 = 0;
int tp1 = 0;
int tn1 = 0;
int tp2 = 0;
int tn2 = 0;
int tp3 = 0;
int tn3 = 0;
int tp4 = 0;
int tn4 = 0;
byte sp0 = LOW;
byte sn0 = LOW;
byte sp1 = LOW;
byte sn1 = LOW;
byte sp2 = LOW;
byte sn2 = LOW;
byte sp3 = LOW;
byte sn3 = LOW;
byte sp4 = LOW;
byte sn4 = LOW;

int curr = 0; //Current millis value

void setup() {
  pinMode(MotorPos_0, OUTPUT);
  pinMode(MotorNeg_0, OUTPUT);
  pinMode(MotorPos_1, OUTPUT);
  pinMode(MotorNeg_1, OUTPUT);
  pinMode(MotorPos_2, OUTPUT);
  pinMode(MotorNeg_2, OUTPUT);
  pinMode(MotorPos_3, OUTPUT);
  pinMode(MotorNeg_3, OUTPUT);
  pinMode(MotorPos_4, OUTPUT);
  pinMode(MotorNeg_4, OUTPUT);
  Serial.begin(9600);
  pinMode(FLEX_PIN_0, INPUT);
  pinMode(FLEX_PIN_1, INPUT);
  pinMode(FLEX_PIN_2, INPUT);
  pinMode(FLEX_PIN_3, INPUT);
  pinMode(FLEX_PIN_4, INPUT);

  //Calibrate and print cal variables in serial monitor
  Calibrate();
  Serial.print("cal_0: ");
  Serial.println(cal_0);
  Serial.print("cal_1: ");
  Serial.println(cal_1);
  Serial.print("cal_2: ");
  Serial.println(cal_2);
  Serial.print("cal_3: ");
  Serial.println(cal_3);
  Serial.print("cal_4: ");
  Serial.println(cal_4);
}

void loop() {
  delay(500);
  curr = millis();
  
  //uncomment next line to make sure all motors are working properly
//    TestMotors();
  
  ShowCalibratedVals();
//    ShowVals();
  
  //Start all relevant motors
  StartMotors();
  
  //turn off all motors based on interval set earlier
  StopMotors();
  //  AdjustCal();
}

//-1: negative, 0: no actuation, 1: positive
//Determines how the inputted motor should actuate
//First calculates difference between current voltage reading and base voltage from Calibrate()
//Switch statement based on which motor you input. All 5 motors get called in StartMotors but made it a separate function for readability
//If the difference between measured and base voltage is greater than 20 and the finger is not already actuated then it returns 1, signaling actuation
//If the difference is less than 15 but the finger is actuated it will return to its original position
//This allows the finger to remain actuated if the flex snesor stays flexed when this method is called
int FindActuate(int MotorNum) {
  int flexVal;
  bool actuated;
  switch (MotorNum) {

    //First initialize variables for use within the method
    case 0:
      flexVal = analogRead(FLEX_PIN_0) - cal_0;
      actuated = actuated_0;
      break;
    case 1:
      flexVal = analogRead(FLEX_PIN_1) - cal_1;
      actuated = actuated_1;
      break;
    case 2:
      flexVal = analogRead(FLEX_PIN_2) - cal_2;
      actuated = actuated_2;
      break;
    case 3:
      flexVal = analogRead(FLEX_PIN_3) - cal_3;
      actuated = actuated_3;
      break;
    case 4:
      flexVal = analogRead(FLEX_PIN_4) - cal_4;
      actuated = actuated_4;
      break;
  }

  //Determine updated state of inputted motor
  //these reference numbers may not be accurate for other sensors/circuits, change as needed
  //This is for 5V input directly from arduino

  //New idea: Have 3 or 4 different possible positions, find values that correspond to these and change the possible outputs to match these values
  if (flexVal > 20 && !actuated) {
    return 1;
  }
  if (flexVal <= 15 && actuated) {
    return -1;
  }
  return 0;
}


//Determines which direction each motor should be run using FindActuate(), then starts motor accordingly.
//Updates actuated_# state variable
//If FindActuate returns 0 there is no response
void StartMotors() {
  switch (FindActuate(0)) {
    case 1:
      Serial.println("Positive 0");
      sp0 = HIGH;
      tp0 = curr;
      digitalWrite(MotorPos_0, HIGH);
      actuated_0 = true;
      break;
    case -1:
      Serial.println("Negative 0");
      sn0 = HIGH;
      tn0 = curr;
      digitalWrite(MotorNeg_0, HIGH);
      actuated_0 = false;
      break;
  }
  switch (FindActuate(1)) {
    case 1:
      Serial.println("Positive 1");
      sp1 = HIGH;
      tp1 = curr;
      digitalWrite(MotorPos_1, HIGH);
      actuated_1 = true;
      break;
    case -1:
      Serial.println("Negative 1");
      sn1 = HIGH;
      tn1 = curr;
      digitalWrite(MotorNeg_1, HIGH);
      actuated_1 = false;
      break;
  }
  switch (FindActuate(2)) {
    case 1:
      Serial.println("Positive 2");
      sp2 = HIGH;
      tp2 = curr;
      digitalWrite(MotorPos_2, HIGH);
      actuated_2 = true;
      break;
    case -1:
      Serial.println("Negative 2");
      sn2 = HIGH;
      tn2 = curr;
      digitalWrite(MotorNeg_2, HIGH);
      actuated_2 = false;
      break;
  }
  switch (FindActuate(3)) {
    case 1:
      Serial.println("Positive 3");
      sp3 = HIGH;
      tp3 = curr;
      digitalWrite(MotorPos_3, HIGH);
      actuated_3 = true;
      break;
    case -1:
      Serial.println("Negative 3");
      sn3 = HIGH;
      tn3 = curr;
      digitalWrite(MotorNeg_3, HIGH);
      actuated_3 = false;
      break;
  }
  switch (FindActuate(4)) {
    case 1:
      Serial.println("Positive 4");
      sp4 = HIGH;
      tp4 = curr;
      digitalWrite(MotorPos_4, HIGH);
      actuated_4 = true;
      break;
    case -1:
      Serial.println("Negative 4");
      sn4 = HIGH;
      tn4 = curr;
      digitalWrite(MotorNeg_4, HIGH);
      actuated_4 = false;
      break;
  }
}

//Times how long motor has been running for, if it matches interval it will turn it off
void StopMotors() {
  if (sp0 == HIGH) {
    if (curr - tp0 >= interval || (abs(analogRead(FLEX_PIN_0) - cal_0) <= 4)) {
      sp0 = LOW;
      digitalWrite(MotorPos_0, LOW);
    }
  }

  if (sn0 == HIGH) {
    if (curr - tn0 >= interval || (abs(analogRead(FLEX_PIN_0) - cal_0) <= 4)) {
      sn0 = LOW;
      digitalWrite(MotorNeg_0, LOW);
    }
  }

  if (sp1 == HIGH) {
    if (curr - tp1 >= interval|| (abs(analogRead(FLEX_PIN_1) - cal_1) <= 4)) {
      sp1 = LOW;
      digitalWrite(MotorPos_1, LOW);
    }
  }

  if (sn1 == HIGH) {
    if (curr - tn1 >= interval|| (abs(analogRead(FLEX_PIN_0) - cal_0) <= 4)) {
      sn1 = LOW;
      digitalWrite(MotorNeg_1, LOW);
    }
  }

  if (sp2 == HIGH) {
    if (curr - tp2 >= interval) {
      sp2 = LOW;
      digitalWrite(MotorPos_2, LOW);
    }
  }

  if (sn2 == HIGH) {
    if (curr - tn2 >= interval) {
      sn2 = LOW;
      digitalWrite(MotorNeg_2, LOW);
    }
  }

  if (sp3 == HIGH) {
    if (curr - tp3 >= interval) {
      sp3 = LOW;
      digitalWrite(MotorPos_3, LOW);
    }
  }

  if (sn3 == HIGH) {
    if (curr - tn3 >= interval) {
      sn3 = LOW;
      digitalWrite(MotorNeg_3, LOW);
    }
  }

    if (sp4 == HIGH) {
      if (curr - tp4 >= interval) {
        sp4 = LOW;
        digitalWrite(MotorPos_4, LOW);
      }
    }

    if (sn4 == HIGH) {
      if (curr - tn4 >= interval) {
        sn4 = LOW;
        digitalWrite(MotorNeg_4, LOW);
      }
    }
  }

  //take average value of input over 2 seconds
  //Update 2/28/2023: originally it calibrated each motor at once for one second, but changed it so it calibrates them all at the same time for 2 seconds
  int Calibrate() {
    double avg0 = 0;
    double avg1 = 0;
    double avg2 = 0;
    double avg3 = 0;
    double avg4 = 0;
    int count = 0;
    Serial.println("Calibrating");
    while (millis() < 2000) {
      avg0 += analogRead(FLEX_PIN_0);
      avg1 += analogRead(FLEX_PIN_1);
      avg2 += analogRead(FLEX_PIN_2);
      avg3 += analogRead(FLEX_PIN_3);
      avg4 += analogRead(FLEX_PIN_4);
      count++;
    }
    cal_0 = avg0 / count;
    cal_1 = avg1 / count;
    cal_2 = avg2 / count;
    cal_3 = avg3 / count;
    cal_4 = avg4 / count;
    Serial.println("Calibration Complete");
  }

  //prints voltage values
  void ShowVals() {
    int v0 = analogRead(FLEX_PIN_0);
    int v1 = analogRead(FLEX_PIN_1);
    int v2 = analogRead(FLEX_PIN_2);
    int v3 = analogRead(FLEX_PIN_3);
    int v4 = analogRead(FLEX_PIN_4);
    Serial.print("0: ");
    Serial.println(v0);
    Serial.print("1: ");
    Serial.println(v1);
    Serial.print("2: ");
    Serial.println(v2);
    Serial.print("3: ");
    Serial.println(v3);
    Serial.print("4: ");
    Serial.println(v4);
    Serial.println();
  }

  //Activates each motor independently to test whether things are connected properly
  void TestMotors() {
    Serial.println("Starting 0");
    digitalWrite(MotorPos_0, HIGH);
    delay(1000);
    digitalWrite(MotorPos_0, LOW);
    digitalWrite(MotorNeg_0, HIGH);
    delay(1000);
    digitalWrite(MotorNeg_0, LOW);
    delay(3000);
    Serial.println("Starting 1");
    digitalWrite(MotorPos_1, HIGH);
    delay(1000);
    digitalWrite(MotorPos_1, LOW);
    digitalWrite(MotorNeg_1, HIGH);
    delay(1000);
    digitalWrite(MotorNeg_1, LOW);
    delay(3000);
    Serial.println("Starting 2");
    digitalWrite(MotorPos_2, HIGH);
    delay(1000);
    digitalWrite(MotorPos_2, LOW);
    digitalWrite(MotorNeg_2, HIGH);
    delay(1000);
    digitalWrite(MotorNeg_2, LOW);
    delay(3000);
    Serial.println("Starting 3");
    digitalWrite(MotorPos_3, HIGH);
    delay(1000);
    digitalWrite(MotorPos_3, LOW);
    digitalWrite(MotorNeg_3, HIGH);
    delay(1000);
    digitalWrite(MotorNeg_3, LOW);
    delay(3000);
    Serial.println("Starting 4");
    digitalWrite(MotorPos_4, HIGH);
    delay(1000);
    digitalWrite(MotorPos_4, LOW);
    digitalWrite(MotorNeg_4, HIGH);
    delay(1000);
    digitalWrite(MotorNeg_4, LOW);
  }

  //Prints values - calibration to see change in voltage from base val
  void ShowCalibratedVals() {
    int v0 = analogRead(FLEX_PIN_0) - cal_0;
    int v1 = analogRead(FLEX_PIN_1) - cal_1;
    int v2 = analogRead(FLEX_PIN_2) - cal_2;
    int v3 = analogRead(FLEX_PIN_3) - cal_3;
    int v4 = analogRead(FLEX_PIN_4) - cal_4;
    Serial.print("0: ");
    Serial.println(v0);
    Serial.print("1: ");
    Serial.println(v1);
    Serial.print("2: ");
    Serial.println(v2);
    Serial.print("3: ");
    Serial.println(v3);
    Serial.print("4: ");
    Serial.println(v4);
    Serial.println();
  }
