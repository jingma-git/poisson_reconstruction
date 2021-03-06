#include "poisson_surface_reconstruction.h"
#include <igl/copyleft/marching_cubes.h>
#include <algorithm>
#include "macro.h"
#include "fd_interpolate.h"
#include "fd_grad.h"
#include "fd_partial_derivative.h"
#include <Eigen/IterativeLinearSolvers>
#include <iostream>

void poisson_surface_reconstruction(
    const Eigen::MatrixXd &P,
    const Eigen::MatrixXd &N,
    Eigen::MatrixXd &V,
    Eigen::MatrixXi &F)
{

  using namespace Eigen;
  using namespace std;
  ////////////////////////////////////////////////////////////////////////////
  // Construct FD grid, CONGRATULATIONS! You get this for free!
  ////////////////////////////////////////////////////////////////////////////
  // number of input points
  const int n = P.rows();
  // Grid dimensions
  int nx, ny, nz;
  // Maximum extent (side length of bounding box) of points
  auto extent = (P.colwise().maxCoeff() - P.colwise().minCoeff());
  double max_extent = extent.maxCoeff();
  // padding: number of cells beyond bounding box of input points
  const double pad = 8;
  // choose grid spacing (h) so that shortest side gets 30+2*pad samples
  double h = max_extent / double(30 + 2 * pad);
  // Place bottom-left-front corner of grid at minimum of points minus padding
  Eigen::RowVector3d corner = P.colwise().minCoeff().array() - pad * h;
  // Grid dimensions should be at least 3
  nx = std::max((P.col(0).maxCoeff() - P.col(0).minCoeff() + (2. * pad) * h) / h, 3.);
  ny = std::max((P.col(1).maxCoeff() - P.col(1).minCoeff() + (2. * pad) * h) / h, 3.);
  nz = std::max((P.col(2).maxCoeff() - P.col(2).minCoeff() + (2. * pad) * h) / h, 3.);

  // Compute positions of primary grid nodes
  Eigen::MatrixXd x(nx * ny * nz, 3);
  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      for (int k = 0; k < nz; k++)
      {
        // Convert subscript to index
        const auto ind = i + nx * (j + k * ny);
        x.row(ind) = corner + h * Eigen::RowVector3d(i, j, k);
      }
    }
  }
  Eigen::VectorXd g = Eigen::VectorXd::Zero(nx * ny * nz);
  M_DEBUG("extent:" << extent << " h:" << h << " nx:" << nx << " ny:" << ny << " nz:" << nz << " x.rows=" << x.rows())

  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  Eigen::SparseMatrix<double> Wx, Wy, Wz;
  fd_interpolate(nx - 1, ny, nz, h, corner + RowVector3d(0.5 * h, 0, 0), P, Wx); // Wx: #P by (nx-1)*ny*nz
  fd_interpolate(nx, ny - 1, nz, h, corner + RowVector3d(0, 0.5 * h, 0), P, Wy); // Wy: #P by nx*(ny-1)*nz
  fd_interpolate(nx, ny, nz - 1, h, corner + RowVector3d(0, 0, 0.5 * h), P, Wz); // Wz: #P by nx*ny*(nz-1)
  // 1. distribute input normals to staggered grids
  int xdim = (nx - 1) * ny * nz, ydim = nx * (ny - 1) * nz, zdim = nx * ny * (nz - 1);
  VectorXd Vx = Wx.transpose() * N.col(0); // (nx-1)*ny*nz by 1
  VectorXd Vy = Wy.transpose() * N.col(1); // nx*(ny-1)*nz by 1
  VectorXd Vz = Wz.transpose() * N.col(2); // nx*ny*(nz-1) by 1
  VectorXd gridN(xdim + ydim + zdim);      // normal on the staggered grid position
  gridN.block(0, 0, xdim, 1) = Vx;
  gridN.block(xdim, 0, ydim, 1) = Vy;
  gridN.block(xdim + ydim, 0, zdim, 1) = Vz;

  // 2. solve the implict value g
  Eigen::SparseMatrix<double> G;
  fd_grad(nx, ny, nz, h, G);
  M_DEBUG("Grad: " << G.rows() << ", " << G.cols())
  Eigen::BiCGSTAB<SparseMatrix<double>> solver;
  solver.compute(G.transpose() * G);
  g = solver.solve(G.transpose() * gridN);

  // 3. determine iso-value
  Eigen::SparseMatrix<double> W; // #P by nx*ny*nz
  fd_interpolate(nx, ny, nz, h, corner, P, W);
  double sigma = (W * g).mean(); // #P
  M_DEBUG(<< "sigma: " << sigma);
  g.array() -= sigma;

  ////////////////////////////////////////////////////////////////////////////
  // Run black box algorithm to compute mesh from implicit function: this
  // function always extracts g=0, so "pre-shift" your g values by -sigma
  ////////////////////////////////////////////////////////////////////////////
  igl::copyleft::marching_cubes(g, x, nx, ny, nz, V, F);
}
