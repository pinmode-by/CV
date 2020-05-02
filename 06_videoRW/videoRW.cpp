#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;
using std::cout;
using std::endl;
using std::vector;

// finding center of mass of binary image
std::pair<bool,cv::Point> findCenterLine(const Mat& mat,
                     vector<cv::Point>& centers)  {
 int cx = 0, cy = 0, cnt = 0;
 for (int x = 0; x < mat.rows; ++x) {
   int sumRow = 0, counter = 0;
 	 for (int y = 0; y < mat.cols; ++y) {
     if (mat.at<uchar>(x,y)) {
       ++counter;
       cx += y;
       cy += x;
       sumRow += y;
     }
   }
   cnt += counter;
   if ( counter != 0) {
   	centers.push_back(Point(sumRow/counter, x));
   }
 }
 if (!centers.empty()) {
   cx /= cnt;
   cy /= cnt;
 }
 return {!centers.empty(), Point(cx,cy)};           
}

int main(int, char**) {
  // disable the synchronization between the C and C++ standard streams 
  std::cout.sync_with_stdio(false);
  const std::string liveWin("Live");  
  const std::string video_file("Simplex8.avi");
  //const int videoDevice = 0;
	//const std::string videoDevice("/dev/video0");
  
  VideoCapture cap;
  cap.open(video_file); 
	//cap.open(videoDevice); 
  if (!cap.isOpened()) {
      cout << "\nERROR! Unable to open video stream " << endl;
      return -1;
  }
  
  // info about video stream
	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
  int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
  int fps = cap.get(CAP_PROP_FPS);
  cout << "Frame size : " << Size(frame_width, frame_height) << endl;
  cout << "Capturing FPS : " << fps << endl;  
  // only for video file
  const int frames =  cap.get(CAP_PROP_FRAME_COUNT);
  const int fourcc = static_cast<int>(cap.get(CAP_PROP_FOURCC));
  const std::string fourcc_str = format("%c%c%c%c", fourcc & 0xFF,
      (fourcc >> 8) & 0xFF, (fourcc >> 16) & 0xFF, (fourcc >> 24) & 0xFF);
  cout << "Total number of frames :" << frames << endl;  
  cout << "Total time of video : " << ((double)frames/fps) << " s"<< endl;
  cout << "FOURCC (codec) : " << fourcc_str << endl;
  
  // set new parametres for vide camara
  //frame_width = 320; 
  //frame_height = 240; 
  //cap.set(CAP_PROP_FRAME_WIDTH, frame_width);
  //cap.set(CAP_PROP_FRAME_HEIGHT, frame_height);
  //fps = 30; 
  //cap.set(CAP_PROP_FPS, fps);
  
  //cout << "New frame width: " << cap.get(CAP_PROP_FRAME_WIDTH) << endl;
  //cout << "         height: " << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;
  //cout << "New capturing FPS: " << cap.get(CAP_PROP_FPS) << endl;
  
  int th = 140;
  
  const std::string resWin("Simplex on the Line");

  namedWindow(resWin, WINDOW_AUTOSIZE);
  createTrackbar("Threshold", resWin, &th,
                  255, 0, 0);
	//namedWindow(liveWin, WINDOW_AUTOSIZE); 
    
	cout << "Press any key to start: " ;
  char ch;
  std::cin >> ch;
  cout << endl;
  
  // for writing video 
  // const std::string result("Simplex8.avi");
  // MJPG, DIVX, XVID
  // ideoWriter out_video(result, VideoWriter::fourcc('D', 'I', 'V', 'X'),
  //                     fps, Size(frame_width,frame_height), true); 
  // last arg - true/false - color or grey
  
  //if (!out_video.isOpened()) {
  //  cout  << "Could not open the output video for write: " << result << endl;
  //  return -1;
  // }
  
  // main loop
  Mat res(Size(2*frame_width, frame_height), CV_8UC3);
  double lastAngle = 0;
  
  for (;;) {
    Mat src_line(res(Rect(Point(0,0), Size(frame_width, frame_height))));
    Mat line_gray, line_bin_inv;
    cap >> src_line;
    if (src_line.empty()) {
      cout << "The end of the stream" << endl;
      break;
    }		

    //Mat frame_blur;
    //medianBlur(src_line, frame_blur, 5);
    //cvtColor(frame_blur, line_gray, COLOR_BGR2GRAY);
    
    const int frame = cap.get(CAP_PROP_POS_FRAMES);
    const std::string frame_str = std::to_string(frame);
    cvtColor(src_line, line_gray, COLOR_BGR2GRAY);
    threshold(line_gray, line_bin_inv,
              th, 255, cv::THRESH_BINARY_INV);
   
    Mat line_bin(res(Rect(Point(frame_width,0), 
                 Size(frame_width, frame_height))));
    cvtColor(line_bin_inv, line_bin, COLOR_GRAY2BGR);
    line(line_bin, Point((line_bin.cols-1)/2, line_bin.rows-1),
      Point((line_bin.cols-1)/2, 0), Scalar(0, 255, 255), 1);
    
    vector<cv::Point> centers;
    auto[is_line, cp] = findCenterLine(line_bin_inv, centers);
    
    // calculate angel 
    double angle = 0;
    if (is_line) {
			// on line
      int dx = cp.x - (line_bin.cols - 1) / 2;
      int dy = (line_bin.rows - 1) - cp.y; 
      
      if (dy != 0) {
        angle = atan2(dy, dx) * 180 / CV_PI - 90;	
      } else {
      // +90/-90
        angle = (dx > 0) ? 90 : -90; 
        cp = (dx > 0) ? Point(0,line_bin.rows-1) :
                        Point(line_bin.cols-1, line_bin.rows-1);
      }
      lastAngle = angle; 		
    } else {   
      angle = (lastAngle > 0) ? 90 : -90; 
      cp = (lastAngle > 0) ? Point(0, line_bin.rows-1) :
                            Point(line_bin.cols-1, line_bin.rows-1 );  
    }
    
    angle = round(angle * 100);
    cout << "angle : " << angle/100.0 << endl;
    
    // drawing 
    for ( const cv::Point& p : centers ) {
       circle(line_bin, p, 1, Scalar(0, 0, 255), FILLED);
    }
    circle(line_bin, cp, 5, Scalar(0, 255, 0), FILLED);
    arrowedLine(line_bin, Point((line_bin.cols-1)/2, 
                 line_bin.rows-1), cp, 
                 Scalar(255, 0, 0), 2, 8, 0, 0.05);
    putText(src_line, frame_str, Point(270, 30) ,
          FONT_HERSHEY_DUPLEX , 0.7, Scalar(0, 0, 255), 1);
    line(res, Point((res.cols-1)/2, res.rows-1),
          Point((res.cols-1)/2, 0), Scalar(0, 255, 0), 2);
     
    imshow(resWin, res);
    if (waitKey(20) >= 0) {
      cout << "Stop of the stream" << endl;
      break;
    }
    //out_video << res;
  }
  
  //cout << "\nEnded writing to the file : <" << result << ">" << endl;
  //out_video.release();
  cap.release();	
  
  return 0;
}
