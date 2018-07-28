/*
Copyright 2018 giursino

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "KnxMessage.h"
#include "log.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace log;

KnxMessage::KnxMessage():
  m_message()
{
}

KnxMessage::KnxMessage(const std::vector<uint8_t>& message):
  m_message(message)
{

}

bool KnxMessage::set_raw(const std::vector<uint8_t>& message)
{
  m_message = message;
  return true;
}

bool KnxMessage::get_raw(std::vector<uint8_t> &message) const
{
  message = m_message;
  return true;
}

std::string KnxMessage::get_string() const
{
  std::stringstream ssMessage;
  for (uint8_t byte : m_message) {
    ssMessage << "0x" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << int(byte) << " ";
  }
  return ssMessage.str();
}

bool KnxMessage::get_src(KnxAddr &addr) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }

  addr.set_value((m_message[1]<<8) + m_message[2]);
  return true;
}

bool KnxMessage::get_dest(KnxAddr &addr) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }

  addr.set_value((m_message[3]<<8) + m_message[4]);
  return true;
}

bool KnxMessage::is_data_frame() const {
  return ((m_message[0] & 0x53) == 0x10);
}

bool KnxMessage::get_frame_type(FrameType &value) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (is_data_frame()) {
    // L_Data-Frame
    if ((m_message[0] & 0x80) == 0x80) {
      value=FrameType::L_Data_Standard;
    }
    else{
      value=FrameType::L_Data_Extended;
    }
    return true;
  }
  else if (m_message[0] == 0xF0) {
    value=FrameType::L_Poll_Data;
    return true;
  }
  else if (m_message[0] == 0xCC) {
    value=FrameType::ACK;
    return true;
  }
  else if (m_message[0] == 0x0C) {
    value=FrameType::NAK;
    return true;
  }
  else if (m_message[0] == 0xC0) {
    value=FrameType::BUSY;
    return true;
  }
  else if (m_message[0] == 0x00) {
    value=FrameType::NAK_and_BUSY;
    return true;
  }
  return false;
}

bool KnxMessage::is_frame_repeated() const
{
  if (!is_message_valid()) {
    return false;
  }

  if (is_data_frame()) {
    return ((m_message[0] & 0x20) == 0x20);
  }
  return false;
}

bool KnxMessage::get_priority(Priority &value) const
{
  if (!is_message_valid()) {
    return false;
  }

  FrameType frame;
  if (!get_frame_type(frame)) {
    return false;
  }

  if ((frame != FrameType::L_Data_Standard) && (frame != FrameType::L_Data_Extended)) {
    return false;
  }

  switch (m_message[0] & 0x0C) {
  case 0x00:
    value = Priority::system;
    break;
  case 0x08:
    value = Priority::urgent;
    break;
  case 0x04:
    value = Priority::normal;
    break;
  case 0x0C:
    value = Priority::low;
    break;
  }
  return true;
}

bool KnxMessage::is_destination_broadcast_address() const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }
  return ((m_message[3] == 0x00) && (m_message[4] == 0x00));
}

bool KnxMessage::is_destination_group_address() const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }
  return ((m_message[5] & 0x80) == 0x80);
}

bool KnxMessage::get_hopcount(uint8_t &hopcount) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }
  hopcount = (m_message[5] & 0x70) >> 4;
  return true;
}

bool KnxMessage::get_length(uint8_t &length) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }
  length = m_message[5] & 0x0F;
  return true;
}

bool KnxMessage::get_transport_layer_services(TransportLayerServices &value) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }

  if (is_destination_group_address()) {
    switch (m_message[6] & 0xFC) {
    case 0x00:
      if (is_destination_broadcast_address()) {
        value = TransportLayerServices::T_Data_Broadcast;
        return true;
      }
      else {
        value = TransportLayerServices::T_Data_Group;
        return true;
      }
      break;
    case 0x04:
      value = TransportLayerServices::T_Data_Tag_Group;
      return true;
      break;
    }
  }
  else {
    if (m_message[6] == 0x80){
      value = TransportLayerServices::T_Connect;
      return true;
    }
    else if (m_message[6] == 0x81) {
      value = TransportLayerServices::T_Disconnect;
      return true;
    }
    else if ((m_message[6] & 0xFC) == 0x00) {
      value = TransportLayerServices::T_Data_Individual;
      return true;
    }
    else if ((m_message[6] & 0xC0) == 0x40) {
      value = TransportLayerServices::T_Data_Connected;
      return true;
    }
    else if ((m_message[6] & 0xC3) == 0xC2) {
      value = TransportLayerServices::T_ACK;
      return true;
    }
    else if ((m_message[6] & 0xC3) == 0xC3) {
      value = TransportLayerServices::T_NAK;
      return true;
    }
  }
  return false;

}

bool KnxMessage::get_transport_layer_sequence_num(uint8_t &value) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }

  TransportLayerServices tcf;
  if ((get_transport_layer_services(tcf)) &&
      (tcf == TransportLayerServices::T_Data_Connected ||
       tcf == TransportLayerServices::T_ACK ||
       tcf == TransportLayerServices::T_NAK))
  {
    value = ((m_message[6] & 0x3C) >> 2);
    return true;
  }
  return false;
}

bool KnxMessage::get_apci(ApplicationLayerServices& value) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }

  switch (((m_message[6] << 8) + m_message[7]) & 0x03C0) {
  case 0x0040:
    value = ApplicationLayerServices::A_GroupValue_Response;
    return true;
    break;
  case 0x0080:
    value = ApplicationLayerServices::A_GroupValue_Write;
    return true;
    break;
  case 0x0180:
    value = ApplicationLayerServices::A_ADC_Read;
    return true;
    break;
  case 0x01C0:
    value = ApplicationLayerServices::A_ADC_Response;
    return true;
    break;
  case 0x0200:
    value = ApplicationLayerServices::A_Memory_Read;
    return true;
    break;
  case 0x0240:
    value = ApplicationLayerServices::A_Memory_Response;
    return true;
    break;
  case 0x0280:
    value = ApplicationLayerServices::A_Memory_Write;
    return true;
    break;
  }

  switch (((m_message[6] << 8) + m_message[7]) & 0x03FF) {
  case 0x0000:
    value = ApplicationLayerServices::A_GroupValue_Read;
    return true;
    break;
  case 0x00C0:
    value = ApplicationLayerServices::A_IndividualAddress_Write;
    return true;
    break;
  case 0x0100:
    value = ApplicationLayerServices::A_IndividualAddress_Read;
    return true;
    break;
  case 0x0140:
    value = ApplicationLayerServices::A_IndividualAddress_Response;
    return true;
    break;
  case 0x02C7:
    value = ApplicationLayerServices::A_FunctionPropertyCommand;
    return true;
    break;
  case 0x02C8:
    value = ApplicationLayerServices::A_FunctionPropertyState_Read;
    return true;
    break;
  case 0x02C9:
    value = ApplicationLayerServices::A_FunctionPropertyState_Response;
    return true;
    break;
  case 0x0300:
    value = ApplicationLayerServices::A_DeviceDescriptor_Read;
    return true;
    break;
  case 0x0340:
    value = ApplicationLayerServices::A_DeviceDescriptor_Response;
    return true;
    break;
  case 0x0380:
    value = ApplicationLayerServices::A_Restart;
    return true;
    break;
  case 0x03D5:
    value = ApplicationLayerServices::A_PropertyValue_Read;
    return true;
    break;
  case 0x03D6:
    value = ApplicationLayerServices::A_PropertyValue_Response;
    return true;
    break;
  case 0x03D7:
    value = ApplicationLayerServices::A_PropertyValue_Write;
    return true;
    break;
  case 0x03E5:
    value = ApplicationLayerServices::A_Link_Read;
    return true;
    break;
  case 0x03E6:
    value = ApplicationLayerServices::A_Link_Response;
    return true;
    break;
  case 0x03E7:
    value = ApplicationLayerServices::A_Link_Write;
    return true;
    break;
  }
  return false;
}

bool KnxMessage::get_payload(std::vector<uint8_t>& payload) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return  false;
  }

  ApplicationLayerServices apci;

  if (!get_apci(apci)) {
    return false;
  }

  switch (apci) {
  case ApplicationLayerServices::A_GroupValue_Write:
  case ApplicationLayerServices::A_GroupValue_Response:
  {
    std::uint8_t len;
    if (!get_length(len)) {
      return false;
    }
    payload.clear();
    if (len==1) {
      payload.push_back(m_message[7] & 0x3F);
    }
    else {
      for(int i=0; i<(len-1); i++) {
        payload.push_back(m_message[8+i]);
      }
    }
    break;
  }
  case ApplicationLayerServices::A_PropertyValue_Write:
  case ApplicationLayerServices::A_PropertyValue_Response:
  {
    std::uint8_t len;
    if (!get_length(len)) {
      return false;
    }
    payload.clear();
    for(int i=0; i<(len-1); i++) {
      payload.push_back(m_message[8+i]);
    }
    break;
  }
  default:
    // not implemented
    FILE_LOG(logDEBUG) << "APCI not implemented: " << static_cast<std::underlying_type<ApplicationLayerServices>::type>(apci) << std::endl;
    return false;
    break;
  }
  return true;
}

bool KnxMessage::get_application_layer(ApplicationLayerServices& service, ApplicationLayerPayload& payload) const
{
  if (!is_message_valid()) {
    return false;
  }

  if (!is_data_frame()) {
    return false;
  }

  if (!get_apci(service)) {
    return false;
  }

  payload.m_raw_value.clear();
  if (!get_payload(payload.m_raw_value)) {
    return false;
  }

  return true;
}

bool KnxMessage::is_message_valid() const
{
  if (m_message.size() == 0) {
    return false;
  }
  // TODO: check last octect
  return true;
}

