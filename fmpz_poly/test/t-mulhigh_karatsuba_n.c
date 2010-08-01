/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

   Copyright (C) 2010 William Hart

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpz_poly.h"
#include "ulong_extras.h"

int main(void)
{
   int i, result;
   printf("mulhigh_karatsuba_n....");
   fflush(stdout);
   
   fmpz_poly_randinit();
   
   // check aliasing of a and b
   for (ulong i = 0; i < 2000UL; i++) 
   {
      fmpz_poly_t a, b, c;
      long len;

      fmpz_poly_init(a);
      fmpz_poly_init(b);
      fmpz_poly_init(c);
      len = n_randint(50);
      fmpz_poly_randtest(b, len, n_randint(200));
      fmpz_poly_randtest(c, len, n_randint(200));
     
      fmpz_poly_mulhigh_karatsuba_n(a, b, c, len);
      fmpz_poly_mulhigh_karatsuba_n(b, b, c, len);
      
      result = (fmpz_poly_equal(a, b));
      if (!result)
      {
         printf("FAIL:\n");
         fmpz_poly_print(a), printf("\n\n");
         fmpz_poly_print(b), printf("\n\n");
         abort();
      }

      fmpz_poly_clear(a);
      fmpz_poly_clear(b);
      fmpz_poly_clear(c);
   }

   // check aliasing of a and c
   for (i = 0; i < 2000; i++) 
   {
      fmpz_poly_t a, b, c;
      long len;
	
      fmpz_poly_init(a);
      fmpz_poly_init(b);
      fmpz_poly_init(c);
      len = n_randint(50);
      fmpz_poly_randtest(b, len, n_randint(200));
      fmpz_poly_randtest(c, len, n_randint(200));
   	  
      fmpz_poly_mulhigh_karatsuba_n(a, b, c, len);
      fmpz_poly_mulhigh_karatsuba_n(c, b, c, len);
      
      result = (fmpz_poly_equal(a, c));
      if (!result)
      {
         printf("FAIL:\n");
         fmpz_poly_print(a), printf("\n\n");
         fmpz_poly_print(c), printf("\n\n");
         abort();
      }

      fmpz_poly_clear(a);
      fmpz_poly_clear(b);
      fmpz_poly_clear(c);
   }

   // compare with mul_basecase
   for (i = 0; i < 2000; i++) 
   {
      fmpz_poly_t a, b, c, d;
      long len;

      fmpz_poly_init(a);
      fmpz_poly_init(b);
      fmpz_poly_init(c);
      fmpz_poly_init(d);
      len = n_randint(50);
      fmpz_poly_randtest(b, len, n_randint(200));
      fmpz_poly_randtest(c, len, n_randint(200));
   
      fmpz_poly_mul_classical(a, b, c);
      if (len) _fmpz_vec_zero(a->coeffs, FLINT_MIN(len - 1, a->length));
      fmpz_poly_mulhigh_karatsuba_n(d, b, c, len);
      
      result = (fmpz_poly_equal(a, d));
      if (!result)
      {
         printf("FAIL:\n");
         fmpz_poly_print(a), printf("\n\n");
         fmpz_poly_print(d), printf("\n\n");
         abort();
      }

      fmpz_poly_clear(a);
      fmpz_poly_clear(b);
      fmpz_poly_clear(c);
      fmpz_poly_clear(d);
   }

   fmpz_poly_randclear();
   _fmpz_cleanup();
   printf("PASS\n");
   return 0;
}
