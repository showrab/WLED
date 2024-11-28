# Usermods Time from a GPS-Module

If you have no internet, but need the time to be set.
This Usermod connects to a GPS (TX) modul on serial1 (RX).

The GPS-Module transmits with NMEA protocoll (https://de.wikipedia.org/wiki/NMEA_0183). 
In the Recommended Minimum Sentence C (RMC) is the time and date as other informations:
```
$GPRMC,162614,A,5230.5900,N,01322.3900,E,10.0,90.0,131006,1.2,E,A*13
$GPRMC,HHMMSS,A,BBBB.BBBB,b,LLLLL.LLLL,l,GG.G,RR.R,DDMMYY,M.M,m,F*PP
```

HHMMSS = time (UTC) 
DDMMYY = date

## Installation 

Uncomment the lines `#define USERMOD_GPS_TIME` in `usermods_list.cpp` and compile!  

## GPS Module
The usermod was tested with:
* UBLOX-NEO-6M GPS

Connect the four pins from UBLOX to an Arduino as follows:

| Ublox | ESP32 |
|-------|---|
| GND | GND|
| TX  | RX Pin 37 (Change in config)|
| Vcc | 5Vdc or 3.3Vdc|

