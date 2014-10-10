// ----------------------------------------------------------------------------
// Command.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Command.h"

Command::Command(char *name="")
{
  setName(name);
  callback_attached_ = false;
  reserved_ = false;
  parameter_count_ = 0;
}

void Command::setName(char *name)
{
  strncpy(name_,name,COMMAND_NAME_STRING_LENGTH_MAX);
}

boolean Command::compareName(char *name_to_compare)
{
  return String(name_).equalsIgnoreCase(name_to_compare);
}

char* Command::getName()
{
  return name_;
}

void Command::printName()
{
  Serial << name_ << endl;
}

void Command::attachCallback(Callback callback)
{
  callback_ = callback;
  callback_attached_ = true;
  reserved_ = false;
}

void Command::addParameter(Parameter parameter)
{
  char* name = parameter.getName();
  if (String(name).length() > 0)
  {
    int parameter_index = getParameterIndex(name);
    if (parameter_index < 0)
    {
      parameter_vector_.push_back(parameter);
      parameter_count_++;
    }
    else
    {
      parameter_vector_[parameter_index] = Parameter(name);
    }
  }
}

int Command::getParameterIndex(char *parameter_name)
{
  int parameter_index = -1;
  for (std::vector<Parameter>::iterator it = parameter_vector_.begin();
       it != parameter_vector_.end();
       ++it)
  {
    if (it->compareName(parameter_name))
    {
      parameter_index = std::distance(parameter_vector_.begin(),it);
      break;
    }
  }
  return parameter_index;
}

void Command::callback()
{
  if ((callback_attached_) && (!isReserved()))
  {
    (*callback_)();
  }
}

void Command::attachReservedCallback(ReservedCallback callback)
{
  reserved_callback_ = callback;
  callback_attached_ = true;
  reserved_ = true;
}

boolean Command::isReserved()
{
  return reserved_;
}

void Command::reservedCallback(DeviceInterface *dev_int)
{
  if ((callback_attached_) && (isReserved()))
  {
    (dev_int->*reserved_callback_)();
  }
}

Generator::JsonArray<COMMAND_HELP_JSON_OBJECT_SIZE> Command::help()
{
  // Generator::JsonObject<COMMAND_HELP_JSON_OBJECT_SIZE> help_json_object;
  // int parameter_count = 0;
  // for (std::vector<Parameter>::iterator it = parameter_vector_.begin();
  //      it != parameter_vector_.end();
  //      ++it)
  // {
  //   if (++parameter_count < COMMAND_HELP_JSON_OBJECT_SIZE)
  //   {
  //     help_json_object[it->getName()] = it->help();
  //   }
  // }
  // help_json_object.printTo(help_string,COMMAND_HELP_STRING_LENGTH_MAX);
  // return help_string;
  help_json_array = Generator::JsonArray<COMMAND_HELP_JSON_OBJECT_SIZE>();
  int parameter_index = 0;
  for (std::vector<Parameter>::iterator it = parameter_vector_.begin();
       it != parameter_vector_.end();
       ++it)
  {
    if (parameter_index < COMMAND_HELP_JSON_OBJECT_SIZE)
    {
      help_json_array.add(it->getName());
      // help_json_array.add("test");
      parameter_index++;
    }
  }
  // char help_string[COMMAND_HELP_STRING_LENGTH_MAX];
  // help_json_array.printTo(help_string,COMMAND_HELP_STRING_LENGTH_MAX);
  return help_json_array;
}
