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
#include "ShellUtils.h"

using namespace Log;

RRDToolWrapper::RRDToolWrapper(const std::string filename):
  m_filename(filename)
{

}

void RRDToolWrapper::CreateDb() const
{
  // RRDTool (https://oss.oetiker.ch/rrdtool/doc/rrdcreate.en.html)
  //
  // Data Source (DS):
  // - step acquisition (every step create a primary data-point, PDP, in DSP): 1 min
  // - heathbeat (max time prior to consider a value *UNKNOWN*): 2 min
  //
  // Archive (RRA):
  // - Temperature:
  //    - consolidate function (function used to create a consolidated data-point (CDP) from a PDP value): AVERAGE
  //    - xff (ratio of allowed *UNKNOWN* PDPs to the number of PDPs in the interval): 0.5
  //    - archive:
  //     - 7 day with 1 min precision
  //     - 3 months with 5 min precision
  //     - 1 year with 30 min precision
  // - Thermostat:
  //    - consolidate function: MAX
  //    - xff: 0.9 (needed only 10% of valid PDP to create a CDP)
  //    - archive:
  //     - 7 day with 1 min precision
  //     - 3 months with 5 min precision
  //     - 1 year with 30 min precision

  std::string cmd = "rrdtool create " + m_filename + " -s 1m "
      "DS:IndoorTemperature1:GAUGE:120:-40:50 RRA:AVERAGE:0.5:1m:7d RRA:AVERAGE:0.5:5m:90d RRA:AVERAGE:0.5:30m:1Y "
      "DS:OutdoorTemperature:GAUGE:120:-40:50 RRA:AVERAGE:0.5:1m:7d RRA:AVERAGE:0.5:5m:90d RRA:AVERAGE:0.5:30m:1Y "
      "DS:Thermostat1:GAUGE:120:0:1 RRA:MAX:0.9:1m:7d RRA:MAX:0.9:5m:90d RRA:MAX:0.9:30m:1Y";

  FILE_LOG(logINFO) << "CMD=" << cmd;
  ExecuteScript(cmd);
}

bool RRDToolWrapper::IsDbValid() const
{
  std::string cmd = "rrdtool info " + m_filename;

  return (ExecuteScript(cmd) == 0);
}

bool RRDToolWrapper::UpdateDb(const Track track, const float value) const
{
  std::string cmd = "rrdtool update " + m_filename + " N:";

  switch (track) {

  case Track::IndoorTemperature1:
    FILE_LOG(logINFO) << "Got TA1=" << value;
    cmd += std::to_string(value) + ":U:U";
    break;

  case Track::Thermostat1:
    FILE_LOG(logINFO) << "Got V1=" << value;
    cmd +=  "U:U:" + std::to_string(value);
    break;

  case Track::OutdoorTemperature:
    FILE_LOG(logINFO) << "Got Text=" << value;
    cmd +=  "U:" + std::to_string(value) + ":U";
    break;

  default:
    FILE_LOG(logINFO) << "Not managed";
    break;
  }

  FILE_LOG(logINFO) << "CMD=" << cmd;

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
