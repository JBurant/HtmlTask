/*
 * author: Jiri Burant
 * date created: 14.9.2017
*/

#include "TaskFacade.h"
#include "TaskTypes.h"
#include <iostream>

int main(int argc, char *argv[])
{
  std::cout << "Start \n";

  if(argc != 2)
  {
    std::cout << "Wrong number of arguments \n, supply the url for download.";
    return RET_ERR_WRONG_NO_ARGUMENTS;
  }

  TaskFacade *taskFacade = new TaskFacade();
  returnCode_t retVal = taskFacade->runTask(argv[1]);  

  delete taskFacade;
    std::cout << "Done \n";
  return retVal;
}
