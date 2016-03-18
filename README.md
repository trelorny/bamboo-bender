# bamboo-bender
Green master all the way - make use of arduino &amp; Java to bring Atlassian Bamboo builds to life

## What does it do?
Utilizing a simple Arduino Setup (Sketch is also included in this repository) this project allows you to map different bammboo status to different actions "in the real world". 

By default the application
* connects to bamboo every 30 seconds
* reads out details about the last build (successful/error and wether it is currently building)
* provides this information to arduino via a parallel port connection
* arduino then "maps" these information to real world events like flashing a diode in different colors and/or flashing a very visible rotating light

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
 
