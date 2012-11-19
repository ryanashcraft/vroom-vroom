
#ifndef TEXT_INTERPRETER_H_
#define TEXT_INTERPRETER_H_

#include <string>
#include <fstream>

#include "file_interpreter.h"

namespace vv {
	using namespace std;

	/**
	 * Interprets a file as a text file for an HTTP response.
	 */
	class TextInterpreter : public vv::FileInterpreter {
	public:
		/**
		 * Creates a text file interpreter with a file path and a mime type string.
		 * @param  path the path of the file to open
		 * @param  mime the mime type string
		 */
		explicit TextInterpreter(const string& path, const string& mime="text/plain");

		/**
		 * Returns the string read from the text file.
		 * @return the string read from the text file
		 */
		string interpret();
	};
}

#endif
