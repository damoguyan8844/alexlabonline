var Browser = {
    isIE: !!(window.attachEvent &&
        navigator.userAgent.indexOf('Opera') === -1),
    isOpera:  navigator.userAgent.indexOf('Opera') > -1,
    isWebKit: navigator.userAgent.indexOf('AppleWebKit/') > -1,
    isGecko:  navigator.userAgent.indexOf('Gecko') > -1 &&
        navigator.userAgent.indexOf('KHTML') === -1,
    isMobileSafari: !!navigator.userAgent.match(/Apple.*Mobile.*Safari/),
    platform: (window.orientation != undefined) ? 'ipod' : (navigator.platform.match(/mac|win|linux/i) || ['other'])[0].toLowerCase(),
    version: parseFloat( ( navigator.userAgent.match(/MSIE (\d+\.\d+)/) || [0,0] )[1] )
};

Object.extend = function(destination, source) { 
	for (var property in source) { 
		if( destination[property] === undefined) {
			destination[property] = source[property];
		}
	}
	return destination;
};

Object.isUndefined = function(object) {
    return typeof object == "undefined";
};
Number.prototype.html = function() {
	return this;
};
Number.prototype.json = function() {
	return this;
};
String.prototype.html = function() {
	return this.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/\"/g, "&quot;").replace(/\n/g, "<br>");
};
String.prototype.xml = function() {
	return this.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/\"/g, "&quot;").replace(/\'/g, "&apos;");
};
String.prototype.json = function() {
	return this.replace(/\\/g, "\\\\").replace(/"/g, "\\\"").replace(/\n/g, "\\n");
};
String.prototype.trim= function()  
{  
    return this.replace(/(^\s*)|(\s*$)/g, "");  
};
String.prototype.toCapital = function(){
	var tmp = this;	
	if(tmp.length>0){
		tmp = tmp.substring(0,1).toUpperCase()+tmp.substr(1).toLowerCase();
	}
	return tmp;
};
String.prototype.uFirst = function(){
	var tmp = this;	if(tmp.length>0){tmp = tmp.substring(0,1).toUpperCase()+tmp.substr(1);}	return tmp;
};
String.prototype.uWord = function() {
	var tmp = this.trim();	if(tmp.indexOf(" ") == -1){	return tmp.toCapital();	}else {	var arr = tmp.split(" ");for(var i=0; i<arr.length; ++i) {arr[i] = arr[i].trim().toCapital();}return arr.join(" ");	}return tmp;
};
String.prototype.Right = function(num){
	return this.substr(this.length-num)
};
String.prototype.Left = function(num){
	if(num < 0) num += this.length;	return this.substr(0, num);
};
String.prototype.isNumeric = function(){
	return /^[+-]?(\d*\.?\d+|\d+\.?\d*)([eE][+-]?\d+)?$/.test(this);
};
String.prototype.isVisibleChars = function(){
	return /^[\w\d\s~`!@#\$%\^&*\(\)\-_+=\?\/\\\[\]\{\}\:;\"\'<>,\.|]+$/.test(this);
};
String.format = function(format){
    var args = Array.prototype.slice.call(arguments, 1);  return format.replace(/\{\{(\d+)\}\}/g, function(m, i){return args[i]; });
};
String.prototype.formatNumber = function(digit)
{
	var num, k, f, n, i, s;
	
	digit = parseInt(digit);
	if(isNaN(digit)) {return this;}
	if(digit<1){return this;}
	
	num = this.replace(/,/g, '');
	k = 1;
	if(!num.isNumeric()) { num = 0;}
	k = Math.pow(10, digit);

	f = Math.round(parseFloat(num)*k*1.0)/(k*1.0);	s = f.toString();	n = s.indexOf(".");

	if(n == -1) {
		s += "."; n = 0;
	}
	else{
		n = s.length-n-1;
	}
	for(i=0; i < digit-n; i++){
		s += "0";
	}
	return s;
};
String.prototype.toCsv = function(){
	var v = this;
	if (v.indexOf('"') != -1 ){ 
		v = v.replace(/\"/g, '""');
	}
	if ( v.indexOf(",") != -1 ){
		v = '"' + v + '"';
	}
	return v;
};
String.prototype.nbsp = function(){
	var v = this.trim();
	if(v==""){
		return "&nbsp;";
	}
	return v;
};
String.prototype.camelCase = function(){
	return this.replace(/-\D/g, function(match){
		return match.charAt(1).toUpperCase();
	});
};
String.prototype.hyphenate = function(){
	return this.replace(/[A-Z]/g, function(match){
		return ('-' + match.charAt(0).toLowerCase());
	});
};
function $A(iterable) {
  if (!iterable) return [];
  if (iterable.toArray) return iterable.toArray();
  var length = iterable.length || 0, results = new Array(length);
  while (length--) results[length] = iterable[length];
  return results;
}
Function.prototype.bind = function() {
    if (arguments.length < 2 && Object.isUndefined(arguments[0])) return this;
    var __method = this, args = $A(arguments), object = args.shift();
    return function() {
      return __method.apply(object, args.concat($A(arguments)));
    }
};

Function.prototype.bindAsEventListener = function() {
    var __method = this, args = $A(arguments), object = args.shift();
    return function(event) {
      return __method.apply(object, [event || window.event].concat(args));
    }
};

function formatCouponRate(v)
{
    var r = parseFloat(v);
    if (isNaN(r)) return "N/A";

    r = r > 1 ? r : r * 100;
    return r.toString().formatNumber(3) + "%";
}
function _parseFloat(n, deft)
{
    n = parseFloat(n);
	if(isNaN(n)) {
	    return deft;
	}
	else{
	    return n;
	}
}

function truncate(input, length) {
	if(!length) length = 255;
	var v = input.value.trim();
	input.value = v.Left(length);
}
if (!Array.prototype.indexOf){
	Array.prototype.indexOf = function(item, from) {
		var len = this.length;
		if (from == null){
			from = 0;
		}
		for (var i = (from < 0) ? Math.max(0, len + from) : from || 0; i < len; i++) {
			if (this[i] == item){
				return i;
			}
		}
		return -1;
	};
}
function indexOf(items, item, from)
{
    var len = items.length;
	for (var i = (from < 0) ? Math.max(0, len + from) : from || 0; i < len; i++){
		if (items[i] === item) return i;
	}
	return -1;
}
function IsArray(val) 
{
	if(val == undefined) return false;
	if(val.constructor == Array) return true;
	return false;
}
function IsObject(val) 
{
	if(val == undefined) return false;
	if(val.constructor == Object) return true;
	return false;
}

function clone(a){
	if (IsObject(a))
	{
		return cloneObject(a);
	}
	else if (IsArray(a))
	{
		return cloneArray(a);
	}
	else {
		return a;
	}
}
function cloneArray(a){
	var c = [];
	for(var i=0; i<a.length; ++i){
		if (IsObject(a[i]))
		{
			c[i] = cloneObject(a[i]);
		}
		else if (IsArray(a[i]))
		{
			c[i] = cloneArray(a[i]);
		}
		else {
			c[i] = a[i];
		}
	}
	return c;
}
function cloneObject(a) {
	var c = {};
	for(var x in a){
		if (IsObject(a[x]))
		{
			c[x] = cloneObject(a[x]);
		}
		else if (IsArray(a[x]))
		{
			c[x] = cloneArray(a[x]);
		}
		else {
			c[x] = a[x];
		}
	}
	return c;
}
//check form
function isDateString(s)
{
    var d = Date.parse(s);
    if(isNaN(d)){
        return false;
    }
    else{
        return true;
    }
}
function IsNumeric(str)
{
    var regu = /^[0-9]*$/;
    var re = new RegExp(regu);   
    if (re.test(str)) {   
      return true;   
    }    
    return false; 
}   
function IsNumLetter(str)
{     
    var regu = /^[_a-zA-Z0-9-]+(.[_a-zA-Z0-9-]+)*$/;//^\w{1,20}$/;      
    var re = new RegExp(regu);   
    if (re.test(str)) {   
      return true;   
    }    
    return false;   
}
function IsPhone(str) 
{ 
	var regu=/^[0-9-()]*$/; 
	var re = new RegExp(regu);   
    if (re.test(str)) {   
      return true;   
    }    
    return false; 
}
function IsEmail(str)
{  
	return(new RegExp(/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/).test(str)); 
}
//end check

function $(id){
	if (id == undefined) return null;
	id = id + "";
	if (id.trim() == "") return null;
	return window.document.getElementById(id);
}
(function(){
    if(typeof HTMLElement!="undefined"&&!HTMLElement.prototype.insertAdjacentElement){
	HTMLElement.prototype.insertAdjacentElement=function(where,parsedNode)	{
		switch(where)		{
			case'beforeBegin':
				this.parentNode.insertBefore(parsedNode,this);
				break;
			case'afterBegin':
				this.insertBefore(parsedNode,this.firstChild);
				break;
			case'beforeEnd':
				this.appendChild(parsedNode);
				break;
			case'afterEnd':
				if(this.nextSibling){
					this.parentNode.insertBefore(parsedNode,this.nextSibling);
				}
				else {
					this.parentNode.appendChild(parsedNode);
				}
				break;
		}
	};
	HTMLElement.prototype.insertAdjacentHTML=function(where,htmlStr)	{
		var r=this.ownerDocument.createRange();
		r.setStartBefore(this);
		var parsedHTML=r.createContextualFragment(htmlStr);
		this.insertAdjacentElement(where,parsedHTML);
	};
	HTMLElement.prototype.insertAdjacentText=function(where,txtStr)	{
		var parsedText=document.createTextNode(txtStr);
		this.insertAdjacentElement(where,parsedText);
	};
	var _emptyTags=	{
		"IMG":true,"BR":true,"INPUT":true,"META":true,"LINK":true,"PARAM":true,"HR":true
	};
	HTMLElement.prototype.__defineGetter__("outerHTML",function(){var attrs=this.attributes;var str="<"+this.tagName;for(var i=0;i<attrs.length;i++)str+=" "+attrs[i].nodeName+"=\""+attrs[i].nodeValue+"\"";if(_emptyTags[this.tagName])return str+">";return str+">"+this.innerHTML+"</"+this.tagName+">";});
	HTMLElement.prototype.__defineSetter__("outerHTML",function(sHTML){var r=this.ownerDocument.createRange();r.setStartBefore(this);var df=r.createContextualFragment(sHTML);this.parentNode.replaceChild(df,this);});
	HTMLElement.prototype.__defineGetter__("innerText",function(){var tmp=this.innerHTML.replace(/<br>/gi,"\n");return tmp.replace(/<[^>]+>/g,"");});
	HTMLElement.prototype.__defineSetter__("innerText",function(txtStr){var parsedText=document.createTextNode(txtStr);this.innerHTML="";this.appendChild(parsedText);});

	HTMLElement.prototype.__defineGetter__("innerHTML2",function()	{
		var attrs=this.attributes;
		var str="<"+this.nodeName;
		for(var i=attrs.length-1;i>0;i--)
		{
			 if(attrs[i].specified && attrs[i].nodeName != "_moz-userdefined")
			 {
				str+=" "+attrs[i].nodeName+"=\""+attrs[i].nodeValue+"\"";
			 }
		}
		if(_emptyTags[this.nodeName]){
			return str+">";
		}
			
		str += ">";
		var iChild = 0;
		while (iChild < this.childNodes.length)
		{
			if (this.childNodes[iChild].nodeType == 1)
			{
				str += this.childNodes[iChild].innerHTML2;
			}
			else if(this.childNodes[iChild].nodeType == 3)
			{
				str += this.childNodes[iChild].nodeValue;
			}
			iChild++;
		}		
		return str+"</"+this.nodeName+">";
	});
}})();

function hasClassName(el, name) {

    var i, list;

    list = el.className.split(" ");
    for (i = 0; i < list.length; i++){
        if (list[i] == name){
            return true;
        }
   }

    return false;
}

function addClassName(el, name)
{
	if(!hasClassName(el, name))
	{
		el.className += " "+name;
	}
}
function removeClassName(el, name) {

    var i, curList, newList;

    if (el.className == null){
        return;
    }

    newList = new Array();
    curList = el.className.split(" ");
    for (i = 0; i < curList.length; i++){
        if (curList[i] != name) {
            newList.push(curList[i]);
        }
    }
    el.className = newList.join(" ");
}
function replaceClassName(el, c1, c2) {
	var i, curList, newList;

    if (el.className == null){
        return;
    }

    newList = new Array();
    curList = el.className.split(" ");
    for (i = 0; i < curList.length; i++){
        if (curList[i] != c1 && curList[i] != c2) {
            newList.push(curList[i]);
        }
    }
    newList.push(c2);
    el.className = newList.join(" ");
}
function getParentElement(el)
{
	if(!el) {return null;}
	var p = el.parentNode;
	while(p && p.nodeType != 1)
	{
		p = p.previousSibling;
	}
	return p;
}
function getFirstElement(el)
{
    if(!el) {return null;}
	var f = el.firstChild;
	while(f && f.nodeType != 1)
	{
		f = f.nextSibling;
	}
	return f;
}
function getPreviousElement(el)
{
    if(!el) {return null;}
	var n = el.previousSibling;
	while(n && n.nodeType != 1)
	{
		n = n.previousSibling;
	}
	return n;
}
function getNextElement(el)
{
    if(!el) {return null;}
	var n = el.nextSibling;
	while(n && n.nodeType != 1)
	{
		n = n.nextSibling;
	}
	return n;
}
function getParentElementByClassName(el, classname)
{
	if(!el) {return null;}
	var parent = getParentElement(el);
	while(parent && !hasClassName(parent, classname))
	{
		parent = getParentElement(parent);
	} 
	return parent;
}
function getParentElementByTagName(el, tagname)
{
	if(!el) {return null;}
	var parent = getParentElement(el);
	tagname = tagname.toUpperCase();
	while(parent && parent.tagName != tagname)
	{
		parent = getParentElement(parent);
	} 
	return parent;
}
function cleanWhitespace(element) {
	if(!element) return;
	for (var i = 0; i < element.childNodes.length; i++) {
		var node = element.childNodes[i];
		if (node.nodeType == 3 && !/\S/.test(node.nodeValue)) 
			element.removeChild(node);
	}
}
function getSize(obj){
    if ((/^(?:body|html)$/i).test(obj.tagName) ){
        return PageSizeHelper.getPageSize();
    }
    return {'w': obj.offsetWidth, 'h':obj.offsetHeight};
}
function getStyle(element, property){
    property = property == 'float' ? 'cssFloat' : property.camelCase();
    var value = element.style[property];
    if (!value || value == 'auto') {
        if (element.currentStyle) {
            value = element.currentStyle[property.camelCase()];
        }
        else{
            var css = document.defaultView.getComputedStyle(element, null);
            value = css ? css[property] : null;
        }
    }
    if (property == 'opacity') return value ? parseFloat(value) : 1.0;
    return value == 'auto' ? null : value;
}

function getRelativePosition(obj, toElement)
{
    var point = new Object();
	
	var Left = obj.offsetLeft;
	var Top  = obj.offsetTop;
	var objParent = obj.offsetParent;
	while (objParent && objParent != toElement && getStyle(objParent, "position") != 'relative')
	{
		Left += objParent.offsetLeft;
		Top  += objParent.offsetTop;
		objParent = objParent.offsetParent;
	}
	point.x = Left;
	point.y = Top;
	return point;
}

function getPosition(obj, toElement)
{
	var point = new Object();
	
	var Left = obj.offsetLeft;
	var Top  = obj.offsetTop;
	var objParent = obj.offsetParent;
	while (objParent && objParent != toElement)
	{
		Left += objParent.offsetLeft;
		Top  += objParent.offsetTop;
		objParent = objParent.offsetParent;
	}
	point.x = Left;
	point.y = Top;
	return point;
}
function _getElementsByName(frm, elementName)
{
	var arr = new Array();
	if(!frm) {
		return arr;
	}
	var elems = frm.elements;
	for(var i=0; elems && i<elems.length; i++)
	{

		if(elems[i].name == elementName)
		{

			arr.push(elems[i]);
		}
	}
	return arr;
}
function getRadioBoxValue(frm, elementName)
{
	var boxes = _getElementsByName(frm, elementName);
	for(var i=0; i<boxes.length; i++)
	{
		if(boxes[i].checked && !boxes[i].disabled){
			return boxes[i].value;
		}
	}
	return "";
}
function Combox_onclick(event, srcDataId, srcCaption, strOnInit, bAllowInput, objThis) 
{
	var inputs = objThis.getElementsByTagName("input");
	if(inputs.length<1) return false;
	if(inputs[1].disabled) return false;
	if(bAllowInput) {
		if (getEventTarget(event) == inputs[1]) {
			return false;
		}
	}
	popupWin(event, srcDataId, srcCaption, objThis);
	if(strOnInit) {
		strOnInit = strOnInit.replace("this", "objThis");
		eval(strOnInit);
	}
}
function getComboxControl(srcDataId, strCaption, strName, strOnInit, bAllowInput, strDefaultText, strDefaultValue)
{
	bAllowInput = bAllowInput ? true : false;
	return "<table id='combo_"+strName+"' class=comboxcontrol onclick=\"Combox_onclick(event, '" + srcDataId + "', '" + strCaption + "'," + (strOnInit ? "'"+strOnInit+"'" : null) + "," + (bAllowInput ? 'true' : 'false') + ", this);return false;\" border=0 cellpadding=0 cellspacing=0><tr>"
			+"<td><input type=hidden name=\""+strName+"_value\" id=\""+strName+"_value\" value=\"" + (strDefaultValue != null ? strDefaultValue : "") + "\"></td>"
			+"<td id=\"cbTdbox_" + strName + "\" class=combox>"
			+"<input name=\""+strName+"_text\" id=\""+strName+"_text\" value=\"" + (strDefaultText != null ? strDefaultText : "") + "\" "+(bAllowInput ? "" : "readonly")+">"
			+"</td>"
			+"<td id=\"cbTdbutton_" + strName + "\" class=cbdbutton>&nbsp;</td>"
			+"</tr></table>";
}			
function stopPropagation(event)
{
	if (Browser.isIE){
		window.event.cancelBubble = true;
	}
	else{		
		event && event.stopPropagation();
	}
}
function preventDefault(event)
{
	if (Browser.isIE){
		window.event.returnValue = false;
	}
	else{
		event && event.preventDefault();
	}
}
function getSrcElement(event) {
	if(window.event) {
		return window.event.srcElement;
	}
	else{
		return event.target;
	}
}
function disableContextMenu(elem) {
	if(!elem.diableContextMenu) {
		elem.oncontextmenu = function(e){
			if(!Browser.isIE) {
				stopPropagation(e);
				preventDefault(e);
			}
			return false;
		};
		elem.diableContextMenu = true;
	}
}
function addEvent(obj, evname, func)
{
    if(document.all) {
        obj.attachEvent("on"+evname, func);       
    }
    else {
        obj.addEventListener(evname, func, false);
    }                
}
function removeEvent(obj, evname, func)
{
    if(document.all) {
        obj.detachEvent("on"+evname, func);       
    }
    else {
        obj.removeEventListener(evname, func, false);
    }                
}
function getEventTarget(event) {
	if(Browser.isIE) {
		return window.event.srcElement;
	}
	else {
		return event.target;
	}
}
function isRightButton(event) {
    var ret = false;
    if(window.event){
        ret = (window.event.button == 2);
    }
    else {
        ret = (event.which == 3);
    }
    return ret;
}
function setCookie(name, value, hours, path, domain, secure) {
  var curCookie = name + "=" + escape(value) +
    ((hours) ? "; expires=" + (new Date((new Date()).getTime()+hours*3600000)).toGMTString() : "") +
    ((path) ? "; path=" + path : "") +
    ((domain) ? "; domain=" + domain : "") +
    ((secure) ? "; secure" : "");
  document.cookie = curCookie;
}

function getCookie(name) {
  var dc = "; " + document.cookie;
  var prefix = "; "+name + "=";
  var begin = dc.indexOf(prefix);
  if (begin == -1) {
    return null;
  }
  else {
	begin += prefix.length;
  }
  var end = dc.indexOf(";", begin);
  if (end == -1) {
	end = dc.length;
  }
  return unescape(dc.substring(begin, end));
}

function isNumeric(strNum){
	return /^[+-]?(\d*\.?\d+|\d+\.?\d*)([eE][+-]?\d+)?$/.test(strNum);
}
function isInt(strNum) {
	return /^[\+|\-]?[0-9]+$/.test(strNum);
}

function clearOptions(sel){
	sel.options.length = 0;
}
function appendOptions(sel, strOptions){
	var o = sel.options;
    var opts = strOptions.trim().split(";");
	for(var i=0; i<opts.length; i++){
		var arrTmp = opts[i].split(":");
        var text, key;
        if(arrTmp.length > 1){
            key = arrTmp[0];
            text = arrTmp[1];
        }
        else{
            key = text = arrTmp[0];
        }
        var op = new Option(text, key);
        o.add(op);
	}
}

var popWin = null;
var popWins =[];
function closePopWin()
{
	if(popWin && !popWin.closed) {popWin.close(); popWin = null;};
}
function openWin(url, winname, feature)
{
	if(typeof(feature) == 'undefined')
	{
        var feature='width='+(document.body.clientWidth-20)+',height='+(document.body.clientHeight-20)+',scrollbars=1,resizable=1';
	}
    var idx1 = -1;
	var idx2 = -1;
	var w = screen.avialWidth;
	var h = screen.availHeight;
	idx1 = feature.indexOf('width=');
	if(idx1 != -1)
	{
		idx2 = feature.indexOf(',', idx1);
		idx2 = (idx2 == -1) ? feature.length : idx2;
		w = parseInt(feature.substring(idx1+6, idx2));
	}
	idx1 = feature.indexOf('height=');
	if(idx1 != -1)
	{
		idx2 = feature.indexOf(',', idx1);
		idx2 = (idx2 == -1) ? feature.length : idx2;
		h = parseInt(feature.substring(idx1+7, idx2));
	}
	var top  = parseInt((screen.availHeight-h)/2);
	var left = parseInt((screen.availWidth -w)/2);
	feature += ',top='+top+',left='+left;
	var win = null;
    try{
	    closePopWin();
	    win=window.open(url, winname, feature);
	    win.opener = self;
	    win.focus();
		popWins[winname] = win;
    }
    catch(e) {
	    alert("The pop-up is blocked. Please allow this pop-up as to continue the processing.");
	    throw "pop-up-blocked";
    }
	return win;
}

function openWindow(url, winname, feature){
    var win = null;
    var win = popWins[winname];
    if(win!=null && win!=undefined && win.closed==false){
       win.location = url;
       win.focus();
    }
    else{
        if(typeof(feature) == 'undefined')
	    {
            var feature="width="+(screen.availWidth*0.80)+",height="+(screen.availHeight*0.80)+",scrollbars=1,resizable=1";
	    }
        openWin(url,winname,feature);
    }
}

var helpList = {
	link: "blank.htm",
	window_open: function(){
		var sFeatures="resizable=1,scrollbars=1,status=1,width=600,height=600";
		openWindow("help/"+this.link+"","THC_Help_report",sFeatures);
	},
	openHelp: function(link){
	    this.link = link;
	    this.window_open();
	}
};

// to mm/dd/yy hh:mm:ss
function thc_getMatchTimeArr(strDate)
{
	var reg_cn = /^(\d{1,4})(-|\/)(\d{1,2})\2(\d{1,2}) (\d{1,2}):(\d{1,2}):(\d{1,2})( AM| PM)?$/;
	var reg_en = /^(\d{1,2})(-|\/)(\d{1,2})\2(\d{1,4}) (\d{1,2}):(\d{1,2}):(\d{1,2})( AM| PM)?$/;
	
	var r = null;
	r = strDate.match(reg_cn);
	if (r != null) {
		var y = r[1];
		var m = r[3];
		r[1] = m;
		r[3] = r[4];
		r[4] = y;
	}
	else {
		r = strDate.match(reg_en);
	}
	return r;	
}
function thc_displayTime(strDate,type)
{
	if (strDate == null) {return "";}
	if (strDate == "" ) {return "";}
	var r = thc_getMatchTimeArr(strDate);
	if ( r== null) {
		return strDate;
	}
	
	if (r[8] == " AM" ) {
	    var hh = parseInt(r[5], 10);
		if (  (!isNaN(hh)) && hh == 12 ) {
			r[5] = 0;
		}
	}
	if (r[8] == " PM" ) {
		var hh = parseInt(r[5], 10);
		if (  (!isNaN(hh)) && hh < 12 ) {
			r[5] = hh + 12;
		}
	}
	if(type==1) //short date
	{
		return String.format("{{0}}\/{{1}}\/{{2}} ", r[1], r[3], r[4]);
	}
	else
	{
		return String.format("{{0}}\/{{1}}\/{{2}} {{3}}:{{4}}:{{5}}", r[1], r[3], r[4].substr(2), r[5], r[6], r[7]);
	}
}
function thc_displayCycleFromDate(strDate)
{
	if (strDate == null) {return "";}
	if (strDate == "" ) {return "";}
	var r = thc_getMatchTimeArr(strDate);

	if ( r == null) {
		return strDate;
	}
	return String.format("{{0}}{{1}}", r[4], ("0" + r[1]).Right(2) );
}
function secToStdFormat(sec)
{
	var sInvalid = "00:00:00";
	if (!sec) return sInvalid;
	
	sec = parseInt(sec);
	if( isNaN(sec) ) return sInvalid;

	var arr = [];
	var t = "00";
	
	t = Math.floor(sec / 3600).toString();
	if(t.length == 1)	t = "0" + t;
	arr[0] = t;
	
	sec = sec % 3600;
	t = ("0" + Math.floor( sec / 60).toString()).Right(2);
	arr[1] = t;	
	
	t = ("0" + (sec % 60).toString()).Right(2);
	arr[2] = t;
	
	return arr.join(":");
}
function DrawSelectionHour(){
	var arr=[];
	var d = new Date();
	var t_option = '<option value="{{0}}" {{2}}>{{1}}</option>';
	for(var i=0; i<24; ++i){
		var h = ("0"+i).Right(2);
		arr.push(String.format(t_option, i, h, i==d.getHours() ? 'selected': ''));
	}
	document.write(arr.join(""));
}
function DrawSelectionMinute(){
	var arr=[];
	var d = new Date();
	var t_option = '<option value="{{0}}" {{2}}>{{1}}</option>';
	for(var i=0; i<60; ++i){
		var m = ("0"+i).Right(2);
		arr.push(String.format(t_option, i, m, i==d.getMinutes() ? 'selected': '') );
	}
	document.write(arr.join(""));
}
function UpdateSelectionTime(id, hOrM)
{
    var obj = $(id);
    if (!obj) return;
    
    var d = new Date();
    var t;
    if (hOrM == "h"){
        t = d.getHours();
    }
    else{
        t = d.getMinutes();
    }
	
	obj.selectedIndex = -1;
	var options = obj.options;
	for(var i=0; i<options.length; ++i){
		if(options[i].value == t){
		    options[i].selected = true;
		    break;
		}
	}
}

// for tabs
var Tabs = {
    activeTab: [],
    synch: function(tabAreaId, idx){
        var tabs = this.getAll(tabAreaId);
        if (tabs.length < 1 || idx > tabs.length) return;
        
        tab = tabs[idx];
	    tab.firstChild.blur();
	    
	    var activeIdx = this.activeTab[tabAreaId];
	    if(idx == activeIdx)
		    return false;

	    if(activeIdx == null){
		    for(var i=0; i<tabs.length; i++){
			    removeClassName(tabs[i], "activeTab");
		    }
	    }
	    else {
		    removeClassName(tabs[activeIdx], "activeTab");
	    }
	    tab.className += " activeTab";
	    this.activeTab[tabAreaId] = idx;
	    return true;
    },
    synchTab: function(oTab){
        var p = getParentElementByTagName(oTab, "div");
        var tabs = this.getAll(p.id);
        return this.synch(p.id, indexOf(tabs, oTab));
    },
    getAll: function(tabAreaId){
        var oTabArea = $(tabAreaId);
        if (!oTabArea) return null;
        
        return oTabArea.getElementsByTagName("SPAN") || [];
    },
    get: function(tabAreaId, idx){
	    return this.getAll(tabAreaId)[idx];
    },
    getActive: function(tabAreaId){
        if(this.activeTab[tabAreaId] == null) return null;
        return this.get(tabAreaId, this.activeTab[tabAreaId]);
    },
    disable: function(tabAreaId) {
	    var tabArea = $(tabAreaId);
	    if(!tabArea) return;
	    if(tabArea.enabled && tabArea.enabled == false) return;
	    var tabs = tabArea.getElementsByTagName("SPAN");
	    for(var i=0; i<tabs.length; i++)
	    {
		    var link = tabs[i].firstChild;
		    link.oldclick = link.onclick;
		    link.onclick = null;
		    link.disabled = true;
	    }
	    tabArea.enabled = false;
    },
    enable: function(tabAreaId) {
	    var tabArea = $(tabAreaId);
	    if(!tabArea) return;
	    if( tabArea.enabled != null && tabArea.enabled == false ) {
		    var tabs = tabArea.getElementsByTagName("SPAN");
		    for(var i=0; i<tabs.length; i++)
		    {
			    var link = tabs[i].firstChild;
			    link.onclick = link.oldclick;
			    link.oldclick = null;
			    link.disabled = false;
		    }
		    tabArea.enabled = true;
	    }
    }
};
// end tabs

function changeAdvancedSettingStatus(link)
{
	if(hasClassName(link, "expanded"))
	{
		removeClassName(link, "expanded");
		addClassName(link, "collapsed");
		addClassName(getNextElement(getParentElement(link)), "display_none");
	}
	else
	{
		removeClassName(link, "collapsed");
		addClassName(link, "expanded");
		removeClassName(getNextElement(getParentElement(link)), "display_none");
	}
}

var PageSizeHelper = {
    getRoot: function(){
        return (document.compatMode && document.compatMode == "CSS1Compat") ? document.documentElement: document.body;
    },
    getViewportSize: function(){
        var w, h;
        if (window.innerWidth!=window.undefined) {
            w = window.innerWidth;
            h = window.innerHeight;
        }
        else {
            var root = this.getRoot();
            w = root.clientWidth;
            h = root.clientHeight;
        }
        return {'w': w, 'h': h};
    },
    getPageSize: function(){
        var root = this.getRoot();

        var w = root.clientWidth;
        w = Math.max(w, root.offsetWidth);
        w = Math.max(w, root.scrollWidth);
        
        var h = root.clientHeight;
        h = Math.max(h, root.offsetHeight);
        h = Math.max(h, root.scrollHeight);
        return {'w': w, 'h': h};
    },
    getScrollXY: function(){
        var x, y;
        if (window.scrollX != undefined) {
			x = window.scrollX;
			y = window.scrollY;
		}
		else {
			x = document.documentElement.scrollLeft + document.body.scrollLeft;
			y = document.documentElement.scrollTop + document.body.scrollTop;
		}
		return {'x': x, 'y': y};
	},
	getBetterX: function (x0, x1, x2, x3, w)
	{	
		if (x1 < x0) x1 = x0;
		if (x2 > x3) x2 = x3;
		
		if (x1 + w <= x3) return x1;
		else if (x2 - w >= x0) return x2 - w;
		else return (x3 - w - x1) < (x0 + w - x2) ? x3 - w : x0;
	},
	getBetterY: function (y0, y1, y2, y3, h)
	{
		if (y1 < y0) y1 = y0;
		if (y2 > y3) y2 = y3;
		
		if (y2 + h <= y3) return y2;
		else if (y1 - h >= y0) return y1 - h;
		else return (y3 - h - y2) < (y0 + h - y1) ? y3 - h : y0;
	},
	ajustPopupPosition: function (event, popupW, popupH, dest)
	{
		var x, y, x0, y0, clientWidth, clientHeight;
		
        var size = this.getViewportSize();
		clientWidth = size.w;
		clientHeight = size.h;
		
		var scroll = this.getScrollXY();
		x0 = scroll.x;
		y0 = scroll.y;
		
		x = y = 0;
	    
		if(dest)
		{
			var pos = getPosition(dest);
			var offsetHeight = dest.offsetHeight;
			var offsetWidth = dest.offsetWidth;
	                
			x = this.getBetterX(x0, pos.x, pos.x+offsetWidth, x0+clientWidth, popupW);
			y = this.getBetterY(y0, pos.y, pos.y+offsetHeight, y0+clientHeight, popupH);
		}
		else
		{
			if (window.event) {
				x = window.event.clientX;
				y = window.event.clientY;
			}
			else {
				x = event.clientX;
				y = event.clientY;
			}
            x += x0;
            y += y0;
			x = this.getBetterX(x0, x, x, x0+clientWidth, popupW);
			y = this.getBetterY(y0, y, y, y0+clientHeight, popupH);
		}
		return {"x": x, "y": y};
	}
};
// resize client area
(function(){
var timer=null;

function afterResize()
{
	var obj = document.getElementById("clientArea");
	if(!obj) return;

	var pos = getPosition(obj);
	var size = PageSizeHelper.getViewportSize();
	var h = size.h - pos.y - 5;
	var w = size.w - pos.x - 5;
	if (w < 100)
	{
		w = 300;
	}
	if (h < 100)
	{
		h = 300;
	}
	obj.style.height = h + "px";
	obj.style.width  = w + "px";
	if(timer != null){
		clearTimeout(timer);
		timer = null;
	}
}
function resizeClientArea()
{
	var obj = $("clientArea");
	if(!obj) return;
	
	if(timer != null){
		clearTimeout(timer);
	}
    timer = setTimeout(afterResize, 500);
}
addEvent(window, "load", resizeClientArea);	
addEvent(window, "resize", resizeClientArea);
})();
// end resize client area


// for mask
var PopupMask = {
    show: function(elm, className){
        var maskId = "popupMask-" + elm.tagName + "-" + elm.id;
        var mask = document.getElementById(maskId);
        if(!mask)
        {
            mask = document.createElement("DIV");
            mask.id = maskId;
            mask.className = className;
            
            if(elm.parentNode && elm.parentNode.nodeType == 1){
                elm.parentNode.appendChild(mask);
            }
            else{
                document.body.appendChild(mask);
            }
        }
        
        var pos = getRelativePosition(elm);
        var style = mask.style;
        style.left = pos.x + "px";
        style.top = pos.y + "px";
        
        var size = getSize(elm);
        style.width = size.w + "px";
        style.height = size.h + "px";

        style.display = "block";
        
        return maskId;
    },
    showOn: function(elm, arrExcludes){
        elm = elm || PageSizeHelper.getRoot();
        this.hideSelectionIn(elm, arrExcludes);
        return this.show(elm, 'popupMask');
    },
    showMessageOn: function(elm, msg){
        var maskId = this.show(elm, 'popupMask');
        var maskId2 = this.show($(maskId), 'popupMask2');
        $(maskId2).innerHTML = msg;
    },
    showOff: function(elm){
        elm = elm || PageSizeHelper.getRoot();
        var maskId = "popupMask-" + elm.tagName + "-" + elm.id;
        var mask = $(maskId);
        
        if(!mask) return;
        this.showSelectionIn(elm);
        mask.style.display = "none";
    },
    hideSelectionIn: function(elm, arrExcludes){
        if (Browser.isIE && Browser.version < 7){
            var ol = elm.getElementsByTagName("select");
            for(var i=0; i<ol.length; i++) {
                if(arrExcludes && (arrExcludes.indexOf(ol[i].id) != -1) ) {
			        // not hide
                }else {
			        this.hideElem(ol[i]);
                }
            }
        }
    },
    showSelectionIn: function(elm){
        if (Browser.isIE && Browser.version < 7){
            var ol = elm.getElementsByTagName("select");
            for(var i=0; i<ol.length; i++) {
                this.showElem(ol[i]);
            }
        }
    },
    showElem: function(elm){
       elm && elm.h && (elm.style.visibility="") && (elm.h=false);
    },
    hideElem: function(elm){
        elm && (elm.style.visibility="hidden") && (elm.h=true);
    }
};

function showMask(arrExcludes)
{
    return PopupMask.showOn(null, arrExcludes);
}
function hideMask()
{
	PopupMask.showOff(null);
}
// end mask 

var PageSetting = {
	instance: "",
	expires: 24*30,
	toString: function(){
		return "";
	},
	load: function() {
		var obj = null;
		if(this.instance == "") return obj;
		var c = getCookie(this.instance);
		if(c) {
			try {
				eval("obj="+c+"");
			}catch (e){
			}
		}
		return obj;
	},
	save: function() {
		if(this.instance == "") return;
		setCookie(this.instance, this.toString(), this.expires);
	}
};
var sequence_no = {
	instance: "sequenceno",
	expires: 24,
	date: new Date(),
	port_no: 1,
	report_no: 1,
	is_init: false,
	toString: function() {
		return "{date:["+this.date.getFullYear()+","+(this.date.getMonth()+1)+","+this.date.getDate()+"],port_no:"+this.port_no+",report_no:"+this.report_no+"}";
	},
	init: function() {		
		var obj = this.load();
		if(obj) {
			obj.date = new Date(obj.date[0],obj.date[1]-1,obj.date[2]);
			this.date = obj.date;
			this.port_no = parseInt(obj.port_no);
			this.report_no = parseInt(obj.report_no);
			if(this.port_no == NaN) this.port_no = 1;
			if(this.report_no == NaN) this.report_no = 1;
			obj = null;
		}
		this.is_init = true;
	},
	getDate:function(digitOfYear) {
		if(!this.is_init) this.init();
		return ""+(""+this.date.getFullYear()).Right(digitOfYear?digitOfYear:4)
			+ ("0"+(this.date.getMonth()+1)).Right(2)
			+ ("0"+this.date.getDate()).Right(2);
	},
	getPortNo:function(){
		if(!this.is_init) {this.init();}
		return this.port_no; 
	},
	getReportNo:function(){
		if(!this.is_init) this.init();
		return this.report_no;
	},
	increaseReportNo:function(){
		++this.report_no;
		this.save();
	}
};
Object.extend(sequence_no, PageSetting);
var CreditRating = {
	arrRating: ["Others","AAA","AA+","AA","AA-","A+","A","A-","BBB+","BBB","BBB-","BB+","BB","BB-","B+","B","B-","CCC"," Unrated"],
	getDescription: function(rating) {
		var r = parseInt(rating, 10);
		if (isNaN(r) || r < 0 || r > 18) {
			return "Unknown";
		}
		return this.arrRating[r];
	}
};
(function(){
var id="__div_page_loading__";
function __showLoading__() {try{document.write("<div id='"+id+"' style='position:absolute;left:0;top:0;'><p><br /><span style='background-color:#ffffff;font-weight:bold;'>Loading, please wait...</span></p></div>");}catch(e){}}
function __hideLoading__() {try{var o=$(id);o.parentNode.removeChild(o);delete o;}catch(e){}finally{removeEvent(window, "load", __hideLoading__);}}
__showLoading__();addEvent(window, "load", __hideLoading__);
})();
var uniqueID = (function() {
    var id = (new Date()).getTime();
    return function() { return id++; };
})();
var jssession = getCookie("jssession");
if(jssession == null) {
	jssession = uniqueID();
	setCookie("jssession", jssession);
};
var IsPageExpired =(function() {
    var clientid = getCookie("CLIENTID");
    return function(){
        return getCookie("CLIENTID")!=clientid;
    };
})();
var PageControl={
	tab: "",
	instance: "",
	isRefresh:0,
	InitTab: function(tab){
		switch(tab)
		{
			case "x":
				this.instance = "PortfolioListPaging";
				break;
			case "s":
			case "c":
				this.instance = "StrategyPortfolioListPaging";
				break;
			case "b":
				this.instance = "BenchmarkListPaging";
				break;
			case "ts":
				this.instance = "ScheduleListPaging";
				break;
			default:
				break;
		}
	},
	PageRefresh: function(tab){
		if(!this.isRefresh) return;
		
		if(tab != "") {
			this.InitTab(tab);
			if(tab=="c")
			{
				SelectActiveFrame(1);
			}
			eval("var obj="+this.instance);
			obj.load();
		}
		else{ // from upload
			var frames = window.parent.frames;
			frames[0].location.replace("PortfolioUpload.asp");
			
			var pwin = frames[1];
			pwin.document.getElementById('selLstCatalog').value=0;
			pwin.PortfolioPaging.searchPattern=0;
			pwin.PortfolioPaging.update();
		}
	},
	PageBack: function(){					
		var openerObj = window.opener.PageControl;
		openerObj.isRefresh = this.isRefresh;
		openerObj.PageRefresh(this.tab); 
	}	
};
function getIframeDocument(frameId)
{
    if(document.all)
    {
        return document.frames[frameId].document;
    }
    else
    {
        return document.getElementById(frameId).contentWindow.document;
    }
}
function getBlockStyle(){
    if (Browser.isIE) {
        return "block";
    }
    else{
        return "table-row";
    }
}
function setDisplay(elem, bShow){
    if(!elem) return;
    if(bShow){
        if (Browser.isIE){
            elem.style.display = bShow ? "block" : "none";
        }
        else{
            var s = "";
            switch(elem.tagName.toUpperCase()){
            case "TR":
                s = "table-row";
                break;
            case "TABLE":
                s = "table";
                break;
            default:
                s = "block";
                break;
            }
            elem.style.display = s;
        }
    }
    else{
        elem.style.display = "none";    
    }
}
function showWarningMessage(objMsg) {
    var id = "div_warning_container";
    var div = $(id);
    if (div == null){
        div = document.createElement("DIV");
        div.className = "warningarea warningtext";
        div.innerHTML = "<div class='warningclose' onclick=\"this.parentNode.style.display='none'\">X</div><div></div>";
        div.onmouseover = function(){
            this.className = "warningarea highlight";
        };
        div.onmouseout = function(){
            this.className = "warningarea warningtext";
        };
        document.body.insertBefore(div, null);
        div = div.childNodes[1];
        div.id = id;
        
    }
    div.innerHTML = objMsg.innerHTML;
}
var Subject = {};
(function(){
var _observers = [];
Subject.prototype = {
	indexOf: function(observer) {
		for(var i=0; i<_observers.length; ++i) {
			if(_observers[i] == observer) {
				return i;
			}
		}
		return -1;
	},
	attachObserver: function(observer) {
		if(this.indexOf(observer) == -1) {
			_observers.push(observer);
		}
	},
	detachObserver: function(observer) {
		var idx = this.indexOf(observer);
		if( idx != -1) {
			_observers.splice(idx, 1);
		}
	},
	notify: function(context) {
		for(var i=0; i<_observers.length; ++i) {
			if(_observers[i].updateUI) _observers[i].updateUI(context);
		}
	}
};})();
var Observer = {};
Observer.prototype = {
	updateUI: function(context){return;}
};