---
layout: post
title:  Auto logger with MilkyWeighs
categories: chillhub update milkyweighs
---
<div id="linechart"></div>
<script type="text/javascript" src="{{ "/images/"| prepend: site.baseurl}}{{page.date | date:"%Y-%m-%d" }}/datalog.js"></script>
<script type="text/javascript" src="{{ "/images/" | prepend: site.baseurl}}{{page.date | date:"%Y-%m-%d" }}/line_chart.js"></script>
<script type="text/javascript">initializeMilkyWeighsChart(Firebase, function(data){LineChart.render('#linechart', data);})</script>
Logging of the MilkyWeigh is off but this was just a short test to see if it was auto detected by the logger script. It was!

## Notes
The MilkyWeigh seems to report at regular intervals instead of when the measurement changes. This means that the free FireBase Account would quickly fill up with logged data as well as bandwidth from the MilkyWeigh alone. For this reason I would suggest not auto logging it with the free FireBase account.
