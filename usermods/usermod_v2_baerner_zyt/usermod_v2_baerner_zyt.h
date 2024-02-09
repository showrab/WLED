#pragma once

#include "wled.h"

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

class BaernerZytUsermod : public Usermod 
{
  private:
    unsigned long lastTime = 0;
    int lastTimeMinutes = -1;
    int lastTimeHours = -1;
    bool testStarted = false;

    // set your config variables to their boot default value (this can also be done in readFromConfig() or a constructor if you prefer)
    bool usermodActive = true;
    bool displayItIs = true;
    int ledOffset = 0;
    int layout = 1;
    bool test = false;
    
    // defines for mask sizes
    #define maskSizeLeds        256
    #define maskSizeMinutes     12
    #define maskSizeHours       6
    #define maskSizeItIs        6
    #define maskSizeMinuteDots  4

    const int maskMinutes16x16_0[12][maskSizeMinutes] = {};
    const int maskHours16x16_0[13][maskSizeHours] = {};
    const int maskMinuteDots0[maskSizeMinuteDots] = {};

    //16x16 matrix wiring
    //  0- 15  ES ISCH FÜF
    // 16- 31   ZÄH VIERTU
    // 32- 47  ZWÄNZG  VOR
    // 48- 63  AB    HALBI    
    // 64- 79  EISDRÜZWÖI
    // 80- 95  VIERI  FÜFI 
    // 96-111  SÄCHSIBNI
    //112-127  NÜNI  ACHTI
    //128-143  ZÄHNI EUFI
    //144-159  1234 ZWÖUFI
    #define FUEF_1  8,   9,  10
    #define ZAEH_1 16,  17,  18
    #define VIERTU_1 20,  21,  22,  23,  24,  25
    #define ZWAENZG_1 32,  33,  34,  35,  36,  37
    #define HAUBI_1 51,  52,  53,  54,  55
    #define AB_1 48,  49
    #define VOR_1 39,  40,  41
    const int maskMinutes16x16_1[12][maskSizeMinutes] = 
    {
      {                   -1}, // 0 - 00
      { FUEF_1,    AB_1,  -1}, // 1 - 05 fünf nach
      { ZAEH_1,    AB_1,  -1}, // 2 - 10 zehn nach
      { VIERTU_1,  AB_1,  -1}, // 3 - 15 viertel nach
      { ZWAENZG_1, AB_1,  -1}, // 4 - 20 zwanzig nach
      { FUEF_1,   VOR_1,  -1}, // 5 - 25 fünf vor halb
      { HAUBI_1,          -1}, // 6 - 30 halb
      { FUEF_1,    AB_1,  -1}, // 7 - 35 fünf nach halb
      { ZWAENZG_1,VOR_1,  -1}, // 8 - 40 zwanzig vor
      { VIERTU_1, VOR_1,  -1}, // 13 - 45 viertel vor
      { ZAEH_1,   VOR_1,  -1}, // 10 - 50 zehn vor
      { FUEF_1,   VOR_1,  -1}, // 11 - 55 fünf vor
    };
    // hour masks
    // Normal wiring
    const int maskHours16x16_1[13][maskSizeHours] = 
    {
      { 64,  65,  66,            -1}, // 01: ein
      { 64,  65,  66,            -1}, // 01: eins
      { 68,  69,  70,  71,       -1}, // 02: zwei
      { 72,  73,  74,            -1}, // 03: drei
      { 80,  81,  82,  83,  84,  -1}, // 04: vier
      { 86,  87,  88,  89,       -1}, // 05: fünf
      { 96,  97,  98,  99, 100, 101}, // 06: sechs
      {100, 101, 102, 103, 104,  -1}, // 07: sieben
      {112, 113, 114, 115, 116,  -1}, // 08: acht
      {118, 119, 120, 121,       -1}, // 09: neun
      {128, 129, 130, 131, 132,  -1}, // 10: zehn
      {134, 135, 136, 137,       -1}, // 11: elf
      {148, 149, 150, 151, 152, 153}  // 12: zwölf and 00: null
    };
    // mask minute dots
    const int maskMinuteDots1[maskSizeMinuteDots] = {144, 145, 146, 147};

