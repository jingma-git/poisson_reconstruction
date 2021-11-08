#include "fd_interpolate.h"
#include "macro.h"

void fd_interpolate(
    const int nx,
    const int ny,
    const int nz,
    const double h,
    const Eigen::RowVector3d &corner,
    const Eigen::MatrixXd &P,
    Eigen::SparseMatrix<double> &W)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here: tri-linear interpolation
  ////////////////////////////////////////////////////////////////////////////
  using namespace Eigen;
  W.resize(P.rows(), nx * ny * nz);
  std::vector<Triplet<double>> coeff;
  auto ind = [&nx, &ny, &nz](int i, int j, int k)
  {
    M_DEBUG(i << ", " << j << ", " << k << ", " << i + nx * (j + k * ny))
    return i + nx * (j + k * ny);
  };

  for (Index idx = 0; idx < P.rows(); ++idx)
  {
    const RowVector3d off = P.row(idx) - corner;
    RowVector3i ijk = (off / h).cast<int>();
    int i = ijk[0], j = ijk[1], k = ijk[2];
    RowVector3d c = ijk.cast<double>() * h; // c: grid corner
    RowVector3d p = off - c;
    RowVector3d q = RowVector3d(h, h, h) - p;
    coeff.emplace_back(idx, ind(i, j, k), q[0] * q[1] * q[2]);
    coeff.emplace_back(idx, ind(i + 1, j, k), p[0] * q[1] * q[2]);
    coeff.emplace_back(idx, ind(i, j, k + 1), q[0] * q[1] * p[2]);
    coeff.emplace_back(idx, ind(i + 1, j, k + 1), p[0] * q[1] * p[2]);
    coeff.emplace_back(idx, ind(i, j + 1, k), q[0] * p[1] * q[2]);
    coeff.emplace_back(idx, ind(i + 1, j + 1, k), p[0] * p[1] * q[2]);
    coeff.emplace_back(idx, ind(i, j + 1, k + 1), q[0] * p[1] * p[2]);
    coeff.emplace_back(idx, ind(i + 1, j + 1, k + 1), p[0] * p[1] * p[2]);
    M_DEBUG(idx << " ijk=" << ijk << " p:" << p)
  }

  W.setFromTriplets(coeff.begin(), coeff.end());
}
