# Temperature plotting

* example of using RRD with temperature: https://anders.com/projects/thermostat-graph/

  ```
  DEF:in=$db:indoorTemperature:AVERAGE
  DEF:out=$db:outdoorTemperature:AVERAGE 
  DEF:mode=$db:thermostatMode:MAX 
  CDEF:heat=mode,.5,+,FLOOR,3,-,UNKN,in,IF 
  CDEF:cool=mode,.5,+,FLOOR,5,-,UNKN,in,IF 
  CDEF:overlap=in,out,LT,in,out,IF 
  CDEF:heat-overlap=heat,out,LT,heat,out,IF 
  CDEF:cool-overlap=cool,out,LT,cool,out,IF 
  AREA:out#eeeeee 
  LINE1:out#aaaaaa:Outdoor 
  AREA:in#ccffcc 
  AREA:overlap#bbeebb 
  LINE1:overlap#99aa99 
  LINE1:in#009900:Indoor 
  AREA:heat#ffcccc 
  LINE1:heat#ff5555:Heat 
  AREA:cool#ccccff 
  LINE1:cool#5555ff:Cool 
  AREA:heat-overlap#ee9999 
  LINE1:heat-overlap#aa5555 
  AREA:cool-overlap#9999ee 
  LINE1:cool-overlap#5555aa";
  ```

* use JS chart: https://www.chartjs.org/docs/latest/general/accessibility.html

* home automation example: https://www.home-assistant.io/

* data logging example: 
 * https://www.briandorey.com/post/mosquitto-mqtt-and-home-assistant-data-logging
 * http://home.briandorey.com/reportday.aspx
 
# C++ improovment

* C++11 signal and slots example: http://simmesimme.github.io/tutorials/2015/09/20/signal-slot
