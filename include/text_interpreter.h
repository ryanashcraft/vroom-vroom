
#ifndef TEXT_INTERPRETER_H_
#define TEXT_INTERPRETER_H_

#include <string>
#include <fstream>

#include "file_interpreter.h"

namespace vv {
	using namespace std;

	class TextInterpreter : public vv::FileInterpreter {
	public:
		explicit TextInterpreter(const string& path, const string& mime="text/plain");
		string interpret();
	};
}

#endif
