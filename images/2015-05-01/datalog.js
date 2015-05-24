
var initializeHumidityChart = function(fb, callback){
  date = new Date().toISOString().substring(0, 10);
  var ref = new fb("https://quelab-chillhub.firebaseio.com/freezer_burn_dht11/41e1b18e-2d12-4306-9211-c1068bf7f76d/2015-05-17");
  ref.limitToLast(100).once("value", function(snapshot) {
    var data = snapshot.val();
    callback(data);
}, function (errorObject) {
  console.log("The read failed: " + errorObject.code);
});
}

// var listenForHumidityChange = function(fb, callback){
//   date = new Date().toISOString().substring(0, 10);
//   var ref = new fb("https://quelab-chillhub.firebaseio.com/freezer_burn_dht11/41e1b18e-2d12-4306-9211-c1068bf7f76d/" + date);
//   ref.limitToLast(5).on("child_added", function(snapshot) {
//     var data = snapshot.val();
//     callback(data);
// }, function (errorObject) {
//   console.log("The read failed: " + errorObject.code);
// });
// }
