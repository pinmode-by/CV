#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath> 

#define RPI_VIDEO

using namespace cv;
using std::vector;
using std::cout;
using std::endl;

static void onMouse(int event, int x, int y, int flags, void* param) {
  Mat *pm = (Mat*)(param);
  switch(event) {
    case EVENT_LBUTTONDOWN: {
      const int channels = pm->channels();
      if (channels == 3) {
        cout << "at (" << x << "," << y << ") value is "
           << (pm->at<Vec3b>(Point(x,y))) << endl;
      } else if (channels == 1){
        cout << "at (" << x << "," << y << ") value is "
           << (int)pm->at<uchar>(Point(x,y)) << endl;
      }
    break;
    }
  }
}

int gray = 128, blue = 128, green = 128, red = 128;
Mat imgG(Size(640, 480), CV_8UC1, Scalar(gray));
Mat imgC(Size(640, 480), CV_8UC3, Scalar(blue, green, red));

void on_trackbarG(int value, void* m) {
  imgG = Scalar(gray);  
  imshow( "Gray Image", imgG);
}


void on_trackbarC(int , void*) {
  imgC = Scalar(blue, green, red);
  imshow("Color Image", imgC);
}
	

cv::Point findCenterLine(const Mat& mat,
                     vector<cv::Point>& centers)  {
 // scanning matrix
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
  
 return Point(cx,cy);            
}

int main(int argc, char **argv) {

// 1 method control - trackbar 
  /*{
  namedWindow("Gray Image", WINDOW_AUTOSIZE);
  createTrackbar("Gray", "Gray Image", &gray,
                  255, on_trackbarG, 0);
  imshow("Gray Image", imgG);
  // on_trackbarG(0, 0);
   
  namedWindow("Color Image", WINDOW_AUTOSIZE);
  createTrackbar("Blue ", "Color Image", &blue,
                  255, on_trackbarC, 0);
  createTrackbar("Green", "Color Image", &green,
                  255, on_trackbarC, 0);
  createTrackbar("Red", "Color Image", &red,
                  255, on_trackbarC, 0);
  imshow("Color Image", imgC);

  }
  */
// 2 method control - trackbar 
  /*
  const std::string image_line("Line1.jpg");
  Mat src_line = imread(image_line, IMREAD_GRAYSCALE);
	if (src_line.empty()) {
  	cout << "Could not open or find the image : " 
        << image_line << endl;
    return -1;
  } 
  
  Mat bw = src_line < 50;
  namedWindow("BW Image", WINDOW_AUTOSIZE);
  imshow("BW Image", bw);
  
  namedWindow("Gray Image", WINDOW_AUTOSIZE);
  createTrackbar("Gray", "Gray Image", &gray,
                  255, 0, 0);
  imshow("Gray Image", imgG);
  
  while(waitKey(1) != 'q') {
    imgG = Scalar(gray);  
    imshow( "Gray Image", imgG);
  }       

*/
// 3. reading source file
  
  const std::string image_line("Line2.jpg");
  Mat src_line = imread(image_line, IMREAD_COLOR);
	if (src_line.empty()) {
  	cout << "Could not open or find the image : " 
         << image_line << endl;
    return -1;
  } 

  
  cout << "Source line :\n";
  cout << "\twidth : " << src_line.cols << '\n';
  cout << "\theight : " << src_line.rows << '\n';
  cout << "\tchannels : " << src_line.channels() << endl;
  
  const std::string srcWin("Source Line");
  const std::string grayWin("Gray Line");
	const std::string binWin("Binary Line");

	Mat line_gray, line_bin_inv;
  cvtColor(src_line, line_gray, COLOR_BGR2GRAY);

  namedWindow(srcWin, WINDOW_AUTOSIZE);
  imshow(srcWin, src_line);
  namedWindow(grayWin, WINDOW_AUTOSIZE);
  imshow(grayWin, line_gray);
  
  int th = 128;
  namedWindow(binWin, WINDOW_AUTOSIZE);
  createTrackbar("Threshold", grayWin, &th,
                  255, 0, 0);
  setMouseCallback(grayWin, onMouse, (void*) &line_gray);
 	
  for (;;) {
    threshold(line_gray, line_bin_inv,
	          th, 255, cv::THRESH_BINARY_INV);
    Mat line_bin;
    cvtColor(line_bin_inv, line_bin, COLOR_GRAY2BGR);
    line(line_bin, Point((line_bin.cols-1)/2, line_bin.rows-1),
              Point((line_bin.cols-1)/2, 0), Scalar(0, 255, 255), 1);
    
    vector<cv::Point> centers;
    Point cp = findCenterLine(line_bin_inv, centers);
    
    //cout << "Center mass of line: (" << cp.x << ", " << cp.y << ")\n";
    
		double angle = 0;
		int px = cp.x - (line_bin.cols - 1) / 2;
		int py = (line_bin.rows - 1) - cp.y; 
		//cout << "px = " << px << "  py = " << py << endl;

		if (py != 0) {
			angle = atan2(py, px) * 180 / CV_PI - 90;
			cout << "angle = " << round(angle*100)/100 << endl;
		}

		for ( const cv::Point& p : centers ) {
       circle(line_bin, p, 1, Scalar(0, 0, 255), FILLED);
    }
    circle(line_bin, cp, 5, Scalar(0, 255, 0), FILLED);
    arrowedLine(line_bin, Point((line_bin.cols-1)/2, 
                 line_bin.rows-1), cp, 
								Scalar(255, 0, 0), 2, 8, 0, 0.05);
    imshow(binWin, line_bin);
   	
    if (waitKey(1) >= 0) {
    	break;
		}
  }
  
	waitKey();

  return 0;
}

