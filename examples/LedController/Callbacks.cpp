// ----------------------------------------------------------------------------
// Callbacks.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Callbacks.h"

using namespace ArduinoJson::Parser;

namespace callbacks
{
// Callbacks must be non-blocking (avoid 'delay')
//
// modular_device.getParameterValue must be cast to either:
// char*
// long
// double
// JsonArray
// JsonObject
//
// For more info read about ArduinoJson v3 JsonParser JsonValues
//
// modular_device.getSavedVariableValue type must match the saved variable default type
// modular_device.setSavedVariableValue type must match the saved variable default type

void setLedOnCallback()
{
  non_block_blink.stop();
  digitalWrite(constants::led_pin, HIGH);
}

void setLedOffCallback()
{
  non_block_blink.stop();
  digitalWrite(constants::led_pin, LOW);
}

void getLedPinCallback()
{
  modular_device.addToResponse("led_pin",constants::led_pin);
}

void blinkLedCallback()
{
  double duration_on = modular_device.getParameterValue(constants::duration_on_parameter_name);
  double duration_off = modular_device.getParameterValue(constants::duration_off_parameter_name);
  long count = modular_device.getParameterValue(constants::count_parameter_name);
  non_block_blink.setDurationOn(duration_on);
  non_block_blink.setDurationOff(duration_off);
  non_block_blink.setCount(count);
  non_block_blink.start();
}
}