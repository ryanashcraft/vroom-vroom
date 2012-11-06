
#ifndef PATH_RESOLVER_H_
#define PATH_RESOLVER_H_

#include <string>
#include <stdio.h>

namespace vv {
	using namespace std;

	string get_public_directory();
	string get_directory_from_path(string path);
	string resolve_path(string path, string cd);
	string translate_path(string path);
}

#endif
