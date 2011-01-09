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

    Copyright (C) 2009 William Hart
    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "long_extras.h"

int
main(void)
{
    int i, result;
    fmpz_randstate_t state;

    printf("divexact_si....");
    fflush(stdout);

    fmpz_randinit(state);

    for (i = 0; i < 100000; i++)
    {
        fmpz_t a, c;
        mpz_t e, f, g;
        long n;

        fmpz_init(a);
        fmpz_init(c);
        mpz_init(e);
        mpz_init(f);
        mpz_init(g);

        fmpz_randtest(a, state, 200);
        n = z_randtest_not_zero();
        fmpz_mul_si(c, a, n);

        fmpz_get_mpz(e, c);

        fmpz_divexact_si(a, c, n);
        mpz_divexact_ui(f, e, FLINT_ABS(n));
        if (n < 0)
            mpz_neg(f, f);

        fmpz_get_mpz(g, a);

        result = (mpz_cmp(f, g) == 0);
        if (!result)
        {
            printf("FAIL:\n");
            gmp_printf("n = %ld, e = %Zd, f = %Zd, g = %Zd\n", n, e, f, g);
            abort();
        }

        fmpz_clear(a);
        fmpz_clear(c);
        mpz_clear(e);
        mpz_clear(f);
        mpz_clear(g);
    }

    /* Test aliasing of a and c */
    for (i = 0; i < 100000; i++)
    {
        fmpz_t a, c;
        mpz_t d, f, g;
        long n;

        fmpz_init(a);
        fmpz_init(c);
        mpz_init(d);
        mpz_init(f);
        mpz_init(g);

        fmpz_randtest(a, state, 200);
        n = z_randtest_not_zero();
        fmpz_mul_si(c, a, n);

        fmpz_get_mpz(d, c);

        fmpz_divexact_si(c, c, n);
        mpz_divexact_ui(f, d, FLINT_ABS(n));
        if (n < 0)
            mpz_neg(f, f);

        fmpz_get_mpz(g, c);

        result = (mpz_cmp(f, g) == 0);

        if (!result)
        {
            printf("FAIL;\n");
            gmp_printf("d = %Zd, n = %ld, f = %Zd, g = %Zd\n", d, n, f, g);
            abort();
        }

        fmpz_clear(a);
        fmpz_clear(c);
        mpz_clear(d);
        mpz_clear(f);
        mpz_clear(g);
    }

    fmpz_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
