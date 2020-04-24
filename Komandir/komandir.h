enum COMMAND_TYPE {
  WRONG,
  MD,
  DW,
  DR,
  RV,
  WV
};

struct COMMAND {
  bool isValid;
  COMMAND_TYPE command;
  int pin;
  String arguments;
};

String checkForToken(String terminators);
int splitString(String *, int, String, char);
COMMAND_TYPE cmdToEnum(String);
void processCommand(COMMAND);
COMMAND parseTokens(String*, int);
bool writeVar(int, String);
String readVar(int);