    //16x16 matrix wiring (11x9 Schweizerdeutsch)
    //  0- 15  ES ISCH FÜF
    // 16- 31  ZÄH  VIERTU
    // 32- 47  ZWÄNZG AB 
    // 48- 63  VOR HALBI Z
    // 64- 79  EISZWÖIDRÜW
    // 80- 95  UVIERIFÜFIÖ
    // 96-111  FSÄCHSIBNIU
    //112-127  IACHTINÜNIF
    //128-143  ZÄHNI+1234I
    #define FUEF_2  8,   9,  10
    #define ZAEH_2 16,  17,  18
    #define VIERTU_2 21,  22,  23,  24,  25,  26
    #define ZWAENZG_2 32,  33,  34,  35,  36,  37
    #define HAUBI_2 52,  53,  54,  55,  56
    #define AB_2 39,  40
    #define VOR_2 48,  49,  50
    const int maskMinutes16x16_2[12][maskSizeMinutes] = 
    {
      {                         -1 }, //  0 - 00
      { FUEF_2, AB_2,           -1 }, //  1 - 05 fünf ab
      { ZAEH_2, AB_2,           -1 }, //  2 - 10 zehn ab
      { VIERTU_2, AB_2,         -1 }, //  3 - 15 viertel ab
      { ZWAENZG_2, AB_2,        -1 }, //  4 - 20 zwanzig ab
      { FUEF_2, VOR_2, HAUBI_2, -1 }, //  5 - 25 fünf vor halb
      { HAUBI_2,                -1 }, //  6 - 30 halb
      { FUEF_2, AB_2,HAUBI_2,   -1 }, //  7 - 35 fünf ab halb
      { ZWAENZG_2, VOR_2,       -1 }, //  8 - 40 zwanzig vor
      { VIERTU_2, VOR_2,        -1 }, // 13 - 45 viertel vor
      { ZAEH_2, VOR_2,          -1 }, // 10 - 50 zehn vor
      { FUEF_2, VOR_2,          -1 }, // 11 - 55 fünf vor
    };
    // hour masks
    // Normal wiring
    const int maskHours16x16_2[13][maskSizeHours] = 
    {
      { 64,  65,  66,  -1,  -1,  -1}, // 01: ein
      { 64,  65,  66,  -1,  -1,  -1}, // 01: eins
      { 67,  68,  69,  70,  -1,  -1}, // 02: zwei
      { 71,  72,  73,  -1,  -1,  -1}, // 03: drei
      { 81,  82,  83,  84,  85,  -1}, // 04: vier
      { 86,  87,  88,  89,  -1,  -1}, // 05: fünf
      { 97,  98,  99, 100, 101, 102}, // 06: sechs
      {101, 102, 103, 104, 105,  -1}, // 07: sieben
      {113, 114, 115, 116, 117,  -1}, // 08: acht
      {118, 119, 120, 121,  -1,  -1}, // 09: neun
      {128, 129, 130, 131, 132,  -1}, // 10: zehn
      { 64,  80,  96, 112,  -1,  -1}, // 11: elf
      { 58,  74,  90, 106, 122, 138}  // 12: zwölf and 00: null
    };
    // mask minute dots
    const int maskMinuteDots2[maskSizeMinuteDots] = {134, 135, 136, 137};


    // "minute" masks 
    //  0- 15  ZWÄNZGFÜ FÜF   =6,7,23    AB =34,50
    // 16- 31  ÄVIERTUF ZÄH   =0,16,32   VOR=17,33,49 
    // 32- 47  HOAHAUBI ZWÄNZG=1,2,3,4,5
    // 48- 63  ZRB ADRÜ VIERTU=17,18,19,20,21,22
    // 64- 79  WÄSÄCHS+ HAUBI =35,36,37,38,39
    // 80- 95  ÖVHSHFI1 EIS   =113,98,83
    // 96-111  UIINTÜB2 ZWÖI  =48,64,80,97
    //112-127  FEUFIFN3 DRÜ   =53,54,55 
    //128-143  IRNUNII4 VIER  =81,97,113,129,128

