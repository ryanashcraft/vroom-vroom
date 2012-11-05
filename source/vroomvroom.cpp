
#include <v8.h>
#include <fstream>
#include <string>
#include <streambuf>

#include <stdlib.h>
#include <signal.h>

#include "http_server.h"

using namespace std;
using namespace v8;

HTTPServer *s;

void exit_handler(int signal){
	delete s;
	exit(1);
}

void usage() {
	fprintf(stderr, "usage: [port]\n");
}

void listen_to_signal(int signal, void(*handler)(int)) {
	struct sigaction sig_action;

	sig_action.sa_handler = handler;
	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_flags = 0;

	sigaction(signal, &sig_action, NULL);
}

int main(int argc, char* argv[]) {
	using namespace v8;
	unsigned short port;

	if (argc == 2) {
		port = atoi(argv[1]);
	} else if (argc == 1) {
		port = 8000;
	} else {
		usage();
		return -1;
	}

	s = new HTTPServer(port);

	listen_to_signal(SIGINT, exit_handler);

	while (true) {
		s->handle();
	}

	delete s;

	return 0;
}
