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

class BaernerZytUsermod : public Usermod {
  private:
    unsigned long lastTime = 0;
    unsigned long lastTime_2 = 0;
    int lastTimeMinutes = -1;
    int lastTimeHours = -1;
    bool testStarted = false;

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
    
    // defines for mask sizes
    #define maskSizeLeds          16*16
    #define maskSizeMinutes       12
    #define maskSizeHours         6
    #define maskSizeItIs          6
    #define maskSizeMinuteDots    4
    #define maskSizeMinuteWords   9

    #define MAX_LAYOUTS           6


    //#define SEGMENT             strip._segments[strip.getCurrSegmentId()]

    //--------------Layout 0 (16x16 leres Layout 0)---------------- 
    #define layoutCols_0 16
    //--------------Layout 1 (11x10 Schweizerdeutsch)----------------
    //  0- 10  ES-ISCH-FÜF
    // 11- 21  -ZÄH-VIERTU
    // 22- 32  ZWÄNZG--VOR
    // 33- 43  AB----HALBI    
    // 44- 54  EISDRÜZWÖI-
    // 55- 65  VIERI--FÜFI 
    // 66- 76  SÄCHSIBNI--
    // 77- 87  NÜNI--ACHTI
    // 88- 98  ZÄHNI-EUFI-
    // 99-110  1234-ZWÖUFI
    #define FUEF_1        8,9,10
    #define ZAEH_1        12,13,14
    #define VIERTU_1      16,17,18,19,20,21
    #define ZWAENZG_1     22,23,24,25,26,27
    #define HAUBI_1       39,40,41,42,43
    #define AB_1          33,34
    #define VOR_1         30,31,32
    #define EIS_1         44,45,46,      -1
    #define ZWOEI_1       50,51,52,53,   -1
    #define DRUE_1        47,48,49,      -1
    #define VIERI_1       55,56,57,58,59,-1
    #define FUEFI_1       62,63,64,65,   -1
    #define SAECHSI_1     66,67,68,69,70,71
    #define SIBNI_1       70,71,72,73,74,-1
    #define ACHTI_1       83,84,85,86,87,-1
    #define NUENI_1       77,78,79,80,   -1
    #define ZAENI_1       88,89,90,91,92,-1
    #define EUFI_1        94,95,96,97,   -1
    #define ZWOEUFI_1     104,105,106,107,108,109  
    #define layoutCols_1  11
    #define itIs_1        0, 1, 3, 4, 5, 6
    #define minuteDots_1  99, 100, 101, 102      

    //----------Layout 2 (11x9 Schweizerdeutsch)------------
    //  0- 10  ES-ISCH-FÜF
    // 11- 21  -ZÄH-VIERTU
    // 22- 32  ZWÄNZG-AB--
    // 33- 43  VOR-HALBI-Z
    // 44- 54  EISZWÖIDRÜW
    // 55- 65  EVIERIFÜFIÖ
    // 66- 76  USÄCHSIBNIU
    // 77- 87  FACHTINÜNIF
    // 88- 98  IZÄHNI°°°°I
    #define FUEF_2        8,   9,  10
    #define ZAEH_2        12,  13,  14
    #define VIERTU_2      16,  17,  18,  19,  20,  21
    #define ZWAENZG_2     22,  23,  24,  25,  26,  27
    #define HAUBI_2       37,  38,  39,  40,  41
    #define AB_2          29,  30
    #define VOR_2         33,  34,  35
    #define EIS_2         44,  45,  46,  -1,  -1,  -1
    #define ZWOEI_2       47,  48,  49,  50,  -1,  -1
    #define DRUE_2        51,  52,  53,  -1,  -1,  -1
    #define VIERI_2       56,  57,  58,  59,  60,  -1
    #define FUEFI_2       61,  62,  63,  64,  -1,  -1
    #define SAECHSI_2     67,  68,  69,  70,  71,  72
    #define SIBNI_2       71,  72,  73,  74,  75,  -1
    #define ACHTI_2       78,  79,  80,  81,  82,  -1
    #define NUENI_2       89,  90,  91,  92,  93,  -1
    #define ZAENI_2       83,  84,  85,  86,  -1,  -1
    #define EUFI_2        55,  66,  77,  88,  -1,  -1
    #define ZWOEUFI_2     43,  54,  65,  76,  87,  98
    #define layoutCols_2  11
    #define itIs_2        0, 1, 3, 4, 5, 6
    #define minuteDots_2  94, 95, 96, 97

