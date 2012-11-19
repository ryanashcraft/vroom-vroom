
#ifndef REGEX_MATCHER_H_
#define REGEX_MATCH_ERH_

#include <string>
#include <vector>

#include <regex.h>
#include <cstdlib>

namespace vv {
	using namespace std;

	class RegexMatcher {
	public:
		RegexMatcher(string pattern);
		~RegexMatcher();
		vector<string> find_matches(string input, unsigned int max_matches=32);
	private:
		regex_t c_regex_;
	};
}

#endif
