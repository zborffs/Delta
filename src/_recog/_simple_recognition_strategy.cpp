#include "_recog/_simple_recognition_strategy.hpp"

/**
 * detects edges in image
 * @param input cv::Mat image (of chessboard)
 * @return      cv::Mat image transformed by gray-scaling, blurring, and adaptive thresholding functions
 */
cv::Mat SimpleRecognitionStrategy::detect_edges(cv::Mat &input) {
    cv::Mat gray, blurred, thresh, adap1, adap2;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, blurred, 13);
    cv::threshold(blurred, thresh, 127, 255, cv::THRESH_BINARY);
    cv::adaptiveThreshold(blurred, adap1, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 13, 8);
    cv::adaptiveThreshold(blurred, adap2, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 13, 8);
//    cv::namedWindow("Image", 1);
//    cv::imshow("Image", thresh);
//    cv::waitKey(0);
//
//    cv::namedWindow("Image", 1);
//    cv::imshow("Image", adap1);
//    cv::waitKey(0);
//
//    cv::namedWindow("Image", 1);
//    cv::imshow("Image", adap2);
//    cv::waitKey(0);
    return adap2;
}


/**
 *
 * @return
 */
std::optional<std::string> SimpleRecognitionStrategy::position() {
    std::vector<cv::Mat> inputs(cameras_.size());
    std::vector<cv::Mat> threshold(cameras_.size());
    for (unsigned i = 0; i < cameras_.size(); ++i) {
        inputs[i] = cameras_[i]->take_picture();

        if (!inputs[i].data) {
            spdlog::get("delta_logger")->error("no data {}");
            return std::nullopt;
        }

        threshold[i] = detect_edges(inputs[i]);
    }

//    std::vector<cv::Mat> grays(cameras_.size());
//    for (unsigned i = 0; i < cameras_.size(); ++i) {
//        cv::cvtColor(inputs[i], grays[i], cv::COLOR_BGR2GRAY);
//    }
//
//    std::vector<cv::Mat> blurs(cameras_.size());
//    for (unsigned i = 0; i < cameras_.size(); ++i) {
//        cv::blur(grays[i], blurs[i], cv::Size(10,10));
//    }
//
//
//    std::vector<cv::Mat> edges(cameras_.size());
//    for (unsigned i = 0; i < cameras_.size(); ++i) {
//        cv::Canny(blurs[i], edges[i], 10, 50, 3);
//    }

//    std::vector<cv::Mat> cannys(inputs.size());
//    for (unsigned i = 0; i < cannys.size(); ++i) {
//        cannys[i] = canny_edge_det(inputs[i]);
//    }
//
//    std::vector<cv::Mat> houghs(inputs.size());
//    for (unsigned i = 0; i < cannys.size(); ++i) {
//        houghs[i] = hough_transform(cannys[i]);
//    }
//
//    std::vector<cv::Mat> ips(inputs.size());
//    for (unsigned i = 0; i < cannys.size(); ++i) {
//        ips[i] = intersection_points(houghs[i]);
//    }
//
//    std::vector<cv::Mat> corns(inputs.size());
//    for (unsigned i = 0; i < cannys.size(); ++i) {
//        corns[i] = corners(ips[i]);
//    }
//
//    std::vector<cv::Mat> crops(inputs.size());
//    for (unsigned i = 0; i < cannys.size(); ++i) {
//        crops[i] = crop(corns[i]);
//    }

    return std::nullopt;
}

