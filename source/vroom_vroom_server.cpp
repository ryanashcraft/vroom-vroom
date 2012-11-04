
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <streambuf>

#include <v8.h>

#include "vroom_vroom_server.h"

using namespace std;

VroomVroomServer::VroomVroomServer(unsigned short port) : HTTPServer(port) {

}

string VroomVroomServer::process(const string& message) {
	string type;
	std::stringstream trimmer;
	trimmer << message;
	trimmer >> type;

	if (type == "GET") {
		string path;
		trimmer << message;
		trimmer >> path;

		// remove absolute URL forward slash
		path.erase(0, 1);

		if (path.substr(path.length() - 2) == "vv") {
			string content = "";
			try {
				content = interpret(path);
			} catch (const HTTPException& e) {
				fprintf(stderr, "NOT FOUND: %s\n", path.c_str());
				return NotFound();
			}

			return OK(content);
		}

		return HTTPServer::process(message);
	}

	fprintf(stderr, "BAD REQUEST: %s\n", message.c_str());
	return BadRequest("");
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
		throw HTTPException(404, path);
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
