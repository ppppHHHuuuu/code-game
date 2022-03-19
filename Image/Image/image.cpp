#include <iostream>
struct Image
{
	int* pixels;
	int width;
	int height;
	Image(){}
	Image(int *pic, int _width, int _height) {
		pixels = pic;
		width = _width;
		height = _height;
	}
};

void printImage(const Image& img)
{	
	for (int i = 0; i < img.height; i++) {
		for (int j = 0; j < img.width; j++) {
			std::cout << img.pixels[i * img.width + j] << ' ';
		}
		std::cout << std::endl;
	}
}
//void printImagee(const Image& img)
//{
//	int count = 0;
//	for (int i = 0; i < img.width * img.height; i++) {
//		std::cout << img.pixel[i] << ' ';
//		count++;
//		if (count == img.width) {
//			std::cout << std::endl;
//			count = 0;
//		}
//	}
//}
Image halve(const Image& img) {
	int sophantu = 0;
	for (int i = 0; i < img.height; i++) {
		for (int j = 0; j < img.width; j++) {
			if (i % 2 == 0 || j % 2 == 0) {
				sophantu++;
			}
		}
	}
	Image alter;
	alter.pixels = new int[sophantu];
	int sohang = 0;
	int l = 0;
	for (int i = 0; i < img.height; i++) {
		for (int j = 0; j < img.width; j++) {
			if (i % 2 == 0) {
				sohang++;
			}
			if (i % 2 == 0 || j % 2 == 0) {
				alter.pixels[l] = img.pixels[i * img.width + j];
				l++;
			}
		}
	}
	int socot = sophantu / sohang;
	alter.width = socot;
	alter.height = sohang;
	return alter;
}

int main() {
	int pixels[] = {
	1, 3, 0, 5, 2,
	4, 1, 8, 3, 1,
	3, 3, 1, 3, 2
	};
	Image img = { pixels, 5, 3 };	//printImagee(img);

	printImage(img);
	Image a = halve(img);
	printImage(a);
}