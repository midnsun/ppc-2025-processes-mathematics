#pragma once

#include <mpi.h>

#include <type_traits>

#include "task/include/task.hpp"
#include "zagryadskov_m_max_by_column/common/include/common.hpp"

namespace zagryadskov_m_max_by_column {

class ZagryadskovMMaxByColumnMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit ZagryadskovMMaxByColumnMPI(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;

  template <typename T>
  MPI_Datatype getMPIType() {
    MPI_Datatype datatype;
    if (std::is_same_v<T, char>) {
      datatype = MPI_CHAR;
    } else if (std::is_same_v<T, unsigned char>) {
      datatype = MPI_UNSIGNED_CHAR;
    } else if (std::is_same_v<T, short>) {
      datatype = MPI_SHORT;
    } else if (std::is_same_v<T, unsigned short>) {
      datatype = MPI_UNSIGNED_SHORT;
    } else if (std::is_same_v<T, int>) {
      datatype = MPI_INT;
    } else if (std::is_same_v<T, unsigned>) {
      datatype = MPI_UNSIGNED;
    } else if (std::is_same_v<T, long>) {
      datatype = MPI_LONG;
    } else if (std::is_same_v<T, unsigned long>) {
      datatype = MPI_UNSIGNED_LONG;
    } else if (std::is_same_v<T, long long>) {
      datatype = MPI_LONG_LONG;
    } else if (std::is_same_v<T, float>) {
      datatype = MPI_FLOAT;
    } else if (std::is_same_v<T, double>) {
      datatype = MPI_DOUBLE;
    } else {
      datatype = MPI_DATATYPE_NULL;
    }
    return datatype;
  }
};

}  // namespace zagryadskov_m_max_by_column
