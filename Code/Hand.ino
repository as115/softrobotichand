//Declare Variables
//Pin Numbers
int MotorPos_0 = 12;
int MotorNeg_0 = 11;
int MotorPos_1 = 10;
int MotorNeg_1 = 9;
int MotorPos_2 = 8;
int MotorNeg_2 = 7;
int MotorPos_3 = 6;
int MotorNeg_3 = 5;
int MotorPos_4 = 4;
int MotorNeg_4 = 3;

int MotorPins[5][2] = {{MotorPos_0, MotorNeg_0}, {MotorPos_1, MotorNeg_1}, {MotorPos_2, MotorNeg_2}, {MotorPos_3, MotorNeg_3}, {MotorPos_4, MotorNeg_4}};
int FlexPins[] = {A1, A2, A3, A4, A5};

int intervals[] = {0, 0, 0, 0, 0};


//Do research to find values for these
int positions[] = {40, 100, 150, 250, 300};

//State Variables
int curr_pos[] = {0, 0, 0, 0, 0};

//Calibration variables (will change once Calibrate() is run, had to initialize to 0 for code to run
int cal[] = {0, 0, 0, 0, 0};
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
int times[5][2] = {{tp0, tn0}, {tp1, tn1}, {tp2, tn2}, {tp3, tn3}, {tp4, tn4}};

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
int states[5][2] = {{sp0, sn0}, {sp1, sn1}, {sp2, sn2}, {sp3, sn3}, {sp4, sn4}};

int curr = 0; //Current millis value

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    pinMode(MotorPins[i][0], OUTPUT);
    pinMode(MotorPins[i][1], OUTPUT);
  }
  for (int i = 0; i < 5; i++) {
    pinMode(FlexPins[i], INPUT);
  }
  //Calibrate and print cal variables in serial monitor
  Calibrate();
  for (int i = 0; i < 5; i++) {
    Serial.print("cal[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(cal[i]);
  }
}

void loop() {
  //  delay(200);
  curr = millis();
  //uncomment next line to make sure all motors are working properly
  //        TestMotors();
  //  ShowCalibratedVals();
  //      ShowVals();
  Control(0);
  //    RunMotors();
}

//Finds change in reading from base value. Determines which position the change should correspond to.
//Each position is at a 500 ms interval, so the time to reach each position is 500 times the change in position
//It then activates the motor for the calculated amount of time to reach that position
//If the motor is still running after the set amount of time from when the motor was activated, it will turn off the motor and update the current position
void Control(int idx) {
  //If there are motors currently running and they need to be stopped, stop them and end the method.
  if (states[idx][1] == HIGH) {
    if (abs(curr - times[idx][1]) >= abs(intervals[idx])) {
      states[idx][1] = LOW;
      digitalWrite(MotorPins[idx][1], LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  if (states[idx][0] == HIGH) {
    if (abs(curr - times[idx][0]) >= abs(intervals[idx])) {
      states[idx][0] = LOW;
      digitalWrite(MotorPins[idx][0], LOW);
      return;
    } else {
      //If a motor is running exit method
      return;
    }
  }
  //Difference in current analog read to calibrated value
  int flexVal = analogRead(FlexPins[idx]) - cal[idx];
  //Find which position to go to. Start by initializing variable to -1 to check whether a position has been found, break the loop once there's a new position.
  int go_to = FindClosest(flexVal);
  Serial.print("Going to: ");
  Serial.println(go_to);

  //Find change in position to determine how long motor needs to be run for
  int pos_change = go_to - curr_pos[idx];
  Serial.print("CUrrent position: ");
  Serial.println(curr_pos[idx]);
  Serial.print("pos_change ");
  Serial.println(pos_change);
  //Negative position change
  if (pos_change < 0) {
    //Update time at which Negative Motor began running
    times[idx][1] = curr;
    //Calculate interval. 500 times position change because each position is at a 500 ms interval
    //i.e. if position change of 2, it would take 1000 ms to reach new position
    intervals[idx] = abs(pos_change * 500) + 200;
    //Record current state of motor and start
    states[idx][1] = HIGH;
    digitalWrite(MotorPins[idx][1], HIGH);
    curr_pos[idx] = go_to;
  }
  if (pos_change > 0) {
    Serial.println("Positive change");
    //Same as above but if change is positive
    times[idx][0] = curr;
    intervals[idx] = (pos_change * 500) + 200;
    times[idx][0] = HIGH;
    digitalWrite(MotorPins[idx][0], HIGH);
    Serial.println("Moving");
    curr_pos[idx] = go_to;
  }
  Serial.println();
}

void RunMotors() {
  for (int i = 0; i < 5; i++) {
    Control(i);
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
    avg0 += analogRead(FlexPins[0]);
    avg1 += analogRead(FlexPins[1]);
    avg2 += analogRead(FlexPins[2]);
    avg3 += analogRead(FlexPins[3]);
    avg4 += analogRead(FlexPins[4]);
    count++;
  }
  cal[0] = avg0 / count;
  cal[1] = avg1 / count;
  cal[2] = avg2 / count;
  cal[3] = avg3 / count;
  cal[4] = avg4 / count;

  Serial.println("Calibration Complete");


}

//prints voltage values
void ShowVals() {
Serial.print("0: ");
  Serial.println(analogRead(FlexPins[0]));
  Serial.print("1: ");
  Serial.println(analogRead(FlexPins[1]));
  Serial.print("2: ");
  Serial.println(analogRead(FlexPins[2]));
  Serial.print("3: ");
  Serial.println(analogRead(FlexPins[3]));
  Serial.print("4: ");
  Serial.println(analogRead(FlexPins[4]));
  Serial.println();
}

//Activates each motor independently to test whether things are connected properly
void TestMotors() {
  Serial.println("Starting 0");
  digitalWrite(MotorPos_0, HIGH);
  delay(2000);
  digitalWrite(MotorPos_0, LOW);
  digitalWrite(MotorNeg_0, HIGH);
  delay(2000);
  digitalWrite(MotorNeg_0, LOW);
  delay(1000);
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


int FindClosest(int flexVal) {
  int pos[] = {0, 0, 0, 0, 0};
  for (int i = 0; i < 5; i++) {
    pos[i] = abs(flexVal - positions[i]);
  }
  int small = pos[0];
  int idx_small = 0;
  for (int i = 1; i < 5; i++) {
    if (pos[i] < small) {
      small = pos[i];
      idx_small = i;
    }
  }
  return idx_small;
}
//Prints values - calibration to see change in voltage from base val
void ShowCalibratedVals() {
  Serial.print("0: ");
  Serial.println(analogRead(FlexPins[0]) - cal[0]);
  Serial.print("1: ");
  Serial.println(analogRead(FlexPins[1]) - cal[1]);
  Serial.print("2: ");
  Serial.println(analogRead(FlexPins[2]) - cal[2]);
  Serial.print("3: ");
  Serial.println(analogRead(FlexPins[3]) - cal[3]);
  Serial.print("4: ");
  Serial.println(analogRead(FlexPins[4]) - cal[4]);
  Serial.println();
}
