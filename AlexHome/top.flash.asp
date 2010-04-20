<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link rel="stylesheet" href="inc/flash.css" type="text/css">
<title>Alex's Space , Nothing great was ever achieved without enthusiasm! </title>
<style>
A{cursor:hand;height:2px;behavior:url(other/link.htc);
filter:progid:dximagetransform.microsoft.gradientwipe(duration=1)}
A:hover {color:"#E88D22";text-decoration:none; }
</style>

<SCRIPT LANGUAGE="JavaScript">
<!
var ostat = false;
function scrollstop() {news.scrollAmount = 0;ostat = true;}
function scrollplay() {news.scrollAmount = 5;ostat = false;}
function scrolldir() 
{
    mid = screen.width / 2;mouse = event.x;
    if(mouse > mid) 
    {
        if(ostat == false) 
        {
            speed = (mouse - mid) / 30;
            news.scrollAmount = speed;
        }
        news.direction = "right";
    }
    else 
    {
        if(ostat == false) 
        {
            speed = (mid - mouse) / 30;
            news.scrollAmount = speed;
        }
        news.direction = "left";
    }
}
//  End -->
</script>


<SCRIPT LANGUAGE="JavaScript">

// Set slideShowSpeed (milliseconds)
var slideShowSpeed = 7000;
// Duration of crossfade (seconds)
var crossFadeDuration = 7;
// Specify the image files
var Pic = new Array();
// to add more images, just continue
// the pattern, adding to the array below

Pic[0] = 'pic/flash/pic_top02.jpg'
Pic[1] = 'pic/flash/pic_top03.jpg'
Pic[2] = 'pic/flash/pic_top04.jpg'
Pic[3] = 'pic/flash/pic_top05.jpg'
Pic[4] = 'pic/flash/pic_top06.jpg'
Pic[5] = 'pic/flash/pic_top07.jpg'
Pic[6] = 'pic/flash/pic_top08.jpg'
Pic[7] = 'pic/flash/pic_top09.jpg'
Pic[8] = 'pic/flash/pic_top10.jpg'
Pic[9] = 'pic/flash/pic_top11.jpg'
Pic[10] = 'pic/flash/pic_top12.jpg'
Pic[11] = 'pic/flash/pic_top13.jpg'
Pic[12] = 'pic/flash/pic_top14.jpg'
Pic[13] = 'pic/flash/pic_top15.jpg'
Pic[14] = 'pic/flash/pic_top16.jpg'
Pic[15] = 'pic/flash/pic_top17.jpg'
Pic[16] = 'pic/flash/pic_top18.jpg'
Pic[17] = 'pic/flash/pic_top19.jpg'
Pic[18] = 'pic/flash/pic_top20.jpg'
Pic[19] = 'pic/flash/pic_top01.jpg'

// do not edit anything below this line
var t;
var j = 0;
var p = Pic.length;
var preLoad = new Array();
for (i = 0; i < p; i++) 
{
    preLoad[i] = new Image();
    preLoad[i].src = Pic[i];
}
function runSlideShow() 
{
    if (document.all) 
    {
        document.images.SlideShow.style.filter="blendTrans(duration=2)";
        document.images.SlideShow.style.filter="blendTrans(duration=crossFadeDuration)";
        document.images.SlideShow.filters.blendTrans.Apply();
    }
    document.images.SlideShow.src = preLoad[j].src;
    if (document.all) 
    {
        document.images.SlideShow.filters.blendTrans.Play();
    }
    j = j + 1;
    if (j > (p - 1)) j = 0;
    t = setTimeout('runSlideShow()', slideShowSpeed);
}
</script>

<style>body { background-image: url('pic/flash/bg.gif'); margin-left: 0px; margin-top: 0px }
</style>

<style>
BODY 
{
 SCROLLBAR-FACE-COLOR: #E2E2E2;
 SCROLLBAR-HIGHLIGHT-COLOR: #E2E2E2;
 SCROLLBAR-SHADOW-COLOR: #CCCCCC;
 SCROLLBAR-3DLIGHT-COLOR: #CCCCCC;
 SCROLLBAR-ARROW-COLOR: #C0C0C0;
 SCROLLBAR-TRACK-COLOR: #E2E2E2;
 SCROLLBAR-DARKSHADOW-COLOR: #E2E2E2;
 SCROLLBAR-BASE-COLOR: #E2E2E2;
	margin-bottom: 0px;
}
</style>
</head>
<body onLoad="runSlideShow()" style="overflow-x:hidden">
<table width="1085" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td ><div id="Layer1" style="position:absolute; left:0%; top:0%; width:100%; height:95%; z-index:1" >
      <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,29,0" width="100%" height="100%">
    　　
      <param name="wmode" value="transparent">
      <param name="movie" value="pic/flash/liuxing.swf">
      <param name="quality" value="high">
      <embed src="pic/flash/top.swf" quality="high" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" width="100%" height="100%"></embed>
      </object>
    </div>
    <div style="position:absolute; left:0%; top:0%; width:100%; height:95%; z-index:1"><img src="pic/flash/pic_top01.jpg" name='SlideShow' width="100%" height=100% align="absmiddle"> </div>
   </td>
  </tr>
</table>
</body>
</html>