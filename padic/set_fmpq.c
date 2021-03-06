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

    Copyright (C) 2011 Sebastian Pancratz
 
******************************************************************************/

#include "padic.h"

void padic_set_fmpq(padic_t rop, const fmpq_t op, const padic_ctx_t ctx)
{
    if (fmpq_is_zero(op))
    {
        padic_zero(rop);
    }
    else
    {
        fmpq_t t;

        fmpq_init(t);

        padic_val(rop)  = fmpz_remove(fmpq_numref(t), fmpq_numref(op), ctx->p);
        padic_val(rop) -= fmpz_remove(fmpq_denref(t), fmpq_denref(op), ctx->p);

        if (padic_val(rop) >= ctx->N)
        {
            padic_zero(rop);
        }
        else
        {
            _padic_inv(fmpq_denref(t), 
                       fmpq_denref(t), ctx->p, ctx->N - padic_val(rop));
            fmpz_mul(padic_unit(rop), fmpq_numref(t), fmpq_denref(t));
            _padic_reduce(rop, ctx);
        }
        fmpq_clear(t);
    }
}

