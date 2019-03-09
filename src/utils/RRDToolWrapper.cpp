/*
Copyright 2019 Giuseppe Ursino

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

#include "RRDToolWrapper.h"
#include "log.h"

using namespace Log;

RRDToolWrapper::RRDToolWrapper(const std::string filename):
  m_filename(filename)
{

}

void RRDToolWrapper::CreateDb() const
{
  std::string cmd = "rrdtool create " + m_filename + " -s 30 "
      "DS:IndoorTemperature1:GAUGE:120:-40:50 "
      "DS:OutdoorTemperature:GAUGE:120:-40:50 RRA:AVERAGE:0.5:1:2880 "
      "DS:Thermostat1:GAUGE:120:0:1 RRA:MAX:0.9:1:2880";

  FILE_LOG(logINFO) << "CMD=" << cmd;
}

bool RRDToolWrapper::IsDbValid() const
{
  static bool first_run = true;
  if (first_run) {
    first_run = false;
    return false;
  }
  return true;
}

bool RRDToolWrapper::UpdateDb(const Track track, const float value) const
{
  switch (track) {

  case Track::IndoorTemperature1:
    FILE_LOG(logINFO) << "Got TA1=" << value;
    break;

  case Track::Thermostat1:
    FILE_LOG(logINFO) << "Got V1=" << value;
    break;

  case Track::OutdoorTemperature:
    FILE_LOG(logINFO) << "Got Text=" << value;
    break;

  default:
    FILE_LOG(logINFO) << "Not managed";
    break;
  }
  return true;
}

bool RRDToolWrapper::DumpDb(std::string dump) const
{
  dump = "";
  return true;
}

void RRDToolWrapper::CreateGraph() const
{

}
