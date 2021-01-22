#include "_recog/_simple_recognition_strategy.hpp"

#include <random>

cv::Vec2f SimpleRecognitionStrategy::points_to_line(cv::Vec4f points) {
    auto x1 = points[0];
    auto y1 = points[1];
    auto x2 = points[2];
    auto y2 = points[3];

    auto d = std::sqrt(std::pow(y1-y2, 2) + std::pow(x2-x1, 2));
    auto rho = (y1 * x2 - y2 * x1) / d;
    auto theta = std::atan2(x2 - x1,y1-y2);

    if (rho < 0) {
        theta += PI;
        rho *= -1;
    }

    return cv::Vec2f(rho, theta);
}


bool acceptLinePair(cv::Vec2f line1, cv::Vec2f line2, float minTheta) {
    float theta1 = line1[1], theta2 = line2[1];

    if(theta1 < minTheta)
    {
        theta1 += CV_PI; // dealing with 0 and 180 ambiguities...
    }

    if(theta2 < minTheta)
    {
        theta2 += CV_PI; // dealing with 0 and 180 ambiguities...
    }

    return abs(theta1 - theta2) > minTheta;
}

std::vector<cv::Point2f> lineToPointPair(cv::Vec2f line) {
    std::vector<cv::Point2f> points;

    float r = line[0], t = line[1];
    double cos_t = cos(t), sin_t = sin(t);
    double x0 = r*cos_t, y0 = r*sin_t;
    double alpha = 1000;

    points.push_back(cv::Point2f(x0 + alpha*(-sin_t), y0 + alpha*cos_t));
    points.push_back(cv::Point2f(x0 - alpha*(-sin_t), y0 - alpha*cos_t));

    return points;
}

// the long nasty wikipedia line-intersection equation...bleh...
cv::Point2f computeIntersect(cv::Vec2f line1, cv::Vec2f line2)
{
    std::vector<cv::Point2f> p1 = lineToPointPair(line1);
    std::vector<cv::Point2f> p2 = lineToPointPair(line2);

    float denom = (p1[0].x - p1[1].x)*(p2[0].y - p2[1].y) - (p1[0].y - p1[1].y)*(p2[0].x - p2[1].x);
    cv::Point2f intersect(((p1[0].x*p1[1].y - p1[0].y*p1[1].x)*(p2[0].x - p2[1].x) -
                       (p1[0].x - p1[1].x)*(p2[0].x*p2[1].y - p2[0].y*p2[1].x)) / denom,
                      ((p1[0].x*p1[1].y - p1[0].y*p1[1].x)*(p2[0].y - p2[1].y) -
                       (p1[0].y - p1[1].y)*(p2[0].x*p2[1].y - p2[0].y*p2[1].x)) / denom);

    return intersect;
}

/**
 * detects edges in image
 * @param input cv::Mat image (of chessboard)
 * @return      cv::Mat image transformed by gray-scaling, blurring, and adaptive thresholding functions
 */
cv::Mat SimpleRecognitionStrategy::detect_edges(cv::Mat& input) {
    cv::Mat gray, blurred, detect, hough1, hough2;
    hough1 = input.clone();
    hough2 = input.clone();

    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, blurred, 13); // ksize: 13
    cv::adaptiveThreshold(blurred, detect, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 3, 8); // bsize: 3
    std::vector<cv::Vec4f> lines;
    std::vector<cv::Vec2f> rho_theta_lines;
//    cv::HoughLinesP(detect, lines, 5, PI / 180, 50, 0, 0); // r: 5, threshold: 225, minLineLength: 120, maxLineGap: 200
    cv::HoughLinesP(detect, lines, 5, PI / 180, 225, 120, 200); // r: 5, threshold: 225, minLineLength: 120, maxLineGap: 200
    for (auto l : lines) {
        cv::line(hough1, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 233), 3, cv::LINE_AA); // :thickness: 2
        rho_theta_lines.push_back(points_to_line(l));
    }

    // compute the intersection from the lines detected...
    std::vector<cv::Point2f> intersections;
    for (size_t i = 0; i < rho_theta_lines.size(); i++) {
        for (size_t j = 0; j < rho_theta_lines.size(); j++) {
            cv::Vec2f line1 = rho_theta_lines[i];
            cv::Vec2f line2 = rho_theta_lines[j];
            if(acceptLinePair(line1, line2, CV_PI / 32)) {
                cv::Point2f intersection = computeIntersect(line1, line2);
                intersections.push_back(intersection);
            }
        }
    }

    if(!intersections.empty()) {
        std::vector<cv::Point2f>::iterator i;
        for(i = intersections.begin(); i != intersections.end(); ++i) {
            cv::circle(hough2, *i, 1, cv::Scalar(0, 255, 233), 3);
        }
    }

    cv::imshow("Simple Recognition Strategy", blurred);
    cv::waitKey(0);
    cv::imshow("Simple Recognition Strategy", detect);
    cv::waitKey(0);
    cv::imshow("Simple Recognition Strategy", hough1);
    cv::waitKey(0);
    cv::imshow("Simple Recognition Strategy", hough2);
    cv::waitKey(0);
    return detect;
}

