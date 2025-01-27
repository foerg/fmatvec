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

#ifndef fixed_symmetric_matrix_h
#define fixed_symmetric_matrix_h

#include "types.h"
#include <vector>

namespace fmatvec {

  /*! 
   *  \brief This is a matrix class for symmetric matrices.
   *
   * Template class Matrix of shape type SymmetricFixed. 
   * The template parameter AT defines the
   * atomic type of the matrix. Valid types are int, float,
   * double, complex<float> and complex<double> 
   * */
  template <int M, class AT> class Matrix<Symmetric,Fixed<M>,Fixed<M>,AT> {

    protected:

    /// @cond NO_SHOW

      AT ele[M][M];

      template <class Type, class Row, class Col> inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& copy(const Matrix<Type,Row,Col,AT> &A);
      template <class Row> inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& copy(const Matrix<Symmetric,Row,Row,AT> &A);

    /// @endcond

    public:
      static constexpr bool isVector {false};
      using value_type = AT;
      using shape_type = Symmetric;

      explicit Matrix(Noinit ini) { }
      explicit Matrix(Init ini=INIT, const AT &a=AT()) { init(a); }
      explicit Matrix(Eye ini, const AT &a=1) { init(ini,a); }
      explicit Matrix(int m, int n, Noinit ini) { FMATVEC_ASSERT(m==M && n==M, AT); }
      explicit Matrix(int m, int n, Init ini, const AT &a=AT()) { FMATVEC_ASSERT(m==M && n==M, AT); init(a); }
      explicit Matrix(int m, int n, Eye ini, const AT &a=1) { FMATVEC_ASSERT(m==M && n==M, AT); init(ini,a); }

      /*! \brief Copy Constructor
       *
       * Constructs a copy of the matrix \em A.
       * \param A The matrix that will be copied.
       * */
      Matrix(const Matrix<Symmetric,Fixed<M>,Fixed<M>,AT> &A) = default;

      /*! \brief Copy Constructor
       *
       * Constructs a copy of the matrix \em A.
       * \param A The matrix that will be copied.
       * */
      template<class Row>
      Matrix(const Matrix<Symmetric,Row,Row,AT> &A) {
	FMATVEC_ASSERT(A.size() == M, AT); 
	copy(A);
      }

      /*! \brief Copy Constructor
       *
       * Constructs a copy of the matrix \em A.
       * \param A The matrix that will be copied.
       * */
      template<class Type, class Row, class Col>
      explicit Matrix(const Matrix<Type,Row,Col,AT> &A) {
	FMATVEC_ASSERT(A.rows() == M, AT); 
	FMATVEC_ASSERT(A.cols() == M, AT);
	copy(A);
      }

      /*! \brief Assignment operator
       *
       * Copies the symmetric matrix given by \em A.
       * \param A The matrix to be assigned.
       * \return A reference to the calling matrix.
       * */
      inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& operator=(const Matrix<Symmetric,Fixed<M>,Fixed<M>,AT> &A) = default;

      /*! \brief Assignment operator
       *
       * Copies the symmetric matrix given by \em A.
       * \param A The matrix to be assigned.
       * \return A reference to the calling matrix.
       * */
      template<class Type, class Row, class Col>
      inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& operator=(const Matrix<Type,Row,Col,AT> &A) {
        FMATVEC_ASSERT(A.rows() == A.cols(), AT);
        FMATVEC_ASSERT(M == A.rows(), AT);
        return copy(A);
      }

      /*! \brief Matrix assignment
       *
       * Copies the matrix given by \em A.
       * \param A The matrix to be copied.
       * \return A reference to the calling matrix.
       * */
      template<class Type, class Row, class Col>
      inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& operator<<=(const Matrix<Type,Row,Col,AT> &A) { return operator=(A); }

      //! Resize a fixed matrix
      //! Do nothing for the fixed dimension and throw for any other dimension.
      void resize(int n, int m) {
        if(n!=M || m!=M)
          throw std::runtime_error("A fixed symmetric matrix cannot be resized.");
      }

      /*! \brief Element operator
       *
       * Returns a reference to the element in the i-th row and the j-th column. 
       * \param i The i-th row of the matrix
       * \param j The j-th column of the matrix
       * \return A reference to the element A(i,j).
       * \remark The bounds are checked in debug mode.
       * \sa operator()(int,int) const
       * */
      AT& operator()(int i, int j) {
	FMATVEC_ASSERT(i>=0, AT);
	FMATVEC_ASSERT(j>=0, AT);
	FMATVEC_ASSERT(i<M, AT);
	FMATVEC_ASSERT(j<M, AT);

	return e(i,j);
      }

      /*! \brief Element operator
       *
       * See operator()(int,int) 
       * */
      const AT& operator()(int i, int j) const {
	FMATVEC_ASSERT(i>=0, AT);
	FMATVEC_ASSERT(j>=0, AT);
	FMATVEC_ASSERT(i<M, AT);
	FMATVEC_ASSERT(j<M, AT);

	return e(i,j);
      }

      AT& ei(int i, int j) {
	return ele[i][j];
      }

      const AT& ei(int i, int j) const {
	return ele[i][j];
      }

      AT& ej(int i, int j) {
	return ele[j][i];
      }

      const AT& ej(int i, int j) const {
	return ele[j][i];
      }

       AT& e(int i, int j) {
	return j > i ? ej(i,j) : ei(i,j);
      }

      const AT& e(int i, int j) const {
	return j > i ? ej(i,j) : ei(i,j);
      }

     /*! \brief Pointer operator.
       *
       * Returns the pointer to the first element.
       * \return The pointer to the first element.
       * */
      AT* operator()() {return ele;}

      /*! \brief Pointer operator
       *
       * See operator()() 
       * */
      const AT* operator()() const {return ele;}

      /*! \brief Size.
       *
       * \return The number of rows and columns of the matrix
       * */
      constexpr int size() const {return M;}

      /*! \brief Number of rows.
       *
       * \return The number of rows of the matrix
       * */
      constexpr int rows() const {return M;}

      /*! \brief Number of columns.
       *
       * \return The number of columns of the matrix
       * */
      constexpr int cols() const {return M;}

      /*! \brief Leading dimension.
       *
       * \return The leading dimension of the matrix
       * */
      int ldim() const {return M;}

      /*! \brief Storage convention.
       *
       * Returns the blas-conform storage convention. 
       * The elements are stored in columnmajor form,
       * i.e. the elements are stored columnwise. 
       * \return CblasRowMajor.
       * */
      CBLAS_ORDER blasOrder() const {
        return CblasRowMajor;
      }

      /*! \brief Symmetry convention.
       *
       * Returns the blas-conform symmetry convention. 
       * The elements are stored in the lower triangular
       * part of the array,
       * \return CblasLower.
       * */
      CBLAS_UPLO blasUplo() const {
        return  CblasLower;
      }

      /*! \brief Initialization.
       *
       * Initializes all elements of the calling matrix with 
       * the value given by \em a.
       * \param a Value all elements will be initialized with.
       * \return A reference to the calling matrix.
       * */
      inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& init(const AT &val=AT()); 
      inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& init(Init, const AT &a=AT()) { return init(a); }
      inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& init(Eye, const AT &val=1);
      inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& init(Noinit, const AT &a=AT()) { return *this; }

      /*! \brief Cast to std::vector<std::vector<AT>>.
       *
       * \return The std::vector<std::vector<AT>> representation of the matrix
       * */
      explicit inline operator std::vector<std::vector<AT>>() const;

      /*! \brief std::vector<std::vector<AT>> Constructor.
       * Constructs and initializes a matrix with a std::vector<std::vector<AT>> object.
       * An FMATVEC_ASSERT checks for constant length of each row.
       * \param m The std::vector<std::vector<AT>> the matrix will be initialized with.
       * */
      explicit inline Matrix(const std::vector<std::vector<AT>> &m);

//      /*! \brief Cast to AT.
//       *
//       * \return The AT representation of the matrix
//       * */
//      explicit operator AT() const {
//        FMATVEC_ASSERT(M==1, AT);
//        return ele[0][0];
//      }
//
//      /*! \brief AT Constructor.
//       * Constructs and initializes a matrix with a AT object.
//       * \param x The AT the matrix will be initialized with.
//       * */
//      explicit Matrix(const AT &x) {
//        FMATVEC_ASSERT(M==1, AT);
//        ele[0][0] = x;
//      }
  };

