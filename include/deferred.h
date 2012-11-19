
#ifndef DEFERRED_H_
#define DEFERRED_H_

#include <functional>

namespace vv {
	using namespace std;

	class Deferred {
	public:
		Deferred(function<void()> func);
		~Deferred();
	private:
		function<void()> function_;
	};
}

#endif
