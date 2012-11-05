
#ifndef TEXT_INTERPRETER_H_
#define TEXT_INTERPRETER_H_

#include <string>
#include <fstream>

#include "file_interpreter.h"

class TextInterpreter : public FileInterpreter {
public:
	explicit TextInterpreter(const std::string& path, const std::string& mime="text/plain");
	std::string interpret();
};

#endif
