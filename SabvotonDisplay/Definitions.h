#ifndef Definitions_h
#define Definitions_h

// Throttle OR PAS Passthrough
#define THROTTLE_OUT_PIN A0 // The pin that is connected to your controller's throttle signal wire. Required if you're using either throttle or PAS passthrough.

// Throttle Passthrough
#define THROTTLE_PASSTHROUGH_ENABLED true // Enables / disables throttle passthrough
#define THROTTLE_IN_PIN A1 // The pin that your throttle signal wire is connected to
#define THROTTLE_MIN_VOLTAGE 0.8 // The voltage output from your throttle when being touched lightly. Careful with this, you don't want the throttle activating without being touched. Depending on your throttle you'll like need to adjust this.
#define THROTTLE_MAX_VOLTAGE 4.5 // The voltage output from your throttle when fully pinned (100%). Depending on your throttle you'll like need to adjust this.

// PAS Passthrough
#define PAS_PASSTHROUGH_ENABLED true // Enables / disables PAS passthrough
#define PAS_IN_PIN 2 // The pin that your PAS signal wire is connected to
#define PAS_MAX_RPM 100 // This corresponds to the PAS scaling - if you are unhappy with the responsiveness of the pedal assist, you can adjust this

// Speed Calculations
#define SPEED_UNIT 1 // Determines whether your speed calculation is in MPH or KPH. 0 = KPH, 1 = MPH.
#define WHEEL_DIAMETER 22 // Diameter of your wheel in inches.
#define WHEEL_CIRCUMFERENCE PI * WHEEL_DIAMETER // Chat GPT told me this was fine.

// Stuff that you probably shouldn't touch unless you know what you're doing
#define SABVOTON_READ_INTERVAL_MS 500 // Determines how often we should read the latest values from the controller and publish over BLE (in the main loop).
#define EEPROM_USABLE_CAPACITY_BYTES 8192 // This is the total number of bytes of usable EEPROM capacity (8192 is correct for the Arduino UNO R4 WiFi, different boards will vary).
#define MODE_PARAMS_SIZE_BYTES 26 // This is the total byte count of all parameters making up a mode. Only change this if you're changing the parameters in the Mode class.

#endif