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
 *   mfoerg@users.berlios.de
 *
 */

#include "config.h"
#include "vector.h"

#define FMATVEC_NO_INITIALIZATION
#define FMATVEC_NO_BOUNDS_CHECK

#include "wrapper.h"

namespace fmatvec {

  template <>
  Vector<double>::Vector(const char *strs) : Matrix<General,double>(strs) {
#ifdef FMATVEC_SIZE_CHECK
    assert(n==1);
#endif
  }

  template <>
  void Vector<double>::deepCopy(const Vector<double> &x) { 
    dcopy(m, x.ele, x.inc(), ele, inc());
  }

}
