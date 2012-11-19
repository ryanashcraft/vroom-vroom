
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <cstdlib>

#include "http_response.h"
#include "date.h"
#include "regex_matcher.h"

using namespace std;
using namespace vv;

static unordered_map<int, string> code_to_status_map({
	{100, "HTTP/1.1 100 Continue"},
	{300, "HTTP/1.1 300 OK"},
	{302, "HTTP/1.1 302 Found"},
	{400, "HTTP/1.1 400 Bad Request"},
	{404, "HTTP/1.1 404 Not Found"},
	{500, "HTTP/1.1 500 Internal Server Error"},
	{502, "HTTP/1.0 502 Not Implemented"}
});

HTTPResponse::HTTPResponse(int code, string& mime, string &content, vector<string>& headers, bool head) :
	code_(code),
	status_(code_to_status_map[code]),
	mime_(mime),
	content_(content),
	headers_(headers),
	head_(head)
{

}

HTTPResponse::HTTPResponse(string& status, string& mime, string &content, vector<string>& headers, bool head) :
	code_(status_to_code(status)),
	status_(status),
	mime_(mime),
	content_(content),
	headers_(headers),
	head_(head)
{

}

HTTPResponse::HTTPResponse(int code, bool head) :
	code_(code),
	status_(code_to_status_map[code]),
	head_(head)
{

}

int HTTPResponse::status_to_code(string& status) {
	RegexMatcher code_regex("HTTP/1.1 ([0-9]+)");
	vector<string> code_matches(code_regex.find_matches(status));

	if (code_matches.size() <= 1) {
		return -1;
	}

	return atoi(code_matches[1].c_str());
}

string HTTPResponse::str() {
	stringstream response;
	response << status_ << endl;
	response << "Date: " << Date::now("%a, %d %b %Y %H:%M:%S %Z") << endl;
	response << "Accept-Ranges: bytes" << endl;
	response << "Content-Length: " << content_.length() << endl;

	if (close_) {
		response << "Connection: close" << endl;
	}
	
	if (mime_ != "") {
		response << "Content-Type: " << mime_ << endl;
	}

	for (string& s : headers_) {
		response << s << endl;
	}

	// Empty line for separation of header and content
	response << endl;

	if (!head_) {
		response << content_ << endl;
	}

	return response.str();
}
