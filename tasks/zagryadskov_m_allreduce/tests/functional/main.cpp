#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"
#include "zagryadskov_m_allreduce/common/include/common.hpp"
#include "zagryadskov_m_allreduce/mpi/include/allreduce.hpp"
#include "zagryadskov_m_allreduce/seq/include/allreduce.hpp"

namespace zagryadskov_m_allreduce {

class ZagryadskovMRunFuncTestsAllreduce : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    return std::to_string(test_param);
  }

 protected:
  void SetUp() override {
    TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    int seed = params;
    std::mt19937 e(seed);
    std::uniform_int_distribution<int> gen(-100, 100);
    int op = 0;
    int count = 25 + params * 50'000'000;
    int processes = 4;

    std::get<1>(input_data_) = count;
    std::get<2>(input_data_) = op;
    auto &data_vec = std::get<0>(input_data_);
    data_vec.resize(count * processes);

    for (int i = 0; i < count * processes; ++i) {
      data_vec[i] = gen(e);
    }
  }

  bool CheckTestOutputData(OutType &output_data) final {
    bool res = true;
    OutType example(output_data.size());
    ZagryadskovMAllreduceSEQ::SeqAllreduce<int>(std::get<0>(input_data_).data(), example.data(),
                                                std::get<1>(input_data_), std::get<2>(input_data_));

    for (size_t i = 0; i < output_data.size(); ++i) {
      if (output_data[i] != example[i]) {
        res = false;
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

TEST_P(ZagryadskovMRunFuncTestsAllreduce, GetAllreduce) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 2> kTestParam = {0, 1};  // 0 1

const auto kTestTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<ZagryadskovMAllreduceMPI, InType>(kTestParam, PPC_SETTINGS_zagryadskov_m_allreduce),
    ppc::util::AddFuncTask<ZagryadskovMAllreduceSEQ, InType>(kTestParam, PPC_SETTINGS_zagryadskov_m_allreduce));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = ZagryadskovMRunFuncTestsAllreduce::PrintFuncTestName<ZagryadskovMRunFuncTestsAllreduce>;

INSTANTIATE_TEST_SUITE_P(AllreduceTests, ZagryadskovMRunFuncTestsAllreduce, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace zagryadskov_m_allreduce
