(function() {
	var name = POST.name;
	var redirect = POST.redirect;

	html = "Hello, " + name;

	if (redirect) {
		HEADERS.push("Location: http://apple.com/");
	} else if (name == "Ryan") {
		HEADERS.push("HTTP/1.1 301 Moved Permanently");
		HEADERS.push("Location: http://ryanashcraft.me/");
	}

	return html;
})();