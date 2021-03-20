#include "generalUtils.h"

void checkMemoryAllocation(void *ptr)
{
  if (!ptr)
  {
    printf("The program has quit due to a memory allocation error.\n");
    exit(1); /*chose exit 1 because an error occured*/
  }
}