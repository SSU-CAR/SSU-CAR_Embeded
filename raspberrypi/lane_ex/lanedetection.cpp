#include <opencv2/opencv.hpp>
#include <algorithm>
#include <math.h>
#include <time.h>
#include "curl/curl.h"
#include <sstream>
#include <chrono>
#include <ctime>
#include <nlohmann/json.hpp>
#include <string>
#include <wiringPi.h> // raspberry pi library

using namespace cv;
using namespace std;

class LaneDetection
{
public:
	Mat currFrame;
	Mat HSL;
	int window_height = 80;
	int window_width = 90;
	int height = 720;
	//int height = 480;
	int width = 1280;
	//int width = 640;
	vector<int> start_x = {width / 4, width / 2, 3 * width / 4};
	vector<string> laneTypes;
	Scalar lower_white = Scalar(0,0,200);
	Scalar upper_white = Scalar(180, 255, 255);
	const int RightLED = 21; 
	const int LeftLED = 22;
	string server_url = "http://13.209.80.41:8080/driving/embedded";
	
	
	LaneDetection(Mat startFrame)
	{
		currFrame = Mat(height, width, CV_8UC1, 0.0);
		resize(startFrame, currFrame, currFrame.size());
	}
	
	void nextFrame(Mat &nxt)
    {
		//cap.read(nxt);
        resize(nxt ,currFrame, currFrame.size());
    }
	
	Mat birdeyeview(Mat frame)
    {
		Mat birdeye_view;
		// 480p
        //vector<Point2f> src_points = {
            //Point2f(190, 285),
            //Point2f(470, 285),
            //Point2f(610, 380),
            //Point2f(24, 380)
        //};
        // 720p
        //vector<Point2f> src_points = {
          //  Point2f(400, 420),
            //Point2f(926, 420),
            //Point2f(1230, 576),
            //Point2f(30, 576)
        //};
        vector<Point2f> src_points = {
            Point2f(537, 275),
            Point2f(900, 275),
            Point2f(1240, 430),
            Point2f(132, 430)
        };

        vector<Point2f> dest_points = {
            Point2f(320, 720),
            Point2f(960, 720),
            Point2f(960, 0),
            Point2f(320, 0)
        };

        Mat birdeye_tf = getPerspectiveTransform(src_points, dest_points);
        warpPerspective(frame, birdeye_view, birdeye_tf, frame.size());
        
        return birdeye_view;
    }
	
	void analysis_Lanetype(Mat bird_frame)
    {

        for (const auto& start_xpoint : start_x) 
        {
            string type = slidingWindow(bird_frame, start_xpoint, window_width, window_height);
            laneTypes.push_back(type);
        }

        cout << laneTypes[0] << " " << laneTypes[1] << " " << laneTypes[2] << endl;
        // 시나리오
        //illegal_LaneChange(laneTypes);

    }

    string slidingWindow(Mat& image, int start_x, int window_width, int window_height)
    {
        float whitePixels = 0;
        int window_size = window_height * window_width;
        float whiteratio =0;
        float total_window = image.rows/window_height;
        float window_cnt =0;

        for (int y=image.rows; y>0; y -=window_height)
        {
            whitePixels =0;
            Point tl(start_x - window_width, y - window_height);
            Point br(start_x + window_width, y + window_height);
            rectangle(image, tl, br, Scalar(255, 255, 255), 2);

            for (int height=y; height > y-window_height; height--)
            {
                for (int x=start_x - window_width; x<start_x + window_width; x++)
                {
                    if(image.at<uchar>(height,x) >= 200)
                    {    
                        whitePixels++;
                    }
                }
            }

            whiteratio = whitePixels / window_size;

            if(whiteratio > 0.3)
            {
                window_cnt++;
            }
        }

        if (window_cnt >= total_window *0.85)
            return "Solid";
        else if(window_cnt >= total_window *0.5)
            return "Dashed";
        else
            return "None";
    }
    
    using json = nlohmann::json;
    
    void illegal_LaneChange(vector<string> lanetype)
    {
        //bool LeftLED_state = digitalRead(LeftLED);
        //bool RightLED_state = digitalRead(RightLED);

        //if(lanetype[1] != "None" && (LeftLED_state || RightLED_state))
        if(lanetype[1] != "None" && lanetype[2] != "None")
        {
            //string circumstance = "1";
            //report_illegal(circumstance);
        }

        else if(lanetype[0] != "None")
        {
            //string circumstance = "2";
            //report_illegal(circumstance);
        }
    }

    void report_illegal(string circumstance)
    {
        curl_global_init(CURL_GLOBAL_ALL);

        CURL *curl = curl_easy_init();
        if (!curl) 
        {
            std::cerr << "Failed to initialize libcurl" << std::endl;
        }
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        char buffer[80]; 
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&now_time));
        std::string formatted_time(buffer);
        
        
        json j_data;
        j_data[circumstance] = circumstance;
        j_data["createdAt"] = formatted_time;
        
        std::string s = j_data.dump();
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);        
        curl_easy_setopt(curl, CURLOPT_URL, server_url.c_str()); 
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, s.c_str());     
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, s.size());

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) 
        {
            std::cerr << "Failed to send HTTP request: " << curl_easy_strerror(res) << std::endl;
        }
        else
            cout << "success! " << endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
	
};

int main() {
    Mat start_frame;
    Mat curr_frame;
    Mat hsv_frame;
    VideoCapture cap(0);
    
    if (!cap.isOpened()) {
        cerr << "Error: Can not find /dev/video0" << endl;
        return -1;
    }
    double fps = cap.get(cv::CAP_PROP_FPS);
    cout << "Frame per seconds : " << fps << endl;
    
	cap.read(start_frame);
	LaneDetection detection(start_frame);
	resize(detection.currFrame, start_frame, detection.currFrame.size());
	imwrite("captured_img2.jpg", (start_frame));
    while (true) 
    {
		cap.read(start_frame);
		
        if (start_frame.empty()) {
            cerr << "Error: Can no find information from /dev/video0." << endl;
            break;
        }
        
        detection.nextFrame(start_frame);
        curr_frame = detection.currFrame;
        cvtColor(curr_frame, hsv_frame, COLOR_BGR2HSV);
        hsv_frame = detection.birdeyeview(hsv_frame);        
        inRange(hsv_frame, detection.lower_white, detection.upper_white, hsv_frame);
        medianBlur(hsv_frame, hsv_frame, 5);
        detection.analysis_Lanetype(hsv_frame);
        
        imshow("Lane detected frame", hsv_frame);
        Mat origin_frame;
        origin_frame = Mat(detection.height/2, detection.width/2, CV_8UC1, 0.0);
        resize(curr_frame, origin_frame, origin_frame.size());
        imshow("Original frame", origin_frame);
        
        if (waitKey(1) == 'q') {
            break;
        }
    }

    destroyAllWindows();
    
    return 0;
}
