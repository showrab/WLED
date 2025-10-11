# WIFI setup

## Connect to existing network
Network name (SSID): Name of your local network the clock should connect to.
Network password: Password of your local network
## Configure Access Point
AP SSID: Name of the WLAN the clock will provide for setup. 
(Once configured, you can no more connect to the clock via USB and IP-address 4.3.2.1).
AP passsowrd: Password to the WLAN the clock will provide for setup.
# LED Preferfences
## LED & Hardware setup
Total LEDs: 144
Max. current: 800 mA
## Hardware setup
1: WS281x (Type of LED-Strip) 

Color Order: GRB

Start: 0 

Length: 144 

GPIO: 16 

Reversed (rotated 180°): unchecked 

Skip first LEDs: 0 

Off Refresh: unchecked 
# 2D Configuration
## 2D setup
Strip or panel: 2D Matrix 
## Matrix Generator (keep default values)
Panel dimensions (WxH): 12 x 12 
Horizontal panels: 1 Vertical panels: 1 
1st panel: Top Left 
Orientation: Horizontal 
Serpentine: unchecked 
## Panel setup 
Number of panels: 1 
## LED panel layout (Panel 0):
1st LED: Bottom Right 

Orientation: Horizontal 

Serpentine: Check! 

Dimensions (WxH) ; 12 x 12 

Offset X : 0 Y : 0 

Panel sollte unten rechts einen grünen Punkt zeigen und oben 
rechts einen roten Punkt.
# User Interface
Leave everything on default.
# Sync Interfaces
Leave everything on default.
# Time & Macros
## Time setup
Get time from NTP server: Check if clock will be connected to a WLAN and will have access to the internet. If checked: ntp13.metas.ch 

Use 24h format: Unchecked 

Time zone: CET/CEST 

UTC offset: 0 seconds 

Latititude: N 46,95 

Longitude: E 7,42 

# Usermods
## BaernerZyt
Aktiv: Checked 

LED Matrix Breite: 12 

Zeige ES ISH: Checked 

Minuten Punkte: Unchecked 

Minuten Ausgeschrieben: Checked 

Layout: 4 (12 x 12) 

Test: Unchecked 
## GPS Time Usermod
Enabled: Unchecked 
# Security & Updates
Leave everything on default.
