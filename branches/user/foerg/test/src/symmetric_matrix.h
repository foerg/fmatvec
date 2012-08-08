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

#ifndef symmetric_matrix_h
#define symmetric_matrix_h

#include "types.h"
#include "matrix.h"
#include "_memory.h"

namespace fmatvec {

  /*! 
   *  \brief This is a matrix class for symmetric matrices.
   *
   * Template class Matrix of shape type Symmetric. 
   * The template parameter AT defines the
   * atomic type of the matrix. Valid types are int, float,
   * double, complex<float> and complex<double> 
   * */
  template <class AT> class Matrix<Symmetric,Ref,Ref,AT> {

    protected:

    /// @cond NO_SHOW

      Memory<AT> memory;
      AT *ele;
      int n;
      int lda;

      template <class Type, class Row, class Col> inline void deepCopy(const Matrix<Type,Row,Col,AT> &A); 
      inline void deepCopy(const Matrix<Symmetric,Ref,Ref,AT> &A); 

      const AT* elePtr(int i, int j) const {
	return  j > i ? ele+i*lda+j : ele+i+j*lda; 
      };

      AT* elePtr(int i, int j) {
	return  j > i ? ele+i*lda+j : ele+i+j*lda; 
      };

      Matrix(int n_, int lda_, Memory<AT> memory_, const AT* ele_) : memory(memory_), ele((AT*)ele_), n(n_), lda(lda_) {
      }

    /// @endcond

    public:

      /*! \brief Standard constructor
       *
       * Constructs a matrix with no size. 
       * */
      Matrix() : memory(), ele(0), n(0), lda(0) {
      }

//      template<class Ini=All<AT> >
//        Matrix(int n_, Ini ini=All<AT>()) : memory(n_*n_), ele((AT*)memory.get()), n(n_), lda(n_) {
//          init(ini);
//        }
//      template<class Ini=All<AT> >
//        Matrix(int m_, int n_, Ini ini=All<AT>()) : memory(n_*n_), ele((AT*)memory.get()), n(n_), lda(n_) {
//          init(ini);
//        }

      Matrix(int n_, Noinit) : memory(n_*n_), ele((AT*)memory.get()), n(n_), lda(n_) { }
      Matrix(int n_, Init ini=INIT, const AT &a=0) : memory(n_*n_), ele((AT*)memory.get()), n(n_), lda(n_) { init(a); }
      Matrix(int n_, Eye ini, const AT &a=1) : memory(n_*n_), ele((AT*)memory.get()), n(n_), lda(n_) { init(ini,a); }
      Matrix(int m_, int n_, Noinit) : memory(n_*n_), ele((AT*)memory.get()), n(n_), lda(n_) { }
      Matrix(int m_, int n_, Init ini=INIT, const AT &a=0) : memory(n_*n_), ele((AT*)memory.get()), n(n_), lda(n_) { init(a); }
      Matrix(int m_, int n_, Eye ini, const AT &a=1) : memory(n_*n_), ele((AT*)memory.get()), n(n_), lda(n_) { init(ini,a); }

      /*! \brief Copy Constructor
       *
       * Constructs a reference to the matrix \em A.
       * \attention The physical memory of the matrix \em A will not be copied, only
       * referenced.
       * \param A The matrix that will be referenced.
       * */
      Matrix(const Matrix<Symmetric,Ref,Ref,AT> &A) : memory(A.memory), ele(A.ele) , n(A.n), lda(A.lda) { }


      /*! \brief Element operator
       *
       * See Matrix(const Matrix<Symmetric,Ref,Ref,T>&) 
       * */
      explicit Matrix(const Matrix<General,Ref,Ref,AT>&  A) : memory(A.memory), ele(A.ele) , n(A.n), lda(A.lda) {
#ifndef FMATVEC_NO_SIZE_CHECK
	assert(A.rows() == A.cols());
#endif
      }

      template<class Type, class Row, class Col>
      explicit Matrix(const Matrix<Type,Row,Col,AT> &A) : memory(A.rows()*A.cols()), ele((AT*)memory.get()), n(A.cols()), lda(A.cols()) {
#ifndef FMATVEC_NO_SIZE_CHECK
	assert(A.rows() == A.cols());
#endif

	deepCopy(A);
      }

      /*! \brief Regular Constructor
       *
       * Constructs a symmetric matrix of size n x n with 
       * the pyhsical memory given by \em ele_.
       * \param n_ The number of rows and columns.
       * \param ele_ The physical memory the matrix will point to.
       * */
      Matrix(int n_, AT* ele_) : memory(), ele(ele_), n(n_), lda(n_) { }

      /*! \brief Destructor. 
       * */
      ~Matrix() { }

      Matrix<Symmetric,Ref,Ref,AT>& resize() { 
	n = lda = 0;
	memory.resize(0);
	ele = 0;
        return *this;
      }

      Matrix<Symmetric,Ref,Ref,AT>& resize(int n_, Noinit) { 
	n = n_; lda = n;
	memory.resize(n*n);
	ele = (AT*)memory.get();
        return *this;
      }

      Matrix<Symmetric,Ref,Ref,AT>& resize(int n, Init ini=INIT, const AT &a=0) { return resize(n,Noinit()).init(a); }

      Matrix<Symmetric,Ref,Ref,AT>& resize(int n, Eye ini, const AT &a=1) { return resize(n,Noinit()).init(ini,a); }

      /*! \brief Assignment operator
       *
       * Copies the symmetric matrix given by \em A by calling operator<<().
       * \param A The matrix to be assigned. 
       * \return A reference to the calling matrix.
       * \remark To call operator>>() by default, define FMATVEC_NO_DEEP_ASSIGNMENT
       * \sa operator<<(), operator>>()
       * */
      inline Matrix<Symmetric,Ref,Ref,AT>& operator=(const Matrix<Symmetric,Ref,Ref,AT> &A);

      template<class Type, class Row, class Col>
        inline Matrix<Symmetric,Ref,Ref,AT>& operator=(const Matrix<Type,Row,Col,AT> &A);

      /*! \brief Copy operator
       *
       * Copies the symmetric matrix given by \em A.
       * \param A The matrix to be copied. 
       * \return A reference to the calling matrix.
       * */
      template<class Type, class Row, class Col>
        inline Matrix<Symmetric,Ref,Ref,AT>& operator<<(const Matrix<Type,Row,Col,AT> &A);

      /*! \brief Reference operator
       *
       * References the symmetric matrix given by \em A.
       * \param A The matrix to be referenced. 
       * \return A reference to the calling matrix.
       * */
      inline Matrix<Symmetric,Ref,Ref,AT>& operator>>(const Matrix<Symmetric,Ref,Ref,AT> &A);

      /*! \brief Element operator
       *
       * Returns a reference to the element in the i-th row and the j-th column. 
       * \param i The i-th row of the matrix
       * \param j The j-th column of the matrix
       * \return A reference to the element A(i,j).
       * \remark The bounds are checked by default. 
       * To change this behavior, define
       * FMATVEC_NO_BOUNDS_CHECK.
       * \sa operator()(int,int) const
       * */
      AT& operator()(int i, int j) {
#ifndef FMATVEC_NO_BOUNDS_CHECK
	assert(i>=0);
	assert(j>=0);
	assert(i<n);
	assert(j<n);
#endif
	return e(i,j);
      };

      /*! \brief Element operator
       *
       * See operator()(int,int) 
       * */
      const AT& operator()(int i, int j) const {
#ifndef FMATVEC_NO_BOUNDS_CHECK
	assert(i>=0);
	assert(j>=0);
	assert(i<n);
	assert(j<n);
#endif

	return e(i,j);
      };

      AT& ei(int i, int j) {
	return ele[i+j*lda];
      };

      const AT& ei(int i, int j) const {
	return ele[i+j*lda];
      };

      AT& ej(int i, int j) {
	return ele[i*lda+j];
      };

      const AT& ej(int i, int j) const {
	return ele[i*lda+j];
      };

      AT& e(int i, int j) {
	return j > i ? ej(i,j) : ei(i,j);
      };

      const AT& e(int i, int j) const {
	return j > i ? ej(i,j) : ei(i,j);
      };

      /*! \brief Pointer operator.
       *
       * Returns the pointer to the first element.
       * \return The pointer to the first element.
       * */
      AT* operator()() {return ele;};

      /*! \brief Pointer operator
       *
       * See operator()() 
       * */
      const AT* operator()() const {return ele;};

      /*! \brief Size.
       *
       * \return The number of rows and columns of the matrix
       * */
      int size() const {return n;};

      /*! \brief Number of rows.
       *
       * \return The number of rows of the matrix
       * */
      int rows() const {return n;};

      /*! \brief Number of columns.
       *
       * \return The number of columns of the matrix
       * */
      int cols() const {return n;};

      /*! \brief Leading dimension.
       *
       * \return The leading dimension of the matrix
       * */
      int ldim() const {return lda;};

      /*! \brief Storage convention.
       *
       * Returns the blas-conform storage convention. 
       * The elements are stored in columnmajor form,
       * i.e. the elements are stored columnwise. 
       * \return CblasColMajor.
       * */
      const CBLAS_ORDER blasOrder() const {
	return  CblasColMajor;
      };

      /*! \brief Symmetry convention.
       *
       * Returns the blas-conform symmetry convention. 
       * The elements are stored in the lower triangular
       * part of the array,
       * \return CblasLower.
       * */
      const CBLAS_UPLO blasUplo() const {
	return  CblasLower;
      };

     /*! \brief Matrix duplicating.
       *
       * The calling matrix returns a \em deep copy of itself.  
       * \return The duplicate.
       * */
      inline Matrix<Symmetric,Ref,Ref,AT> copy() const;

      /*! \brief Initialization.
       *
       * Initializes all elements of the calling matrix with 
       * the value given by \em a.
       * \param a Value all elements will be initialized with.
       * \return A reference to the calling matrix.
       * */
      inline Matrix<Symmetric,Ref,Ref,AT>& init(const AT &a); 
      inline Matrix<Symmetric,Ref,Ref,AT>& init(Init, const AT &a=0) { return init(a); };
      inline Matrix<Symmetric,Ref,Ref,AT>& init(Eye, const AT &a=1);
      inline Matrix<Symmetric,Ref,Ref,AT>& init(Noinit, const AT &a=0) { return *this; }

      /*! \brief Submatrix operator.
       *
       * Returns a submatrix of the calling matrix. To preserve the
       * symmetry of the calling matrix, the submatrix is limited to the
       * lower and the upper triangular part of the matrix.
       * \attention The submatrix and the
       * calling matrix will share the same physical memory.
       * \param I Index containing the starting and the ending row. 
       * \param J Index containing the starting and the ending column. 
       * \return A submatrix of the calling matrix.
       * */
      inline Matrix<General,Ref,Ref,AT> operator()(const Index &I, const Index &J);

      /*! \brief Submatrix operator.
       *
       * See operator()(const Index&, const Index&)
       * */
      inline const Matrix<General,Ref,Ref,AT> operator()(const Index &I, const Index &J) const;

      /*! \brief Submatrix operator.
       *
       * Returns a submatrix of the calling matrix. To preserve the
       * symmetry of the calling matrix, the submatrix is limited to the
       * lower and the upper triangular part of the matrix.
       * \attention The submatrix and the
       * calling matrix will share the same physical memory.
       * \param I Index containing the starting and the 
       * ending row and column. 
       * \return A submatrix of the calling matrix.
       * */
      inline Matrix<Symmetric,Ref,Ref,AT> operator()(const Index &I);

      /*! \brief Submatrix operator.
       *
       * See operator()(const Index&)
       * */
      inline const Matrix<Symmetric,Ref,Ref,AT> operator()(const Index &I) const;

      /*! \brief Submatrix operator.
       *
       * Returns a submatrix of the calling matrix. 
       * \attention The submatrix and the
       * calling matrix will share the same physical memory.
       * \param i1 The starting row. 
       * \param j1 The starting column.
       * \param i2 The ending row.
       * \param j2 The ending column.
       * \return A submatrix of the calling matrix.
       * */
      inline Matrix<General,Ref,Ref,AT> operator()(int i1, int j1, int i2, int j2);

      /*! \brief Submatrix operator.
       *
       * See operator()(int,int,int,int);
       * */
      inline const Matrix<General,Ref,Ref,AT> operator()(int i1, int j1, int i2, int j2) const;

      /*! \brief Cast to std::vector<std::vector<AT> >.
       *
       * \return The std::vector<std::vector<AT> > representation of the matrix
       * */
      inline operator std::vector<std::vector<AT> >();
  };