  template <int M, class AT>
    inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>::init(const AT &val) {

      for(int i=0; i<M; i++) 
        for(int j=i; j<M; j++) 
          ej(i,j) = val;

      return *this;
    }

  template <int M, class AT>
    inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>::init(Eye, const AT &val) {

      for(int i=0; i<size(); i++) {
        ej(i,i) = val;
        for(int j=i+1; j<size(); j++) {
          ej(i,j) = 0; 
        }
      }
      return *this;
    }

  template <int M, class AT>
    inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>::operator std::vector<std::vector<AT>>() const {
      std::vector<std::vector<AT>> ret(rows(),std::vector<AT>(cols()));
      for(int r=0; r<rows(); r++) {
        for(int c=r; c<cols(); c++) {
          ret[r][c]=ej(r,c);
	  if(c>r)
	    ret[c][r]=ej(r,c);
	}
      }
      return ret;
    }

  template <int M, class AT>
    inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>::Matrix(const std::vector<std::vector<AT>> &m) {
      if(static_cast<int>(m.size())!=rows())
        throw std::runtime_error("The number of rows does not match.");
      for(int r=0; r<rows(); r++) {
        if(static_cast<int>(m[r].size())!=cols())
          throw std::runtime_error("The rows of the input have different length.");
        for(int c=r; c<cols(); c++) {
          using std::abs;
          ej(r,c)=m[r][c];
          if(c>r && abs(m[r][c]-m[c][r])>abs(m[r][c]*1e-13+1e-13))
            throw std::runtime_error("The input is not symmetric.");
        }
      }
    }

  /// @cond NO_SHOW

  template <int M, class AT> template <class Type, class Row, class Col>
    inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>::copy(const Matrix<Type,Row,Col,AT> &A) {
      for(int i=0; i<M; i++) 
        for(int j=i; j<M; j++)
          ej(i,j) = A.e(i,j);
      return *this;
    }

  template <int M, class AT> template <class Row>
    inline Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>& Matrix<Symmetric,Fixed<M>,Fixed<M>,AT>::copy(const Matrix<Symmetric,Row,Row,AT> &A) {
      for(int i=0; i<M; i++) 
        for(int j=i; j<M; j++)
          ej(i,j) = A.ej(i,j);
      return *this;
    }

  /// @endcond

}

#endif
