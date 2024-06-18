//Line follower robo using IR Array sensor.ino
//pin connections

int m1a = 2;
int m1b = 8;
int m2a = 4;
int m2b = 5;
int en1 = 6;
int en2 = 9;
int s2 = 3;
int s4 = 11;
int s2read;
int s4read;
int s3 = 10;
int s3read;
// int s1 = 12;
// int s1read;
// int s5 = 13;
// int s5read;
int rotL = 0;
int rotR = 0;

void setup() {
  // put your setup code here, to run once:

  //Motors pin
  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);

  //pwm pins
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);

  //sensor pin
  pinMode(s2, INPUT);
  pinMode(s4, INPUT);
  pinMode(s3, INPUT);
  // pinMode(s1, INPUT);
  // pinMode(s5, INPUT);
}


void forward() {
  analogWrite(en1, 125);
  analogWrite(en2, 125);

  digitalWrite(m1a, HIGH);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, HIGH);
}

void lturn() {
  analogWrite(en1, 120);
  analogWrite(en2, 100);
  digitalWrite(m1a, HIGH);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, HIGH);
  digitalWrite(m2b, LOW);
}
void Rturn() {
  analogWrite(en1, 100);
  analogWrite(en2, 120);
  //analogWrite(en2, 50);
  digitalWrite(m1a, LOW);
  digitalWrite(m1b, HIGH);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, HIGH);
}
void stp() {
  analogWrite(en1, 0);
  analogWrite(en2, 0);
  digitalWrite(m1a, LOW);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, LOW);
}

void allZ() {
  analogWrite(en1, 120);
  analogWrite(en2, 120);
  //analogWrite(en2, 50);
  digitalWrite(m1a, LOW);
  digitalWrite(m1b, HIGH);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, HIGH);
}

void loop() {
  //main code to run repeatedly
  analogWrite(en1, 125);
  analogWrite(en2, 125);

  // Reading sensor value
  s2read = digitalRead(s2);
  s4read = digitalRead(s4);
  s3read = digitalRead(s3);
  // s1read = digitalRead(s1);
  // s5read = digitalRead(s5);

  //Condition for robot

  if (s3read == 1)

  {

    forward();
    delay(10);

  }

  // left turn

  else if ((s4read == 0) && (s3read == 0) && (s2read == 1)) {
    rotL = 1;
    rotR = 0;

    lturn();
    delay(10);

  }

  else if ((s2read == 0) && (s3read == 0) && (s4read == 1)) {
    rotL = 0;
    rotR = 1;

    Rturn();
    delay(10);

  }

  else if ((s2read == 0) && (s3read == 0) && (s4read == 0)) {
    if (rotL) {
      lturn();
      delay(10);
 
    } else {
      allZ();
      delay(10);

    }
  }
}
