
#include <iostream>

#include "regex_matcher.h"

using namespace std;
using namespace vv;

RegexMatcher::RegexMatcher(string pattern) {
	::regcomp(&c_regex_, pattern.c_str(), REG_EXTENDED);
}

RegexMatcher::~RegexMatcher() {
	::regfree(&c_regex_);
}

vector<string> RegexMatcher::find_matches(string input, unsigned int max_matches) {
	vector<string> matches;
	regmatch_t c_matches[max_matches];

	int reti = ::regexec(&c_regex_, input.c_str(), max_matches, c_matches, 0);
	
	if (reti == 0) {
		for (int i = 0; i < max_matches; ++i) {
			regmatch_t match = c_matches[i];
			if (match.rm_so >= 0 && match.rm_eo <= input.length()) {
				matches.push_back(input.substr(match.rm_so, match.rm_eo - match.rm_so));
			} else {
				break;
			}
		}
	}
	
	return matches;
}
