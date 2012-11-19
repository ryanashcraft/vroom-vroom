
#ifndef javascript_interpreter_H_
#define javascript_interpreter_H_

#include <v8.h>

#include <string>
#include <fstream>

#include "file_interpreter.h"

namespace vv {
	using namespace std;

	class JavaScriptInterpreter : public vv::FileInterpreter {
	public:
		explicit JavaScriptInterpreter(const string& path);
		virtual ~JavaScriptInterpreter();
		string mime() {
			return "text/html";
		}
		string interpret();
	private:
		static v8::Handle<v8::Value> interpret_file(ifstream& file, const string& path);
		static v8::Handle<v8::Value> Require(const v8::Arguments& args);
		v8::Isolate* isolate_;
	};
}

#endif
