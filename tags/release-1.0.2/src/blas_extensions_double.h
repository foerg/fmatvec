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

#ifndef blas_extensions_double_h
#define blas_extensions_double_h

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

namespace fmatvec {
  void myblas_daxpy(const CBLAS_TRANSPOSE  transa, const CBLAS_TRANSPOSE
      transb, int m, int n, double alpha, const double *a, int lda1, double *b,
      int ldb1);

  void myblas_dcopy(const CBLAS_TRANSPOSE  transa, const CBLAS_TRANSPOSE
      transb, int m, int n, const double *a, int lda1, double *b, int ldb1);

  void myblas_dscal(int n, double da, const double *dx, int incx, double *c);

  void myblas_dscal(const CBLAS_TRANSPOSE  transa, int m, int n, double alpha, const
      double *a, int lda1, double *c, int ldc);

  void myblas_dscal(char transa, int m, int n, double alpha, double *a, int lda1);

  void myblas_daxpy(int n, double da, const double *dx, int incx, const double *dy, int
      incy, double *c);

  void myblas_daxpy(const CBLAS_TRANSPOSE transa, const CBLAS_TRANSPOSE transb,
      int m, int n, double alpha, const double *a, int lda1, const double *b, int
      ldb1, double *c, int ldc);
}
#endif
