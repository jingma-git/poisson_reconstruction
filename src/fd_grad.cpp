#include "fd_grad.h"
#include "fd_partial_derivative.h"
#include "cat.h"
#include "macro.h"

void fd_grad(
    const int nx,
    const int ny,
    const int nz,
    const double h,
    Eigen::SparseMatrix<double> &G)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////

  // Gradients on a regular grid
  Eigen::SparseMatrix<double> Dx, Dy, Dz;
  fd_partial_derivative(nx, ny, nz, h, 0, Dx);
  fd_partial_derivative(nx, ny, nz, h, 0, Dy);
  fd_partial_derivative(nx, ny, nz, h, 0, Dz);
  M_DEBUG("gradients on regular grid")

  egl::cat(1, Dx, Dy, Dz, G);
}
