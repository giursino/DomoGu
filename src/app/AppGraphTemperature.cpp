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

#include "AppGraphTemperature.h"

#include <chrono>
#include <cstdint>
#include "log.h"
#include "knx/KnxFloat.h"

using namespace Log;

AppGraphTemperature::AppGraphTemperature(KnxManager *knx):
  m_knx(knx),
  m_rrd("$HOME/rrd_temperature.db")
{
  FILE_LOG(logINFO) << "AppGraphTemperature loaded";

  m_thread = std::thread([this] {Loop();});
}

AppGraphTemperature::~AppGraphTemperature()
{
  m_thread.join();
  m_knx->Deregister(this);
  FILE_LOG(logINFO) << "AppGraphTemperature unloaded";
}

void AppGraphTemperature::Loop()
{
  bool running = Init();

  FILE_LOG(logINFO) << "Starting loop...";
  while(running) {

    {
      FILE_LOG(logINFO) << "Get indoor temperature...";
      KnxMessage msg({0xBC, 0x11, 0x0F, 0x0C, 0x72, 0xE1, 0x00, 0x00});
      m_knx->SendMessage(msg);
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));

    {
      FILE_LOG(logINFO) << "Get output status...";
      KnxMessage msg({0xBC, 0x11, 0x0F, 0x0C, 0x6F, 0xE1, 0x00, 0x00});
      m_knx->SendMessage(msg);
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    {
      FILE_LOG(logINFO) << "Get outdoor temperature...";
      KnxMessage msg({0xBC, 0x11, 0x0F, 0x0C, 0x60, 0xE1, 0x00, 0x00});
      m_knx->SendMessage(msg);
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }
}

void AppGraphTemperature::OnMessageReceived(KnxMessage &message) const
{
  // TODO: qui non si possono modificare i membri della classe ma si deve notificare
  // tramite eventi la ricezione di un messaggio (thread safe)

  KnxAddr dest_addr;
  if (!message.get_dest(dest_addr)) return;

  ApplicationLayerPayload value;
  ApplicationLayerServices apci;
  if (!message.get_application_layer(apci, value)) return;

  if (apci == ApplicationLayerServices::A_GroupValue_Response) {
    switch (dest_addr.get_value()) {

    case 0x0C72:
    {
      KnxFloat knx_float_value(value.get_value());

      // TODO: possibile problema di threading
      m_rrd.UpdateDb(RRDToolWrapper::Track::IndoorTemperature1, knx_float_value.GetFloat());
      break;
    }

    case 0x0C6F:
    {
      uint8_t status = value.get_value()[0];
      //      octect 1
      //  7 6 5 4 3 2 1 0
      // [- - - - - H C B]
      // H = hot status
      // C = cold status
      // B = boost status
      uint8_t hot_status = (status & 0x04) >> 2;
      uint8_t cold_status = (status & 0x02) >> 1;
      uint8_t boost_status = (status & 0x01);

      // TODO: possibile problema di threading
      m_rrd.UpdateDb(RRDToolWrapper::Track::Thermostat1, hot_status);
      break;
    }

    case 0x0C60:
    {
      KnxFloat knx_float_value(value.get_value());

      // TODO: possibile problema di threading
      m_rrd.UpdateDb(RRDToolWrapper::Track::OutdoorTemperature, knx_float_value.GetFloat());
      break;
    }
    }
  }

}

bool AppGraphTemperature::Init()
{
  FILE_LOG(logINFO) << "Registering...";
  if (!m_knx->Register(this)) return false;

  FILE_LOG(logINFO) << "Checking DB...";
  if (!m_rrd.IsDbValid()) {
    FILE_LOG(logINFO) << "Creating DB...";
    m_rrd.CreateDb();

    if (!m_rrd.IsDbValid()) return false;
  }

  return true;
}
