
#include <v8.h>
#include <fstream>
#include <string>
#include <streambuf>

#include <stdlib.h>

#include "vroom_vroom_server.h"

using namespace v8;

int main(int argc, char* argv[]) {
  // std::ifstream t("test/example.js");

  // if (t.is_open()) {
  //   fprintf(stderr, "200\n");
  // } else {
  //   fprintf(stderr, "404\n");
  // }

  if (argc != 2) {
    return -1;
  }

  HTTPServer s(atoi(argv[1]));

  while (true)
    s.handle();

  return 0;
}
