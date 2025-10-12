

#include <algorithm>
#include <utility>
#include <vector>

namespace zagryadskov_a_matrix_sum {
    template <typename T>
    class myMatrix : public ppc::core::Task {
        std::vector<T> data;
        size_t m;
        size_t n;
        MPI_Datatype datatype;
        public:
        myMatrix (size_t _m, size_t _n) : m(_m), n(_n), data(_m * _n) {
            datatype = 0;
        }
        myMatrix (const myMatrix& copy): m(copy.m), n(copy.n), data(copy.data), datatype(copy.datatype) {}
        myMatrix (myMatrix&& mov): m(mov.m), n(mov.n), data(std::move(copy.data)), datatype(mov.datatype) {}
        const myMatrix& operator= (const myMatrix& copy) {
            m = copy.m;
            n = copy.n;
            data = copy.data;
            datatype = copy.datatype;
            return *this;
        }
        const myMatrix& operator= (myMatrix&& mov) {
            m = mov.m;
            n = mov.n;
            data = std::move(mov.data);
            datatype = mov.datatype;
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
            int world_size;
            int world_rank;
            int rows_size = n / world_size;
            std::vector<T> rows;
            std::vector<T> res;
            rows.resize(rows_size * m);

            if (rank == 0)
                res.resize(n, std::numeric_limits<T>::min());
            if (rank != 0)
               res.resize(rows_size, std::numeric_limits<T>::min());
            MPI_Scatter(data.data(), rows_size, datatype, rows.data(), rows_size, datatype, 0, MPI_COMM_WORLD);

            int64_t j, i;
            T tmp;
            int tmpFlag;
            for (j = 0; j < rows_size; ++j) {
                for (i = 0; i < m; ++i) {
                    tmp = rows[j * m + i];
                    tmpFlag = tmp > res[j];
                    res[j] = tmpFlag * tmp + (!tmpFlag) * res[j];
                }
            }

            MPI_Gather(res.data(), rowsCount, datatype, res.data(), rowsCount, datatype, 0, MPI_COMM_WORLD);
            if (rank == 0) {
                for (j = rows_size * world_size; j < n; ++j) {
                    for (i = 0; i < m; ++i) {
                        tmp = data[j * m + i];
                        tmpFlag = tmp > res[j];
                        res[j] = tmpFlag * tmp + (!tmpFlag) * res[j];
                    }   
                }
            }

            return res;
        }
    };
} // namespace zagryadskov_a_matrix_sum