    //----------Layout 3 (8x10 Schweizerdeutsch)------------
    //  0-  7  -ES-ISCH
    //  8- 15  ZWÄNZGFÜ
    // 16- 23  ÄVIERTUF
    // 24- 31  H°°ABVOR
    // 32- 59  Z°°HAUBI
    // 40- 47  WÄSÄCHSI
    // 48- 55  ÖVHSHFID
    // 56- 63  UIINTÜBR
    // 64- 71  FEUFIFNÜ
    // 72- 79  IRINÜNI-
    #define PLUS_1_3      25,-1
    #define PLUS_2_3      25,33,-1
    #define MINUS_2_3     33,34,-1
    #define MINUS_1_3     34,-1
    #define FUEF_3        14,15,23
    #define ZAEH_3        8,16,24
    #define VIERTU_3      17,18,19,20,21,22
    #define ZWAENZG_3     8,9,10,11,12,13
    #define HAUBI_3       35,36,37,38,39
    #define AB_3          27,28
    #define VOR_3         29,30,31
    #define EIS_3         65,58,51,      -1
    #define ZWOEI_3       32,40,48,57,   -1
    #define DRUE_3        55,63,71,      -1
    #define VIERI_3       49,57,65,73,74,-1
    #define FUEFI_3       53,61,69,78,   -1
    #define SAECHSI_3     42,43,44,45,46,47
    #define SIBNI_3       46,54,62,70,78,-1
    #define ACHTI_3       36,44,52,60,68,-1
    #define NUENI_3       75,76,77,78,   -1
    #define ZAENI_3       32,41,50,59,68,-1
    #define EUFI_3        65,66,67,68,   -1
    #define ZWOEUFI_3     32,40,48,56,64,72
    #define layoutCols_3  8
    #define itIs_3        1, 2, 4, 5, 6, 7
    #define minuteDots_3  25, 26, 33, 34

    //----------Layout 4 (12x12 Schweizerdeutsch)------------
    //  0- 11  --ES-ISCH---
    // 12- 23  GRAD°SCHO--°
    // 24- 35  -GLI°-FASCH°
    // 36- 47  ZWÄNZG--FÜF-
    // 48- 59  -ZÄH--VIERTU
    // 60- 71  VOR-AB-HAUBI
    // 72- 83  FÜFI--NÜNI--
    // 84- 95  EISZWÖI-DRÜ-
    // 96-107  ---SÄCHSIBNI
    //108-119  ACHTI-VIERI-
    //120-131  --ZÄNI--EUFI
    //132-143  ZWÖUFI-GSI--
    #define GRAD_GSI_4    12,13,14,15,139,140,141,-1
    #define GSI_4         139,140,141,-1
    #define GLI_4         25,26,27,-1
    #define SCHO_FASCH_4  17,18,19,20,30,31,32,33,34
    #define FUEF_4        44,45,46
    #define ZAEH_4        49,50,51
    #define VIERTU_4      54,55,56,57,58,59
    #define ZWAENZG_4     36,37,38,39,40,41
    #define HAUBI_4       67,68,69,70,71
    #define AB_4          64,65
    #define VOR_4         60,61,62
    #define EIS_4         84,85,86,           -1
    #define ZWOEI_4       87,88,89,90,        -1
    #define DRUE_4        92,93,94,           -1
    #define VIERI_4       114,115,116,117,118,-1
    #define FUEFI_4       72,73,74,75,        -1
    #define SAECHSI_4     99,100,101,102,103,104
    #define SIBNI_4       103,104,105,106,107,-1
    #define ACHTI_4       108,109,110,111,112,-1
    #define NUENI_4       78,79,80,81,        -1
    #define ZAENI_4       122,123,124,125,    -1
    #define EUFI_4        128,129,130,131,    -1
    #define ZWOEUFI_4     132,133,134,135,136,137
    #define layoutCols_4  12
    #define itIs_4        2, 3, 5, 6, 7, 8
    #define minuteDots_4  16, 23, 28, 35

