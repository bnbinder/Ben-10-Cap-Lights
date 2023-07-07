#include <FastLED.h>
#include <SPI.h>
#include <MFRC522.h>

int watchaDoing = 0;
int lastWatchaDoing = 0;

//--------------------------------------
//            SKYLANDER STUFF
//--------------------------------------

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
const int ledPin = 7; // pin the LED is attached to
int incomingByte;      // variable stores  serial data
String content = ""; //actual rfid tag incoming
String REALcontent = ""; //??? idk im not deleting everything cuz i dont wanna test it again so just leaving it cuz idk lol
unsigned long startTime;
unsigned long RFIDelapsedTime;
unsigned long interval = 250; //leave this, dont change unless you really need to
/*if you want rfid to successfully read skylander, it needs to be half of what you want (every 1 second it updates, 500 mili needs to be interval)
   i dont get why it does this but it does, even tho theres no delay. might be my rfid reader is dogshit or idk

   nvm i fixed it so dont do this???????? idk im just that gud
*/

//--------------------------------------
//              LED STUFF
//--------------------------------------

#define LED_PIN     5
#define NUM_LEDS    123
#define BRIGHTNESS  70
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100 //for entire arduino!!!!!
unsigned long LEDstartTime;
unsigned long LEDelapsedTime;
unsigned long DASHinterval = 1000; //set this to whatever u want
unsigned long DOTinterval = 300; //set this to whatever u want
unsigned long actualInterval; //setting this to either dash or dot
int spaceTimeContinuum = 0; //space in between dashes, dots, and the actual spaces time
CRGB LEDcolor;
int count = 0;
String WPI[] = {"/", ".", "-", "-", "/", ".", "-", "-", ".", "/", ".", ".", "/"}; //this can be any amount of dashes, dots or spaces, like .--..-.--...- idk what this is but seperate it and itll work
int arraySize = sizeof(WPI) / sizeof(WPI[0]);
bool virgin = true;
bool isBlack = NULL;
CRGB test;
int offset = 0;


void setup() {
  //lights so we know what our cap leds are set to without asking someone "hey is my hat goin rainbow"
  pinMode(4, OUTPUT);  // 
  pinMode(1, OUTPUT);  // 
  pinMode(2, OUTPUT);  //
  pinMode(3, OUTPUT);  //
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  startTime = millis();
  LEDstartTime = millis();
}


void loop()
{
  //getting rfid tag and saving it. DO NOT EDIT UNLESS U KNOW WHAT UR DOIN 
  RFIDelapsedTime = millis() - startTime;
  //LEDelapsedTime - millis() - LEDstartTime;

  if (RFIDelapsedTime >= interval)
  {



    Serial.println("Timer expired!");
    startTime = millis();
    //^^^^^^^^^^^^^^^^^^^^^ cant be at bottom of if or else wont work and doesnt run the line, probably theres an error if no skylander on rfid
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
      return;
    }

    byte letter;
    content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      //Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    // Serial.println();
    content.toUpperCase();





    /*
     * if you want some privacy here u go ig
      if (content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
      {
      Serial.println("Authorized access");
      Serial.println();
      delay(3000);
      }

      else   {
      Serial.println(" Access denied");
      delay(3000);
      }
    */



  }
  Serial.println("content: " + content);
  //Serial.println("iscard: " +  mfrc522.PICC_ReadCardSerial());
  //Serial.println("Count" + String(count));
  //Serial.println("spacetimeshit: " + String(spaceTimeContinuum));
  //Serial.println("trueeee??" + String(LEDcolor == CRGB::White));
  //Serial.println(String(LEDcolor.r));
  //Serial.println(String(LEDcolor == CRGB::Black));
  //Serial.println("spacetimebool: " + String(spacetimedone));

  //so it knows when to activate virgin first time boolean
  lastWatchaDoing = watchaDoing;


  if (content == "04E82919")
  {
    watchaDoing = 1;

  }
  if (content == "A48B4C24")
  {
    watchaDoing = 0;

  }
  if ( content == "6629C9A3")
    {
    watchaDoing = 2;
    }
    

  if (watchaDoing > 2)
  {
    watchaDoing = 0;
  }

  if (watchaDoing == 0)
  {
    if (lastWatchaDoing != watchaDoing)
    {
      //Serial.println("watchadoing: " + String(watchaDoing));
      //Serial.println("lastwatchadoing: " + String(lastWatchaDoing));
      virgin = true;
      //Serial.println("virgin run");
    }
    WPIMorseCode();
    digitalWrite(4, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
  }
  if (watchaDoing == 1)
  {
    if (lastWatchaDoing != watchaDoing)
    {
      //Serial.println("watchadoing: " + String(watchaDoing));
      //Serial.println("lastwatchadoing: " + String(lastWatchaDoing));
      //Serial.println("virgin 2 run");
      virgin = true;
    }
    WPIColors();
    Serial.println("writing");
    digitalWrite(4, LOW);
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
  }
  if (watchaDoing == 2)
  {
    if (lastWatchaDoing != watchaDoing)
    {
      virgin = true;
    }
    PRIDEmonthYEEEAAAAHH();
    digitalWrite(4, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
  }
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

//quick way to set all leds to one color, manually do this if u want different colors at different positions (example is rainbow or wpi colors)
void setLEDS(CRGB color, int brightness)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
    leds[i].nscale8(brightness);
  }
}

