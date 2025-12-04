#pragma once

#include <algorithm>

#include "task/include/task.hpp"
#include "zagryadskov_m_allreduce/common/include/common.hpp"

namespace zagryadskov_m_allreduce {

class ZagryadskovMAllreduceSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit ZagryadskovMAllreduceSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;

 public:
  template <typename T>
  static int SeqAllreduce(const void *sendbuf, void *recvbuf, int count, int op) {
    size_t tcount = static_cast<size_t>(count);
    size_t tprocesses = 4;
    // Let it be 4 processes
    // Let op be max
    const T *tsendbuf = reinterpret_cast<const T *>(sendbuf);
    T *trecvbuf = reinterpret_cast<T *>(recvbuf);

    switch (op) {
      case 0:
        for (size_t i = 0; i < tcount; ++i) {
          T tmp = tsendbuf[i];
          for (size_t j = 1; j < tprocesses; ++j) {
            tmp = std::max(tmp, tsendbuf[i + (j * tcount)]);
          }
          trecvbuf[i] = tmp;
        }
        break;
      case 1:
        for (size_t i = 0; i < tcount; ++i) {
          T tmp = tsendbuf[i];
          for (size_t j = 1; j < tprocesses; ++j) {
            tmp = std::min(tmp, tsendbuf[i + (j * tcount)]);
          }
          trecvbuf[i] = tmp;
        }
        break;
      default:
        return -1;
    }

    return 0;
  }
};

}  // namespace zagryadskov_m_allreduce
