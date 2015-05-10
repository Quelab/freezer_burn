---
layout: post
title:  Monitoring Temperature and Humidity with the Sensirion SHT15
categories: chillhub update sht15
---
<div id="linechart"></div>
<script type="text/javascript" src="{{ "/images/"| prepend: site.baseurl}}{{page.date | date:"%Y-%m-%d" }}/datalog.js"></script>
<script type="text/javascript" src="{{ "/images/" | prepend: site.baseurl}}{{page.date | date:"%Y-%m-%d" }}/line_chart.js"></script>
<script type="text/javascript">initializeHumidityChart(Firebase, function(data){LineChart.render('#linechart', data); listenForHumidityChange(Firebase, LineChart.update);})</script>

The above chart is the last 100 measurements taken by the Arduino based temperature humidity sensor. The values are updated in real time with the sensor itself. The current sensor is the SHT15 which is factory calibrated and supports relative humidity from 0-100% as well as temperatures from -40-100C. To make this work required modifications of the ChillHub Arduino library to support signed integer data types. Additionally the temperature sensitivity is actually greater than displayed here, but the ChillHub does not currently support floating point numbers.

## Notes
There is a small bug in the rendering where the x value of the last measurement is shifted one time over. The rest of the graph is much improved over the original version.
