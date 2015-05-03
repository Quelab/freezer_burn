
var LineChart = (function() {
line_chart = {};
data = [];
render = function(id, raw_data){

  Object.keys(raw_data).forEach(function(k) {
    d = raw_data[k];
    data.push(d);
  });

   n = data.length,
      duration = data[n-1].Time - data[0].Time;
      now = data[n-1].Time;

   margin = {top: 6, right: 0, bottom: 20, left: 40},
      width = 960 - margin.right,
      height = 120 - margin.top - margin.bottom;

   x = d3.time.scale()
      .domain([now - (n - 2) * duration, now - duration])
      .range([0, width]);

   y_scale = d3.scale.linear()
      .domain([d3.min(data, function(d){return d.Humidity}), d3.max(data, function(d){ return d.Humidity})])
      .range([height, 0]);

   line = d3.svg.line()
      .interpolate("basis")
      .x(function(d, i) { return x(now - (n - 1 - i) * duration); })
      .y(function(d, i) { return y_scale(d.Humidity); });

   svg = d3.select(id).append("svg")
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .style("margin-left", -margin.left + "px")
    .append("g")
      .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  svg.append("defs").append("clipPath")
      .attr("id", "clip")
    .append("rect")
      .attr("width", width)
      .attr("height", height);

   axis = svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .call(x.axis = d3.svg.axis().scale(x).orient("bottom"));

   y_axis = svg.append("g")
      .attr("class", "y axis")
      .call(y_scale.axis = d3.svg.axis().scale(y_scale).orient("left").ticks(6));

   path = svg.append("g")
      .attr("clip-path", "url(#clip)")
    .append("path")
      .datum(data)
      .attr("class", "line");

   transition = d3.select({}).transition()
      .duration(750)
      .ease("linear");
}
tick = function(new_data) {
    console.log(new_data);
    var now = new_data.Time;
    x.domain([now - (n - 2) * duration, now - duration]);
    y_scale.domain([d3.min(data, function(d){return d.Humidity}), d3.max(data, function(d){ return d.Humidity})]);

    // push the accumulated count onto the back, and reset the count
    data.push(new_data);

    // redraw the line
    svg.select(".line")
        .attr("d", line)
        .attr("transform", null);

    // slide the x-axis left
    axis.call(x.axis);

    // slide the line left
    path.transition()
        .attr("transform", "translate(" + x(now - (n - 1) * duration) + ")");

    // pop the old data point off the front
    data.shift();

}
  line_chart.render = render;
  line_chart.update = tick;
  return line_chart;

})()