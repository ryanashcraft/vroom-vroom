
#include <v8.h>
#include <fstream>
#include <string>
#include <streambuf>

#include <stdlib.h>

#include "http_server.h"

using namespace v8;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return -1;
  }

  HTTPServer s(atoi(argv[1]));

  while (true)
    s.handle();

  return 0;
}
