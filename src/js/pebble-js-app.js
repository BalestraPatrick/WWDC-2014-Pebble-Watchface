Pebble.addEventListener("ready", function(e) {
	var offset = new Date().getTimezoneOffset();
	offset = (offset / 60) * (-1);
    console.log("Ready and current Timezone: " + offset);

    Pebble.sendAppMessage({
         "1" : offset
    });
});