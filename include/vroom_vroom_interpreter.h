
#ifndef VROOM_VROOM_INTERPRETER_H_
#define VROOM_VROOM_INTERPRETER_H_

#include <string>
#include <fstream>

#include "file_interpreter.h"

class VroomVroomInterpreter : public FileInterpreter {
public:
	explicit VroomVroomInterpreter(const std::string& path);
	std::string mime() {
		return "text/html";
	}
	std::string interpret();
};

#endif
