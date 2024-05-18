    #define MAX_LAYOUTS           9     //Number of layouts (deined in this file)
    #define maskSizeLeds          16*16 //Max size if matrix
    // Array size which defines how many letters can be used to write the words
    #define maskSizeMinutes       24    //Array size for minute words
    #define maskSizeHours         6     //Array Size for hour words
    #define maskSizeItIs          24    //Array size for "It is"
    #define maskSizeMinuteDots    4     //Array size for minute dots
    #define maskSizeMinuteWords   9     //Array size for minutes as words

    //--------------Layout 0 (16x16 blank layout)---------------- 
    #define LAYOUT_COLS_0 16

    //--------------Layout 1 Melissa (11x11 (4LED 11te Zeile) Schweizerdeutsch)----------------
    //  0- 10  ES-ISCH-FÜF
    // 11- 21  VIERTU--ZÄH
    // 22- 32  ZWÄNZG--VOR
    // 33- 43  AB-HALBI---    
    // 44- 54  EISZWÖI-DRÜ
    // 55- 65  VIERI-FÜFI-
    // 66- 76  SÄCHSISIBNI
    // 77- 87  ACHTI-NÜNI-
    // 88- 98  ZÄNIEUFI---
    // 99-109  -----ZWÖUFI
    //110-113  °°°°
    #define LAYOUT_COLS_1 11
    #define IT_IS_1       0, 1, 3, 4, 5, 6,-1
    #define MINUTE_DOTS_1 110,111,112,113      
    #define PLUS_1_1      110,-1
    #define PLUS_2_1      110,111,-1
    #define MINUS_2_1     112,113,-1
    #define MINUS_1_1     114,-1
    #define NEXT_HOUR_1   1

    #define MINUTE_COLOR_1 1
    #define ZERO_MIN_1     -1
    #define MIDDAY_MIN_1   -1
    #define MIDNIGHT_MIN_1 -1
    #define FUEF_1        8,9,10
    #define ZAEH_1        19,20,21
    #define VIERTU_1      11,12,13,14,15,16
    #define ZWAENZG_1     22,23,24,25,26,27
    #define HAUBI_1       36,37,38,39,40
    #define TWENTY_FIVE_1 FUEF_1,VOR_1,HAUBI_1
    #define THIRTY_FIVE_1 FUEF_1,AB_1,HAUBI_1
    #define FOURTY_1      ZWAENZG_1,VOR_1
    #define FOURTY_FIVE_1 VIERTU_1,VOR_1
    #define FIFTY_1       ZAEH_1,VOR_1
    #define FIFTY_FIVE_1  FUEF_1,VOR_1
    #define AB_1          33,34
    #define VOR_1         30,31,32

    #define HOUR_COLOR_1  1
    #define ZERO_1        104,105,106,107,108,109
    #define EIS_1         44,45,46,      -1
    #define ZWOEI_1       47,48,49,50,   -1
    #define DRUE_1        52,53,54,      -1
    #define VIERI_1       55,56,57,58,59,-1
    #define FUEFI_1       61,62,63,64,   -1
    #define SAECHSI_1     66,67,68,69,70,71
    #define SIBNI_1       72,73,74,75,76,-1
    #define ACHTI_1       77,78,79,80,81,-1
    #define NUENI_1       83,84,85,86,   -1
    #define ZAEHNI_1      88,89,90,91,   -1
    #define EUFI_1        92,93,94,95,   -1
    #define ZWOEUFI_1     ZERO_1

    //----------Layout 2 Chli (11x9 Schweizerdeutsch)------------
    //  0- 10  ES-ISCH-FÜF
    // 11- 21  -ZÄH-VIERTU
    // 22- 32  ZWÄNZG-AB--
    // 33- 43  VOR-HALBI-Z
    // 44- 54  EISZWÖIDRÜW
    // 55- 65  EVIERIFÜFIÖ
    // 66- 76  USÄCHSIBNIU
    // 77- 87  FACHTINÜNIF
    // 88- 98  IZÄHNI°°°°I
    #define LAYOUT_COLS_2 11
    #define IT_IS_2       0, 1, 3, 4, 5, 6,-1
    #define MINUTE_DOTS_2 94, 95, 96, 97
    #define PLUS_1_2      94,-1
    #define PLUS_2_2      94,95,-1
    #define MINUS_2_2     96,97,-1
    #define MINUS_1_2     97,-1
    #define NEXT_HOUR_2   1

    #define MINUTE_COLOR_2 1
    #define ZERO_MIN_2     -1
    #define MIDDAY_MIN_2   -1    
    #define MIDNIGHT_MIN_2 -1
    #define FUEF_2        8,   9,  10
    #define ZAEH_2        12,  13,  14
    #define VIERTU_2      16,  17,  18,  19,  20,  21
    #define ZWAENZG_2     22,  23,  24,  25,  26,  27
    #define HAUBI_2       37,  38,  39,  40,  41
    #define TWENTY_FIVE_2 FUEF_2,VOR_2,HAUBI_2
    #define THIRTY_FIVE_2 FUEF_2,AB_2,HAUBI_2
    #define FOURTY_2      ZWAENZG_2,VOR_2
    #define FOURTY_FIVE_2 VIERTU_2,VOR_2
    #define FIFTY_2       ZAEH_2,VOR_2
    #define FIFTY_FIVE_2  FUEF_2,VOR_2
    #define AB_2          29,  30
    #define VOR_2         33,  34,  35

    #define HOUR_COLOR_2  1
    #define ZERO_2        43,  54,  65,  76,  87,  98
    #define EIS_2         44,  45,  46,  -1,  -1,  -1
    #define ZWOEI_2       47,  48,  49,  50,  -1,  -1
    #define DRUE_2        51,  52,  53,  -1,  -1,  -1
    #define VIERI_2       56,  57,  58,  59,  60,  -1
    #define FUEFI_2       61,  62,  63,  64,  -1,  -1
    #define SAECHSI_2     67,  68,  69,  70,  71,  72
    #define SIBNI_2       71,  72,  73,  74,  75,  -1
    #define ACHTI_2       78,  79,  80,  81,  82,  -1
    #define NUENI_2       89,  90,  91,  92,  93,  -1
    #define ZAEHNI_2      83,  84,  85,  86,  -1,  -1
    #define EUFI_2        55,  66,  77,  88,  -1,  -1
    #define ZWOEUFI_2     ZERO_2

    //----------Layout 3 Chliner (8x10 Schweizerdeutsch)------------
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
    #define LAYOUT_COLS_3 8
    #define IT_IS_3       1, 2, 4, 5, 6, 7,-1
    #define MINUTE_DOTS_3 25, 26, 33, 34
    #define PLUS_1_3      25,-1
    #define PLUS_2_3      25,33,-1
    #define MINUS_2_3     33,34,-1
    #define MINUS_1_3     34,-1
    #define NEXT_HOUR_3   1

    #define MINUTE_COLOR_3 1
    #define ZERO_MIN_3     -1
    #define MIDDAY_MIN_3   -1
    #define MIDNIGHT_MIN_3 -1
    #define FUEF_3        14,15,23
    #define ZAEH_3        8,16,24
    #define VIERTU_3      17,18,19,20,21,22
    #define ZWAENZG_3     8,9,10,11,12,13
    #define HAUBI_3       35,36,37,38,39
    #define TWENTY_FIVE_3 FUEF_3,VOR_3,HAUBI_3
    #define THIRTY_FIVE_3 FUEF_3,AB_3,HAUBI_3
    #define FOURTY_3      ZWAENZG_3,VOR_3
    #define FOURTY_FIVE_3 VIERTU_3,VOR_3
    #define FIFTY_3       ZAEH_3,VOR_3
    #define FIFTY_FIVE_3  FUEF_3,VOR_3
    #define AB_3          27,28
    #define VOR_3         29,30,31

    #define HOUR_COLOR_3  1
    #define ZERO_3        32,40,48,56,64,72
    #define EIS_3         65,58,51,      -1
    #define ZWOEI_3       32,40,48,57,   -1
    #define DRUE_3        55,63,71,      -1
    #define VIERI_3       49,57,65,73,74,-1
    #define FUEFI_3       53,61,69,78,   -1
    #define SAECHSI_3     42,43,44,45,46,47
    #define SIBNI_3       46,54,62,70,78,-1
    #define ACHTI_3       36,44,52,60,68,-1
    #define NUENI_3       75,76,77,78,   -1
    #define ZAEHNI_3      32,41,50,59,68,-1
    #define EUFI_3        65,66,67,68,   -1
    #define ZWOEUFI_3     ZERO_3

    //----------Layout 4 Martin (12x12 Schweizerdeutsch)------------
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
    #define LAYOUT_COLS_4 12
    #define IT_IS_4       2, 3, 5, 6, 7, 8,-1
    #define MINUTE_DOTS_4 16, 23, 28, 35
    #define PLUS_1_4      12,13,14,15,139,140,141,-1  //GRAD GSI
    #define PLUS_2_4      139,140,141,-1              //GSI
    #define MINUS_2_4     25,26,27,-1                 //GLI
    #define MINUS_1_4     17,18,19,20,30,31,32,33,34  //SCHO FASCH
    #define NEXT_HOUR_4   1

    #define MINUTE_COLOR_4 1
    #define ZERO_MIN_4     -1
    #define MIDDAY_MIN_4   -1
    #define MIDNIGHT_MIN_4 -1
    #define FUEF_4        44,45,46
    #define ZAEH_4        49,50,51
    #define VIERTU_4      54,55,56,57,58,59
    #define ZWAENZG_4     36,37,38,39,40,41
    #define HAUBI_4       67,68,69,70,71
    #define AB_4          64,65
    #define VOR_4         60,61,62
    #define TWENTY_FIVE_4 FUEF_4,VOR_4,HAUBI_4
    #define THIRTY_FIVE_4 FUEF_4,AB_4,HAUBI_4
    #define FOURTY_4      ZWAENZG_4,VOR_4
    #define FOURTY_FIVE_4 VIERTU_4,VOR_4
    #define FIFTY_4       ZAEH_4,VOR_4
    #define FIFTY_FIVE_4  FUEF_4,VOR_4

    #define HOUR_COLOR_4  1
    #define ZERO_4        132,133,134,135,136,137
    #define EIS_4         84,85,86,           -1
    #define ZWOEI_4       87,88,89,90,        -1
    #define DRUE_4        92,93,94,           -1
    #define VIERI_4       114,115,116,117,118,-1
    #define FUEFI_4       72,73,74,75,        -1
    #define SAECHSI_4     99,100,101,102,103,104
    #define SIBNI_4       103,104,105,106,107,-1
    #define ACHTI_4       108,109,110,111,112,-1
    #define NUENI_4       78,79,80,81,        -1
    #define ZAEHNI_4      122,123,124,125,    -1
    #define EUFI_4        128,129,130,131,    -1
    #define ZWOEUFI_4     ZERO_4


    //----------Layout 5 Andre (10x10 Schweizerdeutsch)------------
    //  0-  9  S'ISCH-FÜF
    // 10- 19  VIERTU-ZÄH
    // 20- 29  ZWÄNZG AB-
    // 30- 39  VOR-HAUBI°
    // 40- 49  ZWÖUFIEIS°
    // 50- 59  EUFI-ZWÖI°
    // 60- 69  ZÄHNI-DRÜ°
    // 70- 79  NÜNIVIERI-
    // 80- 89  ACHTI-FÜFI
    // 90- 99  SÄCHSIBNI-
    #define LAYOUT_COLS_5 10
    #define IT_IS_5       0, 1, 2, 3, 4, 5,-1
    #define MINUTE_DOTS_5 39, 49, 59, 69
    #define PLUS_1_5      39,-1
    #define PLUS_2_5      39,49,-1
    #define MINUS_2_5     59,69,-1
    #define MINUS_1_5     69,-1
    #define NEXT_HOUR_5   1
    #define MINUTE_COLOR_5 1
    #define ZERO_MIN_5     -1
    #define MIDDAY_MIN_5   -1
    #define MIDNIGHT_MIN_5 -1
    #define FUEF_5        7,8,9
    #define ZAEH_5        17,18,19
    #define VIERTU_5      10,11,12,13,14,15
    #define ZWAENZG_5     20,21,22,23,24,25
    #define HAUBI_5       34,35,36,37,38
    #define TWENTY_FIVE_5 FUEF_5,VOR_5,HAUBI_5
    #define THIRTY_FIVE_5 FUEF_5,AB_5,HAUBI_5
    #define FOURTY_5      ZWAENZG_5,VOR_5
    #define FOURTY_FIVE_5 VIERTU_5,VOR_5
    #define FIFTY_5       ZAEH_5,VOR_5
    #define FIFTY_FIVE_5  FUEF_5,VOR_5
    #define AB_5          27,28
    #define VOR_5         30,31,32

    #define HOUR_COLOR_5  1
    #define ZERO_5     40,41,42,43,44,45
    #define EIS_5         46,47,48,      -1
    #define ZWOEI_5       55,56,57,58,   -1
    #define DRUE_5        66,67,68,      -1
    #define VIERI_5       74,75,76,77,78,-1
    #define FUEFI_5       86,87,88,89,   -1
    #define SAECHSI_5     90,91,92,93,94,95
    #define SIBNI_5       94,95,96,97,98,-1
    #define ACHTI_5       80,81,82,83,84,-1
    #define NUENI_5       70,71,72,73,   -1
    #define ZAEHNI_5      60,61,62,63,64,-1
    #define EUFI_5        50,51,52,53,   -1
    #define ZWOEUFI_5     ZERO_5

