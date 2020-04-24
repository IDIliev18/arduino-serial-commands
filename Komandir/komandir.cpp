#include <Arduino.h>
#include "komandir.h"

const int VAR_COUNT = 10;
String variables[VAR_COUNT];
String result = "";


String checkForToken(String terminators)
{
  char var;

  if(Serial.available())
  {      
    var = Serial.read();
    if(terminators.indexOf(var) == -1)
    {
      result += var;     
    }
    else{
      String tmp = result;
      result = "";
      return tmp;
    } 
  }
 
  return "";
}

int splitString(String * strArray, int size, String str, char terminator = '/')
{
  int delimPos= -1;
  int lastDelimPos = 0;
  String token;
  int index = 0;
  
  do {
    delimPos = str.indexOf(terminator, delimPos+1);
    token = str.substring(lastDelimPos, delimPos);
    lastDelimPos = delimPos + 1;
    strArray[index++] = token;    
  } while(delimPos != -1);
  
  return index;
}

COMMAND_TYPE cmdToEnum(String command){
  COMMAND_TYPE ct = { WRONG };
  
  if (command == "md") {
    ct = MD;
  } else if (command == "dw") {
    ct = DW;
  } else if (command == "dr") {
    ct = DR;
  }
  else if (command == "rv") {
    ct = RV;
  }
  else if (command == "wv") {
  ct = WV;
  };

  return ct;
}

void processCommand(COMMAND cmd)
{
  switch(cmd.command){
    case MD: {
        Serial.println("pinMode");
        pinMode(cmd.pin, cmd.arguments == "i" ? INPUT : OUTPUT);
      }; break;
    case DW: {
        Serial.println("digitalWrite");
        Serial.println(cmd.arguments);
        digitalWrite(cmd.pin, cmd.arguments == "l" ? LOW : HIGH);
      }; break;
    case DR: {
        Serial.println("digitalRead");
        int value = digitalRead(cmd.pin);
        Serial.println(value);
      }; break;
    case RV: {
        Serial.println("Read variable: ");
        Serial.println(variables[cmd.pin]);
      }; break;
    case WV: {
        Serial.print("Write variable (");
        Serial.print(cmd.pin);
        Serial.print(", ");
        Serial.print(cmd.arguments);
        Serial.println("): ");
        variables[cmd.pin] = cmd.arguments;
      }; break;      
  }
}

COMMAND parseTokens(String* tokens, int size)
{
    COMMAND cmd;
    cmd.isValid = false;    

    if (size > 2 && (tokens[1] == "md" || tokens[1] == "dr" || tokens[1] == "dw" || tokens[1] == "rv" || tokens[1] == "wv"))
    {
        
        cmd.command = cmdToEnum(tokens[1]);
        cmd.pin = tokens[2].toInt();
        if(cmd.pin >= 0)
        {
          if(tokens[1] == "md" && size == 4)
          {
            if(tokens[3] == "i" || tokens[3] == "o")
            {
              cmd.arguments = tokens[3];
              cmd.isValid = true;
            }
            
          }
          else if(tokens[1] == "dw" && size == 4 && (tokens[3] == "h" || tokens[3] == "l"))
          {
              cmd.arguments = tokens[3];
              cmd.isValid = true;
          } else if(tokens[1] == "dr" && size == 3) {
            cmd.isValid = true;
          }
          else if(tokens[1] == "rv"  && size == 3)
          {
            cmd.isValid = true;
          } else if (tokens[1] == "wv" && size == 4) {
            cmd.arguments = tokens[3];
            cmd.isValid = true;  
          }
          
        }
    }

    return cmd;
}

bool writeVar(int index, String value)
{
  if(index < VAR_COUNT)
  {
    variables[index] = value;
    return true;
  }
  return false;
}

String readVar(int index)
{
  if(index < VAR_COUNT)
  {
    return variables[index];
  }
  return "";
}
