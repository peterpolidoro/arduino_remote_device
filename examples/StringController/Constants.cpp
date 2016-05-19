// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace constants
{
const size_t baudrate = 9600;

const size_t model_number = 1002;

// Use semantic versioning http://semver.org/
const long firmware_major = 0;
const long firmware_minor = 1;
const long firmware_patch = 0;

HardwareSerial& serial2 = Serial2;
const size_t serial2_rx_pin = 17;

CONSTANT_STRING(device_name,"string_controller");

// Fields
CONSTANT_STRING(starting_chars_count_field_name,"starting_chars_count");
const long starting_chars_count_min = 1;
const long starting_chars_count_max = 10;
const long starting_chars_count_default = 2;

CONSTANT_STRING(stored_string_field_name,"stored_string");
const char stored_string_default[STORED_STRING_LENGTH_MAX] = "I am a stored string.";

// Parameters
CONSTANT_STRING(string_parameter_name,"string");

CONSTANT_STRING(string2_parameter_name,"string2");

CONSTANT_STRING(count_parameter_name,"count");
const int count_min = 1;
const int count_max = 100;

CONSTANT_STRING(index_array_parameter_name,"index_array");
const int index_array_element_min = 0;
const int index_array_element_max = 10;
const size_t index_array_length_min = 1;
const size_t index_array_length_max = 3;

CONSTANT_STRING(double_echo_parameter_name,"double_echo");

// Methods
CONSTANT_STRING(echo_method_name,"echo");
CONSTANT_STRING(length_method_name,"length");
CONSTANT_STRING(starts_with_method_name,"startsWith");
CONSTANT_STRING(repeat_method_name,"repeat");
CONSTANT_STRING(chars_at_method_name,"charsAt");
CONSTANT_STRING(starting_chars_method_name,"startingChars");
CONSTANT_STRING(set_stored_string_method_name,"setStoredString");
CONSTANT_STRING(get_stored_string_method_name,"getStoredString");

// Errors
CONSTANT_STRING(index_error,"Invalid index.");

}
