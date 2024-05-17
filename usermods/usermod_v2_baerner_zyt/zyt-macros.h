    //-------some makros magic----------
    #define PLUS_1(i)       PLUS_1_##i
    #define PLUS_2(i)       PLUS_2_##i
    #define MINUS_2(i)      MINUS_2_##i
    #define MINUS_1(i)      MINUS_1_##i
    #define NEXT_HOUR(i)    NEXT_HOUR_##i
    #define ZERO(i)         ZERO_##i
    #define FIVE_PAST(i)    FUEF_##i,AB_##i
    #define TEN_PAST(i)     ZAEH_##i,AB_##i
    #define FIFTEEN(i)      VIERTU_##i,AB_##i
    #define TWENTY(i)       ZWAENZG_##i,AB_##i
    #define TWENTY_FIVE(i)  TWENTY_FIVE_##i
    #define THIRTY(i)       HAUBI_##i
    #define THIRTY_FIVE(i)  THIRTY_FIVE_##i
    #define FOURTY(i)       FOURTY_##i
    #define FOURTY_FIVE(i)  FOURTY_FIVE_##i
    #define FIFTY(i)        FIFTY_##i
    #define FIFTY_FIVE(i)   FIFTY_FIVE_##i

    #define ONE(i)          EIS_##i
    #define TWO(i)          ZWOEI_##i
    #define THREE(i)        DRUE_##i
    #define FOUR(i)         VIERI_##i
    #define FIVE(i)         FUEFI_##i
    #define SIX(i)          SAECHSI_##i
    #define SEVEN(i)        SIBNI_##i
    #define EIGHT(i)        ACHTI_##i
    #define NINE(i)         NUENI_##i
    #define TEN(i)          ZAEHNI_##i
    #define ELEVEN(i)       EUFI_##i
    #define TWELFE(i)       ZWOEUFI_##i

    #define MINUTES_ARRAY(i)      {ZERO(i),-1},{FIVE_PAST(i),-1},{TEN_PAST(i),-1},{FIFTEEN(i),-1},{TWENTY(i),-1},{TWENTY_FIVE(i),-1},{THIRTY(i),-1},{THIRTY_FIVE(i),-1},{FOURTY(i),-1},{FOURTY_FIVE(i),-1},{FIFTY(i),-1},{FIFTY_FIVE(i),-1}
    #define HOURS_ARRAY(i)        {ONE(i)},{ONE(i)},{TWO(i)},{THREE(i)},{FOUR(i)},{FIVE(i)},{SIX(i)},{SEVEN(i)},{EIGHT(i)},{NINE(i)},{TEN(i)},{ELEVEN(i)},{TWELFE(i)}
    #define MINUTE_WORDS_ARRAY(i) {-1},{PLUS_1(i)},{PLUS_2(i)},{MINUS_2(i)},{MINUS_1(i)}

    #define LAYOUT_COLS   LAYOUT_COLS_0,\
                          LAYOUT_COLS_1,\
                          LAYOUT_COLS_2,\
                          LAYOUT_COLS_3,\
                          LAYOUT_COLS_4,\
                          LAYOUT_COLS_5,\
                          LAYOUT_COLS_6,\
                          LAYOUT_COLS_7,\
                          LAYOUT_COLS_8,\
                          //insert new layout here
    #define IT_IS         {-1},\
                          {IT_IS_1},\
                          {IT_IS_2},\
                          {IT_IS_3},\
                          {IT_IS_4},\
                          {IT_IS_5},\
                          {IT_IS_6},\
                          {IT_IS_7},\
                          {IT_IS_8},\
                          //insert new layout here
    #define MINUTES       {-1},\
                          {MINUTES_ARRAY(1)},\
                          {MINUTES_ARRAY(2)},\
                          {MINUTES_ARRAY(3)},\
                          {MINUTES_ARRAY(4)},\
                          {MINUTES_ARRAY(5)},\
                          {MINUTES_ARRAY(6)},\
                          {MINUTES_ARRAY(7)},\
                          {MINUTES_ARRAY(8)},\
                          //insert new layout here
                          
    #define NEXT_HOUR     -1,\
                          NEXT_HOUR_1,\
                          NEXT_HOUR_2,\
                          NEXT_HOUR_3,\
                          NEXT_HOUR_4,\
                          NEXT_HOUR_5,\
                          NEXT_HOUR_6,\
                          NEXT_HOUR_7,\
                          NEXT_HOUR_8,\
                          //insert new layout here
    #define HOUR_COLOR    0,\
                          HOUR_COLOR_1,\
                          HOUR_COLOR_2,\
                          HOUR_COLOR_3,\
                          HOUR_COLOR_4,\
                          HOUR_COLOR_5,\
                          HOUR_COLOR_6,\
                          HOUR_COLOR_7,\
                          HOUR_COLOR_8,\
                          //insert new layout here
    #define HOURS         {-1},\
                          {HOURS_ARRAY(1)},\
                          {HOURS_ARRAY(2)},\
                          {HOURS_ARRAY(3)},\
                          {HOURS_ARRAY(4)},\
                          {HOURS_ARRAY(5)},\
                          {HOURS_ARRAY(6)},\
                          {HOURS_ARRAY(7)},\
                          {HOURS_ARRAY(8)},\
                          //insert new layout here
    #define MINUTE_COLOR  0,\
                          MINUTE_COLOR_1,\
                          MINUTE_COLOR_2,\
                          MINUTE_COLOR_3,\
                          MINUTE_COLOR_4,\
                          MINUTE_COLOR_5,\
                          MINUTE_COLOR_6,\
                          MINUTE_COLOR_7,\
                          MINUTE_COLOR_8,\
                          //insert new layout here
    #define MINUTE_DOTS   {-1},\
                          {MINUTE_DOTS_1},\
                          {MINUTE_DOTS_2},\
                          {MINUTE_DOTS_3},\
                          {MINUTE_DOTS_4},\
                          {MINUTE_DOTS_5},\
                          {MINUTE_DOTS_6},\
                          {MINUTE_DOTS_7},\
                          {MINUTE_DOTS_8},\
                          //insert new layout here
    #define MINUTE_WORDS  {{-1}},\
                          {MINUTE_WORDS_ARRAY(1)},\
                          {MINUTE_WORDS_ARRAY(2)},\
                          {MINUTE_WORDS_ARRAY(3)},\
                          {MINUTE_WORDS_ARRAY(4)},\
                          {MINUTE_WORDS_ARRAY(5)},\
                          {MINUTE_WORDS_ARRAY(6)},\
                          {MINUTE_WORDS_ARRAY(7)},\
                          {MINUTE_WORDS_ARRAY(8)},\
                          //insert new layout here


