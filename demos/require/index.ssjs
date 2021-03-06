
(function() {
	var doc = {
		'!DOCTYPE': {
			attr: [
				"html"
			],
			single: true
		},
		'html': {
			'head': {
				'title': 'Oh hai!',
				'link': {
					attr: [
						{'href':'../media/style.css'},
						{'rel':'stylesheet'}
					],
					single: true
				},
				'script': {
					attr: [
						{'src':'../media/main.js'}
					]
				}
			},
			'body': {
				attr: [
					{'class':'foo'}
				],
				'p': {
					attr: [
						{'class':'modal'}
					],
					inner: function() {
						var included = require("example2.ssjs");
						var str = "";
						for (var m in included.user) {
							str += included.user[m] + "<br>";
						}
						str += included.private_func + "<br>";
						str += included.reverse_string("foo fighters") + "<br>";
						str += __CURRENT_DIRECTORY__ + "<br>";

						str += __POST__.filename + "<br>";
						str += __POST__.greeting + "<br>";

						str += __GET__.error + "<br>";
						str += __GET__.message + "<br>";

						return str;
					}
				}
			}
		}
	}

	return json2xml(doc);
})();

function json2xml(o) {
	var toXml = function(v, name, ind) {
		var xml = "";

		if (typeof(v) == "object") {
			var hasChild = false;
			var single = false;

			xml += ind + "<" + name;

			for (var m in v) {
				if (m.charAt(0) == "@")
					xml += " " + m.substr(1) + "=\"" + v[m].toString() + "\"";
				else
					hasChild = true;
			}

			if (v.single) {
				single = true;
			}

			if (v.attr) {
				for (var a in v.attr) {
					if (typeof(v.attr[a]) == "string") {
						xml += " " + v.attr[a];
					} else if (typeof(v.attr[a]) == "object") {
						for (var e in v.attr[a]) {
							xml += " " + e + "=\"" + v.attr[a][e] + "\"";
						}
					}
				}
			}

			xml += hasChild ? ">" : "/>";

			if (hasChild && !single) {
				for (var m in v) {
					if (m == "inner") {
						if (typeof(v[m]) == "function") {
							xml += v[m]();
						} else {
							xml += v[m];
						}
					} else if (m == "#text")
						xml += v[m];
					else if (m == "#cdata")
						xml += "<![CDATA[" + v[m] + "]]>";
					else if (m.charAt(0) != "@"
						&& m != "attr")
						xml += toXml(v[m], m, ind + "\t");
				}

				xml += (xml.lastIndexOf("/") > xml.lastIndexOf(name) ? ind : "")
				+ "</" + name + ">";
			}
		} else {
			xml += ind + "<" + name + ">" + v.toString() +  "</" + name + ">";
		}

		return xml;
	}, xml="";
   
	for (var m in o) {
		xml += toXml(o[m], m, "\n");
	}
	
	return xml;
}
