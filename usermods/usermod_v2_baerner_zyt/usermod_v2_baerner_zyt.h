#pragma once

#include "wled.h"
#include "layouts.h"
#include "zyt-macros.h"

/*
 * Usermods allow you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * 
 * This usermod can be used to drive a wordclock with a 11x10 pixel matrix with WLED. There are also 4 additional dots for the minutes. 
 * The visualisation is described in 4 mask with LED numbers (single dots for minutes, minutes, hours and "clock/Uhr").
 * There are 2 parameters to change the behaviour:
 * 
 * active: enable/disable usermod
 * diplayItIs: enable/disable display of "Es ist" on the clock.
 */

class BaernerZytUsermod : public Usermod {
  private:
    unsigned long lastTime = 0;
    unsigned long lastTime_2 = 0;
    int minUp = 0;
    int hourUp = 0;
    int lastTimeMinutes = 0;
    int lastTimeHours = 0;

    // set your config variables to their boot default value (this can also be done in readFromConfig() or a constructor if you prefer)
    bool usermodActive = true;
    int matrixCols = 16;
    int matrixRows = 16;
    int offset = 0;
    bool displayItIs = true;
    bool minuteDots = true;
    bool minuteWords = true;
    int layout = 1;
    bool test = false;

    //#define SEGMENT             strip._segments[strip.getCurrSegmentId()]

    //define arrays with the zyt-makros
    const int layoutCols[MAX_LAYOUTS] = { LAYOUT_COLS }; 
    const int maskItIs[MAX_LAYOUTS][maskSizeItIs] = { IT_IS };
    const int maskMinutes[MAX_LAYOUTS][14][maskSizeMinutes] = { MINUTES };
    const int maskHours[MAX_LAYOUTS][13][maskSizeHours] = { HOURS };
    const int maskMinuteDots[MAX_LAYOUTS][maskSizeMinuteDots] = { MINUTE_DOTS };
    const int maskMinuteWords[MAX_LAYOUTS][5][maskSizeMinuteWords] = { MINUTE_WORDS };
    const int nextHour[MAX_LAYOUTS] = { NEXT_HOUR };

    // overall mask to define which LEDs are ON and with which added collor (max 16x16)
    uint32_t maskLedsOn[maskSizeLeds] = {};

    //Colors to add to the effect
    uint32_t hourColor[MAX_LAYOUTS] = {HOUR_COLOR};
    uint32_t minuteColor[MAX_LAYOUTS] = {MINUTE_COLOR};
    uint32_t minuteDotsColor = RGBW32(128,0,255,0);
    uint32_t itIsColor = 1;

    // update led mask
    void updateLedMask(const int wordMask[], int arraySize, int layoutCols, uint32_t color) {
      // loop over array
      for (int x=0; x < arraySize; x++) {
        if (wordMask[x]<0) break;
        // draw words on a bigger matrix
        int skip = 0;
        if (matrixCols > layoutCols) {
          int row = wordMask[x] / layoutCols;
          skip = row * (matrixCols - layoutCols);
        }
        // check if mask has a valid LED number
        if (offset + wordMask[x] + skip >= 0 && wordMask[x] + skip < maskSizeLeds) {
          // turn LED on
          maskLedsOn[offset + wordMask[x] + skip] = color;
        }
      }
    }

    // set hours
    void setHours(int hours, bool fullClock)
    {
      int index = hours;
      if (hours >= 13) index = hours - 12;

      // special handling for "ein Uhr" instead of "eins Uhr"
      // if (hours == 1 && fullClock == true) {
      //   index = 0;
      // }
      // update led mask
      updateLedMask(maskHours[layout][index], maskSizeHours, layoutCols[layout], hourColor[layout]);
    }

    // set minutes
    void setMinutes(int index) {
      // update led mask
      updateLedMask(maskMinutes[layout][index], maskSizeMinutes, layoutCols[layout], minuteColor[layout]);
    }

