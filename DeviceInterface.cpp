// ----------------------------------------------------------------------------
// DeviceInterface.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "DeviceInterface.h"

DeviceInterface::DeviceInterface(Stream &stream)
{
  stream_ = &stream;
  setName("");
  model_number_ = 0;
  serial_number_ = 0;
  firmware_number_ = 0;
  Command get_device_info_cmd("getDeviceInfo");
  get_device_info_cmd.attachReservedCallback(&DeviceInterface::getDeviceInfoCallback);
  addCommand(get_device_info_cmd);
  Command get_commands_cmd("getCommands");
  get_commands_cmd.attachReservedCallback(&DeviceInterface::getCommandsCallback);
  addCommand(get_commands_cmd);
  Command get_response_codes_cmd("getResponseCodes");
  get_response_codes_cmd.attachReservedCallback(&DeviceInterface::getResponseCodesCallback);
  addCommand(get_response_codes_cmd);
  Command help_cmd("?");
  help_cmd.attachReservedCallback(&DeviceInterface::help);
  addCommand(help_cmd);
}

void DeviceInterface::setMessageStream(Stream &stream)
{
  stream_ = &stream;
}

void DeviceInterface::processMessage()
{
  while (stream_->available() > 0)
  {
    int message_length = stream_->readBytesUntil(EOL,message_,MESSAGE_LENGTH_MAX);
    if (message_length == 0)
    {
      continue;
    }
    message_[message_length] = '\0';
    response = Generator::JsonObject<JSON_RESPONSE_SIZE>();
    if (message_[0] == JSON_OBJECT_START_CHAR)
    {
      message_type_ = OBJECT_MESSAGE;
      Parser::JsonObject json_object = parser_.parse(message_);
      if (json_object.success())
      {
        processObjectMessage(json_object);
      }
      else
      {
        response["status"] = ERROR;
        response["err_msg"] = "Parsing JSON object message failed! Could be invalid JSON or too many tokens.";
        response["received_msg"] = message_;
      }
    }
    else
    {
      message_type_ = ARRAY_MESSAGE;
      if (message_[0] != JSON_ARRAY_START_CHAR)
      {
        message_type_ = COMMAND_LINE_MESSAGE;
        String msg_str = String("[") + String(message_) + String("]");
        msg_str.toCharArray(message_,MESSAGE_LENGTH_MAX);
      }
      Parser::JsonArray json_array = parser_.parse(message_);
      if (json_array.success())
      {
        processArrayMessage(json_array);
      }
      else
      {
        response["status"] = ERROR;
        response["err_msg"] = "Parsing JSON array message failed! Could be invalid JSON or too many tokens.";
        response["received_msg"] = message_;
      }
      if (!response.containsKey("status"))
      {
        response["status"] = SUCCESS;
      }
      if (message_type_ == COMMAND_LINE_MESSAGE)
      {
        int status = response["status"];
        switch (status)
        {
          case ERROR:
            response["status"] = "error";
            break;
          case SUCCESS:
            response["status"] = "success";
            break;
        }
        if (response.containsKey("command"))
        {
          // Make help response a little less cluttered
          if (strcmp(response["command"],"?") == 0)
          {
            response.remove("command");
            response.remove("status");
          }
        }
        response.prettyPrintTo(*stream_);
      }
      else
      {
        response.printTo(*stream_);
      }
      *stream_ << endl;
    }
  }
}

void DeviceInterface::addCommand(Command command)
{
  char* name = command.getName();
  if (String(name).length() > 0)
  {
    int command_index = getCommandIndexByName(name);
    if (command_index < 0)
    {
      command_vector_.push_back(command);
    }
    else
    {
      command_vector_[command_index] = Command(name);
    }
  }
}

void DeviceInterface::setName(char *name)
{
  strncpy(name_,name,COMMAND_NAME_LENGTH_MAX);
}

void DeviceInterface::setModelNumber(int model_number)
{
  model_number_ = model_number;
}

void DeviceInterface::setFirmwareNumber(int firmware_number)
{
  firmware_number_ = firmware_number;
}

void DeviceInterface::processObjectMessage(Parser::JsonObject &json_object)
{
}

