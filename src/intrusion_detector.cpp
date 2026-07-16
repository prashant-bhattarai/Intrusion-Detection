#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0);
    
    if (!cap.isOpened()) return -1;

    cv::Mat frame;
    cv::Rect safeZone(150, 100, 340, 280); 

    cv::Ptr<cv::BackgroundSubtractor> pBackSubtrac = cv::createBackgroundSubtractorMOG2();


    while (true) {
        cap.read(frame);

        if (frame.empty()) break; 

        cv::rectangle(frame, safeZone, cv::Scalar(130, 205, 45), 2);
        cv::putText(frame, "Robot WorK Space", cv::Point(safeZone.x+100, safeZone.y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(25, 205, 155), 2);
        cv::Mat roi_section = frame(safeZone);
        cv::Mat fMask;

        pBackSubtrac->apply(roi_section, fMask);

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