function getXHR(){
	var http = null;
	if (window.XMLHttpRequest != null)	{
		http = new window.XMLHttpRequest();
	}
	else if (window.ActiveXObject != null) {
		var MS_PROGIDS = [
			"Msxml2.XMLHTTP.6.0",
			"Msxml2.XMLHTTP.5.0",
			"Msxml2.XMLHTTP.4.0",
			"MSXML2.XMLHTTP.3.0",
			"MSXML2.XMLHTTP",
			"Microsoft.XMLHTTP"
		];
		var success = false;
		for (var i = 0; (!success) && (i < MS_PROGIDS.length); i++) {
			try	{
				http = new ActiveXObject(MS_PROGIDS[i]);
				success = true;
			}
			catch (ex)
			{}
		}
	}
	return http;
}
function online()
{
	if(xhr == null) {
		xhr = getXHR();
		if(xhr == null) return;
	}
	xhr.open("HEAD", "online.asp?"+(new Date()).getTime(), true);
	xhr.onreadystatechange = onStateChange;
	xhr.send(null);
}
function nullF(){};
function onStateChange()
{
	if(xhr.readyState == 4){
		var sTimeout = xhr.getResponseHeader("Ajax-Status");
		if (sTimeout == "online" ){
			active();
		}
		else{
			window.top.location.replace("login.asp?logout=1");
		}
		xhr.onreadystatechange = nullF;
	}
};
function active()
{
	timer = setTimeout(online, 1140000); //19 min
}
var xhr = null;
var timer = null;
addEvent(window, "load", active);
window.onunload = function(){
	if(timer){
		clearTimeout(timer);
		timer = null;
	}
	removeEvent(window, "load", active);
	if(xhr){
		xhr.onreadystatechange = nullF;
		xhr = null;
	}
};