

#include <algorithm>
#include <utility>
#include <vector>
#include <limits>

namespace zagryadskov_a_matrix_sum {
    template <typename T>
    class myMatrix : public ppc::core::Task {
        std::vector<T> data;
        size_t m;
        size_t n;
        public:
        myMatrix (size_t _m, size_t _n) : m(_m), n(_n), data(_m * _n) {}
        myMatrix (const myMatrix& copy): m(copy.m), n(copy.n), data(copy.data) {}
        myMatrix (myMatrix&& mov): m(mov.m), n(mov.n), data(std::move(copy.data)) {}
        const myMatrix& operator= (const myMatrix& copy) {
            m = copy.m;
            n = copy.n;
            data = copy.data;
            return *this;
        }
        const myMatrix& operator= (myMatrix&& mov) {
            m = mov.m;
            n = mov.n;
            data = std::move(mov.data);
            return *this;
        }
        std::vector<T>& getData() { 
            return data;
        }
        const std::vector<T>& getData() const { 
            return data;
        }
        T& operator() (size_t i, size_t j) {
            return data[j * m + i];
        }
        const T& operator() (size_t i, size_t j) const {
            return data[j * m + i];
        }
        std::vector<T> getMaxByColumn() const {
            int64_t j, i;
            std::vector<T> res(n, std::numeric_limits<T>::min());
            T tmp;
            int tmpFlag;
            for (j = 0; j < n; ++j) {
                for (i = 0; i < m; ++i) {
                    tmp = data[j * m + i];
                    tmpFlag = tmp > res[j]
                    res[j] = tmpFlag * tmp + (!tmpFlag) * res[j];
                }
            }
            return res;
        }
    };
} // namespace zagryadskov_a_matrix_sum