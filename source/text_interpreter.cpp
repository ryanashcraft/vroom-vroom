
#include <sstream>

#include "text_interpreter.h"
#include "http_exception.h"

using namespace std;
using namespace vv;

TextInterpreter::TextInterpreter(const string& path, const string& mime) : FileInterpreter(path, mime) {
	
}

string TextInterpreter::interpret() {
	ifstream file(path_);

	if (!file.is_open()) {
		throw HTTPException(404, path_);
	}

	std::stringstream buffer;
    buffer << file.rdbuf();
	file.close();

	return buffer.str();
}