//--------------Layout 6 Thomas (11x11 Schweizerdeutsch)----------------
    //  0- 10  ES-ISCH-FÜF
    // 11- 21  VIERTU--ZÄH
    // 22- 32  ZWÄNZG--VOR
    // 33- 43  AB-HALBI---    
    // 44- 54  EISZWÖI-DRÜ
    // 55- 65  -VIERI-FÜFI
    // 66- 76  SÄCHSISIBNI
    // 77- 87  ACHTI-NÜNI-
    // 88- 98  -ZÄNI-EUFI-
    // 99-109  ZWÖUFI-VIVA
    //110-120  °°°°---ZÄME
    #define LAYOUT_COLS_6 11
    #define IT_IS_6       0, 1, 3, 4, 5, 6,-1
    
    #define MINUTE_DOTS_6 110,111,112,113      
    
    #define PLUS_1_6      99,-1
    #define PLUS_2_6      99,100,-1
    #define MINUS_2_6     101,102,-1
    #define MINUS_1_6     102,-1
    #define NEXT_HOUR_6   1

    #define MINUTE_COLOR_6 1
    #define ZERO_MIN_6     106,107,108,109,117,118,119,120
    #define MIDDAY_MIN_6   ZERO_MIN_6
    #define MIDNIGHT_MIN_6 ZERO_MIN_6,17,18,28,29,41,42,43
    #define FUEF_6        8,9,10
    #define ZAEH_6        19,20,21
    #define VIERTU_6      11,12,13,14,15,16
    #define ZWAENZG_6     22,23,24,25,26,27
    #define HAUBI_6       36,37,38,39,40    
    #define TWENTY_FIVE_6 FUEF_6,VOR_6,HAUBI_6
    #define THIRTY_FIVE_6 FUEF_6,AB_6,HAUBI_6
    #define FOURTY_6      ZWAENZG_6,VOR_6
    #define FOURTY_FIVE_6 VIERTU_6,VOR_6
    #define FIFTY_6       ZAEH_6,VOR_6
    #define FIFTY_FIVE_6  FUEF_6,VOR_6
    #define AB_6          33,34
    #define VOR_6         30,31,32

    #define HOUR_COLOR_6  1
    #define ZERO_6        99,100,101,102,103,104
    #define EIS_6         44,45,46,      -1
    #define ZWOEI_6       47,48,49,50,   -1
    #define DRUE_6        52,53,54,      -1
    #define VIERI_6       56,57,58,59,60,-1
    #define FUEFI_6       62,63,64,65,   -1
    #define SAECHSI_6     66,67,68,69,70,71
    #define SIBNI_6       72,73,74,75,76,-1
    #define ACHTI_6       77,78,79,80,81,-1
    #define NUENI_6       83,84,85,86,   -1
    #define ZAEHNI_6      89,90,91,92,   -1
    #define EUFI_6        94,95,96,97,   -1
    #define ZWOEUFI_6     ZERO_6

