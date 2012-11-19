
#include "deferred.h"

using namespace std;
using namespace vv;

Deferred::Deferred(function<void()> func) : function_(func) {

}

Deferred::~Deferred() {
	function_();
}
