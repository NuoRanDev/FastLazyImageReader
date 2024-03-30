#include "LazyImg.h"
#include <iostream>
#include <string>

int main()
{
	lazyimg::Mat* img = new lazyimg::Mat();
	std::string path = "C:\\Users\\Wunuo\\Desktop\\test.png";
	ReadImg(img, path);
	ImgShow("jpg test", img, 720, 480);
	return 0;
}