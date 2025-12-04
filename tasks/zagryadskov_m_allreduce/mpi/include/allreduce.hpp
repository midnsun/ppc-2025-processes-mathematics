#pragma once

#include <mpi.h>

#include <vector>

#include "task/include/task.hpp"
#include "zagryadskov_m_allreduce/common/include/common.hpp"

namespace zagryadskov_m_allreduce {

class ZagryadskovMAllreduceMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit ZagryadskovMAllreduceMPI(const InType &in);

 private:
  OutType temp_vec;
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
  static int myAllreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op,
                         MPI_Comm comm);
};

}  // namespace zagryadskov_m_allreduce