//----------Layout 7 (10x10 Analog watch)------------
    //  0-  9  ° - - - A A - - - °
    // 10- 19  - - L L A A B B - -
    // 20- 29  - KKLL110011BBC C -
    // 30- 39  - KK111 - -122C C -
    // 40- 49  J J91 - - - -23 D D
    // 50- 59  J J98 - - - -43 D D
    // 60- 69  - II887 - -544E E - 
    // 70- 79  - IIHH776655FFE E - 
    // 80- 89  - - H H G G F F - - 
    // 90- 99  ° - - - G G - - - °
    #define LAYOUT_COLS_7 10
    #define IT_IS_7       4,5,12,17,21,28,40,49,50,59,71,78,82,87,94,95,-1
    #define MINUTE_DOTS_7 9,99,90,0
    #define PLUS_1_7      9,-1
    #define PLUS_2_7      9,99,-1
    #define MINUS_2_7     90,0,-1
    #define MINUS_1_7     0,-1
    #define NEXT_HOUR_7   0
    //minutes
    #define MINUTE_COLOR_7 RGBW32(0,255,0,0)
    #define ZERO_MIN_7     4,5,14,15,-1     //A
    #define MIDDAY_MIN_7   4,5,14,15,-1     //A
    #define MIDNIGHT_MIN_7 4,5,14,15,-1     //A
    #define FUEF_7        16,17,26,27,-1    //B
    #define ZAEH_7        27,28,37,38,-1    //C
    #define VIERTU_7      48,49,58, 59,-1   //D
    #define ZWAENZG_7     67,68,77,78,-1    //E
    #define TWENTY_FIVE_7 76,77,86,87,-1    //F
    #define HAUBI_7       84,85,94,95,-1    //G
    #define THIRTY_FIVE_7 72,73,82,83,-1    //H
    #define FOURTY_7      61,62,71,72,-1    //I
    #define FOURTY_FIVE_7 40,41,50,51,-1    //J
    #define FIFTY_7       21,22,31,32,-1    //K
    #define FIFTY_FIVE_7  12,13,22,23,-1    //L
    #define AB_7          -1
    #define VOR_7         -1
    //hours
    #define HOUR_COLOR_7  RGBW32(255,0,0,0)
    #define ZERO_7        24,25,-1
    #define EIS_7         25,26,36,-1
    #define ZWOEI_7       36,37,47,-1
    #define DRUE_7        47,57,-1
    #define VIERI_7       57,66,67,-1
    #define FUEFI_7       66,75,76,-1
    #define SAECHSI_7     74,75,-1
    #define SIBNI_7       63,73,74,-1
    #define ACHTI_7       52,62,63,-1
    #define NUENI_7       42,52,-1
    #define ZAEHNI_7      32,33,42,-1
    #define EUFI_7        33,23,24,-1    
    #define ZWOEUFI_7     ZERO_7
    // #define ZWOEUFI_7     34,35,44,45,-1
    // #define EIS_7         35,36,45,-1
    // #define ZWOEI_7       36,45,46,-1
    // #define DRUE_7        45,46,55,56,-1
    // #define VIERI_7       55,56,66,-1
    // #define FUEFI_7       55,65,66,-1
    // #define SAECHSI_7     54,55,64,65,-1
    // #define SIBNI_7       54,63,64,-1
    // #define ACHTI_7       54,53,63,-1
    // #define NUENI_7       43,44,53,54,-1
    // #define ZAEHNI_7      33,43,44,-1
    // #define EUFI_7        33,34,44,-1
    
