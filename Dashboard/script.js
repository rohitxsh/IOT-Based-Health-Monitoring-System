var req = new XMLHttpRequest();
req.overrideMimeType("application/json");
req.open('GET', 'http://api.thingspeak.com/channels/579775/feed/last.json?api_key=<thingspeak_api_key>', true);
req.onload = function () {
	var jsonResponse = JSON.parse(req.responseText);
	if (jsonResponse == -1)
		window.alert("The device is not responding. Please check the device for any communication issues.");
	else if (jsonResponse == 0)
		window.alert("Please refresh the page!");
	else {
		var bpm = jsonResponse.field1;
		var temp = jsonResponse.field2;
		if (bpm != null)
			document.getElementById("data1").innerHTML = bpm;
		else
			document.getElementById("data1").innerHTML = '--';
		if (temp != null) {
			document.getElementById("data2").innerHTML = temp;
			document.getElementById("data3").innerHTML = (temp * 9 / 5) + 32;
		}
		else {
			document.getElementById("data2").innerHTML = '--';
			document.getElementById("data3").innerHTML = '--';
		}
	}
};
req.send(null);
window.setInterval("reloadIFrame();", 45000); //45 sec.
function reloadIFrame() {
	//var iframe1=document.getElementById("iframeid1");
	//iframe1.src=iframe1.src;
	//var iframe2=document.getElementById("iframeid2");
	//iframe2.src=iframe2.src;

	var req1 = new XMLHttpRequest();
	req1.overrideMimeType("application/json");
	req1.open('GET', 'http://api.thingspeak.com/channels/579775/feed/last.json?api_key=<thingspeak_api_key>', true);
	req1.onload = function () {
		var jsonResponse = JSON.parse(req1.responseText);
		if (jsonResponse === -1)
			window.alert("The device is not responding. Please check the device for any communication issues.");
		else {
			var bpm = jsonResponse.field1;
			var temp = jsonResponse.field2;
			if (bpm != null)
				document.getElementById("data1").innerHTML = bpm;
			else
				document.getElementById("data1").innerHTML = '--';
			if (temp != null) {
				document.getElementById("data2").innerHTML = temp;
				document.getElementById("data3").innerHTML = (temp * 9 / 5) + 32;
			}
			else {
				document.getElementById("data2").innerHTML = '--';
				document.getElementById("data3").innerHTML = '--';
			}
		}
	};
	req1.send(null);
}