  template <class AT>
    inline Matrix<Symmetric,Ref,Ref,AT>& Matrix<Symmetric,Ref,Ref,AT>::operator>>(const Matrix<Symmetric,Ref,Ref,AT> &A) { 

      n=A.n;
      memory = A.memory;
      ele = A.ele;
      lda = A.lda;

      return *this;
    }

  template <class AT>
    inline Matrix<Symmetric,Ref,Ref,AT>& Matrix<Symmetric,Ref,Ref,AT>::operator=(const Matrix<Symmetric,Ref,Ref,AT> &A) { 

      if(!ele) {
        n = A.size();
        lda = n;
        memory.resize(n*n);
        ele = (AT*)memory.get();
      } else {
#ifndef FMATVEC_NO_SIZE_CHECK
        assert(n == A.size());
#endif
      }

      deepCopy(A);

      return *this;
    }

  template <class AT> template< class Type, class Row, class Col>
    inline Matrix<Symmetric,Ref,Ref,AT>& Matrix<Symmetric,Ref,Ref,AT>::operator=(const Matrix<Type,Row,Col,AT> &A) { 
      
      if(!ele) {
        n = A.rows();
        lda = n;
        memory.resize(n*n);
        ele = (AT*)memory.get();
      } else {
#ifndef FMATVEC_NO_SIZE_CHECK
        assert(A.rows() == A.cols());
        assert(n == A.rows());
#endif
      }

      deepCopy(A);

      return *this;
    }


