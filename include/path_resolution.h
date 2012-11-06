
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

string get_exe_directory() {
	char cCurrentPath[FILENAME_MAX];
	string dir(GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)));
	return dir;
}

string get_directory_from_path(string path) {
	size_t extension_index = path.find_last_of("/");

	if (extension_index == string::npos) {
		return path;
	}

	return path.substr(0, extension_index) + "/";
}

string resolve_path(string path, string cd) {
	string exe_dir = get_exe_directory();

	if (cd[cd.length() - 1] != '/') {
		cd += "/";
	}

	if (path[0] == '/') {
		return exe_dir + path;
	} else {
		if (cd[0] == '/') {
			cd.erase(0, 1);
		}

		string full_cd = exe_dir + "/" + cd;

		if (path[0] == '.' && path[1] == '/') {
			path.erase(0, 2);
		} else {
			while (path.length() >= 3 && path[0] == '.' && path[1] == '.' && path[2] =='/') {
				path.erase(0, 3);
				full_cd.erase(full_cd.substr(0, full_cd.length() - 1).find_last_of("/") + 1);
			}
		}

		return full_cd + path;
	}
}

string translate_path(string path) {
	string exe_dir = get_exe_directory();
	
	if (path.length() >= exe_dir.length() && path.substr(0, exe_dir.length()) == exe_dir) {
		path.erase(0, exe_dir.length());
	}

	if (path[0] != '/') {
		path = string("/") + path;
	}

	return path;
}

}

#endif
