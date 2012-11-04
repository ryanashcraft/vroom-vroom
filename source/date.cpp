
#include "date.h"

const size_t MAX_BUFFER_SIZE = 256;

using namespace std;

string Date::now(const string& format) {
	time_t t = time(0);
	struct tm* now = ::localtime(&t);

	char buffer[MAX_BUFFER_SIZE];
	::strftime(buffer, MAX_BUFFER_SIZE, format.c_str(), now);

    return string(buffer);
}