    #define FUEF_3 6,7,23
    #define ZAEH_3 0,16,32
    #define VIERTU_3 17,18,19,20,21,22
    #define ZWAENZG_3 0,1,2,3,4,5
    #define HAUBI_3 35,36,37,38,39
    #define AB_3 34,50
    #define VOR_3 17,33,49
    const int maskMinutes16x16_3[12][maskSizeMinutes] = 
    {
      {                         -1 }, //  0 - 00
      { FUEF_3, AB_3,           -1 }, //  1 - 05 fünf ab
      { ZAEH_3, AB_3,           -1 }, //  2 - 10 zehn ab
      { VIERTU_3, AB_3,         -1 }, //  3 - 15 viertel ab
      { ZWAENZG_3, AB_3,        -1 }, //  4 - 20 zwanzig ab
      { FUEF_3, VOR_3, HAUBI_3, -1 }, //  5 - 25 fünf vor halb
      { HAUBI_3,                -1 }, //  6 - 30 halb
      { FUEF_3, AB_3,HAUBI_3,   -1 }, //  7 - 35 fünf ab halb
      { ZWAENZG_3, VOR_3,       -1 }, //  8 - 40 zwanzig vor
      { VIERTU_3, VOR_3,        -1 }, // 13 - 45 viertel vor
      { ZAEH_3, VOR_3,          -1 }, // 10 - 50 zehn vor
      { FUEF_3, VOR_3,          -1 }, // 11 - 55 fünf vor
    };
    // hour masks
    const int maskHours16x16_3[13][maskSizeHours] = 
    {
      { 113,98,83,       -1},  // 01: EIS
      { 113,98,83,       -1},  // 01: EIS
      { 48,64,80,97,     -1},  // 02: ZWÖI
      { 53,54,55,         -1}, // 03: DRÜ
      { 81,97,113,129,128,-1}, // 04: VIERI
      { 85,101,117,133,   -1}, // 05: FÜFI
      { 66,67,68,69,70,86},    // 06: SÄCHSI
      { 70,86,102,118,134,-1}, // 07: SIBNI
      { 52,68,84,100,116, -1}, // 08: ACHTI
      {130,131,132,133,   -1}, // 09: NÜNI
      { 48,65,82,99,116,  -1}, // 10: ZÄHNI
      {113,114,115,116,   -1}, // 11: EUFI
      { 48,64,80,96,112,128 }  // 12: ZWÖUFI
    };
    // mask minute dots
    const int maskMinuteDots3[maskSizeMinuteDots] = {87, 103, 119, 135};

    // mask "it is"
    const int maskItIs[maskSizeItIs] = {0, 1, 3, 4, 5, 6};

    // overall mask to define which LEDs are on
    int maskLedsOn[maskSizeLeds] = 
    {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //  0- 15  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 16- 31  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 32- 47  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 48- 63  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 64- 79  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 80- 95  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // 96-111  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //112-127  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //128-143  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //144-159  
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //160-175
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //176-191
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //192-207
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //208-223
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //224-239
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0   //240-256
    };

    // update led mask
    void updateLedMask(const int wordMask[], int arraySize)
    {
      // loop over array
      for (int x=0; x < arraySize; x++) {
        if (wordMask[x]<0) break;
        // check if mask has a valid LED number
        if (wordMask[x] >= 0 && wordMask[x] < maskSizeLeds) {
          // turn LED on
          maskLedsOn[wordMask[x]] = 1;
        }
      }
    }

    // set hours
    void setHours(int hours, bool fullClock)
    {
      int index = hours;

      // handle 00:xx as 12:xx
      if (hours == 0) {
        index = 12;
      }

      // check if we get an overrun of 12 o´clock
      if (hours == 13) {
        index = 1;
      }

      // special handling for "ein Uhr" instead of "eins Uhr"
      if (hours == 1 && fullClock == true) {
        index = 0;
      }

      // update led mask
      switch (layout) {
        case 1:
          updateLedMask(maskHours16x16_1[index], maskSizeHours);
          break;
        case 2:
          updateLedMask(maskHours16x16_2[index], maskSizeHours);
          break;
        case 3:
          updateLedMask(maskHours16x16_3[index], maskSizeHours);
          break;
      }
    }

    // set minutes
    void setMinutes(int index) {
      // update led mask
      switch (layout) {
        case 1:
          updateLedMask(maskMinutes16x16_1[index], maskSizeMinutes);
          break;
        case 2:
          updateLedMask(maskMinutes16x16_2[index], maskSizeMinutes);
          break;
        case 3:
          updateLedMask(maskMinutes16x16_3[index], maskSizeMinutes);
          break;
      }
    }

    // set minutes dot
    void setSingleMinuteDots(int minutes)
    {
      // modulo to get minute dots
      int minutesDotCount = minutes % 5;

      // check if minute dots are active
      if (minutesDotCount > 0) {
        // activate all minute dots until number is reached
        for (int i = 0; i < minutesDotCount; i++) {
          // activate LED
          switch (layout) {
            case 1:
              maskLedsOn[maskMinuteDots1[i]] = 1;  
              break;
            case 2:
              maskLedsOn[maskMinuteDots2[i]] = 1;  
              break;
            case 3:
              maskLedsOn[maskMinuteDots3[i]] = 1;  
              break;
          }
        }
      }
    }

