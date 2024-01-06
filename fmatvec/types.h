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

#ifndef types_h
#define types_h

#include <complex>
#include <string>
#include <stdexcept>
#include <boost/current_function.hpp>

#ifdef _MSC_VER
#ifdef fmatvec_EXPORTS
#define FMATVEC_EXPORT __declspec(dllexport)
#else
#define FMATVEC_EXPORT __declspec(dllimport)
#endif
#else
#ifdef fmatvec_EXPORTS
#define FMATVEC_EXPORT __attribute((visibility ("default")))
#else
#define FMATVEC_EXPORT
#endif
#endif


#ifndef HAVE_LIBMKL_INTEL_LP64
#ifndef CBLAS_ENUM_DEFINED_H
   #define CBLAS_ENUM_DEFINED_H
   enum CBLAS_ORDER {CblasRowMajor=101, CblasColMajor=102 };
   enum CBLAS_TRANSPOSE {CblasNoTrans=111, CblasTrans=112, CblasConjTrans=113,
                         AtlasConj=114};
   enum CBLAS_UPLO  {CblasUpper=121, CblasLower=122};
   enum CBLAS_DIAG  {CblasNonUnit=131, CblasUnit=132};
   enum CBLAS_SIDE  {CblasLeft=141, CblasRight=142};
#endif
#define CBLAS_INDEX int

#ifndef ATLAS_ENUM_H
   #define ATLAS_ENUM_H
   #define ATLAS_ORDER CBLAS_ORDER
      #define AtlasRowMajor CblasRowMajor
      #define AtlasColMajor CblasColMajor
   #define ATLAS_TRANS CBLAS_TRANSPOSE
      #define AtlasNoTrans CblasNoTrans
      #define AtlasTrans CblasTrans
      #define AtlasConjTrans CblasConjTrans
   #define ATLAS_UPLO CBLAS_UPLO
      #define AtlasUpper CblasUpper
      #define AtlasLower CblasLower
   #define ATLAS_DIAG CBLAS_DIAG
      #define AtlasNonUnit CblasNonUnit
      #define AtlasUnit CblasUnit
   #define ATLAS_SIDE CBLAS_SIDE
      #define AtlasLeft  CblasLeft
      #define AtlasRight CblasRight
#endif
#else
#include "mkl_cblas.h"
#endif

namespace std {
  template<class T> class shared_ptr;
}

namespace fmatvec {

  /***** initialization types *****/

  class Noinit { };
  class Init { };
  class Eye { };

  static Noinit NONINIT = Noinit();
  static Init INIT = Init();
  static Eye EYE = Eye();

  /***** matrix/vector (sub) types *****/

  class Ref {
  };

  class Var {
  };

  template<int M> class Fixed {
  };

  /*! 
   *  \brief Shape class for general matrices.
   *
   * Class General is a shape class for general matrices.
   * */
  class General {
  };

  /*! 
   *  \brief Shape class for general band matrices.
   *
   * Class GeneralBand is a shape class for general band matrices.
   * */
  class GeneralBand {
  };

  /*! 
   *  \brief Shape class for symmetric matrices.
   *
   * Class Symmetric is a shape class for symmetric matrices.
   * */
  class Symmetric {
  };

  /*! 
   *  \brief Shape class for rotation matrices.
   *
   * Class Rotation is a shape class for rotation matrices.
   * */
  class Rotation {
  };

  /*! 
   *  \brief Shape class for diagonal matrices.
   *
   * Class Diagonal is a shape class for diagonal matrices.
   * */
  class Diagonal {
  };

  /*! 
   *  \brief Shape class for sparse matrices.
   *
   * Class Sparse is a shape class for sparse matrices.
   * */
  class Sparse {
  };

  /*! 
   *  \brief Shape class for sparse matrices.
   *
   * Class SymmetricSparse is a shape class for symmetric sparse matrices.
   * */
  class SymmetricSparse {
  };

  /*!
   *  \brief Dummy shape class for square matrices.
   *
   * This is just a dummy since square matrices have a special class interface without a shape.
   * This shape is just used to detect a square matrix.
   * */
  class Square {
  };

//  template<int M, int N>
//  class GeneralFixed: public BasicType {
//  };
//
//  template<int M>
//  class SymmetricFixed: public BasicType {
//  };
//
//  class GeneralVar: public BasicType {
//  };
//
//  class SymmetricVar: public BasicType {
//  };
//
//  template<int M>
//  class GeneralFixedVar: public BasicType {
//  };
//
//  template<int N>
//  class GeneralVarFixed: public BasicType {
//  };

