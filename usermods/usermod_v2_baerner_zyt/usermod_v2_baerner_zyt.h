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
    int lastTimeMinutes = -1;
    int lastTimeHours = -1;
    bool testStarted = false;

    // set your config variables to their boot default value (this can also be done in readFromConfig() or a constructor if you prefer)
    bool usermodActive = true;
    int matrixCols = 16;
    int matrixRows = 16;
    bool displayItIs = true;
    bool minuteDots = true;
    bool minuteWritten = false;
    int layout = 1;
    bool test = false;
    
    // defines for mask sizes
    #define maskSizeLeds          16*16
    #define maskSizeMinutes       12
    #define maskSizeHours         6
    #define maskSizeItIs          6
    #define maskSizeMinuteDots    4
    #define maskSizeMinuteWritten 9
    //#define SEGMENT             strip._segments[strip.getCurrSegmentId()]

    //--------------Layout 0 (16x16 leres Layout 0)---------------- 

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
    #define FUEF_1    8,9,10
    #define ZAEH_1    12,13,14
    #define VIERTU_1  16,17,18,19,20,21
    #define ZWAENZG_1 22,23,24,25,26,27
    #define HAUBI_1   39,40,41,42,43
    #define AB_1      33,34
    #define VOR_1     30,31,32
    #define layoutCols_1 11 
    const int maskMinutes_1[12][maskSizeMinutes] = 
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
    const int maskHours_1[13][maskSizeHours] = 
    {
      { 44,  45,  46,            -1}, // 01: ein
      { 44,  45,  46,            -1}, // 01: eins
      { 50,  51,  52,  53,       -1}, // 02: zwei
      { 47,  48,  49,            -1}, // 03: drei
      { 55,  56,  57,  58,  59,  -1}, // 04: vier
      { 62,  63,  64,  65,       -1}, // 05: fünf
      { 66,  67,  68,  69,  70,  71}, // 06: sechs
      { 70,  71,  72,  73,  74,  -1}, // 07: sieben
      { 83,  84,  85,  86,  87,  -1}, // 08: acht
      { 77,  78,  79,  80,       -1}, // 09: neun
      { 88,  89,  90,  91,  92,  -1}, // 10: zehn
      { 94,  95,  06,  97,       -1}, // 11: elf
      {104, 105, 106, 107, 108, 109}  // 12: zwölf and 00: null
    };
    // mask minute dots
    const int maskMinuteDots_1[maskSizeMinuteDots] = {99, 100, 101, 102};
    // mask "it is"
    const int maskItIs_1[maskSizeItIs] = {0, 1, 3, 4, 5, 6};

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
    #define FUEF_2     8,   9,  10
    #define ZAEH_2    12,  13,  14
    #define VIERTU_2  16,  17,  18,  19,  20,  21
    #define ZWAENZG_2 22,  23,  24,  25,  26,  27
    #define HAUBI_2   37,  38,  39,  40,  41
    #define AB_2      29,  30
    #define VOR_2     33,  34,  35
    #define layoutCols_2 11 
    const int maskMinutes_2[12][maskSizeMinutes] = 
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
    const int maskHours_2[13][maskSizeHours] = 
    {
      { 44,  45,  46,  -1,  -1,  -1}, // 01: ein
      { 44,  45,  46,  -1,  -1,  -1}, // 01: eins
      { 47,  48,  49,  50,  -1,  -1}, // 02: zwei
      { 51,  52,  53,  -1,  -1,  -1}, // 03: drei
      { 56,  57,  58,  59,  60,  -1}, // 04: vier
      { 61,  62,  63,  64,  -1,  -1}, // 05: fünf
      { 67,  68,  69,  70,  71,  72}, // 06: sechs
      { 71,  72,  73,  74,  75,  -1}, // 07: sieben
      { 78,  79,  80,  81,  82,  -1}, // 08: acht
      { 83,  84,  85,  86,  -1,  -1}, // 09: neun
      { 89,  90,  91,  92,  93,  -1}, // 10: zehn
      { 55,  66,  77,  88,  -1,  -1}, // 11: elf
      { 43,  54,  65,  76,  87,  98}  // 12: zwölf and 00: null
    };
    // mask minute dots
    const int maskMinuteDots_2[maskSizeMinuteDots] = {94, 95, 96, 97};
    // mask "it is"
    const int maskItIs_2[maskSizeItIs] = {0, 1, 3, 4, 5, 6};

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

    #define FUEF_3       14,15,23
    #define ZAEH_3       8,16,24
    #define VIERTU_3     17,18,19,20,21,22
    #define ZWAENZG_3    8,9,10,11,12,13
    #define HAUBI_3      35,36,37,38,39
    #define AB_3         27,28
    #define VOR_3        29,30,31
    #define layoutCols_3 8
    const int maskMinutes_3[12][maskSizeMinutes] = 
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
    const int maskHours_3[13][maskSizeHours] = 
    {
      { 65,58,51,      -1 }, // 01: EIS
      { 65,58,51,      -1 }, // 01: EIS
      { 32,40,48,57,   -1 }, // 02: ZWÖI
      { 55,63,71,      -1 }, // 03: DRÜ
      { 49,57,65,73,74,-1 }, // 04: VIERI
      { 53,61,69,78,   -1 }, // 05: FÜFI
      { 42,43,44,45,46,47 }, // 06: SÄCHSI
      { 46,54,62,70,78,-1 }, // 07: SIBNI
      { 36,44,52,60,68,-1 }, // 08: ACHTI
      { 75,76,77,78,   -1 }, // 09: NÜNI
      { 32,41,50,59,68,-1 }, // 10: ZÄHNI
      { 65,66,67,68,   -1 }, // 11: EUFI
      { 32,40,48,56,64,72 }  // 12: ZWÖUFI
    };
    // mask minute dots
    const int maskMinuteDots_3[maskSizeMinuteDots] = {25, 26, 33, 34};
    // mask "it is"
    const int maskItIs_3[maskSizeItIs] = {0, 1, 3, 4, 5, 6};

    //----------Layout 4 (12x12 Schweizerdeutsch)------------
    //  0- 11  --ES-ISCH---
    // 12- 23  GRAD-SCHO---
    // 24- 35  -GLI--FASCH-
    // 36- 47  ZWÄNZG--FÜF-
    // 48- 59  -ZÄH--VIERTU
    // 60- 71  VOR-AB-HAUBI
    // 72- 83  FÜFI--NÜNI--
    // 84- 95  EISZWÖI-DRÜ-
    // 96-107  ---SÄCHSIBNI
    //108-119  ACHTI-VIERI-
    //120-131  --ZÄNI--EUFI
    //132-143  ZWÖUFI-GSI--
    #define FUEF_4 44,45,46
    #define ZAEH_4 49,50,51
    #define VIERTU_4 54,55,56,57,58,59
    #define ZWAENZG_4 36,37,38,39,40,41
    #define HAUBI_4 67,68,69,70,71
    #define AB_4 64,65
    #define VOR_4 60,61,62
    #define layoutCols_4 12
    const int maskMinutes_4[12][maskSizeMinutes] = 
    {
      {                         -1 }, //  0 - 00
      { FUEF_4, AB_4,           -1 }, //  1 - 05 fünf ab
      { ZAEH_4, AB_4,           -1 }, //  2 - 10 zehn ab
      { VIERTU_4, AB_4,         -1 }, //  3 - 15 viertel ab
      { ZWAENZG_4, AB_4,        -1 }, //  4 - 20 zwanzig ab
      { FUEF_4, VOR_4, HAUBI_4, -1 }, //  5 - 25 fünf vor halb
      { HAUBI_4,                -1 }, //  6 - 30 halb
      { FUEF_4, AB_4,HAUBI_4,   -1 }, //  7 - 35 fünf ab halb
      { ZWAENZG_4, VOR_4,       -1 }, //  8 - 40 zwanzig vor
      { VIERTU_4, VOR_4,        -1 }, // 13 - 45 viertel vor
      { ZAEH_4, VOR_4,          -1 }, // 10 - 50 zehn vor
      { FUEF_4, VOR_4,          -1 }, // 11 - 55 fünf vor
    };
    // hour masks
    const int maskHours_4[13][maskSizeHours] = 
    {
      { 84,85,86,           -1}, // 01: EIS
      { 84,85,86,           -1}, // 01: EIS
      { 87,88,89,90,        -1}, // 02: ZWÖI
      { 92,93,94,           -1}, // 03: DRÜ
      { 114,115,116,117,118,-1}, // 04: VIERI
      { 72,73,74,75,        -1}, // 05: FÜFI
      { 99,100,101,102,103,104}, // 06: SÄCHSI
      { 103,104,105,106,107,-1}, // 07: SIBNI
      { 108,109,110,111,112,-1}, // 08: ACHTI
      { 78,79,80,81,        -1}, // 09: NÜNI
      { 122,123,124,125,    -1}, // 10: ZÄHNI
      { 128,129,130,131,    -1}, // 11: EUFI
      { 132,133,134,135,136,137} // 12: ZWÖUFI
    };
    // mask minute dots
    const int maskMinuteDots_4[maskSizeMinuteDots] = {10, 11, 22, 23};
    // mask "it is"
    const int maskItIs_4[maskSizeItIs] = {2, 3, 5, 6, 7, 8};
    const int mask1MinuteWritten_4[maskSizeMinuteWritten] = {12,13,14,15,139,140,141,-1};
    const int mask2MinuteWritten_4[maskSizeMinuteWritten] = {139,140,141,-1};
    const int mask3MinuteWritten_4[maskSizeMinuteWritten] = {25,26,27,-1};
    const int mask4MinuteWritten_4[maskSizeMinuteWritten] = {17,18,19,20,30,31,32,33,34};

