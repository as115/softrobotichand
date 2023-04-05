//Declare Variables
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


int interval0 = 0;
int interval1 = 0;
int interval2 = 0;
int interval3 = 0;
int interval4 = 0;


//Do research to find values for these
const int p0 = 20;
const int p1 = 64;
const int p2 = 135;
const int p3 = 175;
const int p4 = 200;
const int tolerance = 35;

//State Variables
int curr_position_0 = 0;
int curr_position_1 = 0;
int curr_position_2 = 0;
int curr_position_3 = 0;
int curr_position_4 = 0;


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
  if (curr_position_0 < 0 || curr_position_0 > 4) {
    Serial.println("Something has gone terribly wrong");
    delay(100000000000000);
  }
  //uncomment next line to make sure all motors are working properly
  //      TestMotors();
  ShowCalibratedVals();
//      ShowVals();
  ControlMotor0();
//    RunMotors();
}

//Finds change in reading from base value. Determines which position the change should correspond to.
//Each position is at a 500 ms interval, so the time to reach each position is 500 times the change in position\
//It then activates the motor for the calculated amount of time to reach that position
//If the motor is still running after the set amount of time from when the motor was activated, it will turn off the motor and update the current position
void ControlMotor0() {
  //If there are motors currently running and they need to be stopped, stop them and end the method.
  if (sn0 == HIGH) {
    if (curr - tn0 >= interval0) {
      sn0 = LOW;
      digitalWrite(MotorNeg_0, LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  if (sp0 == HIGH) {
    if (curr - tp0 >= interval0) {
      sp0 = LOW;
      digitalWrite(MotorPos_0, LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  //Difference in current analog read to calibrated value
  int flexVal = analogRead(FLEX_PIN_0) - cal_0;
  Serial.print("flexVal: ");
  Serial.println(flexVal);
  //Find which position to go to. Start by initializing variable to -1 to check whether a position has been found, break the loop once there's a new position.
  int go_to = -1;
  while (go_to == -1) {
    Serial.println("In goto loop");
    //If value is within a certain tolerance of the reading required to actuate to that voltage then it will adjust go_to to that position
    if (abs(flexVal - p0) <= tolerance) {
      go_to = 0;
      break;
    }
    if (abs(flexVal - p1) <= tolerance) {
      go_to = 1;
      break;
    }
    if (abs(flexVal - p2) <= tolerance) {
      go_to = 2;
      break;
    }
    if (abs(flexVal - p3) <= tolerance) {
      go_to = 3;
      break;
    }
    if (abs(flexVal - p4) <= tolerance) {
      go_to = 4;
      break;
    }
    Serial.println("No position found");
    break;
  }
  Serial.print("Going to: ");
  Serial.println(go_to);
  
  //Find change in position to determine how long motor needs to be run for
  int pos_change = go_to - curr_position_0;
  Serial.print("CUrrent position: ");
  Serial.println(curr_position_0);
  Serial.print("pos_change ");
  Serial.println(pos_change);
  //Negative position change
  if (pos_change < 0) {
    //Update time at which Negative Motor began running
    tn0 = curr;
    //Calculate interval. 500 times position change because each position is at a 500 ms interval
    //i.e. if position change of 2, it would take 1000 ms to reach new position
    interval0 = abs(pos_change * 500);
    //Record current state of motor and start
    sn0 = HIGH;
    digitalWrite(MotorNeg_0, HIGH);
    curr_position_0 = go_to;
  }
  if (pos_change > 0) {
    Serial.println("Positive change");
    //Same as above but if change is positive
    tp0 = curr;
    interval0 = pos_change * 500;
    sp0 = HIGH;
    digitalWrite(MotorPos_0, HIGH);
    Serial.println("Moving");
    curr_position_0 = go_to;
  }
  Serial.println();
}

//Same as above but for motor 1
void ControlMotor1() {
  //If there are motors currently running and they need to be stopped, stop them and end the method.
  if (sn1 == HIGH) {
    if (curr - tn1 >= interval1) {
      sn1 = LOW;
      digitalWrite(MotorNeg_1, LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  if (sp1 == HIGH) {
    if (curr - tp1 >= interval1) {
      sp1 = LOW;
      digitalWrite(MotorPos_1, LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  //Difference in current analog read to calibrated value
  int flexVal = analogRead(FLEX_PIN_1) - cal_1;
  //Find which position to go to. Start by initializing variable to -1 to check whether a position has been found, break the loop once there's a new position.
  int go_to = -1;
  while (go_to == -1) {
    //If value is within a certain tolerance of the reading required to actuate to that voltage then it will adjust go_to to that position
    if (abs(flexVal - p0) <= tolerance) {
      go_to = 0;
      break;
    }
    if (abs(flexVal - p1) <= tolerance) {
      go_to = 1;
      break;
    }
    if (abs(flexVal - p2) <= tolerance) {
      go_to = 2;
      break;
    }
    if (abs(flexVal - p3) <= tolerance) {
      go_to = 3;
      break;
    }
    if (abs(flexVal - p4) <= tolerance) {
      go_to = 4;
      break;
    }
    Serial.println("No position found");
    break;
  }
  Serial.print("Going to: ");
  Serial.println(go_to);
  
  //Find change in position to determine how long motor needs to be run for
  int pos_change = go_to - curr_position_1;
  //Negative position change
  if (pos_change < 0) {
    //Update time at which Negative Motor began running
    tn1 = curr;
    //Calculate interval. 500 times position change because each position is at a 500 ms interval
    //i.e. if position change of 2, it would take 1000 ms to reach new position
    interval1 = abs(pos_change * 500);
    //Record current state of motor and start
    sn1 = HIGH;
    digitalWrite(MotorNeg_1, HIGH);
    curr_position_1 = go_to;
  }
  if (pos_change > 0) {
    Serial.println("Positive change");
    //Same as above but if change is positive
    tp1 = curr;
    interval1 = pos_change * 500;
    sp1 = HIGH;
    digitalWrite(MotorPos_1, HIGH);
    Serial.println("Moving");
    curr_position_1 = go_to;
  }
}

void ControlMotor2() {
    if (sn2 == HIGH) {
    if (curr - tn2 >= interval2) {
      sn2 = LOW;
      digitalWrite(MotorNeg_2, LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  if (sp2 == HIGH) {
    if (curr - tp2 >= interval2) {
      sp2 = LOW;
      digitalWrite(MotorPos_2, LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  //Difference in current analog read to calibrated value
  int flexVal = analogRead(FLEX_PIN_2) - cal_2;
  //Find which position to go to. Start by initializing variable to -1 to check whether a position has been found, break the loop once there's a new position.
  int go_to = -1;
  while (go_to == -1) {
    //If value is within a certain tolerance of the reading required to actuate to that voltage then it will adjust go_to to that position
    if (abs(flexVal - p0) <= tolerance) {
      go_to = 0;
      break;
    }
    if (abs(flexVal - p1) <= tolerance) {
      go_to = 1;
      break;
    }
    if (abs(flexVal - p2) <= tolerance) {
      go_to = 2;
      break;
    }
    if (abs(flexVal - p3) <= tolerance) {
      go_to = 3;
      break;
    }
    if (abs(flexVal - p4) <= tolerance) {
      go_to = 4;
      break;
    }
    Serial.println("No position found");
    break;
  }
  //Find change in position to determine how long motor needs to be run for
  int pos_change = go_to - curr_position_2;
  //Negative position change
  if (pos_change < 0) {
    //Update time at which Negative Motor began running
    tn2 = curr;
    //Calculate interval. 500 times position change because each position is at a 500 ms interval
    //i.e. if position change of 2, it would take 1000 ms to reach new position
    interval2 = abs(pos_change * 500);
    //Record current state of motor and start
    sn2 = HIGH;
    digitalWrite(MotorNeg_2, HIGH);
    curr_position_2 = go_to;
  }
  if (pos_change > 0) {
    //Same as above but if change is positive
    tp2 = curr;
    interval2 = pos_change * 500;
    sp2 = HIGH;
    curr_position_2 = go_to;
  }
}

void ControlMotor3() {
  if (sn3 == HIGH && (curr - tn3 >= interval3)) {
    sn3 = LOW;
    digitalWrite(MotorNeg_3, LOW);
    curr_position_3 = curr_position_3 - interval3 / 500;
    return;
  }
  if (sp3 == HIGH && (curr - tp3 >= interval3)) {
    sn3 = LOW;
    digitalWrite(MotorPos_3, LOW);
    curr_position_3 = curr_position_3 + interval3 / 500;
    return;
  }
  int flexVal = analogRead(FLEX_PIN_3) - cal_3;
  int go_to = -1;
  while (go_to == -1) {
    if (abs(flexVal - p0) <= tolerance) {
      go_to = 0;
      break;
    }
    if (abs(flexVal - p1) <= tolerance) {
      go_to = 1;
      break;
    }
    if (abs(flexVal - p2) <= tolerance) {
      go_to = 2;
      break;
    }
    if (abs(flexVal - p3) <= tolerance) {
      go_to = 3;
      break;
    }
    if (abs(flexVal - p4) <= tolerance) {
      go_to = 4;
      break;
    }
  }
  int pos_change = go_to - curr_position_3;
  if (pos_change < 0) {
    tn3 = curr;
    interval3 = abs(pos_change * 500);
    sn3 = HIGH;
    digitalWrite(MotorNeg_3, HIGH);
  }
  if (pos_change > 0) {
    tp3 = curr;
    interval3 = pos_change * 500;
    sp3 = HIGH;
    digitalWrite(MotorPos_3, HIGH);
  }
}

void ControlMotor4() {
    if (sn4 == HIGH) {
    if (curr - tn4 >= interval4) {
      sn4 = LOW;
      digitalWrite(MotorNeg_4, LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  if (sp4 == HIGH) {
    if (curr - tp4 >= interval1) {
      sp4 = LOW;
      digitalWrite(MotorPos_4, LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  //Difference in current analog read to calibrated value
  int flexVal = analogRead(FLEX_PIN_4) - cal_4;
  //Find which position to go to. Start by initializing variable to -1 to check whether a position has been found, break the loop once there's a new position.
  int go_to = -1;
  while (go_to == -1) {
    //If value is within a certain tolerance of the reading required to actuate to that voltage then it will adjust go_to to that position
    if (abs(flexVal - p0) <= tolerance) {
      go_to = 0;
      break;
    }
    if (abs(flexVal - p1) <= tolerance) {
      go_to = 1;
      break;
    }
    if (abs(flexVal - p2) <= tolerance) {
      go_to = 2;
      break;
    }
    if (abs(flexVal - p3) <= tolerance) {
      go_to = 3;
      break;
    }
    if (abs(flexVal - p4) <= tolerance) {
      go_to = 4;
      break;
    }
    Serial.println("No position found");
    break;
  }
  Serial.print("Going to: ");
  Serial.println(go_to);
  
  //Find change in position to determine how long motor needs to be run for
  int pos_change = go_to - curr_position_4;
  //Negative position change
  if (pos_change < 0) {
    //Update time at which Negative Motor began running
    tn4 = curr;
    //Calculate interval. 500 times position change because each position is at a 500 ms interval
    //i.e. if position change of 2, it would take 1000 ms to reach new position
    interval4 = abs(pos_change * 500);
    //Record current state of motor and start
    sn4 = HIGH;
    digitalWrite(MotorNeg_4, HIGH);
    curr_position_4 = go_to;
  }
  if (pos_change > 0) {
    Serial.println("Positive change");
    //Same as above but if change is positive
    tp4 = curr;
    interval1 = pos_change * 500;
    sp4 = HIGH;
    digitalWrite(MotorPos_4, HIGH);
    Serial.println("Moving");
    curr_position_4 = go_to;
  }
}

void RunMotors() {
  ControlMotor0();
  ControlMotor1();
  ControlMotor2();
  ControlMotor3();
  ControlMotor4();
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
