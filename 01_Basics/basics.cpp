#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using std::vector;
using std::cout;
using std::endl;

template <class T>
void printVector(const vector<T>& vec) {
  for (const auto& item : vec) {
    cout << item << ' '; 
  }
  cout << endl;
}

int main(int argc, char **argv) {

  //const std::string image_path = (argc > 1) ? argv[1] : "./";
  //const std::string image(image_path + "color.jpg");
  
  // 0. openCV and building projects:
  // pkg-config --flags --libs opencv4
  // cmake .. && make
  // CMakeList -> Makefile
  
  // 1. CV_VERSION
  cout << "openCV version:" << CV_VERSION << "\n\n";
  
  // 2. Point 
  // public attributs : x, y
  Point p0;         // default constructor (0,0)
  Point p1(100, 5); // constructor with param
  Point p2(20,60);
  Point p3 = p2;    // copy constructor
  Point p4(p3);     // copy constructor
  Point p5(30, 200); 
  p5 = p1;					// copy assignment operator or operator assigment        
  cout << "Point p1 : " << p1 << endl;
  cout << "Point p5 : " << p5 << endl;
  // vector of Points
  vector<Point> vecP = {p0, p1, p2, p3, p4, p5};
  cout << "Points: " ;
  printVector(vecP);
  cout << endl;
  
  // 3. Size
  // public attributs : width, height
  Size size0;           // default constructor
  Size size1(640, 480); // copy constructor
  Size size2(size1);    // copy constructor
  cout << "size0 : " << size0 << endl;
  cout << "size2: " << size2.width << " x " << size2.height << endl;
  cout << endl;

  // 4. Rect 
  // public attributs : x, y, width, height
  Rect r0;                  //default constructor
  Rect r1(20, 10, 150, 80); // x, y, width, height
  Rect r2(Point(30,60), Size(300,200));
  Rect r3(Point(40,60), Point(100,80));
  Rect r4(r1); 
  Rect r5 (r3);
  r5 = r1;
  cout << "r5: " << r5 << endl;
  vector<Rect> vecR {r0, r1, r2, r3, r4, r5};
  printVector(vecR);
  cout << endl;
  
  // 5.Scalar 
  //
  const Scalar Black(0, 0, 0);
  const Scalar Red(0, 0, 255);
  const Scalar Yellow(0, 255, 255);
  const Scalar White(255, 255, 255);
  const Scalar Color(25, 68, 250);
  cout << "Scalar Color: " << Color << endl; 
  const Scalar black {0}; 
	cout << "Scalar black : " << black << endl; 
  cout << endl;
  
	// uchar - unsigned char 
  // typedef Vec<uchar, 3> cv::Vec3b

  // 6. Mat
  // some constructurs
  // Mat ()
  // Mat (int rows, int cols, int type)
  // Mat (Size size, int type)
  // Mat (int rows, int cols, int type, const Scalar &s);
  // Mat (Size size, int type, const Scalar &s)
  
  Mat img0;   // create an empty image
  cout << "This image is " << img0.rows << " x "
					 << img0.cols << std::endl;
  Mat mat0(4, 3, CV_8UC3, Scalar(135, 200, 180));
  cout << "\nmat0 :\n" ;
  cout << "\twidth: " << mat0.cols << '\n';
  cout << "\theight: " << mat0.rows << '\n';
  cout << "\tsize: " << mat0.size() << '\n';
  cout << "\tchannels: " << mat0.channels() << '\n';
  cout << "mat0 : \n" << mat0 << endl;
 
  Mat mat1(Size(640,480), CV_8UC3, Scalar::all((0)));
  Mat mat2 = Mat::zeros(Size(320,240), CV_8UC3); 
  Mat mat3 = Mat(Size(640,480), CV_8UC3, Black); 
  mat3 = Red;
  
	// show red image
  namedWindow("mat3: Red", WINDOW_AUTOSIZE);
  imshow("mat3: Red", mat3);
  waitKey();
  
  // copy constuctor
  Mat mat4(mat3); // Red 
  // operator assigment
  Mat mat5;
  mat5 = mat4;  // Red
  mat5 = Yellow;
	// all these images point to the same data block
	namedWindow("Mat4: Yellow", WINDOW_AUTOSIZE);
  imshow("Mat4: Yellow", mat4);
	namedWindow("Mat3: Yellow", WINDOW_AUTOSIZE);
  imshow("Mat3: Yellow", mat3);
  waitKey();
  
  return 0;
}

