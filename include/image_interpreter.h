
#ifndef IMAGE_INTERPRETER_H_
#define IMAGE_INTERPRETER_H_

#include <string>
#include <fstream>

#include "file_interpreter.h"

class ImageInterpreter : public FileInterpreter {
public:
	explicit ImageInterpreter(const std::string& path);
	std::string mime() = 0;
	std::string interpret();
};

#endif
