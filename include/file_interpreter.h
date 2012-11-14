
#ifndef FILE_INTERPRETER_H_
#define FILE_INTERPRETER_H_

#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <unordered_map>

namespace vv {
	using namespace std;
	
	class FileInterpreter {
	public:
		explicit FileInterpreter(const string& path, const string& mime);
		static unique_ptr<FileInterpreter> file_interpreter_for_path(const string& path);
		virtual string mime() {
			return mime_;
		}
		virtual string interpret() = 0;
		void set_get_data(unordered_map<string, string>&& get_data);
		void set_post_data(unordered_map<string, string>&& post_data);
		vector<string> get_headers() const {
			return headers_;
		}
	protected:
		const string path_;
		const string mime_;
		unordered_map<string, string> get_data_;
		unordered_map<string, string> post_data_;
		vector<string> headers_;
	};
}

#endif
