
#include <v8.h>
#include <fstream>
#include <string>
#include <streambuf>

#include "vroom_vroom_server.h"

using namespace std;

VroomVroomServer::VroomVroomServer(unsigned short port) : HTTPServer(port) {

}

string VroomVroomServer::process(const string& message) {
	return interpret(message);
}

string VroomVroomServer::interpret(const string& path) {
	using namespace v8;

	// Create a stack-allocated handle scope.
	HandleScope handle_scope;

	// Create a new context.
	Persistent<Context> context = Context::New();

	// Enter the created context for compiling and
	// running the hello world script. 
	Context::Scope context_scope(context);

	std::ifstream t(path);

	if (!t.is_open()) {
		fprintf(stderr, "ERROR: (404) file not found at path %s\n", path.c_str());
		// exception
		return "";
	}

	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
	    std::istreambuf_iterator<char>());

	// Create a string containing the JavaScript source code.
	Handle<String> source = String::New(str.c_str());

	t.close();

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
