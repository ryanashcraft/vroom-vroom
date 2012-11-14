
#ifndef VROOM_VROOM_INTERPRETER_H_
#define VROOM_VROOM_INTERPRETER_H_

#include <v8.h>

#include <string>
#include <fstream>

#include "file_interpreter.h"

namespace vv {
	using namespace std;

	class VroomVroomInterpreter : public vv::FileInterpreter {
	public:
		explicit VroomVroomInterpreter(const string& path);
		virtual ~VroomVroomInterpreter();
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