  template <class AT> template< class Type, class Row, class Col>
    inline Matrix<Symmetric,Ref,Ref,AT>& Matrix<Symmetric,Ref,Ref,AT>::operator<<(const Matrix<Type,Row,Col,AT> &A) { 
      
#ifndef FMATVEC_NO_SIZE_CHECK
      assert(A.rows() == A.cols());
#endif

      if(n!=A.rows()) {
	n = A.rows();
	lda = n;
	memory.resize(n*n);
	ele = (AT*)memory.get();
      } 

      deepCopy(A);

      return *this;
    }

  template <class AT>
    inline Matrix<Symmetric,Ref,Ref,AT>&  Matrix<Symmetric,Ref,Ref,AT>::init(const AT &val) {

      for(int i=0; i<rows(); i++) 
        for(int j=i; j<cols(); j++) 
          ej(i,j) = val;

      return *this;
    }

  template <class AT>
    inline Matrix<Symmetric,Ref,Ref,AT>&  Matrix<Symmetric,Ref,Ref,AT>::init(Eye, const AT &val) {

      for(int i=0; i<size(); i++) {
        ej(i,i) = val;
        for(int j=i+1; j<size(); j++) {
          ej(i,j) = 0; 
        }
      }
      return *this;
    }

  template <class AT>
    inline Matrix<Symmetric,Ref,Ref,AT> Matrix<Symmetric,Ref,Ref,AT>::copy() const {

      Matrix<Symmetric,Ref,Ref,AT> A(n,NONINIT);
      A.deepCopy(*this);

      return A;
    }


