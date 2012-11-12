
#include <iostream>
#include <unordered_map>

#include "file_interpreter.h"

#include "text_interpreter.h"
#include "vroom_vroom_interpreter.h"
#include "binary_interpreter.h"
#include "path_resolution.h"

using namespace std;

static unordered_map<string, string> binary_extension_to_mime_map({
	{"png", "image/png"},
	{"jpg", "image/jpg"},
	{"jpeg", "image/jpeg"},
	{"gif", "image/gif"}
});

static unordered_map<string, string> text_extension_to_mime_map({
	{"htm", "text/html"},
    {"html", "text/html"},
    {"xml", "text/xml"},
    {"txt", "text/plain"},
    {"css", "text/css"},
    {"js", "application/javascript"}
});

FileInterpreter::FileInterpreter(const string& path, const string& mime) : path_(path), mime_(mime) {

}

unique_ptr<FileInterpreter> FileInterpreter::file_interpreter_for_path(const string& path) {
	string extension = vv::get_extension_from_path(path);
	
	if (extension == "ssjs") {
		return unique_ptr<FileInterpreter>(new VroomVroomInterpreter(path));
	} else if (binary_extension_to_mime_map.count(extension) > 0) {
		return unique_ptr<FileInterpreter>(new BinaryInterpreter(path, binary_extension_to_mime_map[extension]));
	}else if (text_extension_to_mime_map.count(extension) > 0) {
		return unique_ptr<FileInterpreter>(new TextInterpreter(path, text_extension_to_mime_map[extension]));
	}

	return unique_ptr<FileInterpreter>(new TextInterpreter(path));
}

void FileInterpreter::set_post_data(unordered_map<string, string>&& post_data) {
	post_data_ = move(post_data);
}
