
#ifndef DATE_H_
#define DATE_H_

#include <string>
#include <ctime>

class Date {
public:
	static std::string now(const std::string& format);
};

#endif
