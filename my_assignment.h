#include "protocol.h"
#include <arpa/inet.h>

void doAssignment(calcProtocol *protocol_ptr)
{
  uint32_t arith = ntohl(protocol_ptr->arith);
  const char *result_str;
  if (arith < 5)
  {
    int32_t v1 = ntohl(protocol_ptr->inValue1);
    int32_t v2 = ntohl(protocol_ptr->inValue2);
    int32_t result;

    if (arith == 1)
    {
      result = v1 + v2;
      result_str = "add";
    }
    else if (arith == 2)
    {
      result = v1 - v2;
      result_str = "sub";
    }
    else if (arith == 3)
    {
      result = v1 * v2;
      result_str = "mul";
    }
    else if (arith == 4)
    {
      result = v1 / v2;
      result_str = "div";
    }
    printf("%s %d %d\n", result_str, v1, v2);
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
      result_str = "fadd";
    }
    else if (arith == 6)
    {
      result = v1 - v2;
      result_str = "fsub";
    }
    else if (arith == 7)
    {
      result = v1 * v2;
      result_str = "fmul";
    }
    else if (arith == 8)
    {
      result = v1 / v2;
      result_str = "fdiv";
    }
    printf("%s %8.8g %8.8g \n", result_str, v1, v2);
    protocol_ptr->inResult = result;
  }
}