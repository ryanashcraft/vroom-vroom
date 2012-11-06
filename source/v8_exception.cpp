
#include <v8.h>

#include <iostream>
#include <sstream>
#include <fstream>

#include "v8_exception.h"

using namespace std;

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

V8Exception::V8Exception(v8::TryCatch* try_catch, const string& path) {
	// http://v8.googlecode.com/svn/trunk/samples/shell.cc
	stringstream str;

	v8::HandleScope handle_scope;
	v8::String::Utf8Value exception(try_catch->Exception());
	const char* exception_string = ToCString(exception);
	v8::Handle<v8::Message> message = try_catch->Message();
	if (message.IsEmpty()) {
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		str << exception_string << endl;
	} else {
		// Print (filename):(line number): (message).
		int linenum = message->GetLineNumber();

		str << path << ":" << linenum << ": " << exception_string << endl;

		// Print line of source code.
		v8::String::Utf8Value sourceline(message->GetSourceLine());
		string sourceline_string = string(ToCString(sourceline));

		str  << "<code>";
		str << sourceline_string.substr(0, message->GetStartColumn());
		str << "<u>" << sourceline_string.substr(message->GetStartColumn(), message->GetEndColumn() - message->GetStartColumn()) << "</u>";
		str << sourceline_string.substr(message->GetEndColumn());
		str << "</code>" << endl;
	}

	message_ = str.str();
}
