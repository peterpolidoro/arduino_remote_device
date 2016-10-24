// ----------------------------------------------------------------------------
// FieldTester.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "FieldTester.h"


void FieldTester::setup()
{
  // Server Setup
  modular_server_.setup();

  // Pin Setup

  // Add Server Streams
  modular_server_.addServerStream(Serial);

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);
  modular_server_.setFormFactor(constants::form_factor);

  // Add Hardware Info
  modular_server_.addHardwareInfo(constants::hardware_info);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
                              fields_,
                              parameters_,
                              methods_,
                              callbacks_);

  // Fields
  modular_server::Field & double_field = modular_server_.createField(constants::double_field_name,constants::double_default);

  modular_server::Field & bool_field = modular_server_.createField(constants::bool_field_name,constants::bool_default);
  bool_field.attachPreSetValueFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::preSetFieldValueHandler));
  bool_field.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::postSetFieldValueHandler));

  modular_server::Field & long_array_field = modular_server_.createField(constants::long_array_field_name,constants::long_array_default);
  long_array_field.setRange(constants::long_array_element_min,constants::long_array_element_max);

  modular_server::Field & double_array_field = modular_server_.createField(constants::double_array_field_name,constants::double_array_default);
  double_array_field.setRange(constants::double_array_element_min,constants::double_array_element_max);

  modular_server::Field & bool_array_field = modular_server_.createField(constants::bool_array_field_name,constants::bool_array_default);
  bool_array_field.attachPreSetValueFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::preSetFieldValueHandler));
  bool_array_field.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::postSetFieldValueHandler));
  bool_array_field.attachPreSetElementValueFunctor(makeFunctor((Functor1<const size_t> *)0,*this,&FieldTester::preSetFieldElementValueHandler));
  bool_array_field.attachPostSetElementValueFunctor(makeFunctor((Functor1<const size_t> *)0,*this,&FieldTester::postSetFieldElementValueHandler));

  modular_server::Field & string_field = modular_server_.createField(constants::string_field_name,constants::string_default);
  string_field.attachPreSetValueFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::preSetFieldValueHandler));
  string_field.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::postSetFieldValueHandler));
  string_field.attachPreSetElementValueFunctor(makeFunctor((Functor1<const size_t> *)0,*this,&FieldTester::preSetFieldElementValueHandler));
  string_field.attachPostSetElementValueFunctor(makeFunctor((Functor1<const size_t> *)0,*this,&FieldTester::postSetFieldElementValueHandler));

  modular_server::Field & odd_field = modular_server_.createField(constants::odd_field_name,constants::odd_default);
  odd_field.setSubset(constants::odd_subset);

  modular_server::Field & mode_field = modular_server_.createField(constants::mode_field_name,constants::mode_ptr_default);
  mode_field.setSubset(constants::mode_ptr_subset);

  modular_server::Field & odd_array_field = modular_server_.createField(constants::odd_array_field_name,constants::odd_array_default);
  odd_array_field.setSubset(constants::odd_subset);

  // Parameters
  modular_server::Parameter & long_array_parameter = modular_server_.createParameter(constants::long_array_parameter_name);
  long_array_parameter.setTypeLong();
  long_array_parameter.setTypeArray();

  modular_server::Parameter & length_parameter = modular_server_.createParameter(constants::length_parameter_name);
  length_parameter.setRange(constants::length_min,constants::length_max);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setTypeLong();
  count_parameter.setSubset(constants::count_subset);

  modular_server::Parameter & count_array_parameter = modular_server_.createParameter(constants::count_array_parameter_name);
  count_array_parameter.setTypeLong();
  count_array_parameter.setRange(constants::count_array_element_min,constants::count_array_element_max);
  count_array_parameter.setArrayLengthRange(constants::count_array_length_min,constants::count_array_length_max);
  count_array_parameter.setSubset(constants::count_subset);

  modular_server::Parameter & direction_parameter = modular_server_.createParameter(constants::direction_parameter_name);
  direction_parameter.setTypeString();
  direction_parameter.setSubset(constants::direction_ptr_subset);

  modular_server::Parameter & direction_array_parameter = modular_server_.createParameter(constants::direction_array_parameter_name);
  direction_array_parameter.setTypeString();
  direction_array_parameter.setArrayLengthRange(constants::direction_array_length_min,constants::direction_array_length_max);
  direction_array_parameter.setSubset(constants::direction_ptr_subset);

  // Methods
  modular_server::Method & get_doubled_method = modular_server_.createMethod(constants::get_doubled_method_name);
  get_doubled_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getDoubledHandler));
  get_doubled_method.setReturnTypeDouble();

  modular_server::Method & get_bool_method = modular_server_.createMethod(constants::get_bool_method_name);
  get_bool_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getBoolHandler));
  get_bool_method.setReturnTypeBool();

  modular_server::Method & get_long_array_fixed_method = modular_server_.createMethod(constants::get_long_array_fixed_method_name);
  get_long_array_fixed_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getLongArrayFixedHandler));
  get_long_array_fixed_method.setReturnTypeArray();

  modular_server::Method & get_long_array_variable_method = modular_server_.createMethod(constants::get_long_array_variable_method_name);
  get_long_array_variable_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getLongArrayVariableHandler));
  get_long_array_variable_method.setReturnTypeArray();

  modular_server::Method & set_long_array_fixed_method = modular_server_.createMethod(constants::set_long_array_fixed_method_name);
  set_long_array_fixed_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::setLongArrayFixedHandler));
  set_long_array_fixed_method.setReturnTypeBool();

  modular_server::Method & set_long_array_variable_method = modular_server_.createMethod(constants::set_long_array_variable_method_name);
  set_long_array_variable_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::setLongArrayVariableHandler));
  set_long_array_variable_method.setReturnTypeBool();

  modular_server::Method & set_long_array_parameter_method = modular_server_.createMethod(constants::set_long_array_parameter_method_name);
  set_long_array_parameter_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::setLongArrayParameterHandler));
  set_long_array_parameter_method.addParameter(long_array_parameter);
  set_long_array_parameter_method.setReturnTypeBool();

  modular_server::Method & get_string_all_method = modular_server_.createMethod(constants::get_string_all_method_name);
  get_string_all_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getStringAllHandler));
  get_string_all_method.setReturnTypeString();

  modular_server::Method & get_string_some_method = modular_server_.createMethod(constants::get_string_some_method_name);
  get_string_some_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getStringSomeHandler));
  get_string_some_method.addParameter(length_parameter);
  get_string_some_method.setReturnTypeString();

  modular_server::Method & get_count_method = modular_server_.createMethod(constants::get_count_method_name);
  get_count_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getCountHandler));
  get_count_method.addParameter(count_parameter);
  get_count_method.setReturnTypeString();

  modular_server::Method & get_count_array_method = modular_server_.createMethod(constants::get_count_array_method_name);
  get_count_array_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getCountArrayHandler));
  get_count_array_method.addParameter(count_array_parameter);
  get_count_array_method.setReturnTypeArray();

  modular_server::Method & get_direction_method = modular_server_.createMethod(constants::get_direction_method_name);
  get_direction_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getDirectionHandler));
  get_direction_method.addParameter(direction_parameter);
  get_direction_method.setReturnTypeString();

  modular_server::Method & get_direction_array_method = modular_server_.createMethod(constants::get_direction_array_method_name);
  get_direction_array_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::getDirectionArrayHandler));
  get_direction_array_method.addParameter(direction_array_parameter);
  get_direction_array_method.setReturnTypeArray();

  modular_server::Method & check_mode_method = modular_server_.createMethod(constants::check_mode_method_name);
  check_mode_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::checkModeHandler));
  check_mode_method.setReturnTypeString();

  modular_server::Method & increment_mode_method = modular_server_.createMethod(constants::increment_mode_method_name);
  increment_mode_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&FieldTester::incrementModeHandler));
  increment_mode_method.setReturnTypeString();

  // Callbacks

  // Begin Streams
  Serial.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server_.startServer();
}

