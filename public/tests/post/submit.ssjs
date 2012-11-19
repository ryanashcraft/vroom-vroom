(function() {
	var name = __POST__.name;
	var redirect = __POST__.redirect;

	html = "Hello, " + name;

	if (redirect) {
		__HEADERS__.push("Location: http://apple.com/");
	} else if (name == "Ryan") {
		__HEADERS__.push("HTTP/1.1 301 Moved Permanently");
		__HEADERS__.push("Location: http://ryanashcraft.me/");
	}

	return html;
})();