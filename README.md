# PowerFlex4m
 Abstraction layer for simple ModBus communications with PowerFlex 4m VFDs

# Hardware Connection
## Arduino side
Tested with DFRobot RS485 Shield V1.0
![rs 485 shield](https://i.imgur.com/X3ageF4.jpg "RS485 Shield")
I use the internal 120ohm terminating resistor

## Powerflex side
![powerflex 4m](https://i.imgur.com/knDs6gM.jpg "PowerFlex 4m")
Connect as per the diagram in the PowerFlex user documentation [Rockwell Automation Document](https://literature.rockwellautomation.com/idc/groups/literature/documents/um/22f-um001_-en-e.pdf "22F-UM001D-EN-E PowerFlex 4M Adjustable Frequency AC Drive FRN 1.xx - 2.xx User Manual"). I use pins 4 and 5 on a standard Cat6 Ethernet lead.

![excerpt](https://i.imgur.com/wY0lp5A.png "Page 101")
