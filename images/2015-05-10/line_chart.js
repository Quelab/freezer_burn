
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
      .domain([d3.min(data, function(d){return d.weight}), d3.max(data, function(d){ return d.weight})])
      .range([height, 0]);

   weight_line = d3.svg.line()
      .interpolate("basis")
      .x(function(d, i) { return x_scale(d.Time); })
      .y(function(d, i) { return y_scale(d.weight); });

   svg = d3.select(id).append("svg")
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .style("margin-left", -margin.left + "px")
    .append("g")
      .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

   x_axis = svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .call(x_scale.axis = d3.svg.axis().scale(x_scale).orient("bottom"));

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
      .text("Weight");

   weight_path = svg.append("g")
      .attr("clip-path", "url(#clip)")
      .append("path")
      .datum(data)
      .attr("class", "line")

   transition = d3.select({}).transition()
      .duration(750)
      .ease("linear");

    // redraw the line
    svg.select(".line")
        .attr("d", weight_line)
        .attr("transform", null);
}
  line_chart.render = render;
  return line_chart;

})()