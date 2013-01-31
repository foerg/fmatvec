/* Copyright (C) 2003-2012  Martin Förg

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

#ifndef var_general_matrix_h
#define var_general_matrix_h

#include "types.h"
#include <stdlib.h>

namespace fmatvec {

  /*! 
   *  \brief This is a matrix class for general matrices.
   *  
   * Template class Matrix with shape type General and atomic type AT. The
   * storage form is dense. The template parameter AT defines the atomic type
   * of the matrix. Valid types are int, float, double, complex<float> and
   * complex<double> 
   * */
  template <class AT> class Matrix<General,Var,Var,AT> {

    public:

 /// @cond NO_SHOW

    protected:

      int M, N;

      AT *ele;

      template <class Type, class Row, class Col> inline void deepCopy(const Matrix<Type,Row,Col,AT> &A); 

 /// @endcond
 
    public:

     /*! \brief Standard constructor
       *
       * Constructs a matrix with no size. 
       * */
      Matrix() : M(0), N(0), ele(0) { }

// Works with -std=gnu++0x only
//      template<class Ini=All<AT> >
//      Matrix(int m, int n, Ini ini=All<AT>()) :  M(m), N(n), ele(new AT[M*N]) {
//        init(ini);
//      }

      Matrix(int m, int n, Noinit) :  M(m), N(n), ele(new AT[M*N]) { }
      Matrix(int m, int n, Init ini=INIT, const AT &a=0) :  M(m), N(n), ele(new AT[M*N]) { init(a); }
      Matrix(int m, int n, Eye ini, const AT &a=1) :  M(m), N(n), ele(new AT[M*N]) { init(ini,a); }

      /*! \brief Copy Constructor
       *
       * Constructs a reference to the matrix \em A.
       * \attention The physical memory of the matrix \em A will not be copied, only
       * referenced.
       * \param A The matrix that will be referenced.
       * */
      Matrix(const Matrix<General,Var,Var,AT> &A) : M(A.M), N(A.N), ele(new AT[M*N]) {
	deepCopy(A);
      }

      template<class Row, class Col>
      Matrix(const Matrix<General,Row,Col,AT> &A) : M(A.rows()), N(A.cols()), ele(new AT[M*N]) {

	deepCopy(A);
      }

      template<class Type, class Row, class Col>
      explicit Matrix(const Matrix<Type,Row,Col,AT> &A) : M(A.rows()), N(A.cols()), ele(new AT[M*N]) {

	deepCopy(A);
      }

      /*! \brief String Constructor. 
       *
       * Constructs and initializes a matrix with a string in a matlab-like
       * notation. The rows are seperated by semicolons, the columns by commas.
       * For example
       * \code 
       * Matrix<General,Var,Var,ouble> A("[3,2;1,2]");
       * \endcode 
       * constructs the matrix
       * \f[ A=\begin{pmatrix}3 & 2\\ 1 & 2\end{pmatrix}  \f]
       * \param str The string the matrix will be initialized with. 
       * */
      Matrix(const char *str);

      /*! \brief Destructor. 
       * */
      ~Matrix() {
	delete[] ele;
      }

       Matrix<General,Var,Var,AT>& resize() {
	delete[] ele;
	M = N = 0;
	ele = 0;
        return *this;
      }

       Matrix<General,Var,Var,AT>& resize(int m, int n, Noinit) {
	delete[] ele;
	M = m;
	N = n;
	ele = new AT[M*N];
        return *this;
      }

      Matrix<General,Var,Var,AT>& resize(int m, int n, Init ini=INIT, const AT &a=0) { return resize(m,n,Noinit()).init(a); }

      Matrix<General,Var,Var,AT>& resize(int m, int n, Eye ini, const AT &a=1) { return resize(m,n,Noinit()).init(ini,a); } 

     /*! \brief Assignment operator
       *
       * Copies the matrix given by \em A.
       * \param A The matrix to be assigned. 
       * \return A reference to the calling matrix.
       * */
      inline Matrix<General,Var,Var,AT>& operator=(const Matrix<General,Var,Var,AT> &A);

      template <class Type, class Row, class Col>
      inline Matrix<General,Var,Var,AT>& operator=(const Matrix<Type,Row,Col,AT> &A);

      /*! \brief Copy operator
       *
       * Copies the matrix given by \em A.
       * \param A The matrix to be copied. 
       * \return A reference to the calling matrix.
       * */
      template<class Type, class Row, class Col>
        inline Matrix<General,Var,Var,AT>& operator<<(const Matrix<Type,Row,Col,AT> &A);

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
	assert(i<M);
	assert(j<N);
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
	assert(i<M);
	assert(j<N);
#endif

	return e(i,j);
      };

      AT& e(int i, int j) {
	return ele[i*N+j];
      };

      /*! \brief Element operator
       *
       * See e(int,int) 
       * */
      const AT& e(int i, int j) const {
	return ele[i*N+j];
      };

      AT& e(int i) {
	return ele[i];
      };

      /*! \brief Element operator
       *
       * See e(int,int) 
       * */
      const AT& e(int i) const {
	return ele[i];
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

      /*! \brief Number of rows.
       *
       * \return The number of rows of the matrix.
       * */
      int rows() const {return M;};

      /*! \brief Number of columns.
       *
       * \return The number of columns of the matrix.
       * */
      int cols() const {return N;};

      /*! \brief Leading dimension.
       *
       * \return The leading dimension of the matrix
       * */
      int ldim() const {return M;};

      /*! \brief Transposed status.
       *
       * Returns the blas-conform transposed status.
       * \return CblasTrans if the matrix is in transposed form, CblasNoTrans
       * otherwise. 
       * */
      const CBLAS_TRANSPOSE blasTrans() const {
	return CblasNoTrans;
      };

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

      /*! \brief Submatrix operator.
       *
       * Returns a submatrix of the calling matrix. 
       * For example
       * \code 
       * B = A(Range<Var,Var>(1,2),Range<Var,Var>(2,4));
       * \endcode
       * yields
       * \f[ 
       * A=\begin{pmatrix}
       * 	a_{00} & a_{01} & a_{02} & a_{03} & a_{04}\\
       * 	a_{10} & a_{11} & a_{12} & a_{13} & a_{14}\\
       * 	a_{20} & a_{21} & a_{22} & a_{23} & a_{24}\\
       * 	a_{30} & a_{31} & a_{32} & a_{33} & a_{34}
       * \end{pmatrix}\quad \Rightarrow \quad
       * B=\begin{pmatrix}
       * 	 a_{12} & a_{13} & a_{14}\\
       * 	 a_{22} & a_{23} & a_{24}
       * \end{pmatrix}
       * \f]
       * \attention The submatrix and the
       * calling matrix will share the same physical memory.
       * \param I Range<Var,Var> containing the starting and the ending row. 
       * \param J Range<Var,Var> containing the starting and the ending column. 
       * \return A submatrix of the calling matrix.
       * */
      //inline Matrix<General,Var,Var,AT> operator()(const Range<Var,Var> &I, const Range<Var,Var> &J);

      /*! \brief Submatrix operator.
       *
       * See operator()(const Range<Var,Var>&, const Range<Var,Var>&)
       * */
      inline const Matrix<General,Var,Var,AT> operator()(const Range<Var,Var> &I, const Range<Var,Var> &J) const;

      template <int M1, int M2, int N1, int N2>
      inline const Matrix<General,Fixed<M2-M1+1>,Fixed<N2-N1+1>,AT> operator()(const Range<Fixed<M1>,Fixed<M2> > &I, const Range<Fixed<N1>,Fixed<N2> > &J) const;

      template <int M1, int M2>
      inline const Matrix<General,Fixed<M2-M1+1>,Var,AT> operator()(const Range<Fixed<M1>,Fixed<M2> > &I, const Range<Var,Var > &J) const;

      template <int N1, int N2>
      inline const Matrix<General,Var,Fixed<N2-N1+1>,AT> operator()(const Range<Var,Var> &I, const Range<Fixed<N1>,Fixed<N2> > &J) const;

      inline const RowVector<Var,AT> row(int i) const;
      inline const Vector<Var,AT> col(int j) const;

      /*! \brief Initialization.
       *
       * Initializes all elements of the calling matrix with 
       * the value given by \em a.
       * \param a Value all elements will be initialized with.
       * \return A reference to the calling matrix.
       * */
      inline Matrix<General,Var,Var,AT>& init(const AT &a=0);
      inline Matrix<General,Var,Var,AT>& init(Init all, const AT &a=0) { return init(a); }
      inline Matrix<General,Var,Var,AT>& init(Eye eye, const AT &a=1);
      inline Matrix<General,Var,Var,AT>& init(Noinit, const AT &a=0) { return *this; }

      /*! \brief Cast to std::vector<std::vector<AT> >.
       *
       * \return The std::vector<std::vector<AT> > representation of the matrix
       * */
      inline operator std::vector<std::vector<AT> >();

      /*! \brief std::vector<std::vector<AT> > Constructor.
       * Constructs and initializes a matrix with a std::vector<std::vector<AT> > object.
       * An assert checks for constant length of each row.
       * \param m The std::vector<std::vector<AT> > the matrix will be initialized with. 
       * */
      inline Matrix(std::vector<std::vector<AT> > m);

      inline const Matrix<General,Var,Var,AT> T() const;

      template<class Row> inline void set(int j, const Vector<Row,AT> &x);

      template<class Col> inline void set(int i, const RowVector<Col,AT> &x);

      template<class Type, class Row, class Col> inline void set(const Range<Var,Var> &I, const Range<Var,Var> &J, const Matrix<Type,Row,Col,AT> &A);

      template<class Row> inline void add(int j, const Vector<Row,AT> &x);

      template<class Col> inline void add(int i, const RowVector<Col,AT> &x);

      template<class Type, class Row, class Col> inline void add(const Range<Var,Var> &I, const Range<Var,Var> &J, const Matrix<Type,Row,Col,AT> &A);

  };

  template <class AT> 
    Matrix<General,Var,Var,AT>::Matrix(const char *strs) : M(0), N(0), ele(0) {
      // if 'strs' is a single scalar, surround it first with '[' and ']'.
      // This is more Matlab-like, because e.g. '5' and '[5]' is just the same.
      // (This functionallitiy is needed e.g. by MBXMLUtils (OpenMBV,MBSim))
      std::istringstream iss(strs);
      char c;
      iss>>c;
      if(c=='[') iss.str(strs);
      else iss.str(std::string("[")+strs+"]");

      int buf=0;
      iss >> c;
      iss >> c;
      if(c!=']') {
        iss.putback(c);
        AT x;
        do {
          iss >> x;
          iss >> c;
          if(c==';') {
            if(buf)
              assert(buf == N);

            buf=N;
            N=0;
            M++;
          }
          else if(c==',')
            N++;
          c='0';
        } while(iss);

        N++; M++;
        ele = new AT[M*N];
        iss.clear();
        iss.seekg(0);
        iss >> c;
        for(int i=0; i<M; i++)
          for(int j=0; j<N; j++) {
            iss >> e(i,j);
            iss >> c;
          }
      }
    }

  template <class AT> template< class Type, class Row, class Col>
    inline Matrix<General,Var,Var,AT>& Matrix<General,Var,Var,AT>::operator=(const Matrix<Type,Row,Col,AT> &A) { 

      if(!ele) {
        delete[] ele;
        M = A.rows(); 
        N = A.cols();
        ele = new AT[M*N];
      } else {
#ifndef FMATVEC_NO_SIZE_CHECK
        assert(M == A.rows());
        assert(N == A.cols());
#endif
      }

      deepCopy(A);

      return *this;
    }

  template <class AT>
    inline Matrix<General,Var,Var,AT>& Matrix<General,Var,Var,AT>::operator=(const Matrix<General,Var,Var,AT> &A) { 

      if(!ele) {
        delete[] ele;
        M = A.rows(); 
        N = A.cols();
        ele = new AT[M*N];
      } else {
#ifndef FMATVEC_NO_SIZE_CHECK
        assert(M == A.rows());
        assert(N == A.cols());
#endif
      }

      deepCopy(A);

      return *this;
    }

  template <class AT>
    inline Matrix<General,Var,Var,AT>&  Matrix<General,Var,Var,AT>::init(const AT &val) {
      for(int i=0; i<M*N; i++) 
        e(i) = val;
      return *this;
    }

  template <class AT>
    inline Matrix<General,Var,Var,AT>&  Matrix<General,Var,Var,AT>::init(Eye eye, const AT &val) {
      for(int i=0; i<M; i++)
        for(int j=0; j<N; j++)
          e(i,j) = (i==j) ? val : 0;
      return *this;
    }

  template <class AT> template< class Type, class Row, class Col>
    inline Matrix<General,Var,Var,AT>& Matrix<General,Var,Var,AT>::operator<<(const Matrix<Type,Row,Col,AT> &A) { 

      if(M!=A.rows() || N!=A.cols()) {
        delete[] ele;
        M = A.rows(); 
        N = A.cols();
        ele = new AT[M*N];
      }

      deepCopy(A);

      return *this;
    }

  template <class AT> 
    inline const Matrix<General,Var,Var,AT> Matrix<General,Var,Var,AT>::operator()(const Range<Var,Var> &I, const Range<Var,Var> &J) const {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(I.end()<M);
      assert(J.end()<N);
#endif
      Matrix<General,Var,Var,AT> A(I.end()-I.start()+1,J.end()-J.start()+1,NONINIT);

      for(int i=0; i<A.rows(); i++) 
        for(int j=0; j<A.cols(); j++)
          A.e(i,j) = e(I.start()+i,J.start()+j);

      return A;
    }

 template <class AT> template <int M1, int M2, int N1, int N2>
    inline const Matrix<General,Fixed<M2-M1+1>,Fixed<N2-N1+1>,AT> Matrix<General,Var,Var,AT>::operator()(const Range<Fixed<M1>,Fixed<M2> > &I, const Range<Fixed<N1>,Fixed<N2> > &J) const {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(M2<M);
      assert(N2<N);
#endif
      Matrix<General,Fixed<M2-M1+1>,Fixed<N2-N1+1>,AT> A(NONINIT);

      for(int i=0; i<A.rows(); i++) 
        for(int j=0; j<A.cols(); j++)
          A.e(i,j) = e(I.start()+i,J.start()+j);

      return A;
    }

 template <class AT> template <int M1, int M2>
    inline const Matrix<General,Fixed<M2-M1+1>,Var,AT> Matrix<General,Var,Var,AT>::operator()(const Range<Fixed<M1>,Fixed<M2> > &I, const Range<Var,Var> &J) const {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(M2<M);
      assert(J.end()<N);
#endif
      Matrix<General,Fixed<M2-M1+1>,Var,AT> A(J.end()-J.start()+1,NONINIT);

      for(int i=0; i<A.rows(); i++) 
        for(int j=0; j<A.cols(); j++)
          A.e(i,j) = e(I.start()+i,J.start()+j);

      return A;
    }

  template <class AT> template <int N1, int N2>
    inline const Matrix<General,Var,Fixed<N2-N1+1>,AT> Matrix<General,Var,Var,AT>::operator()(const Range<Var,Var> &I, const Range<Fixed<N1>,Fixed<N2> > &J) const {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(I.end()<M);
      assert(N2<N);
#endif
      Matrix<General,Var,Fixed<N2-N1+1>,AT> A(I.end()-I.start()+1,NONINIT);

      for(int i=0; i<A.rows(); i++) 
        for(int j=0; j<A.cols(); j++)
          A.e(i,j) = e(I.start()+i,J.start()+j);

      return A;
    }

  template <class AT>
    inline const RowVector<Var,AT> Matrix<General,Var,Var,AT>::row(int i) const {

#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(i>=0);
      assert(i<M);
#endif

      RowVector<Var,AT> x(N,NONINIT);

      for(int j=0; j<N; j++)
        x.e(j) = e(i,j);

      return x;

    }

  template <class AT>
    inline const Vector<Var,AT> Matrix<General,Var,Var,AT>::col(int j) const {

#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(j>=0);
      assert(j<N);
#endif

      Vector<Var,AT> x(M,NONINIT);

      for(int i=0; i<M; i++)
        x.e(i) = e(i,j);

      return x;

    }

  template <class AT>
    inline const Matrix<General,Var,Var,AT> Matrix<General,Var,Var,AT>::T() const {
      Matrix<General,Var,Var,AT> A(N,M,NONINIT);
      for(int i=0; i<N; i++)
        for(int j=0; j<M; j++)
          A.e(i,j) = e(j,i);
      return A;
    }

  template <class AT> template <class Row>
    inline void Matrix<General,Var,Var,AT>::set(int j, const Vector<Row,AT> &x) {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(j<cols());
      assert(rows()==x.size());
#endif
      for(int i=0; i<rows(); i++)
        e(i,j) = x.e(i);
    }

  template <class AT> template <class Col>
    inline void Matrix<General,Var,Var,AT>::set(int i, const RowVector<Col,AT> &x) {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(i<rows());
      assert(cols()==x.size());
#endif
      for(int j=0; j<cols(); j++)
        e(i,j) = x.e(j);
    }

  template <class AT> template<class Type, class Row, class Col>
    inline void Matrix<General,Var,Var,AT>::set(const Range<Var,Var> &I, const Range<Var,Var> &J, const Matrix<Type,Row,Col,AT> &A) {

#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(I.end()<rows());
      assert(J.end()<cols());
      assert(I.size()==A.rows());
      assert(J.size()==A.cols());
#endif

      for(int i=I.start(), ii=0; i<=I.end(); i++, ii++)
        for(int j=J.start(), jj=0; j<=J.end(); j++, jj++)
          e(i,j) = A.e(ii,jj);
    }

  template <class AT> template <class Row>
    inline void Matrix<General,Var,Var,AT>::add(int j, const Vector<Row,AT> &x) {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(j<cols());
      assert(rows()==x.size());
#endif
      for(int i=0; i<rows(); i++)
        e(i,j) += x.e(i);
    }

  template <class AT> template <class Col>
    inline void Matrix<General,Var,Var,AT>::add(int i, const RowVector<Col,AT> &x) {
#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(i<rows());
      assert(cols()==x.size());
#endif
      for(int j=0; j<cols(); j++)
        e(i,j) += x.e(j);
    }

  template <class AT> template<class Type, class Row, class Col>
    inline void Matrix<General,Var,Var,AT>::add(const Range<Var,Var> &I, const Range<Var,Var> &J, const Matrix<Type,Row,Col,AT> &A) {

#ifndef FMATVEC_NO_BOUNDS_CHECK
      assert(I.end()<rows());
      assert(J.end()<cols());
      assert(I.size()==A.rows());
      assert(J.size()==A.cols());
#endif

      for(int i=I.start(), ii=0; i<=I.end(); i++, ii++)
        for(int j=J.start(), jj=0; j<=J.end(); j++, jj++)
          e(i,j) += A.e(ii,jj);
    }

  template <class AT>
    inline Matrix<General,Var,Var,AT>::operator std::vector<std::vector<AT> >() {
      std::vector<std::vector<AT> > ret(rows());
      for(int r=0; r<rows(); r++) {
        ret[r].resize(cols());
        for(int c=0; c<cols(); c++)
          ret[r][c]=e(r,c);
      }
      return ret;
    }

  template <class AT>
    inline Matrix<General,Var,Var,AT>::Matrix(std::vector<std::vector<AT> > m) : M(m.size()), N(m[0].size()), ele(new AT[M*N]) {
      for(int r=0; r<rows(); r++) {
        assert(m[r].size()==cols());
        for(int c=0; c<cols(); c++)
          e(r,c)=m[r][c];
      }
    }

  /// @cond NO_SHOW

  template <class AT> template <class Type, class Row, class Col>
    inline void Matrix<General,Var,Var,AT>::deepCopy(const Matrix<Type,Row,Col,AT> &A) { 
      for(int i=0; i<M; i++) 
        for(int j=0; j<N; j++)
          e(i,j) = A.e(i,j);
    }

  /// @endcond

}

#endif
