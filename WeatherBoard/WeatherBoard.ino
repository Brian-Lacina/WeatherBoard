
#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#define LED_PIN  5
#define NUM_LEDS 32
#define BRIGHTNESS  64
#define COLOR_ORDER GRB
#define LED_TYPE    WS2811
CRGB leds[NUM_LEDS];
boolean zero[] = {true,true,true,true,true,true,false};
boolean one[] = {true,false,false,false,false,true,false};
boolean two[] = {false,true,true,false,true,true,true};
boolean three[] = {true,true,false,false,true,false,true};
boolean four[] = {true,false,false,true,false,true,true};
boolean five[] = {true,true,false,true,true,false,true};
boolean six[] = {true,true,true,true,false,false,true};
boolean seven[] = {true,false,false,true,true,true,false};
boolean eight[] = {true,true,true,true,true,true,true};
boolean nine[] = {true,false,false,true,true,true,true};
boolean blank[] = {false,false,false,false,false,false,false};
int color[]= {50,50,50};
int temp[24];

//KEY FOR LED NUMBERS
//0:AM
//1:PM
//2-8 ones hour seven seg
//9-15 tens hour seven seg
//16-22 Ones temp seven seg
//23-29 tens temp seven seg
//30 negative sign LED
//31 percipitation LED
const char SSID[]     = "********";
const char PASSWORD[] = "***************";
#define WUNDERGROUND "api.wunderground.com"
#define WU_API_KEY "784d8e7a9a373f06-weatherboard"
#define WU_LOCATION "53715"
#define DELAY_NORMAL (5*60*1000)
// 20 minute delay between updates after an error
#define DELAY_ERROR (20*60*1000)
const char WUNDERGROUND_REQ[] =
    "GET /api/" WU_API_KEY "/hourly/q/" WU_LOCATION ".json HTTP/1.1\r\n"
    "User-Agent: ESP8266/0.1\r\n"
    "Accept: */*\r\n"
    "Host: " WUNDERGROUND "\r\n"
    "Connection: close\r\n"
"\r\n";

#define UPDATES_PER_SECOND 100