    // set minutes dot
    void setSingleMinuteDots(int minutesDotCount)
    {
      // check if minute dots are active
      if (minuteDots) {
        // activate all minute dots until number is reached
        for (int i = 0; i < minutesDotCount; i++) {
          // activate LED
          updateLedMask(maskMinuteDots[layout], i+1, layoutCols[layout], minuteDotsColor);      
        }
      }
      if (minuteWords) {
        // activate LED (gli/fasch,...)
        updateLedMask(maskMinuteWords[layout][minutesDotCount], maskSizeMinuteWords, layoutCols[layout], minuteColor[layout]);         
      }
    }

    // update the display
    void updateDisplay(uint8_t hours, uint8_t minutes) {
      // disable/enable complete matrix at the bigging
      for (int x = 0; x < matrixCols *  matrixRows; x++) {
        maskLedsOn[x] = layout==0?RGBW32(255,255,255,255):0;
      } 
      if (layout==0) return;

      // display "it is" if activated
      if (displayItIs) {
        updateLedMask(maskItIs[layout], maskSizeItIs, layoutCols[layout], itIsColor);
      }

      // modulo to get minute dots
      int minutesDotCount = minutes % 5;

      // set single minute dots
      setSingleMinuteDots(minutesDotCount);

      // if minute words add 2 minutes when 2 min before next 5 Minute Sector
      if (minuteWords && minutesDotCount > 2 ) {
        minutes += 2;
      } 
      int nh = nextHour[layout]; //when minutes >=25 say next hour 7:25 -> fünf vor halb acht 
      
      int fullHourIndex = 0;
      if (hours == 0) {fullHourIndex = 13;} 
      if (hours == 12) {fullHourIndex = 12;}

      // switch minutes
      switch (minutes / 5) 
      {
        case 0:
            // full hour
            setMinutes(fullHourIndex);
            setHours(hours, true);
            break;
        case 1:
            // 5 nach
            setMinutes(1);
            setHours(hours, false);
            break;
        case 2:
            // 10 nach
            setMinutes(2);
            setHours(hours, false);
            break;
        case 3:
            // viertel nach
            setMinutes(3);
            setHours(hours, false);
            break;
        case 4:
            // 20 nach
            setMinutes(4);
            setHours(hours, false);
            break;
        case 5:
            // 5 vor halb
            setMinutes(5);
            setHours(hours + nh, false);
            break;
        case 6:
            // halb
            setMinutes(6);
            setHours(hours + nh, false);
            break;
        case 7:
            // 5 nach halb
            setMinutes(7);
            setHours(hours + nh, false);
            break;
        case 8:
            // 20 vor
            setMinutes(8);
            setHours(hours + nh, false);
            break;
        case 9:
            // viertel vor
            setMinutes(9);
            setHours(hours + nh, false);
            break;
        case 10:
            // 10 vor
            setMinutes(10);
            setHours(hours + nh, false);
            break;
        case 11:
            // 5 vor
            setMinutes(11);
            setHours(hours + nh, false);
            break;
      }
    }

  public:
    //Functions called by WLED

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup() {
      //const uint16_t matrixCols = SEGMENT.virtualWidth();
      //const uint16_t  matrixRows = SEGMENT.virtualHeight();
    }

    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() {
    }

    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     * 
     * Tips:
     * 1. You can use "if (WLED_CONNECTED)" to check for a successful network connection.
     *    Additionally, "if (WLED_MQTT_CONNECTED)" is available to check for a connection to an MQTT broker.
     * 
     * 2. Try to avoid using the delay() function. NEVER use delays longer than 10 milliseconds.
     *    Instead, use a timer check as shown here.
     */
    void loop() {
      if (!usermodActive || strip.isUpdating()) return;

      //Test the Layout with fast changing numbers
      if (test) {
        int ms = millis();

        // do it every x*1 seconds
        if (minUp < ms) {
          lastTimeMinutes = lastTimeMinutes + 1;
          if (lastTimeMinutes >=60) {
            lastTimeMinutes = 0;
          }
          if (lastTimeMinutes%10 ==  0) lastTimeHours = (lastTimeHours + 1) % 24;
          
          //show time
          updateDisplay(lastTimeHours, lastTimeMinutes);

          minUp = ms + 200;
        }
      } else {  //Real Time 
        // do it every 5 seconds
        if (millis() - lastTime > 5000) {
          // check the time
          int minutes = minute(localTime);

          // check if we already updated this minute
          if (lastTimeMinutes != minutes) {
            // update the display with new time
            updateDisplay(hour(localTime), minute(localTime));

            // remember last update time
            lastTimeMinutes = minutes;

          }
          // remember last update
          lastTime = millis();
        }
      }
    } //loop


    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    /*
    void addToJsonInfo(JsonObject& root)
    {
    }
    */

