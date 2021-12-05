#include "vcd.h"

int main (void)
{
  int reg1, wire2, real3;

  reg1 = vcd_variable ("reg1", "reg", 8);
  wire2 = vcd_variable ("wire2", "wire", 1);
  real3 = vcd_variable ("real3", "real", 64);
  vcd_start ("test.vcd", "Testing", "1ns");

  vcd_value (0, reg1, 0);
  vcd_value (0, wire2, 0);
  vcd_real  (0, real3, -1.0);
  vcd_value (10, reg1, 0x83);
  vcd_real  (10, real3, 10.0);
  vcd_value (20, wire2, 1);
  vcd_real  (20, real3, 20.0);

  return 0;
}
