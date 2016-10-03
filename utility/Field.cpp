// ----------------------------------------------------------------------------
// Field.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Field.h"


namespace modular_server
{
Field::Field()
{
}

template <>
Field::Field<long>(const ConstantString & name,
                   const long & default_value) :
parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeLong();
}

template <>
Field::Field<double>(const ConstantString & name,
                     const double & default_value) :
parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeDouble();
}

template <>
Field::Field<bool>(const ConstantString & name,
                   const bool & default_value) :
parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeBool();
}

template <>
Field::Field<const ConstantString *>(const ConstantString & name,
                                     const ConstantString * const & default_value) :
  parameter_(name),
  saved_variable_(default_value)
{
  parameter_.setTypeString();
  string_saved_as_char_array_ = false;
}

void Field::setUnits(const ConstantString & name)
{
  parameter_.setUnits(name);
}

void Field::setRange(const long min, const long max)
{
  parameter_.setRange(min,max);
}

void Field::setRange(const double min, const double max)
{
  parameter_.setRange(min,max);
}

void Field::attachPreSetValueCallback(const Functor0 & callback)
{
  pre_set_value_callback_ = callback;
}

void Field::attachPreSetElementValueCallback(const Functor1<const size_t> & callback)
{
  pre_set_element_value_callback_ = callback;
}

void Field::attachPostSetValueCallback(const Functor0 & callback)
{
  post_set_value_callback_ = callback;
}

void Field::attachPostSetElementValueCallback(const Functor1<const size_t> & callback)
{
  post_set_element_value_callback_ = callback;
}

