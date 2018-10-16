# BIKE LIGHT
A usb-powered bike light with a light sensor for automatic operation. The lights connect to power using a male usb connector
and can therefore be used with many electric bikes or be connected to a usb power-bank.
The circuit is intended to be connected directly to 5 volts and has no built in voltage regulation (this could of course be added). The microcontroller
will operate from c.a 2 volts up to 5.5 volts so the exact level is not critical.

## Bill of Materials
* Attiny85
* 8-Pin socket
* USB connector
* IRF 520
* Infrared LED
* NPN Transistor. Many will work, no need to use a beefy one
* LEDs -> Choose whatever you want, just make sure to stay within the current limitations
* Resistors
  1 100 kOhm
  1 10 kOhm
  Resistors for LEDs. The circuit assumes that the LEDs have appropriate resistors mounted
* Capacitors
  2 .1uF ceramic or polyester capacitors
  1 470uF electrolytic capacitor
* Cable connectors
* A USB cable

## Circuit
image of circuit diagram. Image of Boards, links to circuit and gerber, notes on mounting.
### Link to gerber-files

## Current Limitations

## Programming
The code is written in Arduino to make it easy to understand. If you do not have a dedicated 
programmer for the Attiny85, you can use an arduino UNO or arduino UNO compatible board
to program the Tiny. Just follow the guide in the link below.

https://create.arduino.cc/projecthub/arjun/programming-attiny85-with-arduino-uno-afb829


