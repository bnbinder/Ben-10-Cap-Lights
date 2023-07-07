# Ben-10-Cap-Lights

# Installation 
- download ledstriptest.ino
- download <a href = "https://fastled.io/">FastLED</a> and install in arduino library
- deploy onto arduino with rfid and lights wired to it

# How 2 Use
- place an rfid tag onto rfid reader (in my case i used skylanders) to change led config

# How 2 Edit Script
- change `DASHinterval` to change how long the dashes stay up
- change `DOTinterval` to change how long the dots stay up
- change these if statments to config which rfid tag triggers what led command
```
if (content == "04E82919") //<----- the rfid tag that activates this script
{
  watchaDoing = 1; //<----- the config number, which is later set in another if statement
}

if (watchaDoing == 1)
{
  if (lastWatchaDoing != watchaDoing)
  {
    firstTime = true; //<--- need this so the leds know its the first time
                      //the func is running since last led change to do stuff properly
  }
  WPIColors(); //<---- whatever u want
  Serial.println("writing");
  digitalWrite(4, LOW);
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
}
```
