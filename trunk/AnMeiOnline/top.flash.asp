<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<!--<link rel="stylesheet" href="inc/flash.css" type="text/css">-->
<link rel="stylesheet" href="inc/style.css" type="text/css">
<%
function GetLogo()
on error resume next
	Dim  picName	
	picName = "../images/logo.gif"	
	GetLogo = picName
end function
%>
<head>
<title></title>
<link rel="stylesheet" href="inc/style.css" type="text/css">
</head>
<body>
<div class="banner">
    <div class="top"></div>
    <div id="bottom1" class="bottom" >
        <div class="left" style ="background-image:url(<%=GetLogo() %>) "></div>
        <div class="right"></div>        
    </div>
</div>
</body>
</html>