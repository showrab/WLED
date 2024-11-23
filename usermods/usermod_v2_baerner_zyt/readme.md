# Bärner Zyt (Word Clock) Usermod V2

This usermod drives an max 16x16 pixel matrix wordclock with WLED. There are 4 additional dots for the minutes. 
![BärnerZyt](images/BärnerZyt.png)

These are the parameters that controls the behavior of "Bärner Zyt" in the WLED Config -> Usermods menue:
 ![Usermode](images/usermode.png)
* active: enable/disable usermod
* matrixCols: width of the matrix (this alows to show a small layout on a larger matrix)
* ledOffset: number of LEDs before the wordclock LEDs
* dyplay It Is: enable/disable display of "Es ist" on the clock
* minuteDots: show up to 4 dots to see exact minutes between the five minutes
* minuteWords: say the minutes between the five minutes in words (scho fasch füf ab drü)
* layout: Number of the layout
* test: Test mode to test the layouts with fast changing numbers

## Bärner-Zyt Configuration

* WiFi Setup
  * mDNS address: BaernerZyt
  * AP SSID: BaernerZytAP

* LED Preferences
  * Maximum Current: 1000mA
  * LED Type: WS281x
  * Color Order: GRB
  * Lenght: 114 (Layout 1: 10x11 +4) 
  * GPIO: 16
  * Button 0 GPIO: 0 Pushbutton //Button on ESP32 Board: LED ON/OFF
  * Button 1 GPIO: 13 Touch     //Macro presets 20: Brightness
  * Button 2 GPIO: 14 Touch     //Macro presets 30: Effect rotate

* 2D Configuration
  * Strip or panel: 2D Matrix
  * Number of panels: 2 (Layout 1 )
  * Panel 0
    * 1st LED: Bottom Right
    * Orientation: Horizontal
    * Serpentine: true
    * Dimension: 11x10
    * Offset: 0x0
  * Panel 1 (Layout 1)
    * 1st LED: Top Left
    * Orientation: Horizontal
    * Serpentine: true
    * Dimension: 4x1
    * Offset: 0x10
  * Time & Macros
    * Get time from NTP server: true //Sta-Mode,
    * Get time from NTP server: false //GPS           
    ch.pool.ntp.org
    * Use 24h format: true
    * Time zone: CET/CEST
    * Button actions
      |push switch|short|long|double|comment|
      |---|---|---|---|---|
      |Button 0| 0|||on ESP32 Board: ON/OFF|
      |Button 1|20|||Preset 30: rotate brightness|
      |Button 2|30|||Preset 50: rotate effects|
  * Usermods
    * BaernerZyt
      * Aktiv: true
      * LED Matrix Breite: 11 (depends on the layout)
      * Offset: 
      * Zeige ES ISCH: true
      * Minuten Punkte: true
      * Layout: 1
    * GPS Time Usermod
      * Enabled: true //Time from the GPS-Module
      * Pin: 18 //ESP32 RX-Pin zu GPS TX-Pin
      * Period: 3600s
    * AP-Mode Off Usermod
      * Enabled: true  //In AP-Mode turn off the AP-Mode
      * Pin: 4 
      * Touch Button: true  //Touch-Pins: 2,4,12,13,14,15,27,32,33
      * AP Mode On Pixel Pos: 113
* Security & Updates
  * Backup & Restore
    * Restore presets 
      * Browse: WLED/usermods/usermod_v2_baerner_zyt/settings/wled_presets.json Upload
    * Restore configuration
      * Browse: WLED/usermods/usermod_v2_baerner_zyt/settings/wled_cfg.json Upload
  
## Presets
* Brightness

  |Preset #|API command|
  |---|---|
  |20|P1=21;P2=23;PL=~|
  |21|{"bri":32}|
  |22|{"bri":64}|
  |23|{"bri":128}|

* Effects

  |Preset #|API command|
  |---|---|
  |30|P1=31&P2=44&PL=~|
  |31-35|Effekte|
  |36-44|Solid colors||

## WiFi Configuration
Description on how to [connect your Bärner-Zyt to your Router](WiFiSetup_de.md) or scann this QR-Code un the back of your "Bärner-Zyt" watch.
<img src="images/qr-code.png" alt="QR-Code" title="QR-Code" width=300/>

## Layouts

The file [layouts.h](layouts.h) there are already defined some layouts.

To define a new layout increment MAX_LAYOUTS by one, copy a already defined layout and paste everything to the end. Change on all #defines the layout number (#define LAYOUT_COLS_1 -> #define LAYOUT_COLS_8)

In File [zyt-macros.h](zyt-macros.h) add new line every where you find a "//insert new layout here" and set the number to the number of your layout.

## Installation Usermode

* `USERMOD_BAERNERZYT`   - define this to have this usermod included wled00\usermods_list.cpp
* `USERMOD_ID_BAERNERZYT - define this to have this usermod included wled00\const.h


## IDE Visual Studio
### PlatformIO
![Visual Studio](images/VisualCode.png)
* compile: √
* upload: ->
* microcontroller: lolin_s2_mini
* Serial port: cu.usbmodem01

## WLED requirements
WLAN and NTP

## Time
For people wihout a internet connection (Router/Access-Point) we have the possibility to set the time so:
```
curl -X POST "http://192.168.0.102/json/state" -d '{"time":1724433301}' -H "Content-Type: application/json"
```
If you have your cell phone an connect to the BaernerZyt access point somehow the WLED can get the time too.

Or use a GPS-Modul with the Usermod [GPS-Time](../usermod_v2_gps_time/readme.md)

## Change Log

2024/02/09 initial commit

# Hardware
## Microcontroller
esp32 lolin_s2_mini 

## Test-setup
esp32 16x16 LED Matrix

## Front
## Grid
## Backplane