  template <class AT> 
    inline const Matrix<General,Ref,Ref,AT> Matrix<Symmetric,Ref,Ref,AT>::operator()(int i1, int j1, int i2, int j2) const {
      return operator()(Index(i1,i2),Index(j1,j2));
    }

  template <class AT> 
    inline Matrix<General,Ref,Ref,AT> Matrix<Symmetric,Ref,Ref,AT>::operator()(int i1, int j1, int i2, int j2) {
      return operator()(Index(i1,i2),Index(j1,j2));
    }

  template <class AT> 
    inline const Matrix<General,Ref,Ref,AT> Matrix<Symmetric,Ref,Ref,AT>::operator()(const Index &I, const Index &J) const {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(I.end()<n);
      assert(J.end()<n);
#endif

      if(I.start() >= J.start()) {
        assert(J.end() <= I.start());
        return Matrix<General,Ref,Ref,AT>(I.end()-I.start()+1,J.end()-J.start()+1,lda,false,memory,elePtr(I.start(),J.start()));
      } else {
        assert(I.end() <= J.start());
        return Matrix<General,Ref,Ref,AT>(I.end()-I.start()+1,J.end()-J.start()+1,lda,true,memory,elePtr(J.start(),I.start()));
      }
    }

  template <class AT> 
    inline Matrix<General,Ref,Ref,AT> Matrix<Symmetric,Ref,Ref,AT>::operator()(const Index &I, const Index &J) {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(I.end()<n);
      assert(J.end()<n);
#endif

      if(I.start() >= J.start()) {
        assert(J.end() <= I.start());
        return Matrix<General,Ref,Ref,AT>(I.end()-I.start()+1,J.end()-J.start()+1,lda,false,memory,elePtr(I.start(),J.start()));
      } else {
        assert(I.end() <= J.start());
        return Matrix<General,Ref,Ref,AT>(I.end()-I.start()+1,J.end()-J.start()+1,lda,true,memory,elePtr(J.start(),I.start()));
      }
    }

