#include "fd_partial_derivative.h"

void fd_partial_derivative(
    const int nx,
    const int ny,
    const int nz,
    const double h,
    const int dir,
    Eigen::SparseMatrix<double> &D)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  using namespace Eigen;
  std::vector<Eigen::Triplet<double>> trips;

  if (dir == 0)
  {
    D.resize((nx - 1) * ny * nz, nx * ny * nz);

    for (int i = 1; i < nx; ++i)
    {
      for (int j = 0; j < ny; ++j)
      {
        for (int k = 0; k < nz; ++k)
        {
          int idx = (i - 1) + j * (nx - 1) + k * (nx - 1) * ny;
          int idx0 = (i - 1) + j * nx + k * nx * ny;
          int idx1 = i + j * nx + k * nx * ny;
          trips.emplace_back(idx, idx0, -1);
          trips.emplace_back(idx, idx1, 1);
        }
      }
    }
  }
  else if (dir == 1)
  {
    D.resize(nx * (ny - 1) * nz, nx * ny * nz);

    for (int i = 0; i < nx; ++i)
    {
      for (int j = 1; j < ny; ++j)
      {
        for (int k = 0; k < nz; ++k)
        {
          int idx = i + (j - 1) * nx + k * nx * (ny - 1);
          int idx0 = i + (j - 1) * nx + k * nx * ny;
          int idx1 = i + j * nx + k * nx * ny;

          trips.emplace_back(idx, idx0, -1);
          trips.emplace_back(idx, idx1, 1);
        }
      }
    }
  }
  else if (dir == 2)
  {
    D.resize(nx * ny * (nz - 1), nx * ny * nz);

    for (int i = 0; i < nx; ++i)
    {
      for (int j = 0; j < ny; ++j)
      {
        for (int k = 1; k < nz; ++k)
        {
          int idx = i + j * nx + (k - 1) * nx * ny;
          int idx0 = i + j * nx + (k - 1) * nx * ny;
          int idx1 = i + j * nx + k * nx * ny;

          trips.emplace_back(idx, idx0, -1);
          trips.emplace_back(idx, idx1, 1);
        }
      }
    }
  }

  D.setFromTriplets(trips.begin(), trips.end());
}
