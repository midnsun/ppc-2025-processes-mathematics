#include "zagryadskov_m_max_by_column/mpi/include/max_by_column.hpp"

#include <mpi.h>

#include <cstddef>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include "zagryadskov_m_max_by_column/common/include/common.hpp"

namespace zagryadskov_m_max_by_column {

ZagryadskovMMaxByColumnMPI::ZagryadskovMMaxByColumnMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
}

bool ZagryadskovMMaxByColumnMPI::ValidationImpl() {
  bool if_dividable = std::get<1>(GetInput()).size() % std::get<0>(GetInput()) == 0;
  bool res =
      (std::get<0>(GetInput()) > 0) && (!std::get<1>(GetInput()).empty()) && (GetOutput().empty()) && if_dividable;
  return res;
}

bool ZagryadskovMMaxByColumnMPI::PreProcessingImpl() {
  bool if_dividable = std::get<1>(GetInput()).size() % std::get<0>(GetInput()) == 0;
  bool res = (std::get<0>(GetInput()) > 0) && (!std::get<1>(GetInput()).empty()) && if_dividable;
  return res;
}

bool ZagryadskovMMaxByColumnMPI::RunImpl() {
  bool if_dividable = std::get<1>(GetInput()).size() % std::get<0>(GetInput()) == 0;
  bool test_data = (std::get<0>(GetInput()) > 0) && (!std::get<1>(GetInput()).empty()) && if_dividable;
  if (!test_data) {
    return false;
  }

  int world_size = 0;
  int world_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  const auto &n = std::get<0>(GetInput());
  const auto &mat = std::get<1>(GetInput());
  size_t m = mat.size() / n;
  OutType &res = GetOutput();
  OutType local_res;
  OutType columns;
  std::vector<int> sendcounts(world_size);
  std::vector<int> displs(world_size);
  if (!displs.empty()) {
    displs[0] = 0;
  }
  int columns_count = static_cast<int>(n) / world_size;
  using T = std::decay_t<decltype(*mat.begin())>;
  MPI_Datatype datatype = GetMpiType<T>();
  if (datatype == MPI_DATATYPE_NULL) {
    return false;
  }

  size_t i = 0;
  size_t j = 0;
  int r = 0;
  T tmp = std::numeric_limits<T>::lowest();
  bool tmp_flag = false;

  res.assign(n, std::numeric_limits<T>::lowest());
  for (r = 0; r < world_size; ++r) {
    sendcounts[r] = (columns_count + static_cast<int>(r < (static_cast<int>(n) % world_size))) * static_cast<int>(m);
    if (r > 0) {
      displs[r] = displs[r - 1] + sendcounts[r - 1];
    }
  }
  local_res.assign(static_cast<size_t>(sendcounts[world_rank]) / m, std::numeric_limits<T>::lowest());
  columns.resize(sendcounts[world_rank]);
  if (world_rank == 0) {
    MPI_Scatterv(mat.data(), sendcounts.data(), displs.data(), datatype, columns.data(), sendcounts[world_rank],
                 datatype, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatterv(nullptr, sendcounts.data(), displs.data(), datatype, columns.data(), sendcounts[world_rank], datatype,
                 0, MPI_COMM_WORLD);
  }
  for (j = 0; std::cmp_less(j, local_res.size()); ++j) {
    for (i = 0; i < m; ++i) {
      tmp = columns[(j * m) + i];
      tmp_flag = tmp > local_res[j];
      local_res[j] = (static_cast<T>(tmp_flag) * tmp) + (static_cast<T>(!tmp_flag) * local_res[j]);
    }
  }
  for (r = 0; r < world_size; ++r) {
    sendcounts[r] /= static_cast<int>(m);
    if (r > 0) {
      displs[r] = displs[r - 1] + sendcounts[r - 1];
    }
  }
  if (world_rank == 0) {
    MPI_Gatherv(local_res.data(), static_cast<int>(local_res.size()), datatype, res.data(), sendcounts.data(),
                displs.data(), datatype, 0, MPI_COMM_WORLD);
  } else {
    MPI_Gatherv(local_res.data(), static_cast<int>(local_res.size()), datatype, nullptr, sendcounts.data(),
                displs.data(), datatype, 0, MPI_COMM_WORLD);
  }

  MPI_Bcast(res.data(), static_cast<int>(res.size()), datatype, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  return !GetOutput().empty();
}

bool ZagryadskovMMaxByColumnMPI::PostProcessingImpl() {
  return !GetOutput().empty();
}

}  // namespace zagryadskov_m_max_by_column
