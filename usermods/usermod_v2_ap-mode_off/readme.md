# Usermods Access-Point-Mode Off

If you have no WLAN, the AP-Mode is turned on automaticly. But you would like to disable the AP-Mode (because of LED Flickering in AP-Mode).

This Usermod turns the AP-Mode by default **off**.

To turn **on** the AP-Mode again you can press a button. Another press of the button turns the AP-Mode **off** again.

## Installation 

Uncomment the corresponding lines in `usermods_list.cpp` and compile!  

## Button
Push-button 

| Push Button | ESP32 |
|-------|---|
| 1 | GND|
| 2  | Pin 33 (Change in config)|

