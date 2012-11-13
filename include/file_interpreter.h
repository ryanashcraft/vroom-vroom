
#ifndef FILE_INTERPRETER_H_
#define FILE_INTERPRETER_H_

#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <unordered_map>

class FileInterpreter {
public:
	explicit FileInterpreter(const std::string& path, const std::string& mime);
	static std::unique_ptr<FileInterpreter> file_interpreter_for_path(const std::string& path);
	virtual std::string mime() {
		return mime_;
	}
	virtual std::string interpret() = 0;
	void set_post_data(std::unordered_map<std::string, std::string>&& post_data);
	std::vector<std::string> get_headers() const {
		return headers_;
	}
protected:
	const std::string path_;
	const std::string mime_;
	std::unordered_map<std::string, std::string> post_data_;
	std::vector<std::string> headers_;
};

#endif
