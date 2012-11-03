
#include <string>

#include "vroom_vroom_server.h"

using namespace std;

VroomVroomServer::VroomVroomServer(unsigned short port) : HTTPServer(port) {

}

string VroomVroomServer::process(const string message) {
	return "vroom";
}
