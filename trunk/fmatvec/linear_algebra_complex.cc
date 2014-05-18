/* Copyright (C) 2003-2014  Martin Förg

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Contact:
 *   martin.o.foerg@googlemail.com
 *
 */

#include "config.h"
#include "linear_algebra.h"
#include "linear_algebra_fixed.h"
#include "linear_algebra_var.h"
#include "linear_algebra_fixed_var.h"
#include "linear_algebra_var_fixed.h"
#include "linear_algebra_double.h"
#include "wrapper.h"
#include <stdexcept>
#include <sstream>

#define FMATVEC_NO_INITIALIZATION
#define FMATVEC_NO_BOUNDS_CHECK

#define CVT_TRANSPOSE(c) \
   (((c) == CblasNoTrans) ? 'N' : \
    ((c) == CblasTrans) ? 'T' : \
    ((c) == CblasConjTrans) ? 'C' : \
    -1)

#define CVT_UPLO(c) \
   (((c) == CblasUpper) ? 'U' : \
    ((c) == CblasLower) ? 'L' : \
    -1)

#define CVT_SIDE(c) \
   (((c) == CblasLeft) ? 'L' : \
    ((c) == CblasRight) ? 'R' : \
    -1)

template <class T>
std::string toStr(const T& val) {
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

namespace fmatvec {

  Vector<Ref, std::complex<double> > slvLU(const SquareMatrix<Ref, std::complex<double> > &A, const Vector<Ref, std::complex<double> > &x) {

#ifndef FMATVEC_NO_SIZE_CHECK
    assert(A.size() == x.size());
#endif

    Vector<Ref, std::complex<double> > y = x.copy();

#ifndef FMATVEC_NO_VOID_CHECK
    if (x.size() == 0)
      return y;
#endif

    SquareMatrix<Ref, std::complex<double> > B = A.copy();

    int *ipiv = new int[A.size()];

    int info = zgesv(B.blasOrder(), B.size(), 1, B(), B.ldim(), ipiv, y(), y.size());

    delete[] ipiv;

    if(info != 0)
      throw std::runtime_error("Exception in slvLU: dgesv exited with info="+toStr(info));

    return y;
  }

}
