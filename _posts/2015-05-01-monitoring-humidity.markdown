---
layout: post
date:   2015-05-01 23:14:12
categories: chillhub update
---
<div id="linechart"></div>
<script type="text/javascript" src="/images/{{ page.date | date:"%Y-%m-%d" }}/datalog.js"></script>
<script type="text/javascript" src="/images/{{ page.date | date:"%Y-%m-%d" }}/line_chart.js"></script>
<script type="text/javascript">initializeHumidityChart(Firebase, function(data){LineChart.render('#linechart', data); listenForHumidityChange(Firebase, LineChart.update);})</script>
