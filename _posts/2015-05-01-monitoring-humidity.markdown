---
layout: post
title:  Monitoring Temperature and Humidity with the DHT11
date:   2015-05-01 23:14:12
categories: chillhub update
---
<div id="linechart"></div>
<script type="text/javascript" src="{{ "/images/"| prepend: site.baseurl}}{{page.date | date:"%Y-%m-%d" }}/datalog.js"></script>
<script type="text/javascript" src="{{ "/images/" | prepend: site.baseurl}}{{page.date | date:"%Y-%m-%d" }}/line_chart.js"></script>
<script type="text/javascript">initializeHumidityChart(Firebase, function(data){LineChart.render('#linechart', data);})
</script>

The above chart is the last 100 measurements taken by the Arduino based temperature humidity sensor. ~~The values are updated in real time with the sensor itself.~~(Sensor has been removed) The current sensor is the cheapest DHT11 sensor which isn't likely going to be sensitive enough to measure humidity in the freezer compartment.

