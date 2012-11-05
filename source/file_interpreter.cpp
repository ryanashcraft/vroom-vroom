
#include <iostream>
#include <unordered_map>

#include "file_interpreter.h"

#include "text_interpreter.h"
#include "vroom_vroom_interpreter.h"
#include "binary_interpreter.h"

using namespace std;

string get_extension_from_path(const string& path);

static unordered_map<string, string> binary_extension_to_mime_map({
	{"png", "image/png"},
	{"jpg", "image/jpg"},
	{"jpeg", "image/jpeg"},
	{"gif", "image/gif"}
});

FileInterpreter::FileInterpreter(const string& path) : path_(path) {

}

unique_ptr<FileInterpreter> FileInterpreter::file_interpreter_for_path(const string& path) {
	string extension = get_extension_from_path(path);
	
	if (extension == "vv") {
		return unique_ptr<FileInterpreter>(new VroomVroomInterpreter(path));
	} else if (binary_extension_to_mime_map.count(extension) > 0) {
		return unique_ptr<FileInterpreter>(new BinaryInterpreter(path, binary_extension_to_mime_map[extension]));
	}

	return unique_ptr<FileInterpreter>(new TextInterpreter(path));
}

inline string get_extension_from_path(const string& path) {
	size_t extension_index = path.find_last_of(".");

	if (extension_index == string::npos || extension_index == path.length() - 1) {
		return "";
	}

	return path.substr(extension_index + 1);
}
