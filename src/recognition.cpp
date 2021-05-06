#include "recognition.hpp"

std::array<Photo, 64> SegmentClassify::segment(Photo& photo) {
    cv::imshow("Original Photo", photo);
    cv::waitKey(0);
    cv::destroyWindow("Original Photo");

    Photo gray;
    cv::cvtColor(photo, gray, cv::COLOR_BGR2GRAY); // color to gray
    spdlog::get("delta_logger")->info("Converting the photo to a grayscale image.");
    cv::imshow("Gray Photo", gray);
    cv::waitKey(0);
    cv::destroyWindow("Gray Photo");

    Photo blurred;
    cv::medianBlur(gray, blurred, 39); // ksize = 19
    spdlog::get("delta_logger")->info("Blurring the grayscaled photo by way of the Medianblur algorithm with a kernel size of 19.");
    cv::imshow("Median Blur", blurred);
    cv::waitKey(0);
    cv::destroyWindow("Median Blur");

    Photo detect;
    cv::adaptiveThreshold(blurred, detect, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 5, 10); // 255, adaptive_thresh_gaus_c, thresh_bin_inv, 5, 10(16)
    spdlog::get("delta_logger")->info("Using Canny edge detection algorithm to find edges in the image");
    cv::imshow("Detect", detect);
    cv::waitKey(0);
    cv::destroyWindow("Detect");

    std::vector<cv::Vec2f> lines; lines.reserve(300);
    cv::HoughLines(detect, lines, 2, PI / 180, 305, 0, 0); // 2, PI/180, 305, 0, 0
    std::vector<float> thetas(lines.size());
    Photo hough2 = photo.clone();
    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        thetas[i] = theta;
        cv::Point pt1, pt2;
        double a = std::cos(theta), b = std::sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = std::round(x0 + 5000*(-b));
        pt1.y = std::round(y0 + 5000*(a));
        pt2.x = std::round(x0 - 5000*(-b));
        pt2.y = std::round(y0 - 5000*(a));
        cv::line(hough2, pt1, pt2, cv::Scalar(0, 255, 233), 2, cv::LINE_AA);
    }
    cv::imshow("Hough2", hough2);
    cv::waitKey(0);
    cv::destroyWindow("Hough2");

    std::vector<cv::Point> intersections; intersections.reserve(lines.size() * lines.size() - lines.size());
    for (unsigned i = 0; i < lines.size(); ++i) {
        for (unsigned j = i + 1; j < lines.size(); ++j) {
            float rho1 = lines[i][0];
            float theta1 = lines[i][1];

            float rho2 = lines[j][0];
            float theta2 = lines[j][1];

            float ct1 = cosf(theta1); float ct2 = cosf(theta2); float st1 = sinf(theta1); float st2 = sinf(theta2);
            float det = ct1 * st2 - ct2 * st1;
            float c1 = rho1 * st2 - rho2 * st1;
            float c2 = ct1 * rho2 - ct2 * rho1;

            if (det != 0.0) {
                intersections.emplace_back(c1 / det, c2 / det);
            }
        }
    }

    double threshold_eps{780.0}; // 780
    int threshold_increment{2}; // 2
    Neighbors neighbors;
    for (unsigned i = 0; i < intersections.size(); ++i) {
        int increment{0};
        std::vector<unsigned> temps; temps.reserve(intersections.size());
        for (unsigned j = 0; j < intersections.size(); ++j) {
            if (i == j) {
                continue;
            }

            double eps = std::pow(intersections[i].x - intersections[j].x, 2) + std::pow(intersections[i].y - intersections[j].y, 2);

            if (eps < threshold_eps) {
                ++increment;
                temps.push_back(j);
            }
        }

        if (increment >= threshold_increment) {
            // improve efficiency by only testing whether intersections[i] is in the graph once before the for loop;
            // this requires creating 2 smaller forloops each in an if-else statement
            for (unsigned j = 0; j < temps.size(); ++j) {
                if (neighbors.graphs.find(&intersections[i]) == neighbors.graphs.end()) {
                    // the current point is NOT in the graph
                    if (neighbors.graphs.find(&intersections[temps[j]]) == neighbors.graphs.end()) {
                        // the other point isn't in the graph either, so create a new subgraph
                        neighbors.graphs[&intersections[i]] = neighbors.accs.size();
                        neighbors.graphs[&intersections[temps[j]]] = neighbors.accs.size();
                        neighbors.accs.emplace_back(intersections[i].x + intersections[temps[j]].x, intersections[i].y + intersections[temps[j]].y);
                        neighbors.cardinality.push_back(2);
                    } else {
                        auto index = neighbors.graphs[&intersections[temps[j]]];
                        neighbors.graphs[&intersections[i]] = index;
                        neighbors.accs[index].first += intersections[i].x;
                        neighbors.accs[index].second += intersections[i].y;
                        neighbors.cardinality[index]++;
                    }
                } else {
                    if (neighbors.graphs.find(&intersections[temps[j]]) == neighbors.graphs.end()) {
                        auto index = neighbors.graphs[&intersections[i]];
                        neighbors.graphs[&intersections[temps[j]]] = index;
                        neighbors.accs[index].first += intersections[temps[j]].x;
                        neighbors.accs[index].second += intersections[temps[j]].y;
                        neighbors.cardinality[index]++;
                    } else {
                        // do nothing
                    }
                }
            }
        }
    }

    std::vector<cv::Point> centroids(neighbors.accs.size());
    int card{0};
    for (unsigned i = 0; i < neighbors.cardinality.size(); ++i) {
        card += neighbors.cardinality[i];
    }
    std::cout << intersections.size() << " ?= " << card << std::endl;
    for (unsigned i = 0; i < centroids.size(); ++i) {
        centroids[i].x = neighbors.accs[i].first / neighbors.cardinality[i];
        centroids[i].y = neighbors.accs[i].second / neighbors.cardinality[i];
    }

    Photo intersection_photo = photo.clone();
//    for (size_t i = 0; i < intersections.size(); i++) {
//        std::cout << "x: " << intersections[i].x << ", y: " << intersections[i].y << std::endl;
//        cv::drawMarker(intersection_photo, intersections[i], cv::Scalar(255, 0, 5), cv::MarkerTypes::MARKER_DIAMOND, 50, 10, 12);
//    }
    for (size_t i = 0; i < centroids.size(); i++) {
        cv::drawMarker(intersection_photo, centroids[i], cv::Scalar(0, 0, 255), cv::MarkerTypes::MARKER_DIAMOND, 50, 10, 12);
    }

    cv::imshow("Intersection", intersection_photo);
    cv::waitKey(0);
    cv::destroyWindow("Intersection");

    return std::array<Photo, 64>();
}

/**
 * determines the FEN
 * @param photo
 * @return
 */
std::optional<FEN> SegmentClassify::determine_position(Photo& photo) {
    std::array<Photo, 64> squares = segment(photo);
    return std::nullopt;
}
