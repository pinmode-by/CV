#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;
using std::cout;
using std::endl;


int main(int, char**) {

  const std::string liveWin("Live");  
  const std::string video_file("lineSimplex.avi");
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
	const int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
  const int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
  const int fps = cap.get(CAP_PROP_FPS);
  const int frames =  cap.get(CAP_PROP_FRAME_COUNT);
  const int fourcc = static_cast<int>(cap.get(CAP_PROP_FOURCC));
  const std::string fourcc_str = format("%c%c%c%c", fourcc & 0xFF,
	(fourcc >> 8) & 0xFF, (fourcc >> 16) & 0xFF, (fourcc >> 24) & 0xFF);
  cout << "Frame size : " << Size(frame_width, frame_height) << endl;
  cout << "Capturing FPS : " << fps << endl;  
  cout << "Total number of frames :" << frames << endl;  
  cout << "Total time of video : " << ((double)frames/fps) << " s"<< endl;
  cout << "FOURCC (codec) : " << fourcc_str << endl;
  
 
  namedWindow(liveWin, WINDOW_AUTOSIZE); 
    
	cout << "Press any key to start: " ;
  char ch;
  std::cin >> ch;
  cout << endl;
  
  // main loop
  for(;;) {
    Mat frame;
    cap >> frame; 
    if (frame.empty()) {
      cout << "\nFound the end of the stream" << endl;
      break;
    }
    
    // smth to do
    
    imshow(liveWin, frame);
    waitKey(30);
  }

  cap.release();	
  
  return 0;
}