  template <class AT> 
    inline Matrix<Symmetric,Ref,Ref,AT> Matrix<Symmetric,Ref,Ref,AT>::operator()(const Index &I) {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(I.end()<n);
#endif

      return Matrix<Symmetric,Ref,Ref,AT>(I.end()-I.start()+1,lda,memory,elePtr(I.start(),I.start()));
    }

  template <class AT> 
    inline const Matrix<Symmetric,Ref,Ref,AT> Matrix<Symmetric,Ref,Ref,AT>::operator()(const Index &I) const {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(I.end()<n);
#endif

      return Matrix<Symmetric,Ref,Ref,AT>(I.end()-I.start()+1,lda,memory,elePtr(I.start(),I.start()));
    }

  template <class AT>
    inline Matrix<Symmetric,Ref,Ref,AT>::operator std::vector<std::vector<AT> >() {
      std::vector<std::vector<AT> > ret(rows());
      for(int r=0; r<rows(); r++) {
        ret[r].resize(cols());
        for(int c=0; c<cols(); c++)
          ret[r][c]=operator()(r,c);
      }
      return ret;
    }

  /// @cond NO_SHOW

  template <class AT>
    inline void Matrix<Symmetric,Ref,Ref,AT>::deepCopy(const Matrix<Symmetric,Ref,Ref,AT> &A) { 
      for(int i=0; i<n; i++) 
        for(int j=i; j<n; j++) 
          ej(i,j) = A.ej(i,j);
    }

  template <class AT> template <class Type, class Row, class Col>
    inline void Matrix<Symmetric,Ref,Ref,AT>::deepCopy(const Matrix<Type,Row,Col,AT> &A) { 
      for(int i=0; i<n; i++) 
        for(int j=i; j<n; j++) 
          ej(i,j) = A.e(i,j);
    }

  /// @endcond

}

#endif
