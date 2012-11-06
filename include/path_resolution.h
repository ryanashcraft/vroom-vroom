
#ifndef PATH_RESOLVER_H_
#define PATH_RESOLVER_H_

#include <string>
#include <stdio.h>

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

namespace vv {

using namespace std;

inline string get_exe_directory() {
	char cCurrentPath[FILENAME_MAX];
	string dir(GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)));
	return string(dir).substr(0, dir.find_last_of('\\'));
}

inline string get_directory_from_path(const string& path) {
	size_t extension_index = path.find_last_of("/");

	if (extension_index == string::npos || extension_index == path.length() - 1) {
		return "";
	}

	return path.substr(0, extension_index);
}

}

#endif
