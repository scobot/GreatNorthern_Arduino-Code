# GreatNorthern_Arduino-Code
This Arduino program was created to run the soundtrack and light automation for Rick Araluce's art installation 
"The Great Northern" 
MAD ART Seattle WA, January 17th-February 27th, 2016
http://madartseattle.com/exhibits/the-great-northern/
 
The challenge was to create the feeling of a train that was coming but never arriving.
A low rumbling soundscape created with digital audio software, and a long slow fade up (and down) of LED lights were programmed for Arduino. The whole cycle lasted around 3min and needed to elegantly start with a single power source and then loop indefinitely. 

The code is cobbled together from of ADAFRUIT's AUDIO SHEILD CODE, as well as bits and pieces from various LED fade examples.
I apologies for not giving proper credit, but my note keeping got pushed aside as I tried desperately to find a solution in the aloted time, and not let Rick down! 

It was amazing how hard it was to find code for a long, slow fade up (and down) of an LED. People seem to like blinky lights!
I thought for sure this would be something people do all the time, make something happen at a certain time, but found it very hard to find examples of anyone doing that. So in the end I had to hack together various codes get it to work.

I had to time the lights to the soundtrack, so I used the serial port output to display the Arduino "clock" time numbers.
As these numbers flew by, I coded in short text prints like "FADE LIGHT NOW" and "END AUDIO" so I could figure out when to have the lights fade up, how long to stay on, and fade off all based on the the soundtrack. 
It ended up working really well, thankfully!

Hopefully, if you need this functionality for your project, you will find this code of use.
-Scott Bennett 05-01-2016
