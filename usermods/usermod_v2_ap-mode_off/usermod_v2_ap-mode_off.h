#pragma once

#include "wled.h"

#define MIN_THRESHOLD 700
#define DEBOUNCE_TIME 500 //ms between button press
#define RING_SIZE 10

/*
 * This Usermod turns the AP-Mode by default **off**.
 *
 * To turn **on** the AP-Mode again you can press a button. Another press of the button turns the AP-Mode **off** again.
 */

class ApModeOffUsermod : public Usermod {

  private:
    // Push-Button
    int newIntervall = 0;
    int8_t buttonPin = 33;            //Button is connected to this pin and ground
    bool isTouchButton = false;       //is button a tocuh button?
    bool wasAPModeOffButtonLow = true;
    // Usermod
    static const char _name[];
    bool enabled = false;             //mod is off the first time
    // State
    bool isApModeOn = true;           //AP-Mode on the first time
    bool init = false;                //dont disable AP-Mode if mod is not enabled
    char state[30] = "starting";
    // AP-Mode nn staus pixel
    int apModeOnPixPos = 16;
    uint8_t apPixR = 255;
    uint8_t apPixG = 255;
    uint8_t apPixB = 255;
    //Ringbuffer
    int ring[RING_SIZE] = {0};
    int index = 0;
  public:
    //------start ringbuffer-------
    void addRing(int val) {
      ring[index++] = val;
      if (index >= RING_SIZE) index = 0;
    }
    int maxRing() {
      int max = 0;
      for (int i = 0; i < RING_SIZE; i++) {
        if (ring[i] > max) max = ring[i];
      }
      return max;
    }
    int minRing() {
      int min = 2147483647;
      for (int i = 0; i < RING_SIZE; i++) {
        if (ring[i] < min) min = ring[i];
      }
      return min;
    }
    //------end ringbuffer-------
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
      addRing(100000);
      if (enabled) {
        pinMode(buttonPin, INPUT_PULLUP);
        init = true; //at startup disable AP-Mode
        Serial.println("ApModeOffUsermod setup");
      } else {
        Serial.println("ApModeOffUsermod disabled");
      }
      strcpy(state, "initDone");
    }

    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() {
      //Serial.println("Connected to WiFi!");
    }

    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     */
    void loop() {
      // if usermod is disabled or called during strip updating just exit
      if (!enabled || strip.isUpdating()) return;

      //Is usermod enabled or is button pressed?
      if (init || millis() - newIntervall > DEBOUNCE_TIME) {
        if (init || buttonPressed()) {
          if (isApModeOn) {
            apModeOff();
          } else {
            apModeOn();
          }
          init = false;
        }
        newIntervall = millis();
      }
    }

    bool buttonPressed() {
      bool isAPModeOffButtonPressed;
      int max;
      int min;
      int div;
      int trigger;

      if (isTouchButton) {
        // read touch button value:
        int touchValue = touchRead(buttonPin);
        max = maxRing();
        min = minRing();
        div = max - min;
        trigger = max + MIN_THRESHOLD + 2 * div;
        if (isAPModeOffButtonPressed = touchValue >= trigger) { } else {
          wasAPModeOffButtonLow = true;
          addRing(touchValue);
        }

        //log
        setState("touch = ", touchValue);
      } else {
        //read push button value
        int buttonPressed = digitalRead(buttonPin);
        if (isAPModeOffButtonPressed = buttonPressed == false) {} else {
          wasAPModeOffButtonLow = true;
        };

        //log
        setState("button = ", buttonPressed);
      }
      Serial.print(state);

      Serial.print(", button = ");
      Serial.print(isAPModeOffButtonPressed);

      Serial.print(", max = ");
      Serial.print(max);
      Serial.print(", min = ");
      Serial.print(min);
      Serial.print(", div = ");
      Serial.print(div);
      Serial.print(", triger >= ");
      Serial.print(trigger);
      
      int button1 = touchRead(13);
      int button2 = touchRead(14);
      Serial.print(", GPIO13 >= ");
      Serial.print(button1);
      Serial.print(", GPIO14 >= ");
      Serial.print(button2);
      if (button1 > 16000) {
        shortPressAction(1);
      }
      if (button2 > 15000) {
        shortPressAction(2);
      }
      Serial.print(", touchThreshold >= ");
      Serial.print(touchThreshold);
      Serial.println();

      bool ret = isAPModeOffButtonPressed && wasAPModeOffButtonLow;
      if (isAPModeOffButtonPressed) wasAPModeOffButtonLow = false;
      return ret;
    }

    /**
     * handleButton() can be used to override default button behaviour. Returning true
     * will prevent button working in a default way.
     */
    bool handleButton(uint8_t b) {
      yield();
      if (enabled){
        if (b >=1 && b <=2) return true;
      }
      return false;
    }

    void setState(char key[], int value) {
        char valueChar[7];
        itoa(value, valueChar, 10);
        strcpy(state, key);
        strcat(state, valueChar);
    }

    /*
     * Turn AP-Mode Off, and set global variable apActive = true so it won't restart
     */
    void apModeOff() {
      Serial.println("AP-Mode Off");
      dnsServer.stop();
      WiFi.softAPdisconnect(true); // turn AP off
      apActive = true;             // don't restart AP in wled.cpp
      isApModeOn = false;
    }

    /*
     * set global variable apActive = false to turn AP-Mode on in wled.cpp 
     */
    void apModeOn() {
      Serial.println("AP-Mode On");
      apActive = false; // restart AP in wled.cpp
      isApModeOn = true;
    }

    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    void addToJsonInfo(JsonObject& root) {
      // if "u" object does not exist yet wee need to create it
      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      JsonArray statusArr = user.createNestedArray(FPSTR(_name)); //name
      statusArr.add(state); //value      
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
      pinArray.add(buttonPin);
      top[F("Touch Button")] = isTouchButton;
      top[F("AP-Mode On")] = apModeOnPixPos;
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
      configComplete &= getJsonValue(top["pin"][0], buttonPin, 4);
      configComplete &= getJsonValue(top["Touch Button"], isTouchButton, false);
      configComplete &= getJsonValue(top["AP-Mode On"], apModeOnPixPos, 16);
      
      init = enabled;
      if (!isTouchButton) {
        pinMode(buttonPin, INPUT_PULLUP);
      }
      Serial.print("ApModeOffUsermod enabled=");
      Serial.println(enabled);
      return configComplete;
    }

    /*
     * appendConfigData() is called when user enters usermod settings page
     * it may add additional metadata for certain entry fields (adding drop down is possible)
     * be careful not to add too much as oappend() buffer is limited to 3k
     */
    void appendConfigData() {
      //oappend(SET_F("addInfo('AP-Mode Off Usermod:enabled', 1, 'MC Button-Pin');"));
      oappend(SET_F("addInfo('AP-Mode Off Usermod:Touch Button', 1, 'Touch-Pins: 2,4,12,13,14,15,27,32,33');"));
      oappend(SET_F("addInfo('AP-Mode Off Usermod:AP-Mode On', 1, 'Pixel Pos');"));
    }

    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId() {
      return USERMOD_ID_AP_MODE_OFF;
    }

    /*
     * handleOverlayDraw() is called just before every show() (LED strip update frame) after effects have set the colors.
     * Use this to blank out some LEDs or set them to a different color regardless of the set effect mode.
     * Commonly used for custom clocks (Cronixie, 7 segment)
     */
    void handleOverlayDraw() {
      if (enabled && isApModeOn) {
        strip.setPixelColor(apModeOnPixPos, RGBW32(apPixR,apPixG,apPixB,0));
      }
    }
};

// add more strings here to reduce flash memory usage
const char ApModeOffUsermod::_name[]    PROGMEM = "AP-Mode Off Usermod";