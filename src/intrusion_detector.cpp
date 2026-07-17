#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0);
    
    if (!cap.isOpened()) return -1;

    cv::Mat frame;
    cv::Rect safeZone(150, 100, 340, 280); 

    cv::Ptr<cv::BackgroundSubtractor> pBackSubtrac = cv::createBackgroundSubtractorMOG2();
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));

    while (true) {
        cap.read(frame);

        if (frame.empty()) break; 

        cv::rectangle(frame, safeZone, cv::Scalar(130, 205, 45), 2);
        /* cv::putText(frame, "Robot WorK Space", cv::Point(safeZone.x+100, safeZone.y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(25, 205, 155), 2); */
        cv::Mat roi_section = frame(safeZone);
        cv::Mat fMask;

        pBackSubtrac->apply(roi_section, fMask, 0.001);

        cv::morphologyEx(fMask,  fMask, cv::MORPH_OPEN, kernel);

        bool intrusionDetected = false;
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(fMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < contours.size(); i++) {
            if (cv::contourArea(contours[i]) > 2000) {
                intrusionDetected = true;

                cv::Rect boundingBox = cv::boundingRect(contours[i]);
                boundingBox.x += safeZone.x;
                boundingBox.y += safeZone.y;
                cv::rectangle(frame, boundingBox, cv::Scalar(0, 0, 255), 2);
                }
        }

        if (intrusionDetected) {
            cv::putText(frame, "WARNING: INTRUSION DETECTED!", cv::Point(safeZone.x, safeZone.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
        } else {
            cv::putText(frame, "Robot Work Space - CLEAR", cv::Point(safeZone.x, safeZone.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(25, 205, 155), 2);
        }

        cv::imshow("Workspace Intrusion Monitor", frame);
        cv::imshow("Region of Interest", roi_section);
        cv::imshow("Black and while Mask", fMask);

        if (cv::waitKey(30) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    
    return 0;
}