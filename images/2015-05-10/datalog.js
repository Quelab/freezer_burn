
var initializeMilkyWeighsChart = function(fb, callback){
  var ref = new fb("https://quelab-chillhub.firebaseio.com/milkyWeighs/329e5bfe-2d19-4b23-9883-97bc8117a651/2015-05-10");
  ref.once("value", function(snapshot) {
    var data = snapshot.val();
    callback(data);
}, function (errorObject) {
  console.log("The read failed: " + errorObject.code);
});
}
