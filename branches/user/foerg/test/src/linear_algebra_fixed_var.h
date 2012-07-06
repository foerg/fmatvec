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
 *   martin.o.foerg@googlemail.com
 *
 */

#ifndef linear_algebra_fixed_var_h
#define linear_algebra_fixed_var_h

#include "fixed_var_general_matrix.h"
//#include "var_square_matrix.h"
#include "fixed_vector.h"
#include "var_vector.h"
//#include "var_row_vector.h"
#include "var_symmetric_matrix.h"

namespace fmatvec {

  template <int M, class AT>
    inline Matrix<General<Fixed<M>,Var>, AT> operator+(const Matrix<General<Fixed<M>,Var>, AT> &A1, const Matrix<General<Fixed<M>,Var>, AT> &A2) {
#ifndef FMATVEC_NO_SIZE_CHECK
      assert(A1.cols() == A2.cols());
#endif
      Matrix<General<Fixed<M>,Var>, AT> A3(A1.cols(),NONINIT);
      for(int i=0; i<M; i++) 
        for(int j=0; j<A3.cols(); j++) 
          A3.e(i,j) = A1.e(i,j) + A2.e(i,j);
      return A3;
    }

  template <int M, class AT>
    inline Matrix<General<Fixed<M>,Var>, AT> operator-(const Matrix<General<Fixed<M>,Var>, AT> &A1, const Matrix<General<Fixed<M>,Var>, AT> &A2) {
#ifndef FMATVEC_NO_SIZE_CHECK
      assert(A1.cols() == A2.cols());
#endif
      Matrix<General<Fixed<M>,Var>, AT> A3(A1.cols(),NONINIT);
      for(int i=0; i<M; i++) 
        for(int j=0; j<A3.cols(); j++) 
          A3.e(i,j) = A1.e(i,j) - A2.e(i,j);
      return A3;
    }

  template <int M, class AT>
    inline Matrix<General<Fixed<M>,Var>, AT>& operator+=(const Matrix<General<Fixed<M>,Var>, AT> &A1_, const Matrix<General<Fixed<M>,Var>, AT> &A2) {
#ifndef FMATVEC_NO_SIZE_CHECK
      assert(A1_.cols() == A2.cols());
#endif
      Matrix<General<Fixed<M>,Var>, AT> &A1 = const_cast<Matrix<General<Fixed<M>,Var>, AT> &>(A1_);
      for(int i=0; i<M; i++) 
        for(int j=0; j<A1.cols(); j++) 
	A1.e(i,j) += A2.e(i,j);
      return A1;
    }

  template <int M, int N, class AT>
    inline Matrix<General<Fixed<M>,Var>, AT>& operator+=(const Matrix<General<Fixed<M>,Var>, AT> &A1_, const Matrix<General<Fixed<M>,Fixed<N> >, AT> &A2) {
#ifndef FMATVEC_NO_SIZE_CHECK
      assert(A1_.cols() == A2.cols());
#endif
      Matrix<General<Fixed<M>,Var>, AT> &A1 = const_cast<Matrix<General<Fixed<M>,Var>, AT> &>(A1_);
      for(int i=0; i<M; i++) 
        for(int j=0; j<A1.cols(); j++) 
	A1.e(i,j) += A2.e(i,j);
      return A1;
    }


  template <int M, class AT>
    inline Matrix<General<Fixed<M>,Var>, AT>& operator-=(const Matrix<General<Fixed<M>,Var>, AT> &A1_, const Matrix<General<Fixed<M>,Var>, AT> &A2) {
#ifndef FMATVEC_NO_SIZE_CHECK
      assert(A1_.cols() == A2.cols());
#endif
      Matrix<General<Fixed<M>,Var>, AT> &A1 = const_cast<Matrix<General<Fixed<M>,Var>, AT> &>(A1_);
      for(int i=0; i<M; i++) 
        for(int j=0; j<A1.cols(); j++) 
	A1.e(i,j) -= A2.e(i,j);
      return A1;
    }



  template <int M, class AT>
    inline Matrix<Symmetric<Var,Var>, AT> JTJ(const Matrix<General<Fixed<M>,Var>, AT> &A) { 
      Matrix<Symmetric<Var,Var>, AT> S(A.cols(),NONINIT);
      for(int i=0; i<A.cols(); i++) {
	for(int k=i; k<A.cols(); k++) {
	  S.ej(i,k) = 0;
	  for(int j=0; j<M; j++) 
	    S.ej(i,k) += A.e(j,i)*A.e(j,k);
	}
      }
      return S;
    }

  template <int M, class AT>
  inline Matrix<Symmetric<Var,Var>, AT> JTMJ(const Matrix<Symmetric<Fixed<M>,Fixed<M> >, AT> &B, const Matrix<General<Fixed<M>,Var>, AT> &A) {

    Matrix<Symmetric<Var,Var>, AT> S(A.cols(),NONINIT);
    Matrix<General<Fixed<M>,Var>, AT> C = B*A;

    for(int i=0; i<A.cols(); i++) {
	for(int k=i; k<A.cols(); k++) {
	  S.ej(i,k) = 0;
	  for(int j=0; j<M; j++) 
	    S.ej(i,k) += A.e(j,i)*C.e(j,k);
	}
      }
    return S;
  }

  template <int M, class AT>
    inline Matrix<General<Fixed<M>,Var>, AT> operator-(const Matrix<General<Fixed<M>,Var>, AT> &A) {

      Matrix<General<Fixed<M>,Var>, AT> B(A.cols(),NONINIT);

      for(int i=0; i<M; i++) 
        for(int j=0; j<A.cols(); j++) 
	B.e(i,j)=-A.e(i,j);

      return B;
    }

  template <int M, class AT>
    inline Matrix<General<Fixed<M>,Var>, AT> operator*(const Matrix<General<Fixed<M>,Var>,AT> &A, const AT &a) {
      Matrix<General<Fixed<M>,Var>, AT> B(A.cols(),NONINIT);

      for(int i=0; i<M; i++)
	for(int j=0; j<A.cols(); j++)
	  B.e(i,j) = a*A.e(i,j);
      return B;
    }
}

#endif
