
#include <iostream>
#include <sstream>
#include <string>
#include <streambuf>

#include "vroom_vroom_interpreter.h"
#include "http_exception.h"
#include "v8_exception.h"
#include "path_resolution.h"

using namespace std;
using namespace v8;

string v8_string_to_string(const v8::String::Utf8Value& value) {
  return *value ? string(*value) : "<string conversion failed>";
}

VroomVroomInterpreter::VroomVroomInterpreter(const string& path) : FileInterpreter(path, "text/html") {
	
}

Handle<v8::Value> VroomVroomInterpreter::Require(const v8::Arguments& args) {
	using namespace v8;

	HandleScope handle_scope;

    Handle<Object> object = Handle<Object>::Cast(args[0]);

	String::Utf8Value utfpath(args[0]);
	string path(v8_string_to_string(utfpath));

	Handle<String> cd = Handle<String>::Cast(v8::Context::GetCurrent()->Global()->Get(v8::String::New("CURRENT_DIRECTORY")));
	string current_directory(v8_string_to_string(String::Utf8Value(cd)));
	string resolved_path = vv::resolve_path(path, current_directory);

	if (resolved_path[resolved_path.length() - 1] == '/') {
		v8::Context::GetCurrent()->Global()->Set(v8::String::New("CURRENT_DIRECTORY"), cd);

		return handle_scope.Close(String::New(""));
	}

	ifstream file(resolved_path);

	if (!file.is_open()) {
		v8::Context::GetCurrent()->Global()->Set(v8::String::New("CURRENT_DIRECTORY"), cd);

		return handle_scope.Close(String::New(""));
	}

	Handle<Value> result;
	try {
		result = interpret_file(file, resolved_path);
	} catch (V8Exception& e) {
		v8::Context::GetCurrent()->Global()->Set(v8::String::New("CURRENT_DIRECTORY"), cd);

		return handle_scope.Close(String::New(""));
	}

	v8::Context::GetCurrent()->Global()->Set(v8::String::New("CURRENT_DIRECTORY"), cd);

    return handle_scope.Close(result);
}

Handle<v8::Value> VroomVroomInterpreter::interpret_file(ifstream& file, const string& path) {
	using namespace v8;

	HandleScope handle_scope;
	v8::Handle<v8::Object> global = v8::Context::GetCurrent()->Global();

	v8::Handle<v8::FunctionTemplate> function = v8::FunctionTemplate::New(VroomVroomInterpreter::Require);
	global->Set(v8::String::New("require"), function->GetFunction());

	string public_cd(vv::get_directory_from_path(vv::system_path_to_public_path(path)));
	global->Set(v8::String::New("CURRENT_DIRECTORY"), v8::String::New(public_cd.c_str()));

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

	Local<Value> result;
	if (try_catch.HasCaught()) {
		throw V8Exception(&try_catch, path);
	} else {
		// Run the script to get the result.
		result = script->Run();

		if (result.IsEmpty()) {
			throw V8Exception(&try_catch, path);
		}
	}

	return handle_scope.Close(result);
}

string VroomVroomInterpreter::interpret() {
	ifstream file(path_);

	if (!file.is_open()) {
		throw HTTPException(404, path_);
	}

	HandleScope handle_scope;

	// Create a new context.
	Persistent<Context> context = Context::New();
	Context::Scope context_scope(context);
	v8::Handle<v8::Object> global = v8::Context::GetCurrent()->Global();

	Local<Object> post_object = v8::Object::New();
	for (unordered_map<string, string>::iterator iterator = post_data_.begin(); iterator != post_data_.end(); ++iterator) {
		post_object->Set(v8::String::New(iterator->first.c_str()), v8::String::New(iterator->second.c_str()));
	}
	global->Set(v8::String::New("_POST"), post_object);

	Handle<Value> result;
	try {
		result = interpret_file(file, path_);
	} catch (V8Exception& e) {
		cerr << e.what() << endl;
		return string(e.what());
	}

	Local<String> str(result->ToString());

	// Dispose the persistent context.
	context.Dispose();

	if (*str == nullptr) {
		return "";
	}

	return string(*String::Utf8Value(str));
}
