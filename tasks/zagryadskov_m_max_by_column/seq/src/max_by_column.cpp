#include "zagryadskov_m_max_by_column/seq/include/max_by_column.hpp"

#include <numeric>
#include <algorithm>
#include <utility>
#include <limits>
#include <type_traits>

#include "zagryadskov_m_max_by_column/common/include/common.hpp"
#include "util/include/util.hpp"

namespace zagryadskov_m_max_by_column {

ZagryadskovMTestMaxByColumnSEQ::ZagryadskovMTestMaxByColumnSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
}

bool ZagryadskovMTestMaxByColumnSEQ::ValidationImpl() {
  bool ifDividable = std::get<1>(GetInput()).size() % std::get<0>(GetInput()) == 0;
  return (std::get<0>(GetInput()) > 0) && (std::get<1>(GetInput()).size() > 0) && (GetOutput().size() == 0) && ifDividable;
}

bool ZagryadskovMTestMaxByColumnSEQ::PreProcessingImpl() {
  bool ifDividable = std::get<1>(GetInput()).size() % std::get<0>(GetInput()) == 0;
  return (std::get<0>(GetInput()) > 0) && (std::get<1>(GetInput()).size() > 0) && ifDividable;
}

bool ZagryadskovMTestMaxByColumnSEQ::RunImpl() {
  bool ifDividable = std::get<1>(GetInput()).size() % std::get<0>(GetInput()) == 0;  
  if ((std::get<0>(GetInput()) == 0) || (std::get<1>(GetInput()).size() == 0) || !ifDividable) {
    return false;
  }

  const auto &n = std::get<0>(GetInput());
  const auto &mat = std::get<1>(GetInput());
  int m = mat.size();
  OutType &res = GetOutput();
  OutType rows;
  using T = std::decay_t<decltype(*mat.begin())>;

  int64_t j, i;
  std::vector<T> res(n, std::numeric_limits<T>::min());
  T tmp;
  int tmpFlag;
  for (j = 0; j < n; ++j) {
    for (i = 0; i < m; ++i) {
      tmp = mat[j * m + i];
      tmpFlag = tmp > res[j];
      res[j] = tmpFlag * tmp + (!tmpFlag) * res[j];
    }
}

  return GetOutput().size() > 0;
}

bool ZagryadskovMTestMaxByColumnSEQ::PostProcessingImpl() {
  return GetOutput().size() > 0;
}

}  // zagryadskov_m_max_by_column