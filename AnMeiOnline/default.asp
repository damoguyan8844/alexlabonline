<%
Response.CacheControl = "no-cache" 
Response.AddHeader "Pragma", "no-cache" 
Response.Expires = -1 
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Welcome to AnMei Online !</title>
<meta http-equiv='Expires' content='-1' />
<meta http-equiv="Pragma" content="no-cache" /> 
<meta http-equiv="CACHE-CONTROL" content="NO-CACHE" />
<link rel="stylesheet" href="inc/style.css" type="text/css" />
<script language='JavaScript' type="text/javascript">
if(top.location.href != self.location.href)
{
    top.location.replace(self.location.href);
    
}
function setCookie(name, value, hours, path, domain, secure) {
  var curCookie = name + "=" + escape(value) +
    ((hours) ? "; expires=" + (new Date((new Date()).getTime()+hours*3600000)).toGMTString() : "") +
    ((path) ? "; path=" + path : "") +
    ((domain) ? "; domain=" + domain : "") +
    ((secure) ? "; secure" : "");
  document.cookie = curCookie;
}
function showMenu(hidemenu)
{
	var obj=document.getElementsByTagName("FRAMESET")[1];
	var s=obj.cols;
	if(hidemenu ){
		s="0,6,*";
	}
	else {
		s="180,6,*";
	}
	obj.cols=s;
}
function saveLoadedURL(frame)
{
	try{
		var frm = window.frames[frame.name];
		var url = frm.location.href;
		var i,j
		i = url.indexOf("://");
		j = url.indexOf("/", i > -1 ? i+3 : 0);
		url = url.substr(j);
		setCookie("currentLoadedURL", url);
	}
	catch(e)
	{}
}
</script>
</head>
<%
	dim url
	url = Request.Cookies("currentLoadedURL")
	if url = "" or Instr(url, "default.asp")>0 or Instr(url, "main.asp")>0 then
		url = "mainframe.asp"
	end if	
%>
<frameset rows="61,*,30" style="border:none 0px" border="0">
	<frame src="top.flash.asp" scrolling="yes" frameborder="0" marginwidth="0" marginheight="0" />
	<frameset cols="180,6,*" style="border:none 0px;" border="0">
		<frame src="leftnav.asp" name="nav" frameborder="0" marginwidth="0" marginheight="0" />
		<frame src="midnav.htm" name="mid" scrolling='no' noresize="noresize" frameborder="0" marginwidth="0" marginheight="0"/>
		<frame src="<%= url %>" name="content" onload="saveLoadedURL(this)" frameborder="0" marginwidth="0" marginheight="0" />
	</frameset>
	<frame src="foot.note.asp" scrolling="no" noresize="noresize" frameborder="0" marginwidth="0" marginheight="0" />
</frameset>
</html>
