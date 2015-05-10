
var initializeHumidityChart = function(fb, callback){
  date = new Date().toISOString().substring(0, 10);
  var ref = new fb("https://quelab-chillhub.firebaseio.com/freezer_burn_sht15/78f3a753-4d28-499a-8b08-c20d32619544/" + date);
  ref.limitToLast(100).once("value", function(snapshot) {
    var data = snapshot.val();
    callback(data);
}, function (errorObject) {
  console.log("The read failed: " + errorObject.code);
});
}

var listenForHumidityChange = function(fb, callback){
  date = new Date().toISOString().substring(0, 10);
  var ref = new fb("https://quelab-chillhub.firebaseio.com/freezer_burn_sht15/78f3a753-4d28-499a-8b08-c20d32619544/" + date);
  ref.limitToLast(5).on("child_added", function(snapshot) {
    var data = snapshot.val();
    callback(data);
}, function (errorObject) {
  console.log("The read failed: " + errorObject.code);
});
}