// Private
// Saved Variable Methods
template <>
bool Field::getDefaultValue<long>(long & value)
{
  if (getType() != JsonStream::LONG_TYPE)
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Field::getDefaultValue<double>(double & value)
{
  if (getType() != JsonStream::DOUBLE_TYPE)
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Field::getDefaultValue<bool>(bool & value)
{
  if (getType() != JsonStream::BOOL_TYPE)
  {
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Field::getDefaultValue<const ConstantString *>(const ConstantString * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      stringIsSavedAsCharArray())
  {
    value = NULL;
    return false;
  }
  return saved_variable_.getDefaultValue(value);
}

template <>
bool Field::getDefaultElementValue<long>(long & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
bool Field::getDefaultElementValue<double>(double & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
bool Field::getDefaultElementValue<bool>(bool & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE))
  {
    return false;
  }
  return saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
bool Field::getDefaultElementValue<char>(char & value, const size_t element_index)
{
  if (getType() != JsonStream::STRING_TYPE)
  {
    return false;
  }
  if (!stringIsSavedAsCharArray())
  {
    const ConstantString * cs_ptr;
    getDefaultValue(cs_ptr);
    size_t str_len = cs_ptr->length();
    if (element_index <= str_len)
    {
      char str[str_len+1];
      cs_ptr->copy(str);
      value = str[element_index];
      return true;
    }
    else
    {
      return false;
    }
  }
  return saved_variable_.getDefaultElementValue(value,element_index);
}

template <>
bool Field::setValue<long>(const long & value)
{
  bool success = false;
  preSetValueCallback();
  if (getType() == JsonStream::LONG_TYPE)
  {
    if (parameter_.rangeIsSet())
    {
      long min = parameter_.getMin().l;
      long max = parameter_.getMax().l;
      if ((value >= min) && (value <= max))
      {
        success = saved_variable_.setValue(value);
      }
    }
    else
    {
      success = saved_variable_.setValue(value);
    }
  }
  postSetValueCallback();
  return success;
}

template <>
bool Field::setValue<double>(const double & value)
{
  bool success = false;
  preSetValueCallback();
  if (getType() == JsonStream::DOUBLE_TYPE)
  {
    if (parameter_.rangeIsSet())
    {
      double min = parameter_.getMin().d;
      double max = parameter_.getMax().d;
      if ((value >= min) && (value <= max))
      {
        success = saved_variable_.setValue(value);
      }
    }
    else
    {
      success = saved_variable_.setValue(value);
    }
  }
  postSetValueCallback();
  return success;
}

template <>
bool Field::setValue<bool>(const bool & value)
{
  bool success = false;
  preSetValueCallback();
  if (getType() == JsonStream::BOOL_TYPE)
  {
    success = saved_variable_.setValue(value);
  }
  postSetValueCallback();
  return success;
}

template <>
bool Field::setValue<const ConstantString *>(const ConstantString * const & value)
{
  bool success = false;
  preSetValueCallback();
  if ((getType() == JsonStream::STRING_TYPE) &&
      !stringIsSavedAsCharArray())
  {
    success = saved_variable_.setValue(value);
  }
  postSetValueCallback();
  return success;
}

template <>
bool Field::setValue<ConstantString *>(ConstantString * const & value)
{
  bool success = false;
  preSetValueCallback();
  if ((getType() == JsonStream::STRING_TYPE) &&
      !stringIsSavedAsCharArray())
  {
    success = saved_variable_.setValue(value);
  }
  postSetValueCallback();
  return success;
}

template <>
bool Field::setElementValue<long>(const long & value, const size_t element_index)
{
  bool success = false;
  preSetElementValueCallback(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::LONG_TYPE))
  {
    if (parameter_.rangeIsSet())
    {
      long min = parameter_.getMin().l;
      long max = parameter_.getMax().l;
      if ((value >= min) && (value <= max))
      {
        success = saved_variable_.setElementValue(value,element_index);
      }
    }
    else
    {
      success = saved_variable_.setElementValue(value,element_index);
    }
  }
  postSetElementValueCallback(element_index);
  return success;
}

template <>
bool Field::setElementValue<double>(const double & value, const size_t element_index)
{
  bool success = false;
  preSetElementValueCallback(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::DOUBLE_TYPE))
  {
    if (parameter_.rangeIsSet())
    {
      double min = parameter_.getMin().d;
      double max = parameter_.getMax().d;
      if ((value >= min) && (value <= max))
      {
        success = saved_variable_.setElementValue(value,element_index);
      }
    }
    else
    {
      success = saved_variable_.setElementValue(value,element_index);
    }
  }
  postSetElementValueCallback(element_index);
  return success;
}

template <>
bool Field::setElementValue<bool>(const bool & value, const size_t element_index)
{
  bool success = false;
  preSetElementValueCallback(element_index);
  if ((getType() == JsonStream::ARRAY_TYPE) &&
      (getArrayElementType() == JsonStream::BOOL_TYPE))
  {
    success = saved_variable_.setElementValue(value,element_index);
  }
  postSetElementValueCallback(element_index);
  return success;
}

template <>
bool Field::setElementValue<char>(const char & value, const size_t element_index)
{
  bool success = false;
  preSetElementValueCallback(element_index);
  if ((getType() == JsonStream::STRING_TYPE) &&
      (stringIsSavedAsCharArray()))
  {
    success = saved_variable_.setElementValue(value,element_index);
  }
  postSetElementValueCallback(element_index);
  return success;
}

template <>
bool Field::getValue<long>(long & value)
{
  if (getType() != JsonStream::LONG_TYPE)
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Field::getValue<double>(double & value)
{
  if (getType() != JsonStream::DOUBLE_TYPE)
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Field::getValue<bool>(bool & value)
{
  if (getType() != JsonStream::BOOL_TYPE)
  {
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Field::getValue<const ConstantString *>(const ConstantString * & value)
{
  if ((getType() != JsonStream::STRING_TYPE) ||
      stringIsSavedAsCharArray())
  {
    value = NULL;
    return false;
  }
  return saved_variable_.getValue(value);
}

template <>
bool Field::getElementValue<long>(long & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::LONG_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(value,element_index);
}

template <>
bool Field::getElementValue<double>(double & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::DOUBLE_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(value,element_index);
}

template <>
bool Field::getElementValue<bool>(bool & value, const size_t element_index)
{
  if ((getType() != JsonStream::ARRAY_TYPE) ||
      (getArrayElementType() != JsonStream::BOOL_TYPE))
  {
    return false;
  }
  return saved_variable_.getElementValue(value,element_index);
}

template <>
bool Field::getElementValue<char>(char & value, const size_t element_index)
{
  if (getType() != JsonStream::STRING_TYPE)
  {
    return false;
  }
  if (!stringIsSavedAsCharArray())
  {
    const ConstantString * cs_ptr;
    getValue(cs_ptr);
    size_t str_len = cs_ptr->length();
    if (element_index <= str_len)
    {
      char str[str_len+1];
      cs_ptr->copy(str);
      value = str[element_index];
      return true;
    }
    else
    {
      return false;
    }
  }
  return saved_variable_.getElementValue(value,element_index);
}

void Field::setDefaultValue()
{
  preSetValueCallback();
  saved_variable_.setDefaultValue();
  postSetValueCallback();
}

bool Field::valueIsDefault()
{
  return saved_variable_.valueIsDefault();
}

size_t Field::getArrayLength()
{
  if ((getType() == JsonStream::STRING_TYPE) &&
      (!stringIsSavedAsCharArray()))
  {
    const ConstantString * cs_ptr;
    getValue(cs_ptr);
    return cs_ptr->length() + 1;
  }
  return saved_variable_.getArrayLength();
}

Parameter & Field::getParameter()
{
  return parameter_;
}

const ConstantString & Field::getName()
{
  return parameter_.getName();
}

JsonStream::JsonTypes Field::getType()
{
  return parameter_.getType();
}

JsonStream::JsonTypes Field::getArrayElementType()
{
  return parameter_.getArrayElementType();
}

bool Field::stringIsSavedAsCharArray()
{
  return string_saved_as_char_array_;
}

int Field::findSubsetValueIndex(const long value)
{
  return parameter_.findSubsetValueIndex(value);
}

int Field::findSubsetValueIndex(const char * value)
{
  return parameter_.findSubsetValueIndex(value);
}

Vector<const constants::SubsetMemberType> & Field::getSubset()
{
  return parameter_.getSubset();
}

void Field::preSetValueCallback()
{
  if (pre_set_value_callback_)
  {
    pre_set_value_callback_();
  }
}

void Field::preSetElementValueCallback(const size_t element_index)
{
  if (pre_set_element_value_callback_)
  {
    pre_set_element_value_callback_(element_index);
  }
}

void Field::postSetValueCallback()
{
  if (post_set_value_callback_)
  {
    post_set_value_callback_();
  }
}

void Field::postSetElementValueCallback(const size_t element_index)
{
  if (post_set_element_value_callback_)
  {
    post_set_element_value_callback_(element_index);
  }
}

}
