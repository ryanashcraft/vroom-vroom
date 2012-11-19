
#ifndef DEFERRED_H_
#define DEFERRED_H_

#include <functional>

namespace vv {
	using namespace std;

	/**
	 * Class for constructing objects that call a function on destruction.
	 */
	class Deferred {
	public:
		/**
		 * Creates a Deferred instance with a function.
		 * @param func	the function to call on destruction
		 */
		Deferred(function<void()> func);

		/**
		 * Calls the function.
		 */
		~Deferred();
	private:
		/**
		 * The function to call on destruction.
		 */
		function<void()> function_;
	};
}

#endif
