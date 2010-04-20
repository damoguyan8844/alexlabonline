function showSubMenu(id){
    var menu = document.getElementById(id);
    if(!menu) return;
    if(hasClassName(menu, "on")) {
		removeClassName(menu, "on");
    }
    else {
		addClassName(menu, "on");
    }    
}
function setHighlight(elm, isHighlight)
{
	if(!elm) return;
	if(isHighlight){
		addClassName(elm, "on");
	}
	else{
		removeClassName(elm, "on");
	}
}
function toggleMenu(event)
{
	var elm = getSrcElement(event);
	if(!elm) return;
	if(elm.tagName.toLowerCase() != 'a') return;
	if(getParentElementByTagName(elm, "ul")==null) return;

	elm.blur();
	if(elm.href.Right(1) == '#') return;
	
	setHighlight(currentMenu, false);
	currentMenu = elm;
	setHighlight(currentMenu, true);
}
function getElementsByTagAndClass(tag, className)
{
	var arr = document.getElementsByTagName(tag);
	var arrRet = [];
	for(var i=0; i<arr.length; ++i){
		var elm = arr[i];
		if(hasClassName(elm, className)){
			arrRet.push(elm);
		}
	}
	return arrRet;
}

function initCurrentMenu()
{
    var w = window.parent.frames['content'];
    var url = "";
    if (w) {
	    url = w.location.href;
	}
	if(url.length>0){
		var root = getElementsByTagAndClass("ul", "root")[0];
		if(!root) return;
		var menus = root.getElementsByTagName("a");
		var menu = null;
		for(var i=0; i<menus.length; ++i){
			if(menus[i].href.indexOf(url) != -1) {
				menu = menus[i];
				break;
			}
		}
		if(menu != null){
			currentMenu = menu;
			setHighlight(currentMenu, true);
			var p = getParentElementByTagName(menu, "ul");
			while(p){
				addClassName(p, "on");
				p = getParentElementByTagName(p, "ul");
			}
		}
	}
}
var currentMenu = null;
addEvent(document, "click", toggleMenu);
addEvent(window, "load", initCurrentMenu);
window.onunload = function(){
	removeEvent(document, "click", toggleMenu);
	removeEvent(window, "load", initCurrentMenu);
};