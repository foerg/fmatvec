/* Copyright (C) 2003-2005  Martin Förg

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
 *   martin.foerg@web.de
 *
 */

#include "config.h"
#include "diagonal_matrix.h"

extern "C" {
#include "cblas.h"
}

namespace fmatvec {
//const double Matrix<Diagonal, double>::zero = 0;

  template <>
  void Matrix<Diagonal, double>::deepCopy(const Matrix<Diagonal, double> &A) { 
    cblas_dcopy(n, A.ele,1, ele,1);
  }

}



