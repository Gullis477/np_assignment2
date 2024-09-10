#include "protocol.h"
#include <arpa/inet.h>
#include <map>
#include <string>

void *doAssignment(calcProtocol *protocol_ptr)
{
  uint32_t arith = protocol_ptr->arith;
  if (arith < 5)
  {
    int32_t v1 = ntohl(protocol_ptr->inValue1);
    int32_t v2 = ntohl(protocol_ptr->inValue2);
    int32_t result;
    if (arith == 1)
    {
      result = v1 + v2;
    }
    else if (arith == 2)
    {
      result = v1 - v2;
    }
    else if (arith == 3)
    {
      result = v1 * v2;
    }
    else if (arith == 4)
    {
      result = v1 / v2;
    }
    protocol_ptr->inResult = result;
  }
  else
  {
    double v1 = protocol_ptr->flValue1;
    double v2 = protocol_ptr->flValue2;
    double result;
    if (arith == 5)
    {
      result = v1 + v2;
    }
    else if (arith == 6)
    {
      result = v1 - v2;
    }
    else if (arith == 7)
    {
      result = v1 * v2;
    }
    else if (arith == 8)
    {
      result = v1 / v2;
    }
    protocol_ptr->inResult = result;
  }