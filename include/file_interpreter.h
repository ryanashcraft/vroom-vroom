
#ifndef FILE_INTERPRETER_H_
#define FILE_INTERPRETER_H_

#include <string>
#include <fstream>
#include <memory>

class FileInterpreter {
public:
	explicit FileInterpreter(const std::string& path);
	static std::unique_ptr<FileInterpreter> file_interpreter_for_path(const std::string& path);
	virtual std::string mime() = 0;
	virtual std::string interpret() = 0;
protected:
	const std::string path_;
};

#endif