#pragma once

#include "wled.h"

 //#define loopPeriod 2*60*1000L //alle 2 min zeit vom GPS lesen
/*
 * Usermods allow you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * 
 * This is an example for a v2 usermod.
 * v2 usermods are class inheritance based and can (but don't have to) implement more functions, each of them is shown in this example.
 * Multiple v2 usermods can be added to one compilation easily.
 * 
 * Creating a usermod:
 * This file serves as an example. If you want to create a usermod, it is recommended to use usermod_v2_empty.h from the usermods folder as a template.
 * Please remember to rename the class and file to a descriptive name.
 * You may also use multiple .h and .cpp files.
 * 
 * Using a usermod:
 * 1. Copy the usermod into the sketch folder (same folder as wled00.ino)
 * 2. Register the usermod by adding #include "usermod_filename.h" in the top and registerUsermod(new MyUsermodClass()) in the bottom of usermods_list.cpp
 */

//class name. Use something descriptive and leave the ": public Usermod" part :)
class GpsTimeUsermod : public Usermod {

  private:
    //GPS
    long targetTime = 0;
    int8_t serial1RxPin = 37;
    //Serial1
    boolean isFlushed = false;
    String readString;

    // Private class members. You can declare variables and functions only accessible to your usermod here
    char state[30] = "starting";
    unsigned long lastTime = 0;

    // These config variables have defaults set inside readFromConfig()
    bool enabled = false;
    int period;
    long loopPeriod = 2*60;

    // string that are used multiple time (this will save some flash memory)
    static const char _name[];
  public:

    // non WLED related methods, may be used for data exchange between usermods (non-inline methods should be defined out of class)

    /**
     * Enable/Disable the usermod
     */
    inline void enable(bool enable) { enabled = enable; }

    /**
     * Get usermod enabled/disabled state
     */
    inline bool isEnabled() { return enabled; }

