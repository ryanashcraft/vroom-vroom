
(function() {
	var exports = {};

	exports.user = {
		name: "Ryan Ashcraft",
		age: 20,
		dob: "February 21, 1992"
	};

	var private_func = function() {
		var secrets = require("test2/index.ssjs");
		var str = "";

		for (var m in secrets) {
			str += secrets[m].secret + " ";
		}

		return str;
	}

	exports.reverse_string = function(str) {
		var rev = function(b, s) {
			if (s.length == 0) {
				return b;
			}

			return b + rev(s.substr(s.length - 1), s.substr(0, s.length - 1));
		}

		return __CURRENT_DIRECTORY__ + private_func() + " " + rev(str.substr(str.length - 1), str.substr(0, str.length - 1));
	}

	return exports;
})();