void FieldTester::update()
{
  modular_server_.handleServerRequests();
}

// Handlers must be non-blocking (avoid 'delay')
//
// modular_server_.parameter(parameter_name).getValue(value) value type must be either:
// fixed-point number (int, long, etc.)
// floating-point number (float, double)
// bool
// const char *
// ArduinoJson::JsonArray *
// ArduinoJson::JsonObject *
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.field(field_name).getValue(value) value type must match the field default type
// modular_server_.field(field_name).setValue(value) value type must match the field default type
// modular_server_.field(field_name).getElementValue(value) value type must match the field array element default type
// modular_server_.field(field_name).setElementValue(value) value type must match the field array element default type

void FieldTester::getDoubledHandler()
{
  double value;
  modular_server_.field(constants::double_field_name).getValue(value);
  value *= 2;
  modular_server_.response().returnResult(value);
}

void FieldTester::getBoolHandler()
{
  bool value;
  modular_server_.field(constants::bool_field_name).getValue(value);
  modular_server_.response().returnResult(value);
}

void FieldTester::getLongArrayFixedHandler()
{
  long long_array[constants::LONG_ARRAY_LENGTH];
  modular_server_.field(constants::long_array_field_name).getValue(long_array);
  modular_server_.response().returnResult(long_array);
}

void FieldTester::getLongArrayVariableHandler()
{
  modular_server::Field & field = modular_server_.field(constants::long_array_field_name);
  size_t array_length = field.getArrayLength();
  long long_array[array_length];
  field.getValue(long_array,array_length);
  modular_server_.response().returnResult(long_array,array_length);
}

void FieldTester::setLongArrayFixedHandler()
{
  long long_array[constants::LONG_ARRAY_LENGTH];
  long_array[0] = 1;
  long_array[1] = 2;
  long_array[2] = 9;
  long_array[3] = 10;
  bool success = modular_server_.field(constants::long_array_field_name).setValue(long_array);
  modular_server_.response().returnResult(success);
}

