#include "zagryadskov_m_allreduce/seq/include/allreduce.hpp"

#include <cstddef>
#include <limits>
#include <type_traits>

#include "zagryadskov_m_allreduce/common/include/common.hpp"

namespace zagryadskov_m_allreduce {

ZagryadskovMAllreduceSEQ::ZagryadskovMAllreduceSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
}

bool ZagryadskovMAllreduceSEQ::ValidationImpl() {
  auto &param1 = std::get<0>(GetInput());
  int param2 = std::get<1>(GetInput());
  int param3 = std::get<2>(GetInput());

  bool res = (!param1.empty()) && (param3 >= 0) && (param3 <= 1) && (param2 > 0) &&
             (param1.size() >= static_cast<size_t>(param2));
  return res;
}

bool ZagryadskovMAllreduceSEQ::PreProcessingImpl() {
  return true;
}

bool ZagryadskovMAllreduceSEQ::RunImpl() {
  ZagryadskovMAllreduceSEQ::SeqAllreduce<int>(std::get<0>(GetInput()).data(), GetOutput().data(),
                                              std::get<1>(GetInput()), std::get<2>(GetInput()));
  return true;
}

bool ZagryadskovMAllreduceSEQ::PostProcessingImpl() {
  bool res = !GetOutput().empty();
  return res;
}

}  // namespace zagryadskov_m_allreduce
