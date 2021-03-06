#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

unsigned long previousMillis = 0;
const long interval = 1000;
 
bool blink_state = false;
uint32_t blink_previousMillis = 0;

int notify_count = 0;
int timer = 0;
String a;

///////////////////////////////////////////////////////////////
int sensorvalue = 0;
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
int led = D0;       // digital
int sensorpin = A0; // analog
int buttonpin = D1;
int val = 0;
bool checkConnected = false;
///////////////////////////////////////////////////////////////

const char *ssid = "pleng";
const char *pass = "pleng1234";
const char *host = "http://sound-detector.herokuapp.com/bot.php"; //change this to your linebot server ex.http://numpapick-linebot.herokuapp.com/bot.php

#define APPID "SoundDetector"
#define KEY "q9UMtyMo7ZIczmD"
#define SECRET "G3C0Avh24MZSHavztZv0D3OKx"
#define ALIAS "sounbd" // Alias of this nodemcu

//Freeboard
#define TOPIC "/sounbd/" ALIAS // topic name

//Feed
#define FEEDNAME "SoundDetectorFeed"
#define FEEDKEY "gAXqcFLM6puJP1z2a1rdou2QGWaEbI20"

WiFiClient client;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t *msg, unsigned int msglen)
{
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
  // Handle message
  String msgLINE = (char *)msg;

  //////////////////////////////////////////////////////////////////////////
  //             Control manually by use command LINE ON or OFF.          //
  //////////////////////////////////////////////////////////////////////////

  if (msgLINE == "Open" || msgLINE == "OPEN" || msgLINE == "open")
  {
    digitalWrite(D0, HIGH);
    send_json("Water barrier Opened"); // Show message "Turn on LED" from line
  }
  if (msgLINE == "Close" || msgLINE == "CLOSE" || msgLINE == "close")
  {
    digitalWrite(D0, LOW);
    send_json("Water barrier Closed"); // Show message "Turn off LED" from line
  }
}

void onConnected(char *attribute, uint8_t *msg, unsigned int msglen)
{
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
}

void send_json(String data)
{
  StaticJsonBuffer<300> JSONbuffer; //Declaring static JSON buffer
  JsonObject &JSONencoder = JSONbuffer.createObject();

  JSONencoder["ESP"] = data;

  JsonArray &values = JSONencoder.createNestedArray("values"); //JSON array
  values.add(20);                                              //Add value to array
  values.add(21);                                              //Add value to array
  values.add(23);                                              //Add value to array

  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  HTTPClient http; //Declare object of class HTTPClient

  http.begin(host);                                   //Specify request destination
  http.addHeader("Content-Type", "application/json"); //Specify content-type header

  int httpCode = http.POST(JSONmessageBuffer); //Send the request
  String payload = http.getString();           //Get the response payload

  Serial.println(httpCode); //Print HTTP return code
  Serial.println(payload);  //Print request response payload

  http.end(); //Close connection
}

void setup()
{
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(CONNECTED, onConnected);
  Serial.begin(115200);

  ///////////////////////////////////////////////////////////////
  pinMode(led, OUTPUT); // digital
  pinMode(buttonpin, INPUT);
  pinMode(sensorpin, INPUT);
  ///////////////////////////////////////////////////////////////

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  microgear.init(KEY, SECRET, ALIAS); // initial params to microgear
  microgear.connect(APPID);           // connect to NETPIE
}

void loop()
{
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
  
  if (microgear.connected())
  {
    // Publish to a topic
    String a = String(buttonpin);
    microgear.publish(TOPIC, a); // Broadcast string message to the TOPIC
    microgear.loop();
    String b = String("{\"buttonpin\"} : " + a + ")"); // Make JSON format i.e. {"sound : 50 dB"}
    microgear.writeFeed(FEEDNAME, b, FEEDKEY);
    microgear.loop();

    // Serial.println ("NETPIE is connecting!");
    // Serial.println ("b");

    //////////////////////////////////////////////////////////////////////////
    //      Control Automatically by itself and sent message to user.       //
    //////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    val = digitalRead(sensorpin); // digital
    Serial.println(val);
    ///////////////////////////////////////////////////////////////
  }
  else
  {
    Serial.println("NETPIE is reconnecting...");
    if (timer >= 5000)
    {
      microgear.connect(APPID);
      timer = 0;
    }
    else
    {
      timer += 100;
    }
  }
}

Serial.println(notify_count);
if (digitalRead(buttonpin))
{
  Serial.println(val);
  notify_count++;
}


if (notify_count == 10)
{
  notify_count = 0;
  String message = "Fish is ready to breed , Enter 'open', if you want to open water barrier  or Enter 'close', if you want to close water barrier";

  send_json(message); // When count == 10 line sent message "Test" to user
}
delay(50);


}
