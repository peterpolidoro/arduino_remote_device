// ----------------------------------------------------------------------------
// Controller.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Controller.h"


Controller::Controller()
{
  stored_string_ = String("");
}

void Controller::setup()
{
  // Pin Setup

  // Device Info
  modular_server_.setName(constants::device_name);
  modular_server_.setModelNumber(constants::model_number);
  modular_server_.setFirmwareVersion(constants::firmware_major,constants::firmware_minor,constants::firmware_patch);

  // Add Server Streams
  modular_server_.addServerStream(Serial);
  modular_server_.addServerStream(constants::serial2);

  // Set Storage Arrays
  modular_server_.setSavedVariableStorageArray(saved_variables_);
  modular_server_.setParameterStorageArray(parameters_);
  modular_server_.setMethodStorageArray(methods_);

  // Saved Variables
  modular_server_.createSavedVariable(constants::starting_chars_count_name,constants::starting_chars_count_default);

  // Parameters
  ModularDevice::Parameter& string_parameter = modular_server_.createParameter(constants::string_parameter_name);
  string_parameter.setTypeString();
  ModularDevice::Parameter& string2_parameter = modular_server_.copyParameter(string_parameter,constants::string2_parameter_name);
  ModularDevice::Parameter& count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);
  ModularDevice::Parameter& index_array_parameter = modular_server_.createParameter(constants::index_array_parameter_name);
  index_array_parameter.setTypeArray();
  index_array_parameter.setRange(constants::index_array_element_min,constants::index_array_element_max);
  ModularDevice::Parameter& starting_chars_count_parameter = modular_server_.createParameter(constants::starting_chars_count_name);
  starting_chars_count_parameter.setRange(constants::starting_chars_count_min,constants::starting_chars_count_max);
  ModularDevice::Parameter& double_echo_parameter = modular_server_.createParameter(constants::double_echo_parameter_name);
  double_echo_parameter.setTypeBool();

  // Methods
  ModularDevice::Method& echo_method = modular_server_.createMethod(constants::echo_method_name);
  echo_method.attachCallback(callbacks::echoCallback);
  echo_method.addParameter(string_parameter);
  echo_method.addParameter(double_echo_parameter);
  echo_method.setReturnTypeString();

  ModularDevice::Method& length_method = modular_server_.createMethod(constants::length_method_name);
  length_method.attachCallback(callbacks::lengthCallback);
  length_method.addParameter(string_parameter);
  length_method.setReturnTypeLong();

  ModularDevice::Method& starts_with_method = modular_server_.createMethod(constants::starts_with_method_name);
  starts_with_method.attachCallback(callbacks::startsWithCallback);
  starts_with_method.addParameter(string_parameter);
  starts_with_method.addParameter(string2_parameter);
  starts_with_method.setReturnTypeBool();

  ModularDevice::Method& repeat_method = modular_server_.createMethod(constants::repeat_method_name);
  repeat_method.attachCallback(callbacks::repeatCallback);
  repeat_method.addParameter(string_parameter);
  repeat_method.addParameter(count_parameter);
  repeat_method.setReturnTypeArray();

  ModularDevice::Method& chars_at_method = modular_server_.createMethod(constants::chars_at_method_name);
  chars_at_method.attachCallback(callbacks::charsAtCallback);
  chars_at_method.addParameter(string_parameter);
  chars_at_method.addParameter(index_array_parameter);
  chars_at_method.setReturnTypeArray();

  ModularDevice::Method& starting_chars_method = modular_server_.createMethod(constants::starting_chars_method_name);
  starting_chars_method.attachCallback(callbacks::startingCharsCallback);
  starting_chars_method.addParameter(string_parameter);
  starting_chars_method.setReturnTypeString();

  ModularDevice::Method& set_starting_chars_count_method = modular_server_.createMethod(constants::set_starting_chars_count_method_name);
  set_starting_chars_count_method.attachCallback(callbacks::setStartingCharsCountCallback);
  set_starting_chars_count_method.addParameter(starting_chars_count_parameter);

  ModularDevice::Method& get_starting_chars_count_method = modular_server_.createMethod(constants::get_starting_chars_count_method_name);
  get_starting_chars_count_method.attachCallback(callbacks::getStartingCharsCountCallback);
  get_starting_chars_count_method.setReturnTypeLong();

  ModularDevice::Method& set_stored_string_method = modular_server_.createMethod(constants::set_stored_string_method_name);
  set_stored_string_method.attachCallback(callbacks::setStoredStringCallback);
  set_stored_string_method.addParameter(string_parameter);

  ModularDevice::Method& get_stored_string_method = modular_server_.createMethod(constants::get_stored_string_method_name);
  get_stored_string_method.attachCallback(callbacks::getStoredStringCallback);
  get_stored_string_method.setReturnTypeString();

  // Setup Streams
  Serial.begin(constants::baudrate);
  constants::serial2.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server_.startServer();
}

void Controller::update()
{
  modular_server_.handleServerRequests();
}

ModularDevice::ModularServer& Controller::getModularServer()
{
  return modular_server_;
}

void Controller::setStoredString(String str)
{
  stored_string_ = str;
}

String Controller::getStoredString()
{
  return stored_string_;
}

Controller controller;
