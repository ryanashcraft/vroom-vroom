
#include <vector>
#include <sstream>

#include "image_interpreter.h"
#include "http_exception.h"
#include "base64.h"

using namespace std;

ImageInterpreter::ImageInterpreter(const string& path) : FileInterpreter(path) {
	
}

string ImageInterpreter::interpret() {
	ifstream file(path_, fstream::binary);

	if (!file.is_open()) {
		throw HTTPException(404, path_);
	}
	
    std::string contents(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
    contents.insert(0, "\n");

	return contents;
}