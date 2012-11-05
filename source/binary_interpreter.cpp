
#include <vector>
#include <sstream>

#include "binary_interpreter.h"
#include "http_exception.h"

using namespace std;

BinaryInterpreter::BinaryInterpreter(const string& path, const string& mime) : FileInterpreter(path, mime) {
	
}

string BinaryInterpreter::interpret() {
	ifstream file(path_, fstream::binary);

	if (!file.is_open()) {
		throw HTTPException(404, path_);
	}
	
    std::string contents(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));

	return contents;
}