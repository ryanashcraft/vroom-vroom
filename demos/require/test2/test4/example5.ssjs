
(function() {
	var exports = {};

	var example = require("../../example2.ssjs");
	exports.secret = "<br>" + example.user.name + __CURRENT_DIRECTORY__;

	return exports;
})();
