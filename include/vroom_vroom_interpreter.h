
#ifndef VROOM_VROOM_INTERPRETER_H_
#define VROOM_VROOM_INTERPRETER_H_

#include <string>
#include <fstream>

#include "text_interpreter.h"

class VroomVroomInterpreter : public TextInterpreter {
public:
	explicit VroomVroomInterpreter(const std::string& path);
	std::string interpret();
};

#endif
