#ifdef _DEBUG

#include "LazyImg.h"
#include <iostream>
#include <string>
#include <thread>

int main()
{
	lazyimg::Mat* img_png = new lazyimg::Mat();
	lazyimg::Mat* img_jpg = new lazyimg::Mat();
	std::string png_path = "C:\\Users\\Wunuo\\Desktop\\test.png";
	ReadImg(img_png, png_path);
	img_png->CloneTo(img_jpg);

	std::string jpg_path = "C:\\Users\\Wunuo\\Desktop\\png2jpg.jpg";
	WriteImg(img_jpg, jpg_path);
	ImgShow("jpg test", img_jpg, 720, 480);
	//ImgShow("png test", img_png, 720, 480);
	return 0;
}
#endif // _DEBUG