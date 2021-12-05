#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vcd.h"

struct vcd
{
  const char *name;
  const char *type;
  int width;
  char id[5];
  int value;
  double real;
};

#define VARIABLES 100
struct vcd variable[VARIABLES];
static long long last_time = -1;
static FILE *output = NULL;
static int index = 0;

static void vcd_binary (int value, int width)
{
  int mask;
  fputc ('b', output);
  for (mask = (1 << (width - 1)); mask != 0; mask >>= 1)
    fputc (value & mask ? '1' : '0', output);
}

void vcd_value (long long current_time, int index, int value)
{
  if (variable[index].value == value)
    return;

  if (current_time > last_time) {
    last_time = current_time;
    fprintf (output, "#%lld\n", current_time);
  }

  variable[index].value = value;
  if (variable[index].width == 1)
    fprintf (output, "%d%s\n", value, variable[index].id);
  else {
    vcd_binary (value, variable[index].width);
    fprintf (output, " %s\n", variable[index].id);
  }
}

void vcd_real (long long current_time, int index, double value)
{
  if (variable[index].real == value)
    return;

  if (current_time > last_time) {
    last_time = current_time;
    fprintf (output, "#%lld\n", current_time);
  }

  variable[index].real = value;
  fprintf (output, "r%.16g %s\n", value, variable[index].id);
}

static void vcd_close (void)
{
  fclose (output);
}

void vcd_start (const char *file, const char *name, const char *timescale)
{
  int i;

  output = fopen (file, "w");
  if (output == NULL) {
    fprintf (stderr, "Error opening %s\n", file);
    exit (1);
  }

  atexit (vcd_close);

  fprintf (output, "$version %s $end\n", name);
  fprintf (output, "$timescale %s $end\n", timescale);

  for (i = 0; i < index; i++) {
    fprintf (output, "$var %s %d %s %s",
             variable[i].type,
             variable[i].width,
             variable[i].id,
             variable[i].name);
    if (variable[i].width > 1)
      fprintf (output, " [%d:0]", variable[i].width - 1);
    fprintf (output, " $end\n");
  }

  fprintf (output, "$enddefinitions $end\n");
  fprintf (output, "$dumpvars\n");
  fflush (output);
}

static void vcd_id (char *id, int index)
{
  do {
    *id++ = '!' + (index % 93);
    index /= 93;
  } while (index > 0);
  *id = 0;
}

int vcd_variable (const char *name, const char *type, int width)
{
  if (output != NULL) {
    fprintf (stderr, "Must declare all variables before output.\n");
    exit (1);
  }

  if (index == VARIABLES) {
    fprintf (stderr, "Too many variables.\n");
    exit (1);
  }

  variable[index].name = name;
  variable[index].type = type;
  variable[index].width = width;
  variable[index].value = -1;
  variable[index].real = NAN;
  vcd_id (variable[index].id, index);
  return index++;
}

#ifdef TEST
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
}
#endif
