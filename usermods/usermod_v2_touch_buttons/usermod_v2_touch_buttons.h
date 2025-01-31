#pragma once

#include "wled.h"

#define THRESHOLD 700 
#define DEBOUNCE_TIME 500 //ms between button press

/*
 * This Usermod can handle touch buttons on esp32s2mini. Where the touch value gets bigger when touching.
 *
 * A touch is pressed when the touch value is bigger than lastValue + THRESHOLD
 */

class TouchButtonsUsermod : public Usermod {

  private:
    // touch button
    int newIntervall = 0;
    int lastTouch[WLED_MAX_BUTTONS];

    // Usermod
    static const char _name[];
    bool enabled = true;             //mod is off the first time
    
    // State
    bool init = false;                //dont disable AP-Mode if mod is not enabled
    char state[30] = "starting";
    
  public:

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
      if (enabled) {
        for (uint8_t b=0; b < WLED_MAX_BUTTONS; b++) {
          if(buttonType[b] == BTN_TYPE_TOUCH) {
            pinMode(btnPin[b], INPUT_PULLUP);
            lastTouch[b] = 100000;
          }
        }
        // pinMode(touchPin2, INPUT_PULLUP);
        init = true; //at startup disable AP-Mode
        Serial.println("TouchButtonsUsermod setup");
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
        for (uint8_t b=0; b < WLED_MAX_BUTTONS; b++) {
          if(buttonType[b] == BTN_TYPE_TOUCH) {
            handleTouchButton(b);
          }
        }
        newIntervall = millis();
        init = false;
      }
    }
      
    void handleTouchButton(int button) {
      int8_t pin = btnPin[button];
      int trigger;
      bool isPressed = false;
      // read touch button value:
      int touchValue = touchRead(pin);
      trigger = lastTouch[button] + THRESHOLD;
      
      
      if (isPressed = touchValue >= trigger) { 
        shortPressAction(button);

        Serial.print("touch[");
        Serial.print(pin);
        Serial.print("]=");
        Serial.print(lastTouch[button]);
        Serial.print("->");
        Serial.print(touchValue);
        Serial.print(", trigger = ");
        Serial.print(trigger);
        Serial.print(" ON");
        Serial.println(); 
      }
      lastTouch[button] = touchValue ;
    }

    /**
     * handleButton() can be used to override default button behaviour. Returning true
     * will prevent button working in a default way.
     */
    bool handleButton(uint8_t b) {
      yield();
      if (enabled){
        if(buttonType[b] == BTN_TYPE_TOUCH) {
          return true;
        }
      }
      return false;
    }

    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    void addToJsonInfo(JsonObject& root) {}

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
      return configComplete;
    }

    /*
     * appendConfigData() is called when user enters usermod settings page
     * it may add additional metadata for certain entry fields (adding drop down is possible)
     * be careful not to add too much as oappend() buffer is limited to 3k
     */
    void appendConfigData() {
      //oappend(SET_F("addInfo('AP-Mode Off Usermod:enabled', 1, 'MC Button-Pin');"));
      // oappend(SET_F("addInfo('AP-Mode Off Usermod:Pin', 1, 'Touch-Pins: 2,4,12,13,14,15,27,32,33');"));
    }

    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId() {
      return USERMOD_ID_TOUCH_BUTTONS;
    }

};

// add more strings here to reduce flash memory usage
const char TouchButtonsUsermod::_name[]    PROGMEM = "Touch Buttons Usermod";