    // methods called by WLED (can be inlined as they are called only once but if you call them explicitly define them out of class)

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * readFromConfig() is called prior to setup()
     * You can use it to initialize variables, sensors or similar.
     */
    void setup() {
      // do your set-up here
      //Serial.println("Hello from my usermod!");
      
      //GPS an Serial-1 RX = 37
      Serial1.begin(9600,SERIAL_8N1,serial1RxPin,39);
      strcpy(state, "initDone");
    }

    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() {
      //Serial.println("Connected to WiFi!");
      strcpy(state, "WiFiConected");
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
      // if usermod is disabled or called during strip updating just exit
      // NOTE: on very long strips strip.isUpdating() may always return true so update accordingly
      if (!enabled || strip.isUpdating()) return;

      //GPS String an Serial-1 parsen
      //$GPRMC,191636.00,A,4646.35372,N,00736.67504,E,1.213,,080924,,,A*7E
      //        | | |  |                                      | | |
      //        | | |  ms                                     | | YY
      //        | | s                                         | MM
      //        | min                                         DD
      //        h
      //$GPGGA,191825.00,4646.34913,N,00736.67358,E,1,05,2.30,517.2,M,47.3,M,,*5E
      //        | | |  |
      //        | | |  ms
      //        | | s
      //        | min
      //        h
      
      if (millis() - targetTime > loopPeriod*1000) {
        if (!isFlushed) {
          Serial1.begin(9600,SERIAL_8N1,serial1RxPin,39);
          strcpy(state, "Serial restarted");
          //pinMode(serial1RxPin, INPUT);

          //Serial1.flush();
          isFlushed = true;
        } else {
          if (Serial1.available()) {                // If anything comes in Serial-1
            readString=Serial1.readStringUntil(13); // NMEA data ends with 'return' character, which is ascii(13)
            readString.trim();                      // they say NMEA data starts with "$", but the Arduino doesn't think so.
            //Serial.println(readString);           // All the raw sentences will be sent to monitor, if you want them, maybe to see the labels and data order.

            //Start Parsing by finding data, put it in a string of character array, then removing it, leaving the rest of thes sentence for the next 'find'
            if (readString.startsWith("$GPRMC")) {   // I picked this sentence, you can pick any of the other labels and rearrange/add sections as needed. 
              //Serial.println(readString);          // display raw GPRMC data in Serial Monitor

              //Time is first in RMC sentence. Format: hhmmss.ss
              int pos=readString.indexOf(',');       // look for comma delimetrer
              readString.remove(0, pos+1);           // Remove pos+1 characters starting at index=0, this one strips off "$GPRMC" in my sentence
              char hh[3];
              hh[0] = readString.charAt(0);
              hh[1] = readString.charAt(1);
              hh[2] = 0;
              long hours = strtol(hh,(char **)NULL, 10);

              char mi[3];
              mi[0] = readString.charAt(2);
              mi[1] = readString.charAt(3);
              mi[2] = 0;
              //unsigned int minutes = atoi(mi);
              long minutes = strtol(mi,(char **)NULL, 10);

              char ss[3];
              ss[0] = readString.charAt(4);
              ss[1] = readString.charAt(5);
              ss[2] = 0;
              //unsigned int seconds = atoi(ss);
              long seconds = strtol(ss,(char **)NULL, 10);

              //Date Format DDMMYY
              for (int i = 0; i < 8; i++) {            //Date after eight commas delimetrer
                int pos=readString.indexOf(',');
                readString.remove(0, pos+1); 
              }
              char dd[3];
              dd[0] = readString.charAt(0);
              dd[1] = readString.charAt(1);
              dd[2] = 0;
              //unsigned int day = atoi(dd)-1;
              long day = strtol(dd,(char **)NULL, 10) -1;

              char mm[3];
              mm[0] = readString.charAt(2);
              mm[1] = readString.charAt(3);
              mm[2] = 0;
              //unsigned int month = atoi(mm) -1;
              long month = strtol(mm,(char **)NULL, 10) -1;

              char yy[3];
              yy[0] = readString.charAt(4);
              yy[1] = readString.charAt(5);
              yy[2] = 0;
              //unsigned int year = atoi(yy) + 30;    //Add 30 years because unix Time is seconds after 1970-01-01 00:00:00
              long year = strtol(yy,(char **)NULL, 10) + 30;

              //Unix DateTime from https://stackoverflow.com/questions/20826847/convert-between-date-time-and-time-stamp-without-using-std-library-routines
              static unsigned short days[4][12] =
              {
                  {   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335},
                  { 366, 397, 425, 456, 486, 517, 547, 578, 609, 639, 670, 700},
                  { 731, 762, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
                  {1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
              };

              unsigned int unixTime =  (((year/4*(365*4+1)+ days[year%4][month] + day)*24+hours)*60+minutes)*60+seconds;

              // Serial.print(dd);
              // Serial.print(".");
              // Serial.print(mm);
              // Serial.print(".");
              // Serial.print(yy);
              // Serial.println();

              // Serial.print(hh);
              // Serial.print(":");
              // Serial.print(mi);
              // Serial.print(":");
              // Serial.print(ss);
              // Serial.println();

              // Serial.println(unixTime);

              toki.setTime(unixTime);  //set Time in WLED
              char buf[2];
              strcpy(state, "GPS-Time ");
              strcat(state, yy);
              strcat(state, "-");
              strcat(state, mm);
              strcat(state, "-");
              strcat(state, dd);
              strcat(state, " ");
              strcat(state, hh);
              strcat(state, ":");
              strcat(state, mi);
              strcat(state, ":");
              strcat(state, ss);

              isFlushed = false;
              //pinMode(serial1RxPin, OUTPUT);
              Serial1.end();
              loopPeriod = period;
              targetTime = millis();   // change scheduled time exactly, no slippage will happen
            }
          }
        }
      }
    }

    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    void addToJsonInfo(JsonObject& root){
      // if "u" object does not exist yet wee need to create it
      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      //this code adds "u":{"ExampleUsermod":[20," lux"]} to the info object
      //int reading = 20;
      JsonArray statusArr = user.createNestedArray(FPSTR(_name)); //name
      statusArr.add(state); //value
      //lightArr.add(F(" lux")); //unit
    }

    /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void addToJsonState(JsonObject& root) {}

    /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void readFromJsonState(JsonObject& root) {}

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
      JsonObject top = root.createNestedObject(FPSTR(_name));
      //save these vars persistently whenever settings are saved
      top[F("enabled")] = enabled;
      JsonArray pinArray = top.createNestedArray(F("pin"));
      pinArray.add(serial1RxPin);
      top[F("Periode")] = period;
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

      JsonObject top = root[FPSTR(_name)];
      bool configComplete = !top.isNull();
      configComplete &= getJsonValue(top["enabled"], enabled);
      // "pin" fields have special handling in settings page (or some_pin as well)
      configComplete &= getJsonValue(top["pin"][0], serial1RxPin, 37);
      configComplete &= getJsonValue(top["Periode"], period, 60);  
      return configComplete;
    }

    /*
     * appendConfigData() is called when user enters usermod settings page
     * it may add additional metadata for certain entry fields (adding drop down is possible)
     * be careful not to add too much as oappend() buffer is limited to 3k
     */
    void appendConfigData()
    {
      oappend(SET_F("addInfo('GPS Time Usermod:Periode', 1, '[s]');"));
      oappend(SET_F("addInfo('GPS Time Usermod:enabled', 1, 'MC RX-Pin zu GPS TX-Pin');"));
    }

    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId()
    {
      return USERMOD_ID_GPS_TIME;
    }
};

// add more strings here to reduce flash memory usage
const char GpsTimeUsermod::_name[]    PROGMEM = "GPS Time Usermod";