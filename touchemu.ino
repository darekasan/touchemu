#include <CommandParser.h>
#include <HID-Project.h>
#include <HID-Settings.h>

#define USB_CONFIG_POWER 100

typedef CommandParser<> MyCommandParser;
MyCommandParser parser;

char buffer[255] = {'\0'};
int cnt = 0;

void cmdTouch(MyCommandParser::Argument *args, char *response) {
  Touchscreen.setFinger((uint8_t)args[0].asInt64, (uint16_t)args[1].asInt64, (uint16_t)args[2].asInt64, 127);
  Touchscreen.send();
  strlcpy(response, "ok", MyCommandParser::MAX_RESPONSE_SIZE);
}

void cmdRelease(MyCommandParser::Argument *args, char *response) {
  if(args[0].asInt64 == -1){
    for(int i=0;i<HID_TOUCHSCREEN_MAXFINGERS;i++){
      Touchscreen.releaseFinger(i);
    }
  }else{
    Touchscreen.releaseFinger((uint8_t)args[0].asInt64);
  }
  Touchscreen.send();
  strlcpy(response, "ok", MyCommandParser::MAX_RESPONSE_SIZE);
}

void setup() {
  // put your setup code here, to run once:
  Touchscreen.begin();
  Serial1.begin(115200);
  parser.registerCommand("touch", "iii", &cmdTouch);
  parser.registerCommand("release", "i", &cmdRelease);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    buffer[cnt] = Serial1.read();
    if(buffer[cnt] == '\n'){
      buffer[cnt]='\0';
      char response[MyCommandParser::MAX_RESPONSE_SIZE];
      parser.processCommand(buffer, response);
      Serial1.println(response);
      for(int i=0;i<cnt;i++){
        buffer[i]='\0';
      }
      cnt = 0;
    }else{
      cnt++;
    }

  }
}