    /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void addToJsonState(JsonObject& root) {
    }

    /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void readFromJsonState(JsonObject& root) {
    }

    /*
     * addToConfig() can be used to add custom persistent settings to the cfg.json file in the "um" (usermod) object.
     * It will be called by WLED when settings are actually saved (for example, LED settings are saved)
     * If you want to force saving the current state, use serializeConfig() in your loop().
     * 
     * CAUTION: serializeConfig() will initiate a filesystem write operation.
     * It might cause the LEDs to stutter and will cause flash wear if called too often.
     * Use it sparingly and always in the loop, never in network callbacks!
     * 
     * addToConfig() will make your settings editable through the Usermod Settings page automatically.
     *
     * Usermod Settings Overview:
     * - Numeric values are treated as floats in the browser.
     *   - If the numeric value entered into the browser contains a decimal point, it will be parsed as a C float
     *     before being returned to the Usermod.  The float data type has only 6-7 decimal digits of precision, and
     *     doubles are not supported, numbers will be rounded to the nearest float value when being parsed.
     *     The range accepted by the input field is +/- 1.175494351e-38 to +/- 3.402823466e+38.
     *   - If the numeric value entered into the browser doesn't contain a decimal point, it will be parsed as a
     *     C int32_t (range: -2147483648 to 2147483647) before being returned to the usermod.
     *     Overflows or underflows are truncated to the max/min value for an int32_t, and again truncated to the type
     *     used in the Usermod when reading the value from ArduinoJson.
     * - Pin values can be treated differently from an integer value by using the key name "pin"
     *   - "pin" can contain a single or array of integer values
     *   - On the Usermod Settings page there is simple checking for pin conflicts and warnings for special pins
     *     - Red color indicates a conflict.  Yellow color indicates a pin with a warning (e.g. an input-only pin)
     *   - Tip: use int8_t to store the pin value in the Usermod, so a -1 value (pin not set) can be used
     *
     * See usermod_v2_auto_save.h for an example that saves Flash space by reusing ArduinoJson key name strings
     * 
     * If you need a dedicated settings page with custom layout for your Usermod, that takes a lot more work.  
     * You will have to add the setting to the HTML, xml.cpp and set.cpp manually.
     * See the WLED Soundreactive fork (code and wiki) for reference.  https://github.com/atuline/WLED
     * 
     * I highly recommend checking out the basics of ArduinoJson serialization and deserialization in order to use custom settings!
     */
    void addToConfig(JsonObject& root) {
      JsonObject top = root.createNestedObject(F("BaernerZyt"));
      top[F("Aktiv")] = usermodActive;
      top[F("LED Matrix Breite")] = matrixCols;
      top[F("Offset")] =  offset;
      top[F("Zeige ES ISCH")] = displayItIs;
      top[F("Minuten-Punkte")] = minuteDots;
      top[F("Minuten ausgeschrieben")] = minuteWords;
      top[F("Layout")] = layout;
      top[F("Test")] = test;
    }

    void appendConfigData() {
      oappend(SET_F("addInfo('BaernerZyt:LED Matrix Breite', 1, 'min 1, max 16');"));
      oappend(SET_F("addInfo('BaernerZyt:LED Matrix Höhe', 1, 'min 1, max 16');"));
      oappend(SET_F("addInfo('BaernerZyt:Offset', 1, 'verschiebe Start');"));
      oappend(SET_F("addInfo('BaernerZyt:Layout', 1, '0=16x16 off, 1=Melissa(11x10), 2=Chlie(11x9), 3=chliner(8x10), 4=Martin(12x12), 5=Andre(10x10), 6==Thomas(11x11), 7/8=Analog(10x10)');"));
      oappend(SET_F("addInfo('BaernerZyt:Test', 1, 'alle 3sec eine neue Zeit');"));
    }

