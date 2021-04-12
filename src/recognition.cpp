#include "recognition.hpp"

//bool acceptLinePair(cv::Vec2f line1, cv::Vec2f line2, float minTheta) {
//    float theta1 = line1[1], theta2 = line2[1];
//
//    if(theta1 < minTheta)
//    {
//        theta1 += CV_PI; // dealing with 0 and 180 ambiguities...
//    }
//
//    if(theta2 < minTheta)
//    {
//        theta2 += CV_PI; // dealing with 0 and 180 ambiguities...
//    }
//
//    return abs(theta1 - theta2) > minTheta;
//}
//
//std::vector<cv::Point2f> lineToPointPair(cv::Vec2f line) {
//    std::vector<cv::Point2f> points;
//
//    float r = line[0], t = line[1];
//    double cos_t = cos(t), sin_t = sin(t);
//    double x0 = r*cos_t, y0 = r*sin_t;
//    double alpha = 1000;
//
//    points.push_back(cv::Point2f(x0 + alpha*(-sin_t), y0 + alpha*cos_t));
//    points.push_back(cv::Point2f(x0 - alpha*(-sin_t), y0 - alpha*cos_t));
//
//    return points;
//}
//
//// the long nasty wikipedia line-intersection equation...bleh...
//cv::Point2f computeIntersect(cv::Vec2f line1, cv::Vec2f line2) {
//    std::vector<cv::Point2f> p1 = lineToPointPair(line1);
//    std::vector<cv::Point2f> p2 = lineToPointPair(line2);
//
//    float denom = (p1[0].x - p1[1].x)*(p2[0].y - p2[1].y) - (p1[0].y - p1[1].y)*(p2[0].x - p2[1].x);
//    cv::Point2f intersect(((p1[0].x*p1[1].y - p1[0].y*p1[1].x)*(p2[0].x - p2[1].x) -
//                           (p1[0].x - p1[1].x)*(p2[0].x*p2[1].y - p2[0].y*p2[1].x)) / denom,
//                          ((p1[0].x*p1[1].y - p1[0].y*p1[1].x)*(p2[0].y - p2[1].y) -
//                           (p1[0].y - p1[1].y)*(p2[0].x*p2[1].y - p2[0].y*p2[1].x)) / denom);
//
//    return intersect;
//}

std::array<Photo, 64> SegmentClassify::segment(Photo& photo) {
    Photo gray;
    cv::cvtColor(photo, gray, cv::COLOR_BGR2GRAY); // color to gray
//    cv::imshow("Gray Photo", gray);
//    cv::waitKey(0);

    Photo blurred;
    cv::medianBlur(gray, blurred, 19); // ksize = 19
//    cv::imshow("Median Blur", blurred);
//    cv::waitKey(0);

    Photo detect;
    cv::adaptiveThreshold(blurred, detect, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 5, 10); // 255, adaptive_thresh_gaus_c, thresh_bin_inv, 5, 10
//    cv::imshow("Detect", detect);
//    cv::waitKey(0);

    std::vector<cv::Vec2f> lines; lines.reserve(300);
    cv::HoughLines(detect, lines, 2, PI / 180, 305, 0, 0); // 2, PI/180, 305, 0, 0
    std::vector<float> thetas(lines.size());
//    Photo hough2 = input.clone();
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
//        cv::line(hough2, pt1, pt2, cv::Scalar(0, 255, 233), 2, cv::LINE_AA);
    }

//    cv::imshow("Simple Recognition Strategy", hough2);
//    cv::waitKey(0);
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