    //----------Layout 4 (10x10 Schweizerdeutsch)------------
    //  0-  9  S'ISCH-FÜF
    // 10- 19  ZÄH-VIERTU
    // 20- 29  ZWÄNZG AB-
    // 30- 39  VOR-HAUBI°
    // 40- 49  ZWÖUFIEIS°
    // 50- 59  EUFI-ZWÖI°
    // 60- 69  ZÄHNI-DRÜ°
    // 70- 79  NÜNIVIERI-
    // 80- 89  ACHTI-FÜFI
    // 90- 99  SÄCHSIBNI-
    #define PLUS_1_5      39,-1
    #define PLUS_2_5      39,49,-1
    #define MINUS_2_5     59,69,-1
    #define MINUS_1_5     69,-1
    #define FUEF_5        7,8,9
    #define ZAEH_5        10,11,12
    #define VIERTU_5      14,15,16,17,18,19
    #define ZWAENZG_5     20,21,22,23,24,25
    #define HAUBI_5       34,35,36,37,38
    #define AB_5          27,28
    #define VOR_5         30,31,32
    #define EIS_5         46,47,48,      -1
    #define ZWOEI_5       55,56,57,58,   -1
    #define DRUE_5        66,67,68,      -1
    #define VIERI_5       74,75,76,77,78,-1
    #define FUEFI_5       86,87,88,89,   -1
    #define SAECHSI_5     90,91,92,93,94,95
    #define SIBNI_5       94,95,96,97,98,-1
    #define ACHTI_5       80,81,82,83,84,-1
    #define NUENI_5       70,71,72,73,   -1
    #define ZAENI_5       60,61,62,63,64,-1
    #define EUFI_5        50,51,52,53,   -1
    #define ZWOEUFI_5     40,41,42,43,44,45
    #define layoutCols_5  10
    #define itIs_5        0, 1, 2, 3, 4, 5
    #define minuteDots_5  39, 49, 59, 69

    //----------insert new layout here------------

    //-------some makros magic----------
    #define fivePast(i)   FUEF_##i,AB_##i
    #define tenPast(i)    ZAEH_##i,AB_##i
    #define quaterPast(i) VIERTU_##i,AB_##i
    #define twentyPast(i) ZWAENZG_##i,AB_##i
    #define half(i)       HAUBI_##i
    #define twentyTo(i)   ZWAENZG_##i,VOR_##i
    #define quaterTo(i)   VIERTU_##i,VOR_##i
    #define tenTo(i)      ZAEH_##i,VOR_##i
    #define fiveTo(i)     FUEF_##i,VOR_##i
    #define one(i)        EIS_##i
    #define two(i)        ZWOEI_##i
    #define tree(i)       DRUE_##i
    #define four(i)       VIERI_##i
    #define five(i)       FUEFI_##i
    #define six(i)        SAECHSI_##i
    #define seven(i)      SIBNI_##i
    #define eight(i)      ACHTI_##i
    #define nine(i)       NUENI_##i
    #define ten(i)        ZAENI_##i
    #define eleven(i)     EUFI_##i
    #define twelfe(i)     ZWOEUFI_##i
    #define minutesArray(i) {-1},{fivePast(i),-1},{tenPast(i),-1},{quaterPast(i),-1},{twentyPast(i),-1},{fiveTo(i),half(i),-1},{half(i),-1},{fivePast(i),half(i),-1},{twentyTo(i),-1},{quaterTo(i),-1},{tenTo(i),-1},{fiveTo(i),-1}
    #define hourArray(i)    {one(i)},{one(i)},{two(i)},{tree(i)},{four(i)},{five(i)},{six(i)},{seven(i)},{eight(i)},{nine(i)},{ten(i)},{eleven(i)},{twelfe(i)} 
    
    // Layout with
    const int layoutCols[MAX_LAYOUTS] = {
      layoutCols_0, //Layout 0
      layoutCols_1, //Layout 1
      layoutCols_2, //Layout 2
      layoutCols_3, //Layout 3
      layoutCols_4, //Layout 4
      layoutCols_5, //Layout 5
      //insert new layout here
    }; 

    // mask "it is"
    const int maskItIs[MAX_LAYOUTS][maskSizeItIs] = {
      {},       //Layout 0
      {itIs_1}, //Layout 1
      {itIs_2}, //Layout 2
      {itIs_3}, //Layout 3
      {itIs_4}, //Layout 4
      {itIs_5}, //Layout 5
      //insert new layout here
    };

    // minute mask
    const int maskMinutes[MAX_LAYOUTS][12][maskSizeMinutes] = 
    {
      {},                //Layout 0
      {minutesArray(1)}, //Layout 1
      {minutesArray(2)}, //Layout 2
      {minutesArray(3)}, //Layout 3
      {minutesArray(4)}, //Layout 4
      {minutesArray(5)}, //Layout 5
      //insert new layout here
    };
    // hour masks
    const int maskHours[MAX_LAYOUTS][13][maskSizeHours] = 
    {
      {},             //Layout 0
      {hourArray(1)}, //Layout 1
      {hourArray(2)}, //Layout 2
      {hourArray(3)}, //Layout 3
      {hourArray(4)}, //Layout 4
      {hourArray(5)}, //Layout 5
      //insert new layout here
    };
    
    // mask minute dots
    const int maskMinuteDots[MAX_LAYOUTS][maskSizeMinuteDots] = {
      {},             //Layout 0
      {minuteDots_1}, //Layout 1
      {minuteDots_2}, //Layout 2
      {minuteDots_3}, //Layout 3
      {minuteDots_4}, //Layout 4
      {minuteDots_5}, //Layout 5
      //insert new layout here
    };
    
    // mask minute words
    const int maskMinuteWords[MAX_LAYOUTS][5][maskSizeMinuteWords] = {
      { { -1 } },          //Layout 0
      { { -1 } },          //Layout 1
      { { -1 } },          //Layout 2
      { { -1 },
        {PLUS_1_3},    //Layout 3, minute +1
        {PLUS_2_3},    //Layout 3, minute +2 
        {MINUS_2_3},   //Layout 3, minute -2
        {MINUS_1_3},   //Layout 3, minute -1
      },
      { { -1 },
        {GRAD_GSI_4},  //Layout 4, minute +1
        {GSI_4},       //Layout 4, minute +2
        {GLI_4},       //Layout 4, minute -2
        {SCHO_FASCH_4},//Layout 4, minute -1
      },                                
      { { -1 },
        {PLUS_1_5},    //Layout 5, minute +1
        {PLUS_2_5},    //Layout 5, minute +2
        {MINUS_2_5},   //Layout 5, minute -2
        {MINUS_1_5},   //Layout 5, minute 11
      },
      //insert new layout here
    };

    // overall mask to define which LEDs are on max 16x16
    int maskLedsOn[maskSizeLeds] = {};

    // update led mask
    void updateLedMask(const int wordMask[], int arraySize, int layoutCols) {
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
          maskLedsOn[offset + wordMask[x] + skip] = 1;
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
      updateLedMask(maskHours[layout][index], maskSizeHours, layoutCols[layout]);
    }

    // set minutes
    void setMinutes(int index) {
      // update led mask
      updateLedMask(maskMinutes[layout][index], maskSizeMinutes, layoutCols[layout]);
    }

    // set minutes dot
    void setSingleMinuteDots(int minutesDotCount)
    {
      // check if minute dots are active
      if (minuteDots) {
        // activate all minute dots until number is reached
        for (int i = 0; i < minutesDotCount; i++) {
          // activate LED
          updateLedMask(maskMinuteDots[layout], i+1, layoutCols[layout]);      
        }
      }
      if (minuteWords) {
        // activate LED
        updateLedMask(maskMinuteWords[layout][minutesDotCount], maskSizeMinuteWords, layoutCols[layout]);         
      }
    }

    // update the display
    void updateDisplay(uint8_t hours, uint8_t minutes) {
      // disable/enable complete matrix at the bigging
      for (int x = 0; x < matrixCols *  matrixRows; x++) {
        maskLedsOn[x] = layout==0?1:0;
      } 
      if (layout==0) return;

      // display it is/es ist if activated
      if (displayItIs) {
        updateLedMask(maskItIs[layout], maskSizeItIs, layoutCols[layout]);
      }

      // modulo to get minute dots
      int minutesDotCount = minutes % 5;

      // set single minute dots
      setSingleMinuteDots(minutesDotCount);

      // if minute words add 2 minutes when 2 min before next 5 Minute Sector (gli/fasch)
      if (minuteWords && minutesDotCount > 2 ) {
        minutes += 2;
      } 

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
        case 12:
            // 5 vor (minutesWords)
            setMinutes(0);
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
      if (test) {
        int hours;
        // do it every 3 seconds
        if (millis() - lastTime_2 > 3000) {
          lastTimeHours = lastTimeHours + 1;
          lastTime_2 = millis();
        }
        // do it every 1 seconds
        if (millis() - lastTime > 1000) {
          int minutes = lastTimeMinutes + 1;
          hours = lastTimeHours;
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
      top[F("LED Matrix Höhe")] =  matrixRows;
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
      oappend(SET_F("addInfo('BaernerZyt:Layout', 1, '0=16x16 off, 1=11x10, 2=11x9, 3=8x10, 4=12x12, 5=10x10');"));
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
      configComplete &= getJsonValue(top[F("LED Matrix Höhe")],  matrixRows);
      configComplete &= getJsonValue(top[F("Offset")],  offset);
      configComplete &= getJsonValue(top[F("Minuten-Punkte")], minuteDots);
      configComplete &= getJsonValue(top[F("Minuten ausgeschrieben")], minuteWords);
      configComplete &= getJsonValue(top[F("Layout")], layout);
      configComplete &= getJsonValue(top[F("Test")], test);

      return configComplete;
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
          if (maskLedsOn[x] == 0) {
            // set pixel off
            strip.setPixelColor(x, RGBW32(0,0,0,0));
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