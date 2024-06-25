#define UNICODE

#include <windows.h>

#include <chrono>
#include <fstream>
#include <iostream>

#include "config.h"
#include "svc.h"
#include "util.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  svc_start();
  return 1;
}
