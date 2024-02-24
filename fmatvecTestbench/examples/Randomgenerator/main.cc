#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  undef __STRICT_ANSI__ // to define _controlfp which is not part of ANSI and hence not defined in mingw
#  include <cfloat>
#  define __STRICT_ANSI__
#endif
#include <config.h>
#include <cassert>
#include <cfenv>
#include "fmatvec.h"
#include "fmatvecTestbench/randomgenerator.h"
#include <iostream>

using namespace std;
using namespace fmatvec;

int main (int argc, char* argv[])
{
#ifdef _WIN32
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
  _controlfp(~(_EM_ZERODIVIDE | _EM_INVALID | _EM_OVERFLOW), _MCW_EM);
#else
  assert(feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW)!=-1);
#endif

  // Creating Randomgenerator
  Randomgenerator *generator = new Randomgenerator();

  // setting range for random numbers ]-Range/2;Range/2[
  generator->setRange(500);

  // setting number of decimal places
  generator->setDecimalPlaces(3);

  // initializing random generator
  generator->init();

  for(int i=0; i<100; i++) {
    cout << "Zufallszahl= " << (*generator)() << endl;
  }

  return 0;
}

