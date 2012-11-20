(function() {
	function request(key) {
		var val = __POST__[key];
		if (val) {
			return val;
		} else {
			return __GET__[key];
		}
	}

	var name = request('name');

	if (name) {
		return "Hello, " + name + "!";
	} else {
		return "Hello, World!";
	}
})();