
#include <v8.h>
#include <fstream>
#include <string>
#include <streambuf>

#include <stdlib.h>

#include "vroom_vroom_server.h"

using namespace v8;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return -1;
  }

  VroomVroomServer s(atoi(argv[1]));
  s.handle();

  return 0;
}
