
#ifndef PATH_RESOLVER_H_
#define PATH_RESOLVER_H_

#include <string>
#include <stdio.h>

namespace vv {
	using namespace std;

	bool file_exists(const std::string& filename);
	string get_public_directory();
	string get_directory_from_path(string path);
	string get_extension_from_path(string path);
	string resolve_path(string path, string cd);
	string system_path_to_public_path(string path);
}

#endif