//----------Layout 8 (9x9 Analog watch)------------
    //  0-  9  ° - L - A - B - ° -
    // 10- 19  - - - - - - - - - -
    // 20- 29  K - - 1 0 1 - - C -
    // 30- 39  - - 1 - - - 2 - - -
    // 40- 49  J - 9 - + - 3 - D -
    // 50- 59  - - 8 - - - 4 - - -
    // 60- 69  I - - 7 6 5 - - E -
    // 70- 79  - - - - - - - - - -
    // 80- 89  ° - H - G - F - ° -
    // 90- 99  - - - - - - - - - -
    #define LAYOUT_COLS_8 10
    #define IT_IS_8       2,4,6, 20,23,24,25,28, 32,36, 40,42,46,48, 52,56, 60,63,64,65,68, 82,84,86
    #define MINUTE_DOTS_8 8,88,80,0
    #define PLUS_1_8      8,-1
    #define PLUS_2_8      8,88,-1
    #define MINUS_2_8     88,0,-1
    #define MINUS_1_8     0,-1
    #define NEXT_HOUR_8   0
    //minutes
    #define MINUTE_COLOR_8 RGBW32(128,255,0,0)
    #define ZERO_MIN_8     4,-1     //A
    #define MIDDAY_MIN_8   4,-1     //A
    #define MIDNIGHT_MIN_8 4,-1     //A
    #define FUEF_8        6,-1     //B
    #define ZAEH_8        28,-1    //C
    #define VIERTU_8      48,-1    //D
    #define ZWAENZG_8     68,-1    //E
    #define TWENTY_FIVE_8 86,-1    //F
    #define HAUBI_8       84,-1    //G
    #define THIRTY_FIVE_8 82,-1    //H
    #define FOURTY_8      60,-1    //I
    #define FOURTY_FIVE_8 40,-1    //J
    #define FIFTY_8       20,-1    //K
    #define FIFTY_FIVE_8  2,-1     //L
    #define AB_8          -1
    #define VOR_8         -1
    //hours
    #define HOUR_COLOR_8  RGBW32(255,0,128,0)
    #define ZERO_8        24,-1
    #define EIS_8         25,-1
    #define ZWOEI_8       36,-1
    #define DRUE_8        46,-1
    #define VIERI_8       56,-1
    #define FUEFI_8       65,-1
    #define SAECHSI_8     64,-1
    #define SIBNI_8       63,-1
    #define ACHTI_8       52,-1
    #define NUENI_8       42,-1
    #define ZAEHNI_8      32,-1
    #define EUFI_8        23,-1     
    #define ZWOEUFI_8     ZERO_8   
  
    
    //----------insert new layout here------------
