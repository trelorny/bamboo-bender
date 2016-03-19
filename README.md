# bamboo-bender
Green master all the way - make use of arduino &amp; Java to bring Atlassian Bamboo builds to life

## What does it do?
Utilizing a simple Arduino Setup (Sketch is also included in this repository) this project allows you to map different bammboo status to different actions "in the real world". 

By default the application
* connects to bamboo every 30 seconds
* reads out details about the last build (successful/error and wether it is currently building)
* provides this information to arduino via a parallel port connection
* arduino then "maps" these information to real world events like flashing a diode in different colors and/or flashing a very visible rotating light

One demo in an early state can be seen in this video:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=t6Ux2xYcfyk
" target="_blank"><img src="http://img.youtube.com/vi/t6Ux2xYcfyk/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

## Involved Components
As this is a project constiting of two parts (hardware with arduino and software with Java 8) the used components are listed seperately below.

* Java 8
  * JSSC for communicating via parallel port
  * Quartz Scheduler
  * JAXP-API for XML parsing
  * Logback & SLF4J as Logging Framework

* Arduino
  * Arduino UNO R3 (or similar)
  * RGB LED Module or different colored LEDs
  * Small Breadboard or circuit board
  * Optional Housing & one of a cardboard Futuramas Bender to give a face to the project
  * USB Cable 
  * Wires
  * Optional: Relay Module, External 12V rotating light + 12V Power Supply
  * 
 
## Available parallel port commands
| Command        | Effect                                                                                                            |
| -------------- |:------------------------------------------------------------------------------------------------------------------|
| BUILDING_ON    | Causes a fade-in & fade-out effect of the main indicator                                                          |
| BUILDING_OFF   | Causes main indicator to shine instead of fading                                                                  |
| ERROR_ON       | Causes main indicator to show red color                                                                           |
| SUCCESS_ON     | Causes main indicator to show green color                                                                         |
| LIGHTS_ON      | Switches the relay on and will show a better recognized indicator (i.e. flashing or rotating light)               |
| LIGHTS_OFF     | Switches the relay off and                                                                                        |
| MASTER_RED_ON  | Shows red lights on secondary indicators (for example the eyes)                                                   |
| MASTER_RED_OFF | Switches off red lights on secondary indicators                                                                   |
| LOWER_CIGAR    | Switches the servo to be lowered (if not already in lowered position)                                             |
| RAISE_CIGAR    | Switches the servo to be raised (if not already in raised position)                                               |

Commands can and should be concatenated using an arbitrary character, for example the pipe symbol "|". The string

``` BUILDING_ON|SUCCESS_ON|MASTER_RED_ON|LOWER_CIGAR ```

would cause the Bender figure used in this project to show green pulsing main indicator (the RGB LED in Benders antenna), a red secondary indicator (Benders eyes will shine in red color) and his cigar is in lowered position.
