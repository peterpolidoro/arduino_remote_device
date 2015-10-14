// ----------------------------------------------------------------------------
// Server.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_SERVER_H_
#define _MODULAR_DEVICE_SERVER_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Streaming.h"
#include "JsonParser.h"
#include "Array.h"
#include "MemoryFree.h"
#include "ConstantVariable.h"
#include "GenericSerial.h"
#include "Parameter.h"
#include "Method.h"
#include "SavedVariable.h"
#include "Constants.h"
#include "JsonPrinter.h"


namespace ModularDevice
{
class Server
{
public:
  Server(GenericSerial &serial);
  void setSerial(GenericSerial &serial);
  void setName(const ConstantString &name);
  void setModelNumber(const unsigned int model_number);
  void setSerialNumber(const unsigned int serial_number);
  void setFirmwareNumber(const unsigned int firmware_number);
  Method& createMethod(const ConstantString &method_name);
  Method& copyMethod(Method method,const ConstantString &method_name);
  Parameter& createParameter(const ConstantString &parameter_name);
  Parameter& copyParameter(Parameter parameter,const ConstantString &parameter_name);
  ArduinoJson::Parser::JsonValue getParameterValue(const ConstantString &name);
  template<typename T>
  SavedVariable& createSavedVariable(const ConstantString &saved_variable_name,
                                     const T &default_value);
  template<typename T>
  SavedVariable& createSavedVariable(const ConstantString &saved_variable_name,
                                     const T default_value[],
                                     const unsigned int array_length);
  template<typename T>
  void setSavedVariableValue(const ConstantString &saved_variable_name,
                             const T &value);
  template<typename T>
  void setSavedVariableValue(const ConstantString &saved_variable_name,
                             const T value[],
                             const unsigned int array_index);
  template<typename T>
  void getSavedVariableValue(const ConstantString &saved_variable_name,
                             T &value);
  template<typename T>
  void getSavedVariableValue(const ConstantString &saved_variable_name,
                             T value[],
                             const unsigned int array_index);
  template<typename T>
  void addToResponse(const char *key, T value);
  template<typename T>
  void addToResponse(T value);
  void addNullToResponse(const char *key);
  void addNullToResponse();
  void addBoolToResponse(const char *key, const bool value);
  void addBoolToResponse(const bool value);
  void addKeyToResponse(const char *key);
  template<typename T>
  void sendErrorResponse(T error);
  void startResponseObject();
  void stopResponseObject();
  void startResponseArray();
  void stopResponseArray();
  void resetDefaults();
  void startServer(const int baudrate);
  void handleRequest();
private:
  GenericSerial generic_serial_;
  char request_[constants::STRING_LENGTH_REQUEST];
  ArduinoJson::Parser::JsonParser<constants::JSON_PARSER_SIZE> parser_;
  ArduinoJson::Parser::JsonArray request_json_array_;
  Array<Method,constants::METHOD_COUNT_MAX> method_array_;
  Array<Parameter,constants::PARAMETER_COUNT_MAX> parameter_array_;
  Array<SavedVariable,constants::SAVED_VARIABLE_COUNT_MAX> saved_variable_array_;
  const ConstantString *name_ptr_;
  unsigned int model_number_;
  unsigned int firmware_number_;
  int request_method_index_;
  int parameter_count_;
  JsonPrinter response_;
  bool error_;
  unsigned int eeprom_index_;
  const ConstantString *eeprom_init_name_ptr_;
  bool eeprom_uninitialized_;
  unsigned int eeprom_initialized_index_;

  void processRequestArray();
  int processMethodString(char *method_string);
  int findMethodIndex(char *method_name);
  int findMethodIndex(const ConstantString &method_name);
  int countJsonArrayElements(ArduinoJson::Parser::JsonArray &json_array);
  void executeMethod();
  void methodHelp(int method_index);
  void verboseMethodHelp(int method_index);
  int processParameterString(char *parameter_string);
  int findParameterIndex(const char *parameter_name);
  int findParameterIndex(const ConstantString &parameter_name);
  void parameterHelp(Parameter &parameter);
  bool checkParameters();
  bool checkParameter(int parameter_index, ArduinoJson::Parser::JsonValue json_value);
  int findSavedVariableIndex(const ConstantString &saved_variable_name);
  unsigned int getSerialNumber();
  void initializeEeprom();

  // reserved methods
  void getDeviceInfoCallback();
  void getMethodIdsCallback();
  void getResponseCodesCallback();
  void getParametersCallback();
  void help();
  void verboseHelp();
};
}
#include "ServerDefinitions.h"

#endif
