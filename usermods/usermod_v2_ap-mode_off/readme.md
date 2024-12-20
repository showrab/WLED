# Usermods Access-Point-Mode Off

If you have no WLAN, the AP-Mode is turned on automaticly. But you would like to disable the AP-Mode (because of LED Flickering in AP-Mode).

This usermod turns the AP-Mode by default **off**.

To turn **on** the AP-Mode again you can press a button. Another press of the button turns the AP-Mode **off** again.

If connection to a Router (Sta-Mode), this usermod does not have any effect.

## Warning
Touch Buttons are only on Pin 2,4,12,13,14,15,27,32,33 possible. 

If you misconfigured the buttons and can not enter in AP-Mode. You have to press the button 0 (Button on ESP32 Board) for 6s. This deletes the network configuration and turns on the AP-Mode after. Now you have to reconfigure the WiFi settings an can change the usermod button setting.

## Installation 

Uncomment the lines `#define USERMOD_AP_MODE_OFF`in `usermods_list.cpp` and compile!  

## Button
### Push-button 

| Push Button | ESP32 |
|-------|---|
| 1  | Pin 33 (Change in config)|
| 2 | GND|


### Touch-button 
Button configuration for BaernerZyt usermod

| Touch Button | ESP32 | comment |
|-------|---|---|
| Alu/Copper foil  | Pin 7 | foil around 4th minute LED or under the two most right letters |


