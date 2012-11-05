
#ifndef BINARY_INTERPRETER_H_
#define BINARY_INTERPRETER_H_

#include <string>
#include <fstream>

#include "file_interpreter.h"

class BinaryInterpreter : public FileInterpreter {
public:
	explicit BinaryInterpreter(const std::string& path, const std::string& mime);
	std::string interpret();
};

#endif
