
#ifndef DATE_H_
#define DATE_H_

#include <string>
#include <ctime>

namespace vv {
	using namespace std;

	/**
	 * Represents date and time.
	 */
	class Date {
	public:
		/**
		 * Static function for a generating a formatted string from the current time.
		 * @param  format the date format for the generated string
		 * @return        the generated formatted string for the current time
		 */
		static string now(const string& format);
	};
}

#endif
