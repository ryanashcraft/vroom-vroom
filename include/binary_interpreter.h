
#ifndef BINARY_INTERPRETER_H_
#define BINARY_INTERPRETER_H_

#include <string>
#include <fstream>

#include "file_interpreter.h"

namespace vv {
	using namespace std;

	class BinaryInterpreter : public vv::FileInterpreter {
	public:
		explicit BinaryInterpreter(const string& path, const string& mime);
		string interpret();
	};
}

#endif