    // update the display
    void updateDisplay(uint8_t hours, uint8_t minutes) {
      // disable/enable complete matrix at the bigging
      for (int x = 0; x < maskSizeLeds; x++) {
        maskLedsOn[x] = layout==0?1:0;
      } 
      if (layout==0) return;

      // display it is/es ist if activated
      if (displayItIs)
      {
        updateLedMask(maskItIs, maskSizeItIs);
      }

      // set single minute dots
      setSingleMinuteDots(minutes);

      // switch minutes
      switch (minutes / 5) 
      {
        case 0:
            // full hour
            setMinutes(0);
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
            setHours(hours + 1, false);
            break;
        case 6:
            // halb
            setMinutes(6);
            setHours(hours + 1, false);
            break;
        case 7:
            // 5 nach halb
            setMinutes(7);
            setHours(hours + 1, false);
            break;
        case 8:
            // 20 vor
            setMinutes(8);
            setHours(hours + 1, false);
            break;
        case 9:
            // viertel vor
            setMinutes(9);
            setHours(hours + 1, false);
            break;
        case 10:
            // 10 vor
            setMinutes(10);
            setHours(hours + 1, false);
            break;
        case 11:
            // 5 vor
            setMinutes(11);
            setHours(hours + 1, false);
            break;
        }
    }

  public:
    //Functions called by WLED

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup() 
    {
    }

    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() 
    {
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
      if (test) {
        // do it every 1 seconds
        if (millis() - lastTime > 3000) {
          int minutes = lastTimeMinutes + 4;
          int hours = lastTimeHours + 1;
          
          //check valid time
          if (minutes>=60) {
            hours++; 
            minutes=0;
          }
          if (hours>12) {
            hours = 1;
          }

          //show time
          updateDisplay(hours, minutes);

          lastTimeMinutes = minutes;
          lastTimeHours = hours;
          // remember last update
          lastTime = millis();
        }
      } else {
        // do it every 5 seconds
        if (millis() - lastTime > 5000) {
          // check the time
          int minutes = minute(localTime);

          // check if we already updated this minute
          if (lastTimeMinutes != minutes) {
            // update the display with new time
            updateDisplay(hourFormat12(localTime), minute(localTime));

            // remember last update time
            lastTimeMinutes = minutes;

          }
          // remember last update
          lastTime = millis();
        }
      }
    }

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
    void addToJsonState(JsonObject& root)
    {
    }

    /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void readFromJsonState(JsonObject& root)
    {
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
    void addToConfig(JsonObject& root)
    {
      JsonObject top = root.createNestedObject(F("BaernerZyt"));
      top[F("Active")] = usermodActive;
      top[F("Display It Is")] = displayItIs;
      top[F("Led Offset")] = ledOffset;
      top[F("Layout")] = layout;
      top[F("Test")] = test;
    }

    void appendConfigData()
    {
      oappend(SET_F("addInfo('BaernerZyt:Led Offset', 1, 'Number of LEDs before the letters');"));
      oappend(SET_F("addInfo('BaernerZyt:Layout', 1, '0=16x16 off, 1=11x10, 2=11x9, 3=8x9');"));
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
    bool readFromConfig(JsonObject& root)
    {
      // default settings values could be set here (or below using the 3-argument getJsonValue()) instead of in the class definition or constructor
      // setting them inside readFromConfig() is slightly more robust, handling the rare but plausible use case of single value being missing after boot (e.g. if the cfg.json was manually edited and a value was removed)

      JsonObject top = root[F("BaernerZyt")];

      bool configComplete = !top.isNull();

      configComplete &= getJsonValue(top[F("Active")], usermodActive);
      configComplete &= getJsonValue(top[F("Display It Is")], displayItIs);
      configComplete &= getJsonValue(top[F("Led Offset")], ledOffset);
      configComplete &= getJsonValue(top[F("Layout")], layout);
      configComplete &= getJsonValue(top[F("Test")], test);

      return configComplete;
    }

    /*
     * handleOverlayDraw() is called just before every show() (LED strip update frame) after effects have set the colors.
     * Use this to blank out some LEDs or set them to a different color regardless of the set effect mode.
     * Commonly used for custom clocks (Cronixie, 7 segment)
     */
    void handleOverlayDraw()
    {
      // check if usermod is active
      if (usermodActive == true)
      {
        // loop over all leds
        for (int x = 0; x < maskSizeLeds; x++)
        {
          // check mask
          if (maskLedsOn[x] == 0)
          {
            // set pixel off
            strip.setPixelColor(x + ledOffset, RGBW32(0,0,0,0));
          }
        }
      }
    }

    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId()
    {
      return USERMOD_ID_BAERNERZYT;
    }

   //More methods can be added in the future, this example will then be extended.
   //Your usermod will remain compatible as it does not need to implement all methods from the Usermod base class!
};