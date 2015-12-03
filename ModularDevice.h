// ----------------------------------------------------------------------------
// ModularDevice.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef MODULAR_DEVICE_H
#define MODULAR_DEVICE_H
#include "utility/Server.h"


namespace ModularDevice
{
class Method;
void getMemoryFreeCallback();
void resetDefaultsCallback();
void setSerialNumberCallback();

class ModularDevice
{
public:
  ModularDevice(Stream &stream);
  void addServerStream(Stream &stream);
  void setName(const ConstantString &device_name);
  void setModelNumber(const unsigned int model_number);
  void setFirmwareVersion(const unsigned char firmware_major,const unsigned char firmware_minor,const unsigned char firmware_patch);
  Method& createMethod(const ConstantString &method_name);
  Method& copyMethod(Method &method,const ConstantString &method_name);
  Parameter& createParameter(const ConstantString &parameter_name);
  Parameter& copyParameter(Parameter &parameter,const ConstantString &parameter_name);
  ArduinoJson::JsonVariant getParameterValue(const ConstantString &parameter_name);
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
  template<typename K>
  void addKeyToResponse(K key);
  template<typename T>
  void addToResponse(T value);
  template<typename K, typename T>
  void addToResponse(K key, T value);
  void addNullToResponse();
  template<typename K>
  void addNullToResponse(K key);
  template<typename T>
  void sendErrorResponse(T error);
  void addResultKeyToResponse();
  template<typename T>
  void addResultToResponse(T value);
  void beginResponseObject();
  void endResponseObject();
  void beginResponseArray();
  void endResponseArray();
  void startServer();
  void stopServer();
  void handleServerRequests();
  void resetDefaults();
  void setSerialNumber(const unsigned int serial_number);
private:
  Server server_;
};
}
#include "ModularDeviceDefinitions.h"

extern ModularDevice::ModularDevice modular_device;

#endif
