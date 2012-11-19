
#ifndef FILE_INTERPRETER_H_
#define FILE_INTERPRETER_H_

#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <unordered_map>

namespace vv {
	using namespace std;
	
	/**
	 * Interprets a file to generate HTTP response content. Also determines mime type string and may
	 * produce some additional HTTP response headers.
	 */
	class FileInterpreter {
	public:
		/**
		 * Creates an abstract file interpreter with a path and mime string.
		 * @param  path the path of the file to open
		 * @param  mime the mime type string
		 * @return      an abstract file interpreter
		 */
		explicit FileInterpreter(const string& path, const string& mime);

		/**
		 * Creates an instance of a FileInterpreter subclass. The subclass is determined from the
		 * file extension.
		 * @param  path the file path
		 * @return a unique pointer to a FileInterpreter instance determined by file extension
		 */
		static unique_ptr<FileInterpreter> file_interpreter_for_path(const string& path);

		/**
		 * Returns the mime type string.
		 * @return the mime type string
		 */
		virtual string mime() {
			return mime_;
		}

		/**
		 * Returns a generated string interpretation of a file.
		 * @return string interpretation of file
		 */
		virtual string interpret() = 0;

		/**
		 * Sets the GET data from the HTTP request.
		 * @param unordered_map<string, string>   the GET data parameters as key-value pair
		 */
		void set_get_data(unordered_map<string, string>&& get_data);

		/**
		 * Sets the POST data from the HTTP request.
		 * @param unordered_map<string, string>   the POST data parameters as key-value pair
		 */
		void set_post_data(unordered_map<string, string>&& post_data);

		/**
		 * Returns the extra headers (if any) generated from interpreting the file.
		 * @return the extra headers
		 */
		vector<string> get_headers() const {
			return headers_;
		}
	protected:
		/**
		 * The path for the file to interpret.
		 */
		const string path_;

		/**
		 * The mime type string.
		 */
		const string mime_;

		/**
		 * The HTTP request GET data stored as a map of key-value pairs.
		 */
		unordered_map<string, string> get_data_;

		/**
		 * The HTTP request POST data stored as a map of key-value pairs.
		 */
		unordered_map<string, string> post_data_;

		/**
		 * The extra headers generated from interpreting the file.
		 */
		vector<string> headers_;
	};
}

#endif
