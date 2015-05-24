
var initializeHumidityChart = function(fb, address, callback){
  date = new Date().toISOString().substring(0, 10);
  var ref = new fb(address + date);
  ref.limitToLast(100).once("value", function(snapshot) {
    var data = snapshot.val();
    callback(data);
}, function (errorObject) {
  console.log("The read failed: " + errorObject.code);
});
}

var listenForHumidityChange = function(fb, address, callback){
  date = new Date().toISOString().substring(0, 10);
  var ref = new fb(address + date);
  ref.limitToLast(5).on("child_added", function(snapshot) {
    var data = snapshot.val();
    callback(data);
}, function (errorObject) {
  console.log("The read failed: " + errorObject.code);
});
}
