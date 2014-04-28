private variable MODULE_NAME = "cfitsio";
prepend_to_slang_load_path (".");
set_import_module_path (".:" + get_import_module_path ());

require ("fits");

private variable Failed = 0;


variable filename = "tests/test_bit-valued_columns.fits";
variable fits = fits_read_table (filename);

variable NROWS = 1024;

variable i;
_for i (1, 32, 1)
{
  variable col = fits_read_col (filename, "x$i"$);
  variable mask = (1 << i) - 1;
  Failed += any( (col & mask) != ([1:NROWS] & mask) );
}


if (Failed == 0)
  message ("Passed");
else
  message ("Failed");
