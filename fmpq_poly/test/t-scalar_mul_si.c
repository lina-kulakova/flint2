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
#include "fmpq_poly.h"
#include "long_extras.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    fmpz_randstate_t state;

    printf("scalar_mul_si....");
    fflush(stdout);

    fmpq_poly_randinit(state);

    /* Check aliasing of a and b */
    for (i = 0; i < 10000; i++)
    {
        fmpq_poly_t a, b;
        long n = z_randtest();

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_randtest(a, state, n_randint(100), n_randint(200));

        fmpq_poly_scalar_mul_si(b, a, n);
        fmpq_poly_scalar_mul_si(a, a, n);

        result = (fmpq_poly_equal(a, b));
        if (!result)
        {
            printf("FAIL:\n");
            fmpq_poly_print(a), printf("\n\n");
            fmpq_poly_print(b), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
    }

    /* Compare with fmpq_poly_scalar_mul_ui */
    for (i = 0; i < 10000; i++)
    {
        fmpq_poly_t a, b;
        ulong n = n_randbits(FLINT_BITS - 1);

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_randtest(a, state, n_randint(100), n_randint(200));

        fmpq_poly_scalar_mul_ui(b, a, n);
        fmpq_poly_scalar_mul_si(a, a, n);

        result = (fmpq_poly_equal(a, b));
        if (!result)
        {
            printf("FAIL:\n");
            fmpq_poly_print(a), printf("\n\n");
            fmpq_poly_print(b), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
    }

    /* Check n2 * (n1 a) == (n1*n2) a */
    for (i = 0; i < 10000; i++)
    {
        fmpq_poly_t a, b, c;
        long n1, n2;
        ulong m;

        n1 = (long) n_randbits(FLINT_BITS / 2);
        n2 = (long) n_randbits(FLINT_BITS / 2 - 1);
        m = n_randlimb();
        if (m & 1UL)
            n1 = -n1;
        if (m & 2UL)
            n2 = -n2;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(c);
        fmpq_poly_randtest(a, state, n_randint(100), n_randint(200));

        fmpq_poly_scalar_mul_si(b, a, n1);
        fmpq_poly_scalar_mul_si(c, b, n2);
        fmpq_poly_scalar_mul_si(b, a, n1 * n2);

        result = (fmpq_poly_equal(b, c));
        if (!result)
        {
            printf("FAIL:\n");
            printf("n1 = %lu, n2 = %lu:\n\n", n1, n2);
            fmpq_poly_print(a), printf("\n\n");
            fmpq_poly_print(b), printf("\n\n");
            fmpq_poly_print(c), printf("\n\n");
            abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(c);
    }

    fmpq_poly_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
