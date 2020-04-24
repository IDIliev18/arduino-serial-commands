#include "komandir.h"

String buffer;
String tokens[10];
int tokenCount;
COMMAND cmd;
unsigned int lastMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
}

//  /{cmd}/{pin}/{values}
//  /mr/8/i
// ["", "mr", "tt", "i"]
byte b = 1;

void loop() {
  // put your main code here, to run repeatedly:
  // /md/8/i
  // /m/

  if (millis() - lastMillis > 500){
    lastMillis = millis();
    digitalWrite(12, b);
    b = !b;
  }

  writeVar(0, String(millis()));
  writeVar(1, String(b));


  buffer = checkForToken("^\n");
  if(buffer != "")
  {
    Serial.print("String size: ");
    Serial.println(buffer.length());
    Serial.println(buffer);
    // ["", "mr", "8", "i"]
    tokenCount = splitString(tokens, 10, buffer, '/');
    Serial.print("Token count: ");
    Serial.println(tokenCount);
    for (int i = 0; i < tokenCount; i++)
    {
      Serial.println(tokens[i]);
    }
  
    cmd = parseTokens(tokens, tokenCount);
    Serial.print("Is valid: ");
    Serial.println(cmd.isValid);
    processCommand(cmd);
  }
 
}
