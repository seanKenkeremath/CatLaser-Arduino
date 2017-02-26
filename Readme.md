
#Parts
* 2x TowerPro SG90 9G Mini Servos
* HX1838 IR receiver + bundled remote http://a.co/7YYYZHd
* Arduino Uno (+ power supply)
* 22 Ohm resistor
* 5v laser diode http://a.co/90wOem9
* Hot Glue

![Connecting the servos and diode](http://i.imgur.com/CmEq70B.jpg)

#Hooking it all up
* Attach servos such that there is a base servo that controls the vertical axis and a second servo hot glued to the end (via plastic spinny thing that comes with the servo) that controls the horizontal axis. See above image
* Glue laser diode to one of the plastic connectors for the top, horizontal axis servo and attach to servo.
* GND from Arduino and ground pins for HX1838 IR Receiver, servos, and laser diode should all be connect as common ground
* 5v power from Arduino connects to both power cables of servos as well as right pin (VCC) of IR Receiver
* Pin 6 from Arduino connects to 22 Ohm resistor which then connects to + of laser diode
* Pin 11 from Arduino connects to left pin (Signal) of IR Receiver
* Pin 9 (PWM) from Arduino connects to signal pin of base servo (Vertical axis)
* Pin 6 (PWM) from Arduino connects to signal pin of second servo (Horizontal axis, the one with the diode on it)
* Upload catlaser.ino to the Arduino

Note: The code is currently set to use the Ch- button the bundled IR remote as power and the +/- buttons to increase and decrease speed of the laser. If you want to use a different remote you'll have to adjust that part of the code.