  template<class AT1, class AT2> struct OperatorResult;
  
  #define FMATVEC_OPERATORRESULT1(AT, ATRes) \
  template<> struct OperatorResult<AT, AT> { typedef ATRes Type; };

  #define FMATVEC_OPERATORRESULT2(AT1, AT2, ATRes) \
  template<> struct OperatorResult<AT1, AT2> { typedef ATRes Type; }; \
  template<> struct OperatorResult<AT2, AT1> { typedef ATRes Type; };
  
  FMATVEC_OPERATORRESULT1(double, double)
  FMATVEC_OPERATORRESULT1(int, int)
  FMATVEC_OPERATORRESULT1(std::complex<double>, std::complex<double>)

  FMATVEC_OPERATORRESULT2(double, std::complex<double>, std::complex<double>)
  FMATVEC_OPERATORRESULT2(int, std::complex<double>, std::complex<double>)
  FMATVEC_OPERATORRESULT2(int, double, double)

  // FMATVEC_ASSERT(expr, AT) is fully equal to assert(expr) if no spezialization of AssertUseException for AT exists.
  // FMATVEC_ASSERT(expr, AT) is similar to assert(expr) but throws an exception instead of calling abort regardless whether
  // NDEBUG is defined or not if their is a spezialization of AssertUseException for AT with value = true
  #define FMATVEC_ASSERT(expr, AT) fmatvec_assert<AssertUseException<AT>::value>(expr, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, #expr);

  // a template which defined if for AT exceptions or assert should be used in fmatvec.
  // spezialize this template to use exceptions.
  // see also ast.h for a spezialization.
  template<typename AT> struct AssertUseException { constexpr static bool value = false; };

  template<bool value>
  inline void fmatvec_assert(bool expr, std::string_view file, int line, std::string_view func, std::string_view exprStr);

  template<>
  inline void fmatvec_assert<false>(bool expr, std::string_view file, int line, std::string_view func, std::string_view exprStr) {
    #ifndef NDEBUG
      if(!expr) {
        // print with fprintf since printing with higher level functions like cerr may be redirected.
        fprintf(stderr, "%s", (std::string(file)+":"+std::to_string(line)+": "+
                         std::string(func)+": Assertion `"+std::string(exprStr)+"' failed.").c_str());
        std::abort();
      }
    #endif
  }
  
  template<>
  inline void fmatvec_assert<true>(bool expr, std::string_view file, int line, std::string_view func, std::string_view exprStr) {
    if(!expr)
      throw std::runtime_error(std::string(file)+":"+std::to_string(line)+": "+
            std::string(func)+": Assertion `"+std::string(exprStr)+"' failed.");
  }

  // tag dispatching
  struct Transpose {};

  class ErrorType;
  class SymbolicExpression;
  template<class, class> class Vector;
  template<class, class> class RowVector;
  template<class, class, class, class> class Matrix;
  template<class, class> class SquareMatrix;

  // Replace in a fmatvec scalar, vector or matrix the AT with ATNew.
  template<class MatVec, class NewAT>
  struct ReplaceAT;
  
  template<class NewAT>
  struct ReplaceAT<ErrorType, NewAT> {
    using Type = ErrorType;
  };
  
  template<class NewAT>
  struct ReplaceAT<double, NewAT> {
    using Type = NewAT;
  };

  template<class NewAT>
  struct ReplaceAT<SymbolicExpression, NewAT> {
    using Type = NewAT;
  };
  
  template<class Shape, class OldAT, class NewAT>
  struct ReplaceAT<Vector<Shape, OldAT>, NewAT> {
    using Type = Vector<Shape, NewAT>;
  };
  
  template<class Shape, class OldAT, class NewAT>
  struct ReplaceAT<RowVector<Shape, OldAT>, NewAT> {
    using Type = RowVector<Shape, NewAT>;
  };
  
  template<class MatType, class RowShape, class ColShape, class OldAT, class NewAT>
  struct ReplaceAT<Matrix<MatType, RowShape, ColShape, OldAT>, NewAT> {
    using Type = Matrix<MatType, RowShape, ColShape, NewAT>;
  };
  
  template<class Shape, class OldAT, class NewAT>
  struct ReplaceAT<SquareMatrix<Shape, OldAT>, NewAT> {
    using Type = SquareMatrix<Shape, NewAT>;
  };

  // Replace in a fmatvec scalar, vector or matrix the OldAT with NewAT.
  template<class MatVec, class NewAT> // handles MatVec being a scalar
  struct ReplaceAT {
    using Type = NewAT;
  };

}

#endif
