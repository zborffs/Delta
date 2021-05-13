// This is an interactive demo, so feel free to change the code and click the 'Run' button.

// This simple program uses the mlpack::neighbor::NeighborSearch object
// to find the nearest neighbor of each point in a dataset using the L1 metric,
// and then print the index of the neighbor and the distance of it to stdout.

#include <mlpack/core.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>
#include <armadillo>
#include <iostream>

using namespace mlpack;
using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
using namespace mlpack::metric; // ManhattanDistance
using namespace mlpack::ann;

int main() {
    std::cout << "MLPack Example " << std::endl;
    // Load the data from data.csv (hard-coded).  Use CLI for simple command-line
    // parameter handling.
    arma::mat data("0.339406815,0.843176636,0.472701471; \
                  0.212587646,0.351174901,0.81056695;  \
                  0.160147626,0.255047893,0.04072469;  \
                  0.564535197,0.943435462,0.597070812");
    data = data.t();
    std::cout << data << std::endl;

    // Use templates to specify that we want a NeighborSearch object which uses
    // the Manhattan distance.
    NeighborSearch<NearestNeighborSort, ManhattanDistance> nn(data);

    // Create the object we will store the nearest neighbors in.
    arma::Mat<size_t> neighbors;
    arma::mat distances; // We need to store the distance too.

    // Compute the neighbors.
    nn.Search(1, neighbors, distances);

    // Write each neighbor and distance using Log.
    for (size_t i = 0; i < neighbors.n_elem; ++i) {
        std::cout << "Nearest neighbor of point " << i << " is point "
                  << neighbors[i] << " and the distance is " << distances[i] << "." << std::endl;
    }

    /// next step
    // Load the training set and testing set.
    arma::mat trainData;
    data::Load("/Users/zachbortoff/Downloads/mlpack/build/thyroid_train.csv", trainData, true);
    arma::mat testData;
    data::Load("/Users/zachbortoff/Downloads/mlpack/build/thyroid_test.csv", testData, true);
    // Split the labels from the training set and testing set respectively.
    arma::mat trainLabels = trainData.row(trainData.n_rows - 1);
    arma::mat testLabels = testData.row(testData.n_rows - 1);
    trainData.shed_row(trainData.n_rows - 1);
    testData.shed_row(testData.n_rows - 1);
    // Initialize the network.
    FFN<> model;
    model.Add<Linear<> >(trainData.n_rows, 8);
    model.Add<SigmoidLayer<> >();
    model.Add<Linear<> >(8, 3);
    model.Add<LogSoftMax<> >();
    // Train the model.
    model.Train(trainData, trainLabels);
    // Use the Predict method to get the predictions.
    arma::mat predictionTemp;
    model.Predict(testData, predictionTemp);
    /*
      Since the predictionsTemp is of dimensions (3 x number_of_data_points)
      with continuous values, we first need to reduce it to a dimension of
      (1 x number_of_data_points) with scalar values, to be able to compare with
      testLabels.
      The first step towards doing this is to create a matrix of zeros with the
      desired dimensions (1 x number_of_data_points).
      In predictionsTemp, the 3 dimensions for each data point correspond to the
      probabilities of belonging to the three possible classes.
    */
    arma::mat prediction = arma::zeros<arma::mat>(1, predictionTemp.n_cols);
    // Find index of max prediction for each data point and store in "prediction"
    for (size_t i = 0; i < predictionTemp.n_cols; ++i)
    {
        // we add 1 to the max index, so that it matches the actual test labels.
        prediction(i) = arma::as_scalar(arma::find(
                arma::max(predictionTemp.col(i)) == predictionTemp.col(i), 1)) + 1;
    }
    /*
      Compute the error between predictions and testLabels,
      now that we have the desired predictions.
    */
    size_t correct = arma::accu(prediction == testLabels);
    double classificationError = 1 - double(correct) / testData.n_cols;
    // Print out the classification error for the testing dataset.
    std::cout << "Classification Error for the Test set: " << classificationError << std::endl;

    return 0;
}