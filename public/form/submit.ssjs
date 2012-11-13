(function() {
	var name = POST.name;
	var redirect = POST.redirect;

	html = "Hello, " + name;

	if (redirect) {
		HEADERS.push("Location: http://apple.com/");
	}

	return html;
})();