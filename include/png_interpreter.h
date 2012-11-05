
#ifndef PNG_INTERPRETER_H_
#define PNG_INTERPRETER_H_

#include <string>
#include <fstream>

#include "image_interpreter.h"

class PNGInterpreter : public ImageInterpreter {
public:
	explicit PNGInterpreter(const std::string& path) : ImageInterpreter(path) {

	}
	
	std::string mime() {
		return "image/png";
	}
};

#endif
