
#include <stdio.h>  /* defines FILENAME_MAX */

#include <iostream>
#include <sstream>
#include <string>
#include <streambuf>

#include "vroom_vroom_interpreter.h"
#include "http_exception.h"
#include "v8_exception.h"

using namespace std;
using namespace v8;

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

inline string get_exe_directory() {
	char cCurrentPath[FILENAME_MAX];
	string dir(GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)));
	return string(dir).substr(0, dir.find_last_of('\\'));
}

string V8ExceptionToString(v8::TryCatch* handler);

inline string get_directory_from_path(const string& path) {
	size_t extension_index = path.find_last_of("/");

	if (extension_index == string::npos || extension_index == path.length() - 1) {
		return "";
	}

	return path.substr(0, extension_index);
}

VroomVroomInterpreter::VroomVroomInterpreter(const string& path) : FileInterpreter(path, "text/html") {
	
}

Handle<v8::Value> VroomVroomInterpreter::Include(const v8::Arguments& args) {
	using namespace v8;

	// We will be creating temporary handles so we use a handle scope.
	HandleScope handle_scope;

    Handle<Object> object = Handle<Object>::Cast(args[0]);

	String::AsciiValue ascii_path(args[0]);
	std::string path(*ascii_path);

	if (path[0] != '/') {
		Handle<Value> cd = v8::Context::GetCurrent()->Global()->Get(v8::String::New("CURRENT_DIRECTORY"));
		String::Utf8Value cd_str(cd);

		path = string(*cd_str) + "/" + path;
	} else {
		path = get_exe_directory() + path;
	}

	ifstream file(path);

	if (!file.is_open()) {
		return handle_scope.Close(String::New(""));
	}

	Handle<Value> result;
	try {
		result = interpret_file(file, path);
	} catch (V8Exception& e) {
		cerr << e.what() << endl;
		return handle_scope.Close(String::New(e.what()));
	}

    return handle_scope.Close(result);
}

Handle<v8::Value> VroomVroomInterpreter::interpret_file(ifstream& file, const string& path) {
	using namespace v8;

	// Create a stack-allocated handle scope.
	HandleScope handle_scope;

	// Create a new context.
	Persistent<Context> context = Context::New();

	// Enter the created context for compiling and
	// running the hello world script. 
	Context::Scope context_scope(context);

	//Get the context's global scope (that's where we'll put the constructor)
	v8::Handle<v8::Object> global = context->Global();
	 
	//create function template for our constructor
	//it will call the constructPoint function
	v8::Handle<v8::FunctionTemplate> function = v8::FunctionTemplate::New(VroomVroomInterpreter::Include);
	 
	//set the function in the global scope -- that is, set "Point" to the constructor
	global->Set(v8::String::New("include"), function->GetFunction());

	global->Set(v8::String::New("CURRENT_DIRECTORY"), v8::String::New(get_directory_from_path(path).c_str()));

	std::string str;

	file.seekg(0, std::ios::end);   
	str.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(file)),
	    std::istreambuf_iterator<char>());

	// Create a string containing the JavaScript source code.
	Handle<String> source = String::New(str.c_str());

	file.close();

	v8::TryCatch try_catch;

	// Compile the source code.
	Handle<Script> script = Script::Compile(source);

	Handle<Value> result;
	if (try_catch.HasCaught()) {
		throw V8Exception(&try_catch, path);
	} else {
		// Run the script to get the result.
		result = script->Run();

		if (result.IsEmpty()) {
			throw V8Exception(&try_catch, path);
		}
	}

	// Dispose the persistent context.
	context.Dispose();

	return result;
}

string VroomVroomInterpreter::interpret() {
	ifstream file(path_);

	if (!file.is_open()) {
		throw HTTPException(404, path_);
	}

	Handle<Value> result;
	try {
		result = interpret_file(file, path_);
	} catch (V8Exception& e) {
		cerr << e.what() << endl;
		return string(e.what());
	}

	String::Utf8Value str(result);

	if (*str == nullptr) {
		return "";
	}

	return string(*str);
}
