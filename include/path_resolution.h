
#ifndef PATH_RESOLVER_H_
#define PATH_RESOLVER_H_

#include <string>
#include <stdio.h>

namespace vv {
	using namespace std;

	/**
	 * Check if file exists at given file path
	 * @param  path the requested file path
	 * @return          true if the file exists at given path
	 */
	bool file_exists(const string& path);

	/**
	 * Returns the absoulte path of the public directory.
	 * @return the public directory path
	 */
	string get_public_directory();

	/**
	 * If the path is a file, returns the directory that contains the file.
	 * If the path is a directory, it simply returns that directory.
	 * @param  path the file path to extract a directory
	 * @return      the path of the directory
	 */
	string get_directory_from_path(string path);

	/**
	 * Returns the extension string from a file path.
	 * @param  path the file path
	 * @return      the extension of the file
	 */
	string get_extension_from_path(string path);

	/**
	 * Converts a relative path to an absolute one.
	 * @param  path the requested file path
	 * @param  cd   the current directory path
	 * @return      the resolved absolute path
	 */
	string resolve_path(string path, string cd);

	/**
	 * Converts an absolute system file path to its visible path from the public
	 * directory.
	 * @param  path the absolute system file path
	 * @return      the visible file path from the public directory
	 */
	string system_path_to_public_path(string path);
}

#endif
