
#ifndef DATE_H_
#define DATE_H_

#include <string>
#include <ctime>

namespace vv {
	using namespace std;

	class Date {
	public:
		static string now(const string& format);
	};
}

#endif
