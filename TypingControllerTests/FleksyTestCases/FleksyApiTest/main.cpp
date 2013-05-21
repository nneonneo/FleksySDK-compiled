#include <iostream>
#include "FLTypingControllerTester.h"

using namespace std;

FLTypingControllerTester tester;

int main(int argc, const char * argv[]) {
  int returnResult = 1;

  try {
    tester.setup();
    tester.runTests(false);
    returnResult = 0;
  } catch (std::runtime_error er) {
    printf("Error while running tests: %s\n", er.what());
  } catch (std::exception ex) {
    printf("Exception while running tests: %s\n", ex.what());
  } catch(...){
    printf("I'm here ERROR!\n");
  }
  
  return returnResult;
}