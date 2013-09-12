/* File : example.h */

#include <ruby.h>


class DllChecker {
public:
  DllChecker();
  ~DllChecker();
  static void launchDllsInstaller(VALUE sodDllDir, VALUE exeName);
};



  
