enum COMMAND_TYPE {
  WRONG = 0,
  MD = 1,
  DW = 2,
  DR = 3
};

struct COMMAND {
  bool isValid;
  COMMAND_TYPE command;
  int pin;
  String arguments;
};

String buffer;
String tokens[10];
int tokenCount;
COMMAND cmd;
String result = "";
unsigned int lastMillis = 0;


void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
}

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
  } while(delimPos!=-1);
  
  return index;
}

//  /{cmd}/{pin}/{values}
//  /mr/8/i
// ["", "mr", "tt", "i"]

COMMAND_TYPE cmdToEnum(String command){
  COMMAND_TYPE ct = { WRONG };
  
  if (command == "md") {
    ct = MD;
  } else if (command == "dw") {
    ct = DW;
  } else if (command == "dr") {
    ct = DR;
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
  }
}

COMMAND parseTokens(String* tokens, int size)
{
    COMMAND cmd;
    cmd.isValid = false;

    if (size > 2 && (tokens[1] == "md" || tokens[1] == "dr" || tokens[1] == "dw"))
    {
        
        cmd.command = cmdToEnum(tokens[1]);
        cmd.pin = tokens[2].toInt();
        if(cmd.pin > 0)
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
          
        }
    }

    return cmd;
}

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
  


  
  buffer = checkForToken("^\n");
  if(buffer != "")
  {
    Serial.print("String size: ");
    Serial.println(buffer.length());
    Serial.println(buffer);
    // ["", "mr", "8", "i"]
    tokenCount = splitString(tokens, 10, buffer);
    Serial.println(tokenCount);
    for (int i = 0; i < tokenCount; i++)
    {
      Serial.println(tokens[i]);
    }
  
    cmd = parseTokens(tokens, tokenCount);
    Serial.println(cmd.isValid);
    processCommand(cmd);
  }
  
}
