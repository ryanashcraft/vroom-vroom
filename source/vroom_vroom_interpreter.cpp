
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <streambuf>

#include <v8.h>

#include "vroom_vroom_interpreter.h"
#include "http_exception.h"

using namespace std;
using namespace v8;

VroomVroomInterpreter::VroomVroomInterpreter(const string& path) : FileInterpreter(path) {
	
}

string VroomVroomInterpreter::interpret() {
	using namespace v8;

	// Create a stack-allocated handle scope.
	HandleScope handle_scope;

	// Create a new context.
	Persistent<Context> context = Context::New();

	// Enter the created context for compiling and
	// running the hello world script. 
	Context::Scope context_scope(context);

	std::ifstream file(path_);

	if (!file.is_open()) {
		throw HTTPException(404, path_);
	}

	std::string str;

	file.seekg(0, std::ios::end);   
	str.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(file)),
	    std::istreambuf_iterator<char>());

	// Create a string containing the JavaScript source code.
	Handle<String> source = String::New(str.c_str());

	file.close();

	// Compile the source code.
	Handle<Script> script = Script::Compile(source);

	// Run the script to get the result.
	Handle<Value> result = script->Run();

	// Dispose the persistent context.
	context.Dispose();

	// Convert the result to an ASCII string and print it.
	String::AsciiValue ascii(result);

	return string(*ascii);
}