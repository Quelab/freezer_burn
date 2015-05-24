---
layout: post
title:  Monitoring Temperature and Humidity with the HTU21D
categories: chillhub update HTU21D fritzing
---
<div id="linechart"></div>
<script type="text/javascript" src="{{ "/images/"| prepend: site.baseurl}}{{page.date | date:"%Y-%m-%d" }}/datalog.js"></script>
<script type="text/javascript" src="{{ "/images/" | prepend: site.baseurl}}{{page.date | date:"%Y-%m-%d" }}/line_chart.js"></script>
<script type="text/javascript">
  var address = "https://quelab-chillhub.firebaseio.com/freezer_burn_htu21d/644a5f55-1d08-43fa-874a-404ce401430f/"
  initializeHumidityChart(Firebase, address, function(data){LineChart.render('#linechart', data);
  listenForHumidityChange(Firebase, address, LineChart.update);})
</script>

The above chart is the last 100 measurements taken by the Arduino based temperature humidity sensor. The values are updated in real time with the sensor itself.

## HTU21D Freezer Burn Sensor
![fritzing_htu21d]({{ "/images/"| prepend: site.baseurl}}{{page.date | date:"%Y-%m-%d" }}/fritzing_htu21d.png)

 The current sensor is the [HTU21D](https://www.sparkfun.com/products/12064) from SparkFun running the following [Arduino sketch](https://github.com/Quelab/freezer_burn/tree/master/freezer_burn).

* factory calibrated
* supports relative humidity from 0-100% (&plusmn;2)%
* temperatures from (-40)&ndash;(+125)&deg;C.
* I&#178;C Interface (other sensors used have custom serial interfaces)

 Just like the SHT15, modifications to the ChillHub Arduino library were required to support signed integer data types. Additionally the temperature sensitivity is actually greater than displayed here, but the ChillHub does not currently support floating point numbers.

<p style="clear:both;">
</p>

### Notes
* The button was added to signal when an experiment is started or stopped. For instance adding an open container to the freezer compartment.
* Digital Input Configured as pull up has reverse logic, but allowed me to eliminate the external pull down resisters
* Modifications were required to the [chillhub-firmware](https://github.com/Quelab/chillhub-firmware) repository to detect the FTDI usb interface on the Arduino Nano.
* The Arduino Nano serial port doesn't reset properly after programming this requires a disconnect/reconnect to the computer after each program upload.
* Lots of code to debounce the buttons and mark the experiment as started.