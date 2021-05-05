#ifndef DELTA_BSPLINES_HPP
#define DELTA_BSPLINES_HPP

#include <Eigen/Core>

class BSplines {
    template <unsigned P>
    static int find_span(int n, double u, const Eigen::Matrix<double, P + 1, 1>& U);

    template <unsigned P>
    static Eigen::Matrix<double, P + 1, 1> basis_funs(int i, double u, int p, const Eigen::Matrix<double, P + 1, 1>& U);
};

template<unsigned int P>
int BSplines::find_span(int n, double u, const Eigen::Matrix<double, P + 1, 1> &U) {
    return 0;
}

template<unsigned int P>
Eigen::Matrix<double, P + 1, 1> BSplines::basis_funs(int i, double u, int p, const Eigen::Matrix<double, P + 1, 1> &U) {
    return Eigen::Matrix<double, P + 1, 1>();
}

#endif //DELTA_BSPLINES_HPP