//w p i in morse code
void WPIMorseCode()
{
  //if its your first time doing morse func, reset timer
  if (virgin)
  {
    count = 0;
    LEDstartTime = millis();
    LEDelapsedTime = 0;
    virgin = false;
    isBlack = NULL;
  }
  //if we are a dot, dot amount of time on the cap
  if (WPI[count] == ".")
  {
    actualInterval = DOTinterval;
  }
  //if we are a dash, dash amount of time on the cap
  if (WPI[count] == "-")
  {
    actualInterval = DASHinterval;
  }
  //get current time minus start time
  LEDelapsedTime = millis() - LEDstartTime;
  //Serial.println("elapsed: " + String(LEDelapsedTime));
  //Serial.println("start: " + String(LEDstartTime));

//if the cuirrent symbol isnt a space, and the cap leds arent black or are null (meaning this is the first time the func is being run)
  if (WPI[count] != "/" && (!isBlack || isBlack == NULL))
  {
    //not space time, but dot or dash time
    spaceTimeContinuum = 0;
    //if the lights havent been on for dash or dot amount of time
    if (LEDelapsedTime < actualInterval)
    {
      //set the lights to white
      LEDcolor = CRGB::White;
      //we are not black, but white
      isBlack = false;
    }
    else //if the lights have been on for dot or dash amount of time
    {
      //turn off leds
      LEDcolor = CRGB::Black;
      //we are now black lights
      isBlack = true;
      //reset timer
      LEDstartTime = millis();
      LEDelapsedTime = 0;
    }
  }
  //if we are a space and we arent black
  else if((!isBlack || isBlack == NULL)) //make elseif
  {
    //make cap lights black
    LEDcolor = CRGB::Black;
    //we are black lights
    isBlack = true;
    //set space in between this next thing to space time (1000 milis)
    spaceTimeContinuum = 1000;
    //ong
    Serial.println("im being acgtivateD");
  }
  //if we are black and we have passed the time in between symbols
  if (isBlack && LEDelapsedTime > 200 + spaceTimeContinuum)
  {
    //move on to next symbol
    count += 1;
    //reset timer
    LEDstartTime = millis();
    //lights should be white next time this function runs
    isBlack = false;
  }
  //edg case
  if (count == arraySize - 1)
  {
    count = 0;
  }

//after u know which color it should be, set everything to that color
  setLEDS(LEDcolor, BRIGHTNESS);
}


//wpi colors, red and white (im pretty sure)
void WPIColors()
{
  //Serial.println(offset);
  if (virgin)
  {
    LEDstartTime = millis();
    LEDelapsedTime = 0;
    offset = 0;
    virgin = false;
    //Serial.print("virgin run");
  }
  //because it isnt one single color and a complex amount of 
  //colors, manually set strip rather than call a func to do so
  for (int i = 0; i < NUM_LEDS; i++)
  {
    LEDelapsedTime = millis() - LEDstartTime;
    if (i <= NUM_LEDS / 2.0)
    {
      leds[(i + offset) % NUM_LEDS] = CRGB::Red;
      leds[(i + offset) % NUM_LEDS].nscale8(BRIGHTNESS);
    }
    else {
      leds[(i + offset) % NUM_LEDS] = CRGB::White;
      leds[(i + offset) % NUM_LEDS].nscale8(BRIGHTNESS);
    }
    if (LEDelapsedTime > 100)
    {
      offset = (offset + 1) % NUM_LEDS;
      LEDstartTime = millis();
    }
  }
}


//le rainbow
void PRIDEmonthYEEEAAAAHH()
{
  if (virgin)
  {
    LEDstartTime = millis();
    LEDelapsedTime = 0;
    offset = 0;
    virgin = false;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    LEDelapsedTime = millis() - LEDstartTime;

    if (i < NUM_LEDS / 6.0) {
      // Red
      leds[(i + offset) % NUM_LEDS] = CRGB::Red;
      leds[(i + offset) % NUM_LEDS].nscale8(BRIGHTNESS);
    }
    else if (i < (NUM_LEDS * 2.0) / 6 && i >= NUM_LEDS / 6.0) {
      // Orange
      leds[(i + offset) % NUM_LEDS] = CRGB::Orange;
      leds[(i + offset) % NUM_LEDS].nscale8(BRIGHTNESS);
    }
    else if (i < (NUM_LEDS * 3.0) / 6.0 && i >= (NUM_LEDS * 2.0) / 6.0) {
      // Yellow
      leds[(i + offset) % NUM_LEDS] = CRGB::Yellow;
      leds[(i + offset) % NUM_LEDS].nscale8(BRIGHTNESS);
    }
    else if (i < NUM_LEDS * 4.0 / 6.0 && i >= (NUM_LEDS * 3.0) / 6.0) {
      // Green
      leds[(i + offset) % NUM_LEDS] = CRGB::Green;
      leds[(i + offset) % NUM_LEDS].nscale8(BRIGHTNESS);
    }
    else if (i < NUM_LEDS * 5.0 / 6.0 && i >= (NUM_LEDS * 4.0) / 6.0) {
      // Blue
      leds[(i + offset) % NUM_LEDS] = CRGB::Blue;
      leds[(i + offset) % NUM_LEDS].nscale8(BRIGHTNESS);
    }
    else {
      // Purple
      leds[(i + offset) % NUM_LEDS] = CRGB::Purple;
      leds[(i + offset) % NUM_LEDS].nscale8(BRIGHTNESS);
    }

    if (LEDelapsedTime > 100) {
      offset = (offset + 1) % NUM_LEDS;
      LEDstartTime = millis();
    }
  }
}
