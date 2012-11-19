
#include <sys/stat.h>

#include "path_resolution.h"

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

namespace vv {
	using namespace std;

	/**
	 * @see http://stackoverflow.com/questions/4316442/stdofstream-check-if-file-exists-before-writing
	 */
	bool file_exists(const string& path) {
	    struct stat buf;

	    if (stat(path.c_str(), &buf) != -1) {
	        return true;
	    }

	    return false;
	}

	string get_public_directory() {
		char cCurrentPath[FILENAME_MAX];
		string dir(GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)));
		return dir + "/public/";
	}

	string get_directory_from_path(string path) {
		size_t extension_index = path.find_last_of("/");

		if (extension_index == string::npos) {
			return path;
		}

		return path.substr(0, extension_index) + "/";
	}

	string get_extension_from_path(string path) {
		size_t extension_index = path.find_last_of(".");

		if (extension_index == string::npos || extension_index == path.length() - 1) {
			return "";
		}

		return path.substr(extension_index + 1);
	}

	string resolve_path(string path, string cd) {
		string pubdir = get_public_directory();

		if (cd[cd.length() - 1] != '/') {
			cd += "/";
		}

		if (path[0] == '/') {
			return pubdir + path;
		} else {
			if (cd[0] == '/') {
				cd.erase(0, 1);
			}

			string full_cd = pubdir + "/" + cd;

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

	string system_path_to_public_path(string path) {
		string pubdir = get_public_directory();

		if (path.length() >= pubdir.length() && path.substr(0, pubdir.length()) == pubdir) {
			path.erase(0, pubdir.length());
		}

		if (path[0] != '/') {
			path = string("/") + path;
		}

		return path;
	}
}
