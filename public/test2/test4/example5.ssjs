
(function() {
	var exports = {};

	var example = require("../../example2.ssjs");
	exports.secret = "<br>" + example.user.name + CURRENT_DIRECTORY;

	return exports;
})();