void setup() { 
  Serial.begin(115200);
  WiFi.begin(SSID,PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  delay (3000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
//  currentBlending = LINEARBLEND;
  // put your setup code here, to run once:

}

static char respBuf[4096];
void loop() {



  // connect to weather underground
   WiFiClient httpclient;
  const int httpPort = 80;
  if (!httpclient.connect(WUNDERGROUND, httpPort)) {
    Serial.println("connection failed");
    delay(100000);
    return;
  }
  // sends request
  httpclient.print(WUNDERGROUND_REQ);
  httpclient.flush();

  //Found this code on github... not sure what it does yet
int respLen = 0;
  bool skip_headers = true;
  while (httpclient.connected() || httpclient.available()) {
    if (skip_headers) {
      String aLine = httpclient.readStringUntil('\n');
      //Serial.println(aLine);
      // Blank line denotes end of headers
      if (aLine.length() <= 1) {
        skip_headers = false;
      }
    }
    else {
      int bytesIn;
      bytesIn = httpclient.read((uint8_t *)&respBuf[respLen], sizeof(respBuf) - respLen);
      Serial.print(F("bytesIn ")); Serial.println(bytesIn);
      if (bytesIn > 0) {
        respLen += bytesIn;
        if (respLen > sizeof(respBuf)) respLen = sizeof(respBuf);
      }
      else if (bytesIn < 0) {
        Serial.print(F("read error "));
        Serial.println(bytesIn);
      }
    }
    delay(1);
  }
httpclient.stop();


 if (respLen >= sizeof(respBuf)) {
    Serial.print(F("respBuf overflow "));
    Serial.println(respLen);
    delay(DELAY_ERROR);
    return;
  }
  // Terminate the C string
  respBuf[respLen++] = '\0';
  Serial.print(F("respLen "));
  Serial.println(respLen);
  //Serial.println(respBuf);

  if (getWeather(respBuf)) {
    delay(DELAY_NORMAL);
  }
  else {
    delay(10000);
  }
}
// end of code I got on github
bool getWeather (char *json){

 StaticJsonBuffer<3*1024> jsonBuffer;
    // Skip characters until first '{' found
    // Ignore chunked length, if present
    char *jsonstart = strchr(json, '{');
    //Serial.print(F("jsonstart ")); Serial.println(jsonstart);
    if (jsonstart == NULL) {
      Serial.println(F("JSON data missing"));
      return false;
     }
     json = jsonstart;

  // Parse JSON
    JsonObject& root = jsonBuffer.parseObject(json);
    if (!root.success()) {
       Serial.println(("jsonBuffer.parseObject() failed"));
       return false;
    } 
   // for(int i =0; i < 24; i++){ 
    //  JsonObject& current = root["temp"][0];
   //   JsonObject& percip = root["snow"];
    //  JsonObject& rain = root["condition"];
     // temp[i]= current[i];
   // }



  
  } 

 
  // put your main code here, to run repeatedly:


void getTemperature(){
  
}
void getColor(int temp){
  
  if (temp < 0){
    color[0]= 10;
    color[1]= 5;
    color[2]= 58;
  }
  else if (temp >= 0 && temp <20){
    color[0]=10 ;
    color[1]= 26;
    color[2]= 63;
  }
 else if (temp >=20 && temp <32){
    color[0]= 21;
    color[1]= 68;
    color[2]= 121;
  }
 else if(temp>=32 && temp <40){
    color[0]= 56;
    color[1]= 187;
    color[2]= 202;
  }
 else if(temp >= 40 && temp < 50){
    color[0]= 37;
    color[1]= 235;
    color[2]= 195;
  }
 else if(temp>=50 && temp <60){
    color[0]= 122;
    color[1]= 176;
    color[2]= 82;
  }
 else if(temp >=60 && temp<70){
    color[0]= 238;
    color[1]= 231;
    color[2]= 38;
  }
  else if(temp>=70 && temp<80){
    color[0]= 245;
    color[1]= 168;
    color[2]= 14;
  }
 else if(temp>=80 && temp < 90){
    color[0]= 245;
    color[1]= 83;
    color[2]= 14;
  }
 else  if(temp >=90 && temp<100){
    color[0]= 225;
    color[1]= 51;
    color[2]= 0;
  }
 else{
    color[0]= 247;
    color[1]= 15;
    color[2]= 15;
  }
}
void displayNumbers(int temps[], boolean percipitation[], boolean snow[]){
  int tensTemp= 0;
  int onesTemp= 0;
  int tensTime= 0;
  int onesTime= 0;
  int temp =0;
  for( int i =0 ; i < 24; i++){
    temp = (int)temps[i];
    getColor(temp);
    updateSevenSeg(23, temp/10);
    updateSevenSeg(16, temp%10);
    updateSevenSeg(9, i/10);
    updateSevenSeg(2,i%10);
    if (temp<0){
      updateLED(31);
    }
    if (percipitation[i]){
      updateLED(31);
    }
    if (snow[i]){
      updateLED(20);
    }
    delay(100);
  }
}

void updateLED(int num){
  leds[num].setRGB(color[0], color[1], color[2]);
}
void setBlank(boolean num[]){
  for(int i = 0; i< 7; i++){
    blank[i] = num[i];
  }
}
String getNumber(int n){
  switch(n){
    case 0:
      setBlank(zero);
      break;
    case 1:
       setBlank(one);
       break;
    case 2:
       setBlank(two);
       break;
     case 3:
       setBlank(three);
       break;
     case 4:
       setBlank(four);
       break;
     case 5:
       setBlank(five);
       break;
     case 6:
       setBlank(six);
       break;
     case 7:
       setBlank(seven);
       break;
     case 8:
       setBlank(eight);
       break;
     case 9:
       setBlank(nine);
       break;
     default:
       break;;
  }
}
void updateSevenSeg(int start, int num){
  getNumber(num);
  for(int i = 0; i < 7; i++){
      if(blank[i]){
      leds[i+start].setRGB(color[0], color[1], color[2]);
      }
  }
}
void zeroDisplay(){
  for(int i =0; i < 32; i++){
    leds[i]=0x000000; // turn off leds (reset them
  }
  for(int i = 0; i < 7; i++){
    blank[i] = false;
  }
}


