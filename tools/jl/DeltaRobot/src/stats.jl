## Contains functions and objects for statistical analysis
using Statistics, StatsBase

TABLE_HEADER = ["Statistic", "N", "Mean", "Min", "25 Percentile", "Median", "75 Percentile", "Max", "Standard Deviation", "Skewness", "Kurtosis"]

"""
computes the summary statistics of a given data set
"""
function summary_data(name, data)
        n = length(data)
        μ, σ = mean_and_std(data)
        minim, q1, med, q3, maxim = StatsBase.nquantile(data, 4)
        sk = skewness(data)
        ku = kurtosis(data)
        return [name n μ minim q1 med q3 maxim σ sk ku]
end
