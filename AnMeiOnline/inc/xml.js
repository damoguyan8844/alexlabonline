var XML;
XML = {};
XML.NAME = "XML";     
XML.VERSION = 1.0;    


XML.newDocument = function(rootTagName, namespaceURL) {
    if (!rootTagName) rootTagName = "";
    if (!namespaceURL) namespaceURL = "";
    
    if (document.implementation && document.implementation.createDocument) {
        return document.implementation.createDocument(namespaceURL,
                                                      rootTagName, null);
    }
    else {
		MS_PROGIDS = new Array(
			"MSXML2.DOMDocument.7.0",
			"MSXML2.DOMDocument.6.0",
			"MSXML2.DOMDocument.5.0",
			"MSXML2.DOMDocument.4.0",
			"MSXML2.DOMDocument.3.0",
			"MSXML2.DOMDocument"
		);
		var doc = null;
		for (var i = 0; i < MS_PROGIDS.length && !doc; i++)
		{
			try
			{
				doc = new ActiveXObject(MS_PROGIDS[i]);
				//alert(MS_PROGIDS[i]);
			}
			catch (ex)
			{}
		}
		if (doc == null ){
			alert("Cannot create MSXML2.DOMDocument component.");
			return;
		}

        if (rootTagName) {
            // Look for a namespace prefix
            var prefix = "";
            var tagname = rootTagName;
            var p = rootTagName.indexOf(':');
            if (p != -1) {
                prefix = rootTagName.substring(0, p);
                tagname = rootTagName.substring(p+1);
            }

            if (namespaceURL) {
                if (!prefix) prefix = "a0"; // What Firefox uses
            }
            else prefix = "";

            var text = "<" + (prefix?(prefix+":"):"") +  tagname +
                (namespaceURL
                 ?(" xmlns:" + prefix + '="' + namespaceURL +'"')
                 :"") +
                "/>";
            doc.loadXML(text);
        }
        return doc;
    }
};

XML.load = function(url) {
    // Create a new document the previously defined function
    var xmldoc = XML.newDocument();  
    xmldoc.async = false;  // We want to load synchronously
    xmldoc.load(url);      // Load and parse
    return xmldoc;         // Return the document
};

XML.loadAsync = function(url, callback) {
    var xmldoc = XML.newDocument();

    if (document.implementation && document.implementation.createDocument) {
        xmldoc.onload = function() { callback(xmldoc); };
    }
    else {
        xmldoc.onreadystatechange = function() {
            if (xmldoc.readyState == 4) callback(xmldoc);
        };
    }

    xmldoc.load(url);
};

XML.parse = function(text) {
    if (typeof DOMParser != "undefined") {
		//text = text.replace(/(^\s*)|(\s*$)/g, "");
		//text = text.replace(/>[\n\s\t\r]+</g, "><");
		//text = text.replace(/\/>/g, " />");
        return (new DOMParser()).parseFromString(text, "application/xml");
    }
    else if (typeof ActiveXObject != "undefined") {
        var doc = XML.newDocument();  // Create an empty document
        doc.async = false;            // We want to load synchronously
        doc.loadXML(text);            // Parse text into it
        return doc;                   // Return it
    }
    else {
        var url = "data:text/xml;charset=utf-8," + encodeURIComponent(text);
        var request = new XMLHttpRequest();
        request.open("GET", url, false);
        request.send(null);
        return request.responseXML;
    }
};

XML.getDataIsland = function(id) {
    var doc;

    doc = XML.getDataIsland.cache[id];
    if (doc) return doc;
    
    // Look up the specified element
    doc = document.getElementById(id);

    // If there is a "src" attribute, fetch the Document from that URL
    var url = doc.getAttribute('src');
    if (url) {
        doc = XML.load(url);
    }
    else if (!doc.documentElement) {// If this is not already a document...
        var docelt = doc.firstChild;
        while(docelt != null) {
            if (docelt.nodeType == 1 /*Node.ELEMENT_NODE*/) break;
            docelt = docelt.nextSibling;
        }
        
        // Create an empty document
        doc = XML.newDocument();
        
        // If the <xml> node had some content, import it into the new document
        if (docelt) doc.appendChild(doc.importNode(docelt, true));
    }

    // Now cache and return the document.
    XML.getDataIsland.cache[id] = doc;
    return doc;
};

XML.getDataIsland.cache = {}; // Initialize the cache

if(document.implementation&&document.implementation.createDocument)
{
	Node.prototype.__defineGetter__("xml",function()
	{
		return new XMLSerializer().serializeToString(this);
	});
	Node.prototype.__defineGetter__("text", function()
	{
		var iChild = 0;
		var strReturn = "";
		while (iChild < this.childNodes.length)
		{
			if (this.childNodes[iChild].nodeType == 3)
			{
				strReturn += this.childNodes[iChild].nodeValue;
			}
			iChild++;
		}
		return strReturn.trim();
	});	
		

	XMLDocument.prototype.selectNodes=function(sExpr,contextNode)
	{
		var oResult=this.evaluate(sExpr,(contextNode?contextNode:this),this.createNSResolver(this.documentElement),XPathResult.ORDERED_NODE_SNAPSHOT_TYPE,null);
		var nodeList=new Array(oResult.snapshotLength);
		nodeList.expr=sExpr;
		for(i=0;i<nodeList.length;i++)
			nodeList[i]=oResult.snapshotItem(i);
		return nodeList;
	};
	Element.prototype.selectNodes=function(sExpr)
	{
		var doc=this.ownerDocument;
		if(doc.selectNodes)
			return doc.selectNodes(sExpr,this);
		else
            throw "Method selectNodes is only supported by XML Nodes";
	}
	;
	XMLDocument.prototype.selectSingleNode=function(sExpr,contextNode)
	{
		var ctx=contextNode?contextNode:null;
		sExpr+="[1]";
		var nodeList=this.selectNodes(sExpr,ctx);
		if(nodeList.length>0)
			return nodeList[0];
		else
            return null;
	};
	Element.prototype.selectSingleNode=function(sExpr)
	{
		var doc=this.ownerDocument;
		if(doc.selectSingleNode)
			return doc.selectSingleNode(sExpr,this);
		else
            throw "Method selectSingleNode is only supported by XML Nodes";
	};
}