void DeviceInterface::processArrayMessage(Parser::JsonArray &json_array)
{
  char* command_str = json_array[0];
  int command_index = processCommandString(command_str);
  if (!(command_index < 0))
  {
    int array_elements_count = countJsonArrayElements(json_array);
    int argument_count = array_elements_count - 1;
    if ((argument_count == 1) && (String((char*)json_array[1]).equals("?")))
    {
      response["help"] = "You want help!";
    }
    else if (argument_count != command_vector_[command_index].parameter_count_)
    {
      response["status"] = ERROR;
      response["err_msg"] = "Incorrect number of arguments.";
    }
    else
    {
      createArgumentsObjectFromArrayMessage(command_index,json_array);
      executeCommand(command_index);
    }
  }
}

void DeviceInterface::createArgumentsObjectFromArrayMessage(int command_index, Parser::JsonArray &json_array)
{
  arguments = Generator::JsonObject<JSON_ARGUMENTS_SIZE>();
  int parameter_index = 0;
  for (Parser::JsonArrayIterator i=json_array.begin(); i!=json_array.end(); ++i)
  {
    if (i!=json_array.begin())
    {
      char *parameter_name = command_vector_[command_index].parameter_vector_[parameter_index].getName();
      // int parameter_type = char;
      // arguments[parameter_name] = *i;
      arguments[parameter_name] = (char*)*i;
      // arguments[parameter_name] = typeid(int).name();
      // arguments[parameter_name] = (parameter_type*)*i;
      parameter_index++;
    }
  }
  // arguments["test"] = 7;
  // arguments["test"] = 8;
  arguments.prettyPrintTo(*stream_);
}

void DeviceInterface::executeCommand(int command_index)
{
  if (command_vector_[command_index].isReserved())
  {
    command_vector_[command_index].reservedCallback(this);
  }
  else
  {
    command_vector_[command_index].callback();
  }
}

int DeviceInterface::processCommandString(char *command_str)
{
  int command_index = -1;
  int command_id = String(command_str).toInt();
  if (String(command_str).compareTo(String("0")) == 0)
  {
    command_index = 0;
    response["cmd_id"] = 0;
  }
  else if (command_id > 0)
  {
    command_index = command_id;
    response["cmd_id"] = command_id;
  }
  else
  {
    command_index = getCommandIndexByName(command_str);
    response["command"] = command_str;
  }
  if ((command_index < 0) || (command_index >= command_vector_.size()))
  {
    response["status"] = ERROR;
    response["err_msg"] = "Unknown command.";
    command_index = -1;
  }
  return command_index;
}

int DeviceInterface::getCommandIndexByName(char *command_name)
{
  int command_index = -1;
  for (std::vector<Command>::iterator it = command_vector_.begin();
       it != command_vector_.end();
       ++it)
  {
      if (it->compareName(command_name))
      {
        command_index = std::distance(command_vector_.begin(),it);
        break;
      }
    }
  return command_index;
}

int DeviceInterface::countJsonArrayElements(Parser::JsonArray &json_array)
{
  int elements_count = 0;
  for (Parser::JsonArrayIterator i=json_array.begin(); i!=json_array.end(); ++i)
  {
    elements_count++;
  }
  return elements_count;
}

void DeviceInterface::getDeviceInfoCallback()
{
  response["name"] = name_;
  response["model_number"] = model_number_;
  response["serial_number"] = serial_number_;
  response["firmware_number"] = firmware_number_;
}

void DeviceInterface::getCommandsCallback()
{
  for (std::vector<Command>::iterator it = command_vector_.begin();
       it != command_vector_.end();
       ++it)
  {
    int command_index = std::distance(command_vector_.begin(),it);
    if (!command_vector_[command_index].isReserved())
    {
      char* command_name = it->getName();
      int command_id = command_index;
      response[command_name] = command_id;
    }
  }
}

void DeviceInterface::getResponseCodesCallback()
{
  response["rsp_success"] = SUCCESS;
  response["rsp_error"] = ERROR;
}

void DeviceInterface::help()
{
  response["name"] = name_;
  Generator::JsonArray<JSON_COMMANDS_COUNT_MAX> commands;
  for (std::vector<Command>::iterator it = command_vector_.begin();
       it != command_vector_.end();
       ++it)
  {
    int command_index = std::distance(command_vector_.begin(),it);
    if (!command_vector_[command_index].isReserved())
    {
      char* command_name = it->getName();
      commands.add(command_name);
    }
  }
  response["commands"] = commands;
}

DeviceInterface device_interface(Serial);
