
#ifndef VROOM_VROOM_SERVER_H
#define VROOM_VROOM_SERVER_H

#include "http_server.h"

class VroomVroomServer : public HTTPServer {
public:
	VroomVroomServer(unsigned short port);
protected:
	std::string process(const std::string& message);
private:
	std::string interpret(const std::string& path);
};

#endif
