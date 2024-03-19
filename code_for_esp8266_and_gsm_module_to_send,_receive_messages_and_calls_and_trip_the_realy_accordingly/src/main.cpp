#include <SoftwareSerial.h>
const int LED = 5;
int st = 0;
long long int number = 0;
long long int saveNumber = 0;
bool toogle = false;
String phoneNumber;
String message;
//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(4, 2);  //SIM800L Tx & Rx is connected to Arduino #3 & #2

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT");  //Once the handshake test is successful, it will back to OK


  mySerial.println("AT+CMGF=1");  // Configuring TEXT mode

  mySerial.println("AT+CNMI=1,2,0,0,0");  // Decides how newly arrived SMS messages should be handled
  mySerial.println("AT+DDET=1");
  mySerial.println("AT+CSQ");
  updateSerial();
}

void loop() {
  updateSerial();
}

void updateSerial() {
  while (mySerial.available()) {

    message = mySerial.readStringUntil('\n');
    if (message.startsWith("+CMT:")) {
      Serial.println("CMT+");
      int val = message.indexOf('"');
      if (val != -1) {
        Serial.println("found first index: " + String(val));
        int cal = message.indexOf('"', val + 1);
        if (cal != -1) {
          Serial.println("found second index: " + String(cal));
          for (val += 4; val < cal; val++) {
            Serial.println(message[val]);
            number *= 10;                  // Multiply existing number by 10 before adding the new digit
            number += message[val] - '0';  // Convert character to integer and add it to the number
          }
        }
        Serial.println(number);
        phoneNumber = "+91" + String(number);
        saveNumber = number;
      }
    } else if (message.startsWith("On")) {
      digitalWrite(LED, HIGH);
      toogle = true;
      mySerial.print("AT+CMGS=\"");
      mySerial.print(phoneNumber);
      mySerial.println("\"");  // End the phone number
      delay(1000);
      mySerial.println("switch on");
      delay(100);
      mySerial.write(26);
      Serial.println("MESSAGESENT on");
      number = 0;

    } else if (message.startsWith("Off")) {
      digitalWrite(LED, LOW);
      toogle = false;
      mySerial.print("AT+CMGS=\"");
      mySerial.print(phoneNumber);
      mySerial.println("\"");  // End the phone number
      delay(1000);
      mySerial.println("switch off");
      delay(100);
      mySerial.write(26);
      Serial.println("MESSAGESENT of");
      number = 0;

    } else if (message.startsWith("R")) {
      mySerial.println("ATA");
      Serial.println("CALL is ANSWERED");
      answerCall();
    } else if (message.startsWith("S")) {
      String phoneNumber = "+91" + String(number);  // Construct the phone number string
      mySerial.print("AT+CMGS=\"");
      mySerial.print(phoneNumber);
      mySerial.println("\"");  // End the phone number
      delay(1000);
      mySerial.println(toogle ? "switch on" : "switch off");
      delay(100);
      mySerial.write(26);
      Serial.println("MESSAGESENT on");
      saveNumber = number;
      number = 0;
    }
  }
}


void answerCall() {
  while (true) {
    if (mySerial.available()) {
      String ch = mySerial.readStringUntil('\n');
      if (ch.startsWith("+CLIP:")) {
        int val = ch.indexOf('"');
        if (val != -1) {
          Serial.println("found first index: " + String(val));
          int cal = ch.indexOf('"', val + 1);
          if (cal != -1) {
            Serial.println("found second index: " + String(cal));
            for (val += 4; val < cal; val++) {
              Serial.println(ch[val]);
              number *= 10;             // Multiply existing number by 10 before adding the new digit
              number += ch[val] - '0';  // Convert character to integer and add it to the number
            }
          }
          Serial.println(number);
          phoneNumber = "+91" + String(number);
        }
        Serial.println(ch);
        ch = mySerial.readStringUntil('\n');
        Serial.println(ch);
      }
      Serial.println(ch);
      if (ch.indexOf('1') != -1) {
        Serial.println("remothered");
        Serial.println(ch);
        Serial.println("ch is 1");
        digitalWrite(LED, HIGH);
        toogle = true;
        delay(1000);
        mySerial.print("AT+CMGS=\"");
        mySerial.print(phoneNumber);
        mySerial.println("\"");  // End the phone number
        delay(1000);
        mySerial.println("switch on");
        delay(100);
        mySerial.write(26);
        Serial.println("MESSAGESENT on");
        //mySerial.println("AT+CLVL=100");  // Set volume level
        //delay(1000);
        //mySerial.println("AT+TTS=0,\"Switch on\"");  // Play TTS for "Switch off"
        //delay(5000);                                  // Wait for the message to be played

      } 
      else if (ch.indexOf('0') != -1) {
        Serial.println("ch is 0");        
        
        delay(1000);
        mySerial.print("AT+CMGS=\"");
        mySerial.print(phoneNumber);
        mySerial.println("\"");  // End the phone number
        delay(1000);
        mySerial.println("switch off");
        delay(100);
        mySerial.write(26);
        Serial.println("MESSAGESENT off");
        delay(2000);
        // mySerial.println("AT+CLVL=100");  // Set volume level
        //delay(1000);
        //mySerial.println("AT+TTS=0,\"Switch off\"");  // Play TTS for "Switch off"
        //delay(5000);                                  // Wait for the message to be played
        digitalWrite(LED, LOW);
        toogle = false;
      } else if (ch.indexOf("NO") != -1) {
        Serial.println("call is ended");
        break;
      }
    }
  }
  Serial.println("out of the loop");
  updateSerial();
}