void FieldTester::setLongArrayVariableHandler()
{
  modular_server::Field & field = modular_server_.field(constants::long_array_field_name);
  size_t array_length = field.getArrayLength();
  long long_array[array_length-1];
  long_array[0] = -1;
  long_array[1] = -2;
  long_array[2] = 7;
  bool success = field.setValue(long_array,array_length-1);
  modular_server_.response().returnResult(success);
}

void FieldTester::setLongArrayParameterHandler()
{
  ArduinoJson::JsonArray * long_array_ptr;
  modular_server_.parameter(constants::long_array_parameter_name).getValue(long_array_ptr);
  bool success = modular_server_.field(constants::long_array_field_name).setValue(*long_array_ptr);
  modular_server_.response().returnResult(success);
}

void FieldTester::getStringAllHandler()
{
  modular_server::Field & field = modular_server_.field(constants::string_field_name);
  size_t array_length = field.getArrayLength();
  char string[array_length];
  field.getValue(string,array_length);
  modular_server_.response().returnResult(string);
}

void FieldTester::getStringSomeHandler()
{
  long length;
  modular_server_.parameter(constants::length_parameter_name).getValue(length);
  size_t array_length = length + 1;
  char string[array_length];
  modular_server_.field(constants::string_field_name).getValue(string,array_length);
  modular_server_.response().returnResult(string);
}

void FieldTester::getCountHandler()
{
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  switch (count)
  {
    case 10:
    {
      modular_server_.response().returnResult("ten");
      break;
    }
    case 20:
    {
      modular_server_.response().returnResult("twenty");
      break;
    }
    case 30:
    {
      modular_server_.response().returnResult("thirty");
      break;
    }
    case 40:
    {
      modular_server_.response().returnResult("forty");
      break;
    }
    case 50:
    {
      modular_server_.response().returnResult("fifty");
      break;
    }
    case 60:
    {
      modular_server_.response().returnResult("sixty");
      break;
    }
    case 70:
    {
      modular_server_.response().returnResult("seventy");
      break;
    }
    default:
    {
      modular_server_.response().returnResult("???");
      break;
    }
  }
}

void FieldTester::getCountArrayHandler()
{
  ArduinoJson::JsonArray * count_array_ptr;
  bool success = modular_server_.parameter(constants::count_array_parameter_name).getValue(count_array_ptr);
  modular_server_.response().returnResult(count_array_ptr);
}

void FieldTester::getDirectionHandler()
{
  const char * direction;
  modular_server_.parameter(constants::direction_parameter_name).getValue(direction);
  modular_server_.response().returnResult(direction);
}

void FieldTester::getDirectionArrayHandler()
{
  ArduinoJson::JsonArray * direction_array_ptr;
  modular_server_.parameter(constants::direction_array_parameter_name).getValue(direction_array_ptr);
  modular_server_.response().returnResult(direction_array_ptr);
}

void FieldTester::checkModeHandler()
{
  const ConstantString * mode_ptr;
  modular_server_.field(constants::mode_field_name).getValue(mode_ptr);
  if (mode_ptr == &constants::mode_rising)
  {
    modular_server_.response().returnResult("Mode set to rising!");
  }
  else if (mode_ptr == &constants::mode_falling)
  {
    modular_server_.response().returnResult("Mode set to falling!");
  }
  else if (mode_ptr == &constants::mode_change)
  {
    modular_server_.response().returnResult("Mode set to change!");
  }
  else
  {
    modular_server_.response().returnResult("???");
  }
}

void FieldTester::incrementModeHandler()
{
  modular_server::Field & field = modular_server_.field(constants::mode_field_name);
  const ConstantString * mode_ptr;
  field.getValue(mode_ptr);
  if (mode_ptr == &constants::mode_rising)
  {
    field.setValue(&constants::mode_falling);
    modular_server_.response().returnResult("Mode was set to rising, now set to falling!");
  }
  else if (mode_ptr == &constants::mode_falling)
  {
    field.setValue(&constants::mode_change);
    modular_server_.response().returnResult("Mode was set to falling, now set to change!");
  }
  else if (mode_ptr == &constants::mode_change)
  {
    field.setValue(&constants::mode_rising);
    modular_server_.response().returnResult("Mode was set to change, now set to rising!");
  }
  else
  {
    modular_server_.response().returnResult("???");
  }
}

void FieldTester::preSetFieldValueHandler()
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setFieldValueFunctor

  // Serial << " setting field value! ";
}

void FieldTester::postSetFieldValueHandler()
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setFieldValueFunctor

  // Serial << " set field value! ";
}

void FieldTester::preSetFieldElementValueHandler(const size_t element_index)
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setFieldElementValueFunctor

  // Serial << " setting field element value at index " << element_index << "! ";
}

void FieldTester::postSetFieldElementValueHandler(const size_t element_index)
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setFieldElementValueFunctor

  // Serial << " set field element value at index " << element_index << "! ";
}
