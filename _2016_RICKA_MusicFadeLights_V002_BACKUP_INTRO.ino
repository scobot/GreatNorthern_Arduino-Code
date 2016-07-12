//********************* about *********************************************************************************************************

/*This Arduino program was created to run the soundtrack and light automation for Rick Araluce's art installation 
"The Great Northern" 
MAD ART Seattle WA, January 17th-February 27th, 2016
http://madartseattle.com/exhibits/the-great-northern/
 
The challenge was to create the feeling of a train that was coming but never arriving.
A low rumbling soundscape created with digital audio software, and a long slow fade up (and down) of LED lights were programmed for Arduino. 
The whole cycle lasted around 3min and needed to elegantly start with a single power source and then loop indefinitely. 

The code is cobbled together from of ADAFRUIT's AUDIO SHEILD CODE, as well as bits and pieces from various LED fade examples.
I apologies for not giving proper credit, but my note keeping got pushed aside as I tried desperately to find a solution in the aloted time, and not let Rick down! 

It was amazing how hard it was to find code for a long, slow fade up (and down) of an LED. People seem to like blinky lights!
I thought for sure this would be something people do all the time, make something happen at a certain time, but found it very hard to find examples of anyone doing that. 
So in the end I had to hack together various codes get it to work.

I had to time the lights to the soundtrack, so I used the serial port output to display the Arduino "clock" time numbers.
As these numbers flew by, I coded in short text prints like "FADE LIGHT NOW" and "END AUDIO" so I could figure out when to have the lights fade up, 
how long to stay on, and fade off all based on the the soundtrack. 
It ended up working really well, thankfully!

Hopefully, if you need this functionality for your project, you will find this code of use.
-Scott Bennett 05-01-2016*/

//******************** end about, begin code *********************************************************************************************

// a variable to hold values for when progam starts
unsigned long time;

//I think this is for the FADE effect on the LED
float pwmIntervals = 1000;
float R; // The R value in the graph equation

//Use this to FADE DOWN LED at end
int i = 0;

//markers to see if time is passing, and where we are
unsigned long StartFadeHere = 20000;
unsigned long FullBright = 60000;
unsigned long LightStaysOn = 116500; //120000
unsigned long StartFadeDown = 116500; //118000
unsigned long LightIsOff = 120000; //122000

//Libraries
#include <RBD_Timer.h> 
#include <RBD_Light.h>
// include SPI, MP3 and SD libraries
// for MP3 shield
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

//Led pins
//Setup the output PWM pin
//const int outputPin9 = 9;
const int outputPin = 10;
const int outputPin2 = 5;

//I DO need these - allows LED brightness to be "0"
RBD::Light light10(10);
RBD::Light light2(2);
//RBD::Light light9(9);

//RBD Timer
RBD::Timer timer;

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
 
void setup() {
  Serial.begin(9600); //communicate with the serial monitor
  Serial.println("Adafruit VS1053 Library Test");

  // HOW LONG TO RUN LIGHT PROGRAM: 60000 millis = 60sec (1 min)
  //3min = 180sec = 180000millis
  timer.setTimeout(138000); 
  
  //USE TO FADE LIGHT UP exponentially
  // set the pins connected to the LED as an output
  pinMode(outputPin, OUTPUT);
  pinMode(outputPin2, OUTPUT);
  
  // Calculate the R variable (only needs to be done once at setup)
  R = (pwmIntervals * log10(2))/(log10(255));

  //Resets the timer so we know where we are in the "timeline"
  timer.restart();

  // initialise the music player
  if (! musicPlayer.begin()) 
  { 
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));

  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
 
  if (!SD.begin(CARDCS)) {
   Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");
  
  // list files
  printDirectory(SD.open("/"), 0);
  // Set volume for left, right channels. lower numbers == louder volume!
  
  musicPlayer.setVolume(20,20);

  /***** Two interrupt options! *******/
  // *** This method is preferred
 if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT))
   Serial.println(F("DREQ pin is not an interrupt pin"));
}
 
void loop() 
{
  // Start playing a file, then we can do stuff while waiting for it to finish
  if (! musicPlayer.startPlayingFile("track001.mp3")) 
  {
    Serial.println("Could not open file track001.mp3");
   while (1);
  }
  //RESTART TIMER each loop
  timer.restart();
  Serial.println(F("Started playing"));
  Serial.println(timer.getValue());
  
  ////////// BIG LOOP WHILE MUSIC IS PLAYING ///////////////////////
  while (musicPlayer.playingMusic) 
  {
    // file is now playing in the 'background' so now's a good time
    // to do something else like handling LEDs or buttons :)
    //Serial.print(".");
        
    float brightness = 0;

    // BEGIN FADE AT defined TIME LOCATION
    if(timer.getValue()== StartFadeHere)
    {
      Serial.println("Begin to fade light here 20 sec: ");
      Serial.println(timer.getValue());
    
      for (float interval = 0; interval <= pwmIntervals; interval++)
      {
        // Calculate the required PWM value for this interval step
        //brightness = pow (2, (interval / R)) - 1;
        brightness = pow (2, (interval / R)) - 1;
        // Set the LEDs output to the calculated brightness
        analogWrite(outputPin, brightness);
        analogWrite(outputPin2, brightness);
        Serial.println(brightness);
        delay(40);
      }
        Serial.println("Light is FullBright at time: ");
        Serial.println(timer.getValue());   
    }
    //TIMELINE test to see how long light is fullbright
    while((timer.getValue()>FullBright) && (timer.getValue()<LightStaysOn))
      {
        //Serial.println("Light should be FullBright at 50 sec: ");
        Serial.println(timer.getValue());
      }
    
    //TIMELINE Light Starts to fade DOWN/off
    //if(timer.getValue()== StartFadeDown)
    if((timer.getValue()>=StartFadeDown) && (timer.getValue()<LightIsOff))
    {
      Serial.println("Start fade DOWN here: ");
      Serial.println(timer.getValue());
        while(timer.getValue()<LightIsOff)
        {
          for (i = 255; i > 0; i--)
          {
          analogWrite(outputPin, i);
          analogWrite(outputPin2, i);
          delay(40); //time to fade effect of light
          }
        }
      //set the RBD::Light brightness to (0)
      light10.setBrightness(0);
      light2.setBrightness(0);
      //Serial.println("Light is off here: ");
      Serial.println(timer.getValue());
    }
  }/////////////////////////// END OF WHILE MUSIC IS PLAYING //////
 
  Serial.println(F("Done Playing audio"));
  Serial.println(timer.getValue()); 
  if(timer.onRestart()) 
  {
    Serial.println("*****START TIMER AGAIN*****");
  }
}


/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}




