
var LineChart = (function() {
line_chart = {};
data = [];
render = function(id, raw_data){

  Object.keys(raw_data).forEach(function(k) {
    d = raw_data[k];
    data.push(d);
  });

   n = data.length;
   now = data[n-1].Time;
   start = data[0].Time;

   margin = {top: 8, right: 50, bottom: 30, left: 50},
      width = 800 - margin.right - margin.left,
      height = 140 - margin.top - margin.bottom;

   x_scale = d3.time.scale()
     .domain([d3.min(data, function(d){return d.Time}), d3.max(data, function(d){return d.Time})])
      .range([0, width]);

   y_scale = d3.scale.linear()
      .domain([d3.min(data, function(d){return d.Humidity}) - 1, d3.max(data, function(d){ return d.Humidity}) + 4])
      .range([height, 0]);

   temp_scale = d3.scale.linear()
      .domain([d3.min(data, function(d){return d.Temperature}) - 4, d3.max(data, function(d){ return d.Temperature}) + 1])
      .range([height, 0]);

   line = d3.svg.line()
      .interpolate("basis")
      .x(function(d, i) { return x_scale(d.Time); })
      .y(function(d, i) { return y_scale(d.Humidity); });

   temp_line = d3.svg.line()
      .interpolate("basis")
      .x(function(d, i) { return x_scale(d.Time); })
      .y(function(d, i) { return temp_scale(d.Temperature); });

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

   x_axis = svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .call(x_scale.axis = d3.svg.axis().scale(x_scale).orient("bottom").ticks(8));

   y_axis = svg.append("g")
      .attr("class", "y axis")
      .style("fill", "steelblue")
      .call(y_scale.axis = d3.svg.axis().scale(y_scale).orient("left").ticks(6));

  svg.append("text")
      .attr("class", "y label")
      .attr("text-anchor", "end")
      .attr("y", 6)
      .attr("dy", ".75em")
      .attr("transform", "rotate(-90), translate(-2,-50)")
      .style("fill", "steelblue")
      .text("% Rel. Humidity");

  temp_axis = svg.append("g")
      .attr("class", "temp axis")
      .attr("transform", "translate(" + width + " ,0)")
      .style("fill", "red")
      .call(temp_scale.axis = d3.svg.axis().scale(temp_scale).orient("right").tickFormat(d3.format("d")));

  svg.append("text")
      .attr("class", "y label")
      .attr("text-anchor", "end")
      .attr("y", 6)
      .attr("dy", ".75em")
      .attr("transform", "translate(" + (width+45) + ",0)")// + height +")")
      .style("fill", "red")
      .text("\u00B0C");

   path = svg.append("g")
      .attr("clip-path", "url(#clip)")
    .append("path")
      .datum(data)
      .attr("class", "line");

   temp_path = svg.append("g")
      .attr("clip-path", "url(#clip)")
    .append("path")
      .datum(data)
      .attr("class", "temp-line")

   transition = d3.select({}).transition()
      .duration(750)
      .ease("linear");
}
tick = function(new_data) {
    var now = new_data.Time;

    data.push(new_data);
    //x_scale.domain([data[1].Time, data[n].Time]);
    x_scale.domain([d3.min(data, function(d){return d.Time}), d3.max(data, function(d){return d.Time})])
    y_scale.domain([d3.min(data, function(d){return d.Humidity}) - 1, d3.max(data, function(d){ return d.Humidity})+ 4]);
    temp_scale.domain([d3.min(data, function(d){return d.Temperature}) - 4, d3.max(data, function(d){ return d.Temperature}) + 1]);

    // push the accumulated count onto the back, and reset the count

    // redraw the line
    svg.select(".line")
        .attr("d", line)
        .attr("transform", null);

    // redraw the line
    svg.select(".temp-line")
        .attr("d", temp_line)
        .attr("transform", null);

    // slide the x-axis left
    x_axis.call(x_scale.axis);

    // slide the line left
    duration = x_scale(data[1].Time) - x_scale(data[0].Time);
    path.transition()
      .attr("transform", "translate(" + duration + ")");
    temp_path.transition()
      .attr("transform", "translate(" + duration + ")");

    // pop the old data point off the front
    data.shift();

}
  line_chart.render = render;
  line_chart.update = tick;
  return line_chart;

})()