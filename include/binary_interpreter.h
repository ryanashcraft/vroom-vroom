
#ifndef BINARY_INTERPRETER_H_
#define BINARY_INTERPRETER_H_

#include <string>
#include <fstream>

#include "file_interpreter.h"

namespace vv {
	class BinaryInterpreter : public vv::FileInterpreter {
	public:
		explicit BinaryInterpreter(const std::string& path, const std::string& mime);
		std::string interpret();
	};
}

#endif
