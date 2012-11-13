
#include <iostream>
#include <sstream>

#include "http_response.h"
#include "date.h"

using namespace std;

HTTPResponse::HTTPResponse(string status, string& mime, string &content, vector<string>& headers, bool head) :
	status_(status),
	mime_(mime),
	content_(content),
	headers_(headers),
	head_(head)
{

}

HTTPResponse::HTTPResponse(string status, bool head) :
	status_(status),
	head_(head)
{

}

string HTTPResponse::str() {
	stringstream response;
	response << status_ << endl;
	response << "Date: " << Date::now("%a, %d %b %Y %H:%M:%S %Z") << endl;
	response << "Accept-Ranges: bytes" << endl;
	response << "Content-Length: " << content_.length() << endl;
	response << "Connection: close" << endl;
	response << "Content-Type: " << mime_ << endl;

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
