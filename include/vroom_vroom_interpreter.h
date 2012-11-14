
#ifndef VROOM_VROOM_INTERPRETER_H_
#define VROOM_VROOM_INTERPRETER_H_

#include <v8.h>

#include <string>
#include <fstream>

#include "file_interpreter.h"

class VroomVroomInterpreter : public FileInterpreter {
public:
	explicit VroomVroomInterpreter(const std::string& path);
	virtual ~VroomVroomInterpreter();
	std::string mime() {
		return "text/html";
	}
	std::string interpret();
private:
	static v8::Handle<v8::Value> interpret_file(std::ifstream& file, const std::string& path);
	static v8::Handle<v8::Value> Require(const v8::Arguments& args);
	v8::Isolate* isolate_;
};

#endif
