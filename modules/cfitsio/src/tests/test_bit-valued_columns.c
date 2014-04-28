#include <unistd.h>
#include <fitsio.h>

char **array_sprintf(char *fmt, int ncols)
{
  char **str = (char**) malloc (ncols * sizeof (char**));
  int col;
  for (col = 1;  col <= ncols;  col++)
  {
    str[col-1] = (char*) malloc (8);
    sprintf (str[col-1], fmt, col);
  }
  return str;
}

#define NCOLS 32
#define NROWS 1024

/**
 * This program creates a binary-table with bit-valued columns:
 * Column n is a n-bit-vector,  whose value in row r
 * contains the first n bits of r, starting with the most significant bit.
 */
int main(int argc, char **argv)
{
  char *filename = "test_bit-valued_columns.fits";
  fitsfile *fptr;
  int col, row, i, status = 0;
  char **ttype = array_sprintf("x%d", NCOLS);
  char **tform = array_sprintf("%dX", NCOLS);
  char **tunit = NULL;
  char bit_one[] = { 1 };

  if (access(filename, F_OK) != -1)
    unlink(filename);

  if (fits_create_file (&fptr, filename, &status))
    fits_report_error (stderr, status),  exit (status);

  if (fits_create_tbl (fptr, BINARY_TBL, NROWS, NCOLS, ttype, tform, tunit, "BIT-VALUES", &status))
    fits_report_error (stderr, status),  exit (status);

  for (col = 1;  col <= NCOLS;  col++)
    for (row = 1;  row <= NROWS;  row++)
      for (i = 1;  i <= col;  i++)
        if (row & (1U << (col-i)) )
          (void) fits_write_col (fptr, TBIT, col, row, i, 1, bit_one, &status);

  if (fits_close_file (fptr,  &status))
    fits_report_error (stderr, status),  exit (status);
  return status;
}