    /*
     * readFromConfig() can be used to read back the custom settings you added with addToConfig().
     * This is called by WLED when settings are loaded (currently this only happens immediately after boot, or after saving on the Usermod Settings page)
     * 
     * readFromConfig() is called BEFORE setup(). This means you can use your persistent values in setup() (e.g. pin assignments, buffer sizes),
     * but also that if you want to write persistent values to a dynamic buffer, you'd need to allocate it here instead of in setup.
     * If you don't know what that is, don't fret. It most likely doesn't affect your use case :)
     * 
     * Return true in case the config values returned from Usermod Settings were complete, or false if you'd like WLED to save your defaults to disk (so any missing values are editable in Usermod Settings)
     * 
     * getJsonValue() returns false if the value is missing, or copies the value into the variable provided and returns true if the value is present
     * The configComplete variable is true only if the "exampleUsermod" object and all values are present.  If any values are missing, WLED will know to call addToConfig() to save them
     * 
     * This function is guaranteed to be called on boot, but could also be called every time settings are updated
     */
    bool readFromConfig(JsonObject& root) {
      // default settings values could be set here (or below using the 3-argument getJsonValue()) instead of in the class definition or constructor
      // setting them inside readFromConfig() is slightly more robust, handling the rare but plausible use case of single value being missing after boot (e.g. if the cfg.json was manually edited and a value was removed)

      JsonObject top = root[F("BaernerZyt")];

      bool configComplete = !top.isNull();

      configComplete &= getJsonValue(top[F("Aktiv")], usermodActive);
      configComplete &= getJsonValue(top[F("Zeige ES ISCH")], displayItIs);
      configComplete &= getJsonValue(top[F("LED Matrix Breite")], matrixCols);
      configComplete &= getJsonValue(top[F("Offset")],  offset);
      configComplete &= getJsonValue(top[F("Minuten-Punkte")], minuteDots);
      configComplete &= getJsonValue(top[F("Minuten ausgeschrieben")], minuteWords);
      configComplete &= getJsonValue(top[F("Layout")], layout);
      configComplete &= getJsonValue(top[F("Test")], test);

      return configComplete;
    }

    static inline uint32_t qadd32(uint32_t c1, uint32_t c2) {
      return RGBW32(
          qadd8(R(c1), R(c2)),
          qadd8(G(c1), G(c2)),
          qadd8(B(c1), B(c2)),
          qadd8(W(c1), W(c2))
      );
    }

    /*
     * handleOverlayDraw() is called just before every show() (LED strip update frame) after effects have set the colors.
     * Use this to blank out some LEDs or set them to a different color regardless of the set effect mode.
     * Commonly used for custom clocks (Cronixie, 7 segment)
     */
    void handleOverlayDraw() {
      // check if usermod is active
      if (usermodActive == true) {
        // loop over all leds
        for (int x = 0; x < matrixCols * matrixRows; x++) {
          // check mask
          if (maskLedsOn[x] == 0) {   // 0 means don't show his pixel
            // set pixel off
            strip.setPixelColor(x, RGBW32(0,0,0,0));
          } else {
            if (minuteColor[layout] != 1){  // 1 means add color to effect
              uint32_t oldC = strip.getPixelColor(x);
              strip.setPixelColor(x, qadd32(oldC, maskLedsOn[x]));
            }
            // if (minuteColor[layout] != 2){  // 2 means set color
            //   strip.setPixelColor(x, maskLedsOn[x]);
            // }
            //else show pixel with unchanged effect
          }
        }
      }
    }

    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId() {
      return USERMOD_ID_BAERNERZYT;
    }

   //More methods can be added in the future, this example will then be extended.
   //Your usermod will remain compatible as it does not need to implement all methods from the Usermod base class!
};