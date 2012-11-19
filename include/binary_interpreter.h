
#ifndef BINARY_INTERPRETER_H_
#define BINARY_INTERPRETER_H_

#include <string>
#include <fstream>

#include "file_interpreter.h"

namespace vv {
	using namespace std;

	/**
	 * Interprets a file as a binary file for an HTTP response.
	 */
	class BinaryInterpreter : public vv::FileInterpreter {
	public:
		/**
		 * Creates a binary file interpreter with a file path and mime string.
		 * @param  path the path of the file to open
		 * @param  mime the mime type string
		 * @return      a binary file interpreter
		 */
		explicit BinaryInterpreter(const string& path, const string& mime);

		/**
		 * Returns a string of bytes from a binary file.
		 * @return a string of bytes from file
		 */
		string interpret();
	};
}

#endif
