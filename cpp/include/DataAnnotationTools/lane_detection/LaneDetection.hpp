//
// Created by brucknem on 29.06.21.
//

#ifndef DATAANNOTATIONTOOLS_LANEDETECTION_HPP
#define DATAANNOTATIONTOOLS_LANEDETECTION_HPP

#include <utility>
#include <armadillo>

#include "opencv2/opencv.hpp"
#include "opencv2/ximgproc.hpp"
#include "dbscan.h"

namespace data_annotation_tools {
    namespace lane_detection {
        class LaneDetection {

        };

        cv::RNG rng(12345);

        cv::Mat plotHistograms(const cv::Mat &image) {
            cv::Mat result = image.clone();
            cv::cvtColor(image, result, cv::COLOR_BGR2HLS);

            std::vector<cv::Mat> bgr_planes;
            cv::split(result, bgr_planes);

            int histSize = 256;
            float range[] = {0, 256}; //the upper boundary is exclusive
            const float *histRange = {range};
            bool uniform = true, accumulate = false;

            cv::Mat b_hist, g_hist, r_hist;
            calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
            calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
            calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
            int hist_w = 512, hist_h = 400;
            int bin_w = cvRound((double) hist_w / histSize);
            cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
            normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
            normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
            normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
            for (int i = 1; i < histSize; i++) {
                line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
                     cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
                     cv::Scalar(255, 0, 0), 2, 8, 0);
                line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
                     cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
                     cv::Scalar(0, 255, 0), 2, 8, 0);
                line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
                     cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
                     cv::Scalar(0, 0, 255), 2, 8, 0);
            }
            return histImage;
        }

        cv::Mat extractHoughLinesP(const cv::Mat &image) {
            cv::Mat result = image.clone(), buffer;
            cv::cvtColor(image, result, cv::COLOR_BGR2GRAY);

            cv::Canny(result.clone(), result, 100, 200);
            cv::Mat edges = result.clone();

            std::vector<cv::Vec4i> lines;

            // TODO sliders
            cv::HoughLinesP(result, lines, 7, M_PI / 60, 160, 40, 1);
            for (auto l : lines) {
                line(edges, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
            }

            return edges;
        }

        cv::Point2f calculateMean(std::vector<cv::Point> contour) {
            cv::Point2f mean = contour[0];
            for (int x = 1; x < contour.size(); x++) {
                mean += (cv::Point2f) contour[x];
            }
            return {mean.x / contour.size(), mean.y / contour.size()};
        }

        bool intersection(const cv::Vec2f &o1, const cv::Vec2f &p1, const cv::Vec2f &o2, const cv::Vec2f &p2,
                          cv::Vec2f &r) {
            cv::Vec2f x = o2 - o1;
            cv::Vec2f d1 = p1 - o1;
            cv::Vec2f d2 = p2 - o2;

            float cross = d1[0] * d2[1] - d1[1] * d2[0];
            if (abs(cross) < /*EPS*/1e-8)
                return false;

            double t1 = (x[0] * d2[1] - x[1] * d2[0]) / cross;
            r = o1 + d1 * t1;
            return true;
        }

        std::vector<cv::Vec2f> calculateIntersections(const std::vector<std::pair<cv::Vec2f, cv::Vec2f>> &lines,
                                                      const std::pair<cv::Vec2f, cv::Vec2f> &line) {
            std::vector<cv::Vec2f> crosses;
            for (int i = 0; i < lines.size(); i++) {
                cv::Vec2f cross;
                if (intersection(lines[i].first, lines[i].second, line.first, line.second, cross)) {
                    crosses.emplace_back(cross);
                }
            }
            return crosses;
        }

        std::vector<cv::Vec2f> calculateIntersections(const std::vector<std::pair<cv::Vec2f, cv::Vec2f>> &lines) {
            std::vector<cv::Vec2f> crosses;
            for (int i = 0; i < lines.size(); i++) {
                for (int j = 0; j < lines.size(); j++) {
                    if (i == j) {
                        continue;
                    }
                    cv::Vec2f cross;
                    if (!intersection(lines[i].first, lines[i].second, lines[j].first, lines[j].second, cross)) {
                        continue;
                    }
                    if (std::abs(cross[0]) > 1e4 || std::abs(cross[1]) > 1e4) {
                        continue;
                    }
                    crosses.emplace_back(cross);

//                    std::cout << cross << std::endl;
                }
            }
            return crosses;
        }

        cv::Mat drawLines(const cv::Mat &image, const std::vector<std::pair<cv::Vec2f, cv::Vec2f>> &lines,
                          const cv::Scalar &color = cv::Scalar(0, 0, 255)) {
            cv::Mat result = image.clone();
            for (auto &l : lines) {

                line(result, (cv::Point) l.first, (cv::Point) l.second, color, 3, cv::LINE_AA);
            }
            return result;
        }

        std::vector<std::pair<cv::Vec2f, cv::Vec2f>>
        extractHoughLines(const cv::Mat &image, double rho, double theta, int threshold) {
            cv::Mat result = image.clone(), buffer = image.clone();
            if (buffer.channels() > 1) {
                cv::cvtColor(image, buffer, cv::COLOR_BGR2GRAY);
            }

            // Standard Hough Line Transform
            std::vector<cv::Point2f> houghLines; // will hold the results of the detection
            HoughLines(buffer, houghLines, rho + 1e-8, CV_PI / theta, threshold); // runs the actual detection
//            HoughLines(edges, houghLines, 0.7, CV_PI / 120., 120, CV_PI / 36.,
//                       CV_PI - CV_PI / 36.); // runs the actual detection

            std::vector<std::pair<cv::Vec2f, cv::Vec2f>> imageLines;
            double lineLength = 10000.;
            for (auto &i : houghLines) {
                float rho = i.x, theta = i.y;

                double a = cos(theta), b = sin(theta);
                double x0 = a * rho, y0 = b * rho;

                imageLines.emplace_back(std::make_pair<cv::Vec2f, cv::Vec2f>(
                        {(float) (x0 + lineLength * (-b)), (float) (y0 + lineLength * (a))},
                        {(float) (x0 - lineLength * (-b)), (float) (y0 - lineLength * (a))}
                ));
            }

            return imageLines;
        }

        std::vector<std::vector<cv::Point>>
        findRectangles(const cv::Mat &cannyImage, double epsilonFactor, int shapeLower, int shapeUpper,
                       bool withLines) {
            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;

            cv::Mat grayscale = cannyImage.clone();
            if (grayscale.channels() > 1) {
                cv::cvtColor(grayscale, grayscale, cv::COLOR_BGR2GRAY);
            }

            cv::findContours(grayscale, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            std::vector<std::vector<cv::Point>> filteredContours;
            for (const auto &contour : contours) {
                auto epsilon = epsilonFactor * cv::arcLength(contour, true);

                std::vector<cv::Point> approx;
                cv::approxPolyDP(contour, approx, epsilon, true);

                if (approx.size() == 3 || approx.size() < shapeLower || approx.size() > shapeUpper) {
                    continue;
                }

                if ((!withLines || approx.size() != 2) && !cv::isContourConvex(approx)) {
                    continue;
                }

//                filteredContours.emplace_back(approx);
                filteredContours.emplace_back(contour);
            }
            return filteredContours;
        }

        bool isWhite(cv::Vec3b color, int min) {
            for (int i = 0; i < 3; i++) {
                if (color[i] < min) {
                    return false;
                }
            }

            return true;
        }

        std::vector<std::vector<cv::Point>>
        filterByWhiteMean(const cv::Mat &colorImage, const std::vector<std::vector<cv::Point>> &contours,
                          int colorMin) {
            std::vector<std::vector<cv::Point>> filteredContours;
            for (const auto &contour : contours) {
                auto mean = calculateMean(contour);
                cv::Vec3b color;
                for (const auto &point : contour) {
                    auto pointF = (cv::Point2f) point;
                    auto inner = mean - pointF;
                    inner = pointF + inner * 0.2f;

                    color = colorImage.at<cv::Vec3b>((cv::Point) inner);
                    if (!isWhite(color, colorMin)) {
                        continue;
                    }
                }

                color = colorImage.at<cv::Vec3b>((cv::Point) mean);
                if (!isWhite(color, colorMin)) {
                    continue;
                }
                filteredContours.emplace_back(contour);
            }
            return filteredContours;
        }


        cv::Mat
        drawContours(const cv::Mat &drawOver, const std::vector<std::vector<cv::Point>> &contours, bool filled) {
            cv::Mat drawing = drawOver.clone();
            if (drawing.channels() < 2) {
                cv::cvtColor(drawing.clone(), drawing, cv::COLOR_GRAY2BGR);
            }
            int lineType = cv::LINE_8;
            int thickness = 2;
            if (filled) {
                thickness = -1;
            }
            for (size_t i = 0; i < contours.size(); i++) {
                cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
                drawContours(drawing, contours, (int) i, color, thickness, lineType, {}, 0);
            }
            return drawing;
        }

        cv::Mat
        drawContours(cv::Size size, const std::vector<std::vector<cv::Point>> &contours, bool filled) {
            return drawContours(
                    cv::Mat::zeros(std::move(size), CV_8UC3),
                    contours, filled
            );
        }


        std::vector<cv::Vec2f> kmeansCluster(const std::vector<cv::Vec2f> &points, int clusterCount = 2) {
            std::vector<cv::Vec2f> centers;
            cv::Mat labels;
            if (clusterCount <= 0 || clusterCount >= points.size()) {
                return centers;
            }
            auto compactness = cv::kmeans(cv::Mat(points, true), clusterCount, labels,
                                          cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10,
                                                           1.0),
                                          3, cv::KMEANS_PP_CENTERS, centers);
            return centers;
        }

        std::vector<cv::Vec2f> linesToDirections(const std::vector<std::pair<cv::Vec2f, cv::Vec2f>> &lines) {
            std::vector<cv::Vec2f> result;
            for (const auto &line:lines) {
                auto direction = line.first - line.second;
                if (direction[1] <= 0) {
                    direction *= -1;
                }
                result.emplace_back(direction / cv::norm(direction));
            }
            return result;
        }

        std::vector<std::vector<uint>>
        dbscanCluster(std::vector<cv::Vec2f> lines) {
            auto dbscan = DBSCAN<cv::Vec2f, float>();
            dbscan.Run(&lines, 2, 1500.f, 1);

            auto noise = dbscan.Noise;
            std::vector<std::vector<uint>> clusters;
            clusters = dbscan.Clusters;
            return clusters;
        }

        std::vector<std::pair<cv::Vec2f, cv::Vec2f>>
        dbscanCluster(const std::vector<std::pair<cv::Vec2f, cv::Vec2f>> &lines) {
            auto clusters = dbscanCluster(linesToDirections(lines));

            std::vector<std::pair<cv::Vec2f, cv::Vec2f>> result;
            for (const auto &cluster : clusters) {
                std::pair<cv::Vec2f, cv::Vec2f> meanLine;
                for (auto i : cluster) {
                    auto correctedLine = lines[i];
                    if (correctedLine.first[1] < correctedLine.second[1]) {
                        correctedLine = {correctedLine.second, correctedLine.first};
                    }
                    meanLine = {meanLine.first + correctedLine.first, meanLine.second + correctedLine.second};
                }
                meanLine = {meanLine.first / (int) cluster.size(), meanLine.second / (int) cluster.size()};
                result.emplace_back(meanLine);
            }
            return result;
        }

        double distanceFromLine(std::pair<cv::Vec2f, cv::Vec2f> &line, const cv::Vec2f &vanishingPoint) {
            auto origin = line.first;
            auto direction = origin - line.second;

            auto d = abs(((cv::Point) vanishingPoint - (cv::Point) origin).cross((cv::Point) direction)) /
                     cv::norm(direction);
            return d;
        }


        std::vector<double> distancesFromVanishingPoint(const std::vector<std::pair<cv::Vec2f, cv::Vec2f>> &lines,
                                                        const cv::Vec2f &vanishingPoint) {
            std::vector<double> distances;
            for (auto line  : lines) {
                double d = distanceFromLine(line, vanishingPoint);
                distances.emplace_back(d);
            }
            return distances;
        }

        std::vector<std::pair<cv::Vec2f, cv::Vec2f>>
        filterByDistancesFromVanishingPoint(const std::vector<std::pair<cv::Vec2f, cv::Vec2f>> &lines,
                                            const cv::Vec2f &vanishingPoint) {
            auto distances = distancesFromVanishingPoint(lines, vanishingPoint);
            auto mean = cv::mean(distances);

            std::vector<std::pair<cv::Vec2f, cv::Vec2f>> filtered;
            for (int i = 0; i < distances.size(); ++i) {
                if (distances[i] < mean[0]) {
                    filtered.emplace_back(lines[i]);
                }
            }

            return filtered;
        }

        cv::Vec2f getHighestVanishingPoint(std::vector<cv::Vec2f> &clusters) {
            float minY = 1e10;
            cv::Vec2f vanishingPoint;
            for (const auto &cluster : clusters) {
                if (cluster[1] < minY) {
                    minY = cluster[1];
                    vanishingPoint = cluster;
                }
            }
            return vanishingPoint;
        }


        cv::Mat edgeDetection(const cv::Mat &image, int cannyLower, int cannyUpper, int detector) {
            cv::Mat result = image.clone();
            int ddepth = CV_16S;
            switch (detector) {
                case 1: {
                    cv::Mat grad_x, grad_y;
                    cv::Sobel(result, grad_x, ddepth, 1, 0, 3);
                    cv::Sobel(result, grad_y, ddepth, 0, 1, 3);
                    cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, result);
                    break;
                }
                case 2: {
                    cv::Laplacian(result, result, ddepth, 3);
                    break;
                }
                default:
                    cv::Canny(result.clone(), result, cannyLower, cannyUpper);
                    break;
            }
            cv::convertScaleAbs(result, result);
//            cv::threshold(result, result, 180, 255, cv::THRESH_BINARY);
//            cv::imshow("Detection", result);

            return result;
        }

        cv::Mat findContours(const cv::Mat &image, double epsilonFactor, int shapeLower, int shapeUpper, int cannyLower,
                             int cannyUpper, bool withLines, int vanishingPoints, double rho, double theta,
                             int threshold, int colorMin, double scale, int detector, int drawHoughLines,
                             double maskTop) {

            cv::Mat color, grayscale, maskedImage;

            int cutOff = (int) (image.rows * maskTop);

            maskedImage = image.clone();
            maskedImage = maskedImage({0, cutOff, image.cols, image.rows - cutOff}).clone();
            cv::copyMakeBorder(maskedImage, maskedImage, cutOff, 0, 0, 0, cv::BORDER_CONSTANT);
            cv::imshow("Masked", maskedImage);

            cv::cvtColor(maskedImage, grayscale, cv::COLOR_BGR2GRAY);
            cv::resize(maskedImage, color, {}, scale, scale);
            cv::resize(grayscale, grayscale, {}, scale, scale);

            cv::Mat result;
            result = edgeDetection(grayscale, cannyLower, cannyUpper, detector);

            auto contours = findRectangles(result, epsilonFactor, shapeLower, shapeUpper, withLines);
            contours = filterByWhiteMean(color, contours, colorMin);
//            result = drawContours(result, contours, true);
            result = drawContours(result.size(), contours, true);
//            cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
//            cv::ximgproc::thinning(result, result);

            contours = findRectangles(result, epsilonFactor, shapeLower, shapeUpper, withLines);
            result = drawContours(result.size(), contours, true);

            if (drawHoughLines) {
                auto lines = extractHoughLines(drawContours(result.size(), contours, true), rho, theta, threshold);
//                auto lines = extractHoughLines(result, rho, theta, threshold);
                auto intersections = calculateIntersections(lines);
                auto clusters = kmeansCluster(intersections, vanishingPoints);
                cv::Vec2f vanishingPoint = getHighestVanishingPoint(clusters);
                lines = filterByDistancesFromVanishingPoint(lines, vanishingPoint);
//                std::cout << lines.size() << std::endl;
//                result = drawLines(result, lines, {255, 255, 255});
                result = drawLines(result, lines);
            }
//            intersections = calculateIntersections(lines, {{0, (float) image.rows},
//                                                           {1, (float) image.rows},});
//            auto lineClusters = dbscanCluster(intersections);
//            std::vector<std::pair<cv::Vec2f, cv::Vec2f>> clusteredLines;
//            for (auto lineCluster : lineClusters) {
//                for (auto index : lineCluster) {
//                    clusteredLines.emplace_back(lines[index]);
//                }
//            }
//            result = drawLines(result, clusteredLines);

//            result = extractHoughLinesP(result);
//            contours = findRectangles(result, epsilonFactor, shapeLower, shapeUpper, true);
//            result = drawContours(result, contours, true);

            cv::resize(result, result, image.size());
            return result;
        }
    }
}


#endif //DATAANNOTATIONTOOLS_LANEDETECTION_HPP
