#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <fstream>

#include "zagryadskov_m_max_by_column/common/include/common.hpp"
#include "zagryadskov_m_max_by_column/mpi/include/max_by_column.hpp"
#include "zagryadskov_m_max_by_column/seq/include/max_by_column.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace zagryadskov_m_max_by_column {

class ZagryadskovMRunFuncTestsMaxByColumn : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    return test_param;
  }

 protected:
  void SetUp() override {
    TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    std::string inFileName = params;
    std::string abs_path = ppc::util::GetAbsoluteTaskPath(PPC_ID_zagryadskov_m_max_by_column, inFileName);
    std::ifstream inFileStream(abs_path, std::ios::in | std::ios::binary);
    if (!inFileStream.is_open()) {
      throw std::runtime_error("Error opening file!\n");
    }
    size_t m;
    size_t n;
    inFileStream.read(reinterpret_cast<char*>(&m), sizeof(size_t));
    inFileStream.read(reinterpret_cast<char*>(&n), sizeof(size_t));
    std::get<0>(input_data_) = n;
    auto &mat = std::get<1>(input_data_);
    mat.resize(m * n);
    using T = std::decay_t<decltype(*mat.begin())>;
    
    inFileStream.read(reinterpret_cast<char*>(mat.data()), sizeof(T)*m*n);

    inFileStream.close();
  }

  bool CheckTestOutputData(OutType &output_data) final {
    bool res = true;
    size_t n = std::get<0>(input_data_);
    size_t m = std::get<1>(input_data_).size() / n;
    auto &mat = std::get<1>(input_data_);
    if (output_data.size() != n) {
        res = false;
        return res;
    }
    
    for (size_t j = 0; j < n; ++j) {
        for (size_t i = 0; i < m; ++i) {
            if (output_data[j] < mat[j * m + i]) {
                res = false;
            }
        }
    }

    return res;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
};

namespace {

TEST_P(ZagryadskovMRunFuncTestsMaxByColumn, GetMaxByColumn) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 1> kTestParam = {"mat1.bin"};

const auto kTestTasksList =
    std::tuple_cat(ppc::util::AddFuncTask<ZagryadskovMMaxByColumnMPI, InType>(kTestParam, PPC_SETTINGS_zagryadskov_m_max_by_column),
                   ppc::util::AddFuncTask<ZagryadskovMMaxByColumnSEQ, InType>(kTestParam, PPC_SETTINGS_zagryadskov_m_max_by_column));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = ZagryadskovMRunFuncTestsMaxByColumn::PrintFuncTestName<ZagryadskovMRunFuncTestsMaxByColumn>;

INSTANTIATE_TEST_SUITE_P(MaxByColumnTests, ZagryadskovMRunFuncTestsMaxByColumn, kGtestValues, kPerfTestName);

}  // namespace

}  // zagryadskov_m_max_by_column
