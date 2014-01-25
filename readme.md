dAISy - A simple DIY AIS Receiver
=================================

dAISy (do AIS yourself) is a simple AIS receiver based on Silicon Labs [Si4362 EZRadioPro](http://www.silabs.com/products/wireless/ezradiopro/pages/si4362.aspx) receiver, Texas Instruments [MSP430G2553](http://www.ti.com/product/msp430g2553) MCU and [MSP-EXP430G2](http://www.ti.com/tool/msp-exp430g2) v1.5 LaunchPad. The software was developed with Code Composer Studio 5.5.

AIS, short for "Automatic Identification System", is a tracking system for ships. More on [Wikipedia](http://en.wikipedia.org/wiki/Automatic_Identification_System). There are many websites dedicated to tracking ships based on this system, like for example [MarineTraffic](http://www.marinetraffic.com/).

dAISy features:
- integrated radio, no need for external radio with discriminator tap
- hopping between channel A (161.975 MHz) and B (162.025 MHz)
- receives, decodes and validates packets according to ITU-R M.1371-4 (NRZI decoding, bit-destuffing, CRC validation) 
- wraps valid packets into NMEA 0183 sentences (AIVDM)
- sends NMEA sentences to PC via serial (9600 8N1)

The output of dAISy can be processed and visualized by mapping and navigation programs like [OpenCPN](http://opencpn.org).

All content of this project is published under CC BY-NC-SA - [Creative Commons Attribution-NonCommercial-ShareAlike](http://creativecommons.org/licenses/by-nc-sa/4.0/).  

A more detailed description of the project can be found in the [43oh.com forum](http://forum.43oh.com/topic/4833-potm-daisy-a-simple-ais-receiver/).
