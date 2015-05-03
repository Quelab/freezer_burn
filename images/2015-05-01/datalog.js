
var initializeHumidityChart = function(fb, callback){
  var ref = new fb("https://quelab-chillhub.firebaseio.com/users/simplelogin%3A1/devices/chillhubs/d84f6894-b3c0-4dd5-9b82-5632b8a7d1ce/serial_data");
  ref.limitToLast(100).once("value", function(snapshot) {
    var data = snapshot.val();
    callback(data);
}, function (errorObject) {
  console.log("The read failed: " + errorObject.code);
});
}

var listenForHumidityChange = function(fb, callback){
  var ref = new fb("https://quelab-chillhub.firebaseio.com/users/simplelogin%3A1/devices/chillhubs/d84f6894-b3c0-4dd5-9b82-5632b8a7d1ce/serial_data");
  ref.limitToLast(1).on("child_added", function(snapshot) {
    var data = snapshot.val();
    console.log(data);
    callback(data);
}, function (errorObject) {
  console.log("The read failed: " + errorObject.code);
});
}
