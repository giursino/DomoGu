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

#include "KnxTinySerialDriver.h"
#include "log.h"

using namespace log;

KnxTinySerialDriver::KnxTinySerialDriver():
  m_is_loaded(false),
  m_serial_port_name("/dev/serial0")
{

}

bool KnxTinySerialDriver::init()
{
  FILE_LOG(logINFO) << "KnxTinySerial init function";

  if (m_is_loaded) {
    FILE_LOG(logERROR) << "ERROR: driver already loaded";
    return false;
  }

  m_serial_port = new SerialPort(m_serial_port_name);
  try {
    m_serial_port->Open();
  } catch (std::exception) {
    FILE_LOG(logERROR) << "ERROR: cannot open serial port";
    delete (m_serial_port);
    return false;
  }
  if (m_serial_port->IsOpen() == false) {
    FILE_LOG(logERROR) << "ERROR: serial port not open!";
    delete (m_serial_port);
    return false;
  }

  m_driver = new KnxTinySerial(*m_serial_port);
  m_driver->Init();

  m_is_loaded = true;
  return true;
}

bool KnxTinySerialDriver::deinit()
{
  FILE_LOG(logINFO) << "KnxTinySerial deinit function";

  m_driver->DeInit();
  delete (m_driver);

  m_serial_port->Close();
  delete (m_serial_port);

  m_is_loaded = false;
  return true;
}

bool KnxTinySerialDriver::read(KnxMessage &message)
{
  if (!m_is_loaded) return false;
  while (true) {
    std::vector<unsigned char> frame;
    if (m_driver->Read(frame)) {
      message.set_raw(frame);
      return true;
    }
    else if (frame.size()) {
      FILE_LOG(logERROR) << "INVALID MESSAGE";
      message.set_raw(frame);
      return false;
    }
    m_driver->Sleep(loop_ms);
  }
}

bool KnxTinySerialDriver::write(const KnxMessage &message)
{
  if (!m_is_loaded) return false;

  std::vector<uint8_t> tx_frame;
  if (!message.get_raw(tx_frame)) return false;

  //send
  m_driver->Write(tx_frame);

  // wait confirmation
  bool success=false;
  while (true) {
    std::vector<uint8_t> rx_frame;
    success = m_driver->Read(rx_frame);

    if (rx_frame.empty()) {
      FILE_LOG(logWARNING) << "Send confirmation timeout";
      success=false;
      break;
    }
    if ((!success) && (rx_frame[0] == 0x8B)) {
      FILE_LOG(logDEBUG) << "Send confirmed";
      success=true;
      break;
    }
    else if ((!success) && (rx_frame[0] == 0x0B)) {
      FILE_LOG(logERROR) << "Send confirmation error";
      success=false;
      break;
    }
  }
  return success;
}
