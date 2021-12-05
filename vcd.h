/* Declare a variable.  Return an index for use with vcd_value. */
extern int vcd_variable (const char *name, const char *type, int width);

/* Open an output file. */
extern void vcd_start (const char *file, const char *name,
                       const char *timescale);

/* Record a value for an index associated with a variable.
   The current_time must increase monotonically. */
extern void vcd_value (long long current_time, int index, int value);

/* Record a value for an index associated with a real-valued variable.
   The current_time must increase monotonically. */
extern void vcd_real (long long current_time, int index, double value);
