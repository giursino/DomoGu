/*
Copyright 2018 Giuseppe Ursino

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

#ifndef KNXMESSAGE_H
#define KNXMESSAGE_H

#include <vector>
#include <cstdint>
#include <string>

class KnxAddr
{
public:
    KnxAddr(): m_addr(0) {}

    KnxAddr(const std::uint16_t hex_addr): m_addr(hex_addr) {}

    KnxAddr(const std::uint8_t main, const std::uint8_t sub, const std::uint8_t line):
        m_addr (static_cast<std::uint16_t>(
                ((main & 0x0F)<< (8+4)) +
                ((sub & 0x0F) << (8)) +
                (line & 0xFF))
                ) {}

    void set_value(std::uint16_t hex_addr) {m_addr = hex_addr;}

    int get_value() {return m_addr;}

    int get_main() {return ((m_addr & 0xF000) >> (8+4));}
    int get_sub() {return ((m_addr & 0x0F00) >> (8));}
    int get_line() {return (m_addr & 0x00FF);}

private:
    std::uint16_t m_addr;
};

enum class Priority {
    system,
    urgent,
    normal,
    low
};

enum class FrameType {
    L_Data_Standard,
    L_Data_Extended,
    L_Poll_Data,
    ACK,
    NAK,
    BUSY,
    NAK_and_BUSY
};

enum class TransportLayerServices {
    T_Data_Broadcast,
    T_Data_Group,
    T_Data_Tag_Group,
    T_Data_Individual,
    T_Data_Connected,
    T_Connect,
    T_Disconnect,
    T_ACK,
    T_NAK
};

enum class ApplicationLayerServices {
  A_GroupValue_Read,
  A_GroupValue_Response,
  A_GroupValue_Write,
  A_IndividualAddress_Write,
  A_IndividualAddress_Read,
  A_IndividualAddress_Response,
  A_ADC_Read,
  A_ADC_Response,
  A_SystemNetworkParameter_Read,
  A_SystemNetworkParameter_Response,
  A_SystemNetworkParameter_Write,
  A_Memory_Read,
  A_Memory_Response,
  A_Memory_Write,
  A_UserMemory_Read,
  A_UserMemory_Response,
  A_UserMemory_Write,
  A_UserMemoryBit_Write,
  A_UserManufacturerInfo_Read,
  A_UserManufacturerInfo_Response,
  A_FunctionPropertyCommand,
  A_FunctionPropertyState_Read,
  A_FunctionPropertyState_Response,
  A_DeviceDescriptor_Read,
  A_DeviceDescriptor_Response,
  A_Restart,
  A_Open_Routing_Table_Req,
  A_Read_Routing_Table_Req,
  A_Read_Routing_Table_Res,
  A_Write_Routing_Table_Req,
  A_Read_Router_Memory_Req,
  A_Read_Router_Memory_Res,
  A_Write_Router_Memory_Req,
  A_Read_Router_Status_Req,
  A_Read_Router_Status_Res,
  A_Write_Router_Status_Req,
  A_MemoryBit_Write,
  A_Authorize_Request,
  A_Authorize_Response,
  A_Key_Write,
  A_Key_Response,
  A_PropertyValue_Read,
  A_PropertyValue_Response,
  A_PropertyValue_Write,
  A_PropertyDescription_Read,
  A_PropertyDescription_Response,
  A_NetworkParameter_Read,
  A_NetworkParameter_Response,
  A_IndividualAddressSerialNumber_Read,
  A_IndividualAddressSerialNumber_Response,
  A_IndividualAddressSerialNumber_Write,
  A_DomainAddress_Write,
  A_DomainAddress_Read,
  A_DomainAddress_Response,
  A_DomainAddressSelective_Read,
  A_NetworkParameter_Write,
  A_Link_Read,
  A_Link_Response,
  A_Link_Write,
  A_GroupPropValue_Read,
  A_GroupPropValue_Response,
  A_GroupPropValue_Write,
  A_GroupPropValue_InfoReport,
  A_DomainAddressSerialNumber_Read,
  A_DomainAddressSerialNumber_Response,
  A_DomainAddressSerialNumber_Write,
  A_FileStream_InfoReport
};

class ApplicationLayerPayload {
public:

  ApplicationLayerPayload() :
    m_raw_value(){;}

  ApplicationLayerPayload(const std::vector<std::uint8_t> &payload):
    m_raw_value(payload) {;}

  virtual ~ApplicationLayerPayload();

  std::vector<std::uint8_t> m_raw_value;

  virtual std::vector<std::uint8_t> get_value() {return m_raw_value;}

};

class GroupValue: public ApplicationLayerPayload {
public:
  GroupValue() :
    ApplicationLayerPayload() {;}
  GroupValue(const std::vector<std::uint8_t> &payload) :
    ApplicationLayerPayload(payload) {;}
  ~GroupValue();

  std::vector<std::uint8_t> get_value() {return m_raw_value;}
};

class PropertyValue: public ApplicationLayerPayload {
public:
  PropertyValue() :
    ApplicationLayerPayload() {;}
  PropertyValue(const std::vector<std::uint8_t> &payload) :
    ApplicationLayerPayload(payload) {;}
  ~PropertyValue();

  std::uint8_t get_object_index() {return m_raw_value[0];}
  std::uint8_t get_PID() {return m_raw_value[1];}
  std::uint8_t get_nr_elem() {return (m_raw_value[2] & 0xF0)>>4;}
  std::uint8_t get_start_index() {return (m_raw_value[2] & 0x0F)+m_raw_value[3];}
  std::vector<std::uint8_t> get_value() {
    std::vector<std::uint8_t> value(m_raw_value.begin()+4, m_raw_value.end());
    return value;
  }

};


class KnxMessage
{
public:
    KnxMessage();
    KnxMessage(const std::vector<std::uint8_t> &message);

    bool set_raw(const std::vector<std::uint8_t> &message);
    bool get_raw(std::vector<std::uint8_t> &message) const;

    std::string get_string() const;

    bool get_src(KnxAddr &addr) const;
    bool get_dest(KnxAddr &addr) const;

    bool get_frame_type(FrameType &value) const;
    bool is_frame_repeated() const;
    bool get_priority(Priority &value) const;
    bool is_destination_broadcast_address() const;
    bool is_destination_group_address() const;
    bool get_hopcount(std::uint8_t &hopcount) const;
    bool get_length(std::uint8_t &length) const;
    bool get_transport_layer_services(TransportLayerServices &value) const;
    bool get_transport_layer_sequence_num(std::uint8_t &value) const;
    bool get_apci(ApplicationLayerServices &value) const;
    bool get_payload(std::vector<std::uint8_t> &payload) const;
    bool get_application_layer(ApplicationLayerServices &service,
                               ApplicationLayerPayload &payload) const;

private:
    bool is_message_valid() const;
    bool is_data_frame() const;

    std::vector<std::uint8_t> m_message;
};

#endif // KNXMESSAGE_H
