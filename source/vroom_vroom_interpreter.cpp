
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

#define JS_NAME_CURRENT_DIRECTORY "CURRENT_DIRECTORY"
#define JS_NAME_GET_OBJECT "GET"
#define JS_NAME_POST_OBJECT "POST"
#define JS_NAME_HEADERS "HEADERS"

string v8_string_to_string(const String::Utf8Value& value) {
  return *value ? string(*value) : "";
}

VroomVroomInterpreter::VroomVroomInterpreter(const string& path) : FileInterpreter(path, "text/html") {
	isolate_ = Isolate::New();
	isolate_->Enter();
}

VroomVroomInterpreter::~VroomVroomInterpreter() {
	isolate_->Exit();
}

Handle<Value> VroomVroomInterpreter::Require(const Arguments& args) {
	HandleScope handle_scope;

	String::Utf8Value utfpath(args[0]);
	string path(v8_string_to_string(utfpath));

	Handle<String> cd = Handle<String>::Cast(Context::GetCurrent()->Global()->Get(String::New(JS_NAME_CURRENT_DIRECTORY)));
	string current_directory(v8_string_to_string(String::Utf8Value(cd)));
	string resolved_path = vv::resolve_path(path, current_directory);

	if (resolved_path[resolved_path.length() - 1] == '/') {
		Context::GetCurrent()->Global()->Set(String::New(JS_NAME_CURRENT_DIRECTORY), cd);

		return handle_scope.Close(String::New(""));
	}

	ifstream file(resolved_path);

	if (!file.is_open()) {
		Context::GetCurrent()->Global()->Set(String::New(JS_NAME_CURRENT_DIRECTORY), cd);

		return handle_scope.Close(String::New(""));
	}

	Handle<Value> result;
	try {
		result = interpret_file(file, resolved_path);
	} catch (V8Exception& e) {
		Context::GetCurrent()->Global()->Set(String::New(JS_NAME_CURRENT_DIRECTORY), cd);

		return handle_scope.Close(String::New(""));
	}

	Context::GetCurrent()->Global()->Set(String::New(JS_NAME_CURRENT_DIRECTORY), cd);

    return handle_scope.Close(result);
}

Handle<Value> VroomVroomInterpreter::interpret_file(ifstream& file, const string& path) {
	HandleScope handle_scope;
	Handle<Object> global = Context::GetCurrent()->Global();

	// Register require JS function
	Handle<FunctionTemplate> function = FunctionTemplate::New(VroomVroomInterpreter::Require);
	global->Set(String::New("require"), function->GetFunction());

	string public_cd(vv::get_directory_from_path(vv::system_path_to_public_path(path)));
	global->Set(String::New(JS_NAME_CURRENT_DIRECTORY), String::New(public_cd.c_str()));

	std::string str;

	file.seekg(0, std::ios::end);   
	str.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(file)),
	    std::istreambuf_iterator<char>());

	// Create a string containing the JavaScript source code.
	Handle<String> source = String::New(str.c_str());

	file.close();

	TryCatch try_catch;

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
	Handle<Object> global = Context::GetCurrent()->Global();
	
	// Create populated GET array
	Local<Object> get_object = Object::New();
	for (unordered_map<string, string>::iterator iterator = get_data_.begin(); iterator != get_data_.end(); ++iterator) {
		get_object->Set(String::New(iterator->first.c_str()), String::New(iterator->second.c_str()));
	}
	global->Set(String::New(JS_NAME_GET_OBJECT), get_object);

	// Create populated POST array
	Local<Object> post_object = Object::New();
	for (unordered_map<string, string>::iterator iterator = post_data_.begin(); iterator != post_data_.end(); ++iterator) {
		post_object->Set(String::New(iterator->first.c_str()), String::New(iterator->second.c_str()));
	}
	global->Set(String::New(JS_NAME_POST_OBJECT), post_object);

	// Create empty HEADERS array
	global->Set(String::New(JS_NAME_HEADERS), Array::New());

	Handle<Value> result;
	try {
		result = interpret_file(file, path_);
	} catch (V8Exception& e) {
		cerr << e.what() << endl;
		return string(e.what());
	}

	Local<String> str(result->ToString());

	// Get headers (if any)
	Handle<Value> headersv = global->Get(String::New(JS_NAME_HEADERS));
	if (!headersv.IsEmpty() && headersv->IsArray()) {
		Handle<Array> headers = Handle<Array>::Cast<Value>(headersv);
		for (int i = 0; i < headers->Length(); ++i) {
			Handle<Value> header = headers->Get(Number::New(i));
			if (!header.IsEmpty()) {
				headers_.push_back(v8_string_to_string(String::Utf8Value(header)));
			}
		}
	}

	// Dispose the persistent context.
	context.Dispose();

	if (*str == nullptr) {
		return "";
	}

	return string(*String::Utf8Value(str));
}
