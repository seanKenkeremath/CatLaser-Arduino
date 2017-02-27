#Parts
* 2x TowerPro SG90 9G Mini Servos
* 10k Potentiometer
* Toggle Switch (SPST)
* Arduino Uno (+ power supply)
* 22 Ohm resistor
* 5v laser diode http://a.co/90wOem9
* Hot Glue

![Connecting the servos and diode](http://i.imgur.com/CmEq70B.jpg)

#Hooking it all up
* Attach servos such that there is a base servo that controls the vertical axis and a second servo hot glued to the end (via plastic spinny thing that comes with the servo) that controls the horizontal axis. See above image
* Glue laser diode to one of the plastic connectors for the top, horizontal axis servo and attach to servo.
* GND from Arduino and ground pins for Potentiometer, servos, and laser diode should all be connect as common ground
* 5v power from Arduino connects to both power cables of servos as well as input pin of Potentiometer
* Pin 6 from Arduino connects to 22 Ohm resistor which then connects to + of laser diode
* Pin 2 from Arduino connects to Toggle switch which then connects to ground. We will be using the INPUT_PULLUP setting on that pin so we don't have to worry about adding a pullup resistor.
* Analog Pin A0 from Arduino connects to output pin of Potentiometer
* Pin 9 (PWM) from Arduino connects to signal pin of base servo (Vertical axis)
* Pin 6 (PWM) from Arduino connects to signal pin of second servo (Horizontal axis, the one with the diode on it)
* Upload catlaser.ino to the Arduino