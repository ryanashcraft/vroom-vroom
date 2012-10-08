
#include <v8.h>
#include <fstream>
#include <string>
#include <streambuf>

using namespace v8;

int main(int argc, char* argv[]) {

  // Create a stack-allocated handle scope.
  HandleScope handle_scope;

  // Create a new context.
  Persistent<Context> context = Context::New();
  
  // Enter the created context for compiling and
  // running the hello world script. 
  Context::Scope context_scope(context);

  std::ifstream t("example.js");
  std::string str;

  t.seekg(0, std::ios::end);   
  str.reserve(t.tellg());
  t.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());

  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New(str.c_str());

  t.close();

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);
  
  // Run the script to get the result.
  Handle<Value> result = script->Run();
  
  // Dispose the persistent context.
  context.Dispose();

  // Convert the result to an ASCII string and print it.
  String::AsciiValue ascii(result);
  printf("%s\n", *ascii);
  return 0;
}