cv::Mat SimpleRecognitionStrategy::detect_edges2(cv::Mat& input) {
    cv::Mat gray, blurred, detect, hough1, hough2;
    hough1 = input.clone();
    hough2 = input.clone();

    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, blurred, 25); // ksize: 13
//    cv::GaussianBlur(gray, blurred, cv::Size(7, 7), 2.0, 2.0);
    cv::adaptiveThreshold(blurred, detect, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 5, 10); // bsize: 3
    std::vector<cv::Vec4f> lines;
    std::vector<cv::Vec2f> rho_theta_lines;
    cv::HoughLinesP(detect, lines, 5, PI / 180, 500, 100, 100); // r: 5, threshold: 225, minLineLength: 120, maxLineGap: 200
    for (auto l : lines) {
        cv::line(hough1, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 233), 3, cv::LINE_AA); // :thickness: 2
        rho_theta_lines.push_back(points_to_line(l));
    }

    // compute the intersection from the lines detected...
    std::vector<cv::Point2f> intersections;
    for (size_t i = 0; i < rho_theta_lines.size(); i++) {
        for (size_t j = 0; j < rho_theta_lines.size(); j++) {
            cv::Vec2f line1 = rho_theta_lines[i];
            cv::Vec2f line2 = rho_theta_lines[j];
            if(acceptLinePair(line1, line2, CV_PI / 32)) {
                cv::Point2f intersection = computeIntersect(line1, line2);
                intersections.push_back(intersection);
            }
        }
    }

    if(!intersections.empty()) {
        std::vector<cv::Point2f>::iterator i;
        for(i = intersections.begin(); i != intersections.end(); ++i) {
            cv::circle(hough2, *i, 1, cv::Scalar(0, 255, 233), 25);
        }
    }

    cv::imshow("Simple Recognition Strategy", blurred);
    cv::waitKey(0);
    cv::imshow("Simple Recognition Strategy", detect);
    cv::waitKey(0);
    cv::imshow("Simple Recognition Strategy", hough1);
    cv::waitKey(0);
    cv::imshow("Simple Recognition Strategy", hough2);
    cv::waitKey(0);
    return detect;
}

cv::Mat SimpleRecognitionStrategy::detect_edges3(cv::Mat& input) {
//    cv::Mat gray, blurred, detect, hough1, hough2;
//    hough2 = input.clone();
//
//    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
//    cv::GaussianBlur(gray, blurred, cv::Size(9, 9), 1.0, 1.0);
//    cv::adaptiveThreshold(blurred, detect, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 5, 10); // bsize: 5
//
//    cv::imshow("Simple Recognition Strategy", blurred);
//    cv::waitKey(0);
//    cv::imshow("Simple Recognition Strategy", detect);
//    cv::waitKey(0);
////    cv::imshow("Simple Recognition Strategy", hough1);
////    cv::waitKey(0);
//    std::vector<cv::Vec2f> lines; lines.reserve(263);
//    cv::HoughLines(detect, lines, 5, PI / 180, 300, 0, 0); // r: 5, threshold: 225, minLineLength: 120, maxLineGap: 200
//    spdlog::get("delta_logger")->info("lines.size() = {}", lines.size());
//    for (size_t i = 0; i < lines.size(); i++) {
//        float rho = lines[i][0], theta = lines[i][1];
//        cv::Point pt1, pt2;
//        double a = std::cos(theta), b = std::sin(theta);
//        double x0 = a * rho, y0 = b * rho;
//        pt1.x = std::round(x0 + 5000*(-b));
//        pt1.y = std::round(y0 + 5000*(a));
//        pt2.x = std::round(x0 - 5000*(-b));
//        pt2.y = std::round(y0 - 5000*(a));
//        cv::line(hough2, pt1, pt2, cv::Scalar(0, 255, 233), 2, cv::LINE_AA);
////        cv::Point pt(x0, y0);
////        cv::circle(hough2, pt, 1, cv::Scalar(0, 255, 233), 9);
//    }
//
//    cv::imshow("Simple Recognition Strategy", hough2);
//    cv::waitKey(0);
//    return detect;

//======================================================================================================================
    cv::Mat gray, blurred, detect, hough1, hough2;
    hough2 = input.clone();

    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, blurred, 15); // ksize: 41
    cv::adaptiveThreshold(blurred, detect, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 5, 10); // bsize: 5; ADAPTIVE_THRESH_GAUSSIAN_C

    cv::imshow("Simple Recognition Strategy", blurred);
    cv::waitKey(0);
    cv::imshow("Simple Recognition Strategy", detect);
    cv::waitKey(0);
    std::vector<cv::Vec2f> lines; lines.reserve(300);
    cv::HoughLines(detect, lines, 5, PI / 180, 245, 0, 0); // r: 5, threshold: 290~300
    spdlog::get("delta_logger")->info("lines.size() = {}", lines.size());
    std::vector<float> thetas(lines.size());
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

    cv::imshow("Simple Recognition Strategy", hough2);
    cv::waitKey(0);
    auto h = matplot::hist(thetas);
    matplot::show();

    return detect;
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

        threshold[i] = detect_edges3(inputs[i]);
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
