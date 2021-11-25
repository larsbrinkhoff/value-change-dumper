#include "vcd.h"

int main (void)
{
  int reg1, wire2;

  reg1 = vcd_variable ("reg1", "reg", 8);
  wire2 = vcd_variable ("wire2", "wire", 1);
  vcd_start ("test.vcd", "Testing", "1ns");

  vcd_value (0, reg1, 0);
  vcd_value (0, wire2, 0);
  vcd_value (10, reg1, 0x83);
  vcd_value (20, wire2, 1);

  return 0;
}
