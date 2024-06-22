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
    * Get time from NTP server: true            
    ch.pool.ntp.org
    * Use 24h format: true
    * Time zone: CET/CEST
  * Usermods
    *BaernerZyt
      * Aktiv: true
      * LED Matrix Breite: 11 (depends on the layout)
      * Offset: 0
      * Zeige ES ISCH: true
      * Minuten Punkte: true
      * Layout: 1

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