//----------10x10 Schweizerdeutsch)------------
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

    #define FUEF_5       7,8,9
    #define ZAEH_5       10,11,12
    #define VIERTU_5     14,15,16,17,18,19
    #define ZWAENZG_5    20,21,22,23,24,25
    #define HAUBI_5      34,35,36,37,38
    #define AB_5         27,28
    #define VOR_5        30,31,32
    #define layoutCols_5 10
    const int maskMinutes_5[12][maskSizeMinutes] = 
    {
      {                         -1 }, //  0 - 00
      { FUEF_5, AB_5,           -1 }, //  1 - 05 fünf ab
      { ZAEH_5, AB_5,           -1 }, //  2 - 10 zehn ab
      { VIERTU_5, AB_5,         -1 }, //  3 - 15 viertel ab
      { ZWAENZG_5, AB_5,        -1 }, //  4 - 20 zwanzig ab
      { FUEF_5, VOR_5, HAUBI_5, -1 }, //  5 - 25 fünf vor halb
      { HAUBI_5,                -1 }, //  6 - 30 halb
      { FUEF_5, AB_5, HAUBI_5,  -1 }, //  7 - 35 fünf ab halb
      { ZWAENZG_5, VOR_5,       -1 }, //  8 - 40 zwanzig vor
      { VIERTU_5, VOR_5,        -1 }, // 13 - 45 viertel vor
      { ZAEH_5, VOR_5,          -1 }, // 10 - 50 zehn vor
      { FUEF_5, VOR_5,          -1 }, // 11 - 55 fünf vor
    };
    // hour masks
    const int maskHours_5[13][maskSizeHours] = 
    {
      { 46,47,48,      -1 }, // 01: EIS
      { 46,47,48,      -1 }, // 01: EIS
      { 55,56,57,58,   -1 }, // 02: ZWÖI
      { 66,67,68,      -1 }, // 03: DRÜ
      { 74,75,76,77,78,-1 }, // 04: VIERI
      { 86,87,88,89,   -1 }, // 05: FÜFI
      { 90,91,92,93,94,95 }, // 06: SÄCHSI
      { 94,95,96,97,98,-1 }, // 07: SIBNI
      { 80,81,82,83,84,-1 }, // 08: ACHTI
      { 70,71,72,73,   -1 }, // 09: NÜNI
      { 60,61,62,63,64,-1 }, // 10: ZÄHNI
      { 50,51,52,53,   -1 }, // 11: EUFI
      { 40,41,42,43,44,45 }  // 12: ZWÖUFI
    };
    // mask minute dots
    const int maskMinuteDots_5[maskSizeMinuteDots] = {39, 49, 59, 69};
    // mask "it is"
    const int maskItIs_5[maskSizeItIs] = {0, 1, 2, 3, 4, 5};



    // overall mask to define which LEDs are on max 16x16
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
    void updateLedMask(const int wordMask[], int arraySize, int layoutCols) {
      // loop over array
      for (int x=0; x < arraySize; x++) {
        if (wordMask[x]<0) break;
        // draw words on a bigger matrix
        int offset = 0;
        if (matrixCols > layoutCols) {
          int row = wordMask[x] / layoutCols;
          offset = row * (matrixCols - layoutCols);
        }
        // check if mask has a valid LED number
        if (wordMask[x] + offset >= 0 && wordMask[x] + offset < maskSizeLeds) {
          // turn LED on
          maskLedsOn[wordMask[x] + offset] = 1;
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
          updateLedMask(maskHours_1[index], maskSizeHours, layoutCols_1);
          break;
        case 2:
          updateLedMask(maskHours_2[index], maskSizeHours, layoutCols_2);
          break;
        case 3:
          updateLedMask(maskHours_3[index], maskSizeHours, layoutCols_3);
          break;
        case 4:
          updateLedMask(maskHours_4[index], maskSizeHours, layoutCols_4);
          break;
        case 5:
          updateLedMask(maskHours_5[index], maskSizeHours, layoutCols_5);
          break;      }
    }

    // set minutes
    void setMinutes(int index) {
      // update led mask
      switch (layout) {
        case 1:
          updateLedMask(maskMinutes_1[index], maskSizeMinutes, layoutCols_1);
          break;
        case 2:
          updateLedMask(maskMinutes_2[index], maskSizeMinutes, layoutCols_2);
          break;
        case 3:
          updateLedMask(maskMinutes_3[index], maskSizeMinutes, layoutCols_3);
          break;
        case 4:
          updateLedMask(maskMinutes_4[index], maskSizeMinutes, layoutCols_4);
          break;
        case 5:
          updateLedMask(maskMinutes_5[index], maskSizeMinutes, layoutCols_5);
          break;
      }
    }

    // set minutes dot
    void setSingleMinuteDots(int minutesDotCount)
    {
      // check if minute dots are active
      if (minuteDots && minutesDotCount > 0) {
        // activate all minute dots until number is reached
        for (int i = 0; i < minutesDotCount; i++) {
          // activate LED
          switch (layout) {
            case 1:
              updateLedMask(maskMinuteDots_1, i+1, layoutCols_1);  
              break;
            case 2:
              updateLedMask(maskMinuteDots_2, i+1, layoutCols_2); 
              break;
            case 3:
              updateLedMask(maskMinuteDots_3, i+1, layoutCols_3); 
              break;
            case 4:
              updateLedMask(maskMinuteDots_4, i+1, layoutCols_4); 
              break;
            case 5:
              updateLedMask(maskMinuteDots_5, i+1, layoutCols_5); 
              break;
          }
        }
      }
      if (minuteWritten && layout==4 && minutesDotCount > 0) {
        // activate all minute words until number is reached
        for (int i = 0; i < minutesDotCount; i++) {
          // activate LED
          switch (minutesDotCount) {
            case 1:
              updateLedMask(mask1MinuteWritten_4, maskSizeMinuteWritten, layoutCols_4);  
              break;
            case 2:
              updateLedMask(mask2MinuteWritten_4, maskSizeMinuteWritten, layoutCols_4);  
              break;
            case 3:
              updateLedMask(mask3MinuteWritten_4, maskSizeMinuteWritten, layoutCols_4);  
              break;
            case 4:
              updateLedMask(mask4MinuteWritten_4, maskSizeMinuteWritten, layoutCols_4);  
              break;
          }
        }
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
      if (displayItIs)
      {
        switch (layout) {
          case 1:
            updateLedMask(maskItIs_1, maskSizeItIs, layoutCols_1);
            break;
          case 2:
            updateLedMask(maskItIs_2, maskSizeItIs, layoutCols_2);
            break;
          case 3:
            updateLedMask(maskItIs_3, maskSizeItIs, layoutCols_3);
            break;
          case 4:
            updateLedMask(maskItIs_4, maskSizeItIs, layoutCols_4);
            break;                              
          case 5:
            updateLedMask(maskItIs_5, maskSizeItIs, layoutCols_5);
            break;                              
        }
      }
      // modulo to get minute dots
      int minutesDotCount = minutes % 5;
      // set single minute dots
      setSingleMinuteDots(minutesDotCount);
      if (minutesDotCount > 2 ) {
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
      top[F("Zeige ES ISCH")] = displayItIs;
      top[F("Minuten-Punkte")] = minuteDots;
      top[F("Minuten ausgeschrieben")] = minuteWritten;
      top[F("Layout")] = layout;
      top[F("Test")] = test;
    }

    void appendConfigData() {
      oappend(SET_F("addInfo('BaernerZyt:LED Matrix Breite', 1, 'min 1, max 16');"));
      oappend(SET_F("addInfo('BaernerZyt:LED Matrix Höhe', 1, 'min 1, max 16');"));
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
      configComplete &= getJsonValue(top[F("Minuten-Punkte")], minuteDots);
      configComplete &= getJsonValue(top[F("Minuten ausgeschrieben")], minuteWritten);
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