#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include "core/allheaders.h"
#include "strngs.h"
#include "api/baseapi.h"
#pragma comment(lib, "tesseract40.lib")
#pragma comment(lib, "leptonica-1.76.0.lib")
#pragma comment(lib, "libpng16.lib")
using namespace std;
string UTF8ToGBK(const char* strUTF8)
{

	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}
void test(char *argv)
{
	char *outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	if (api->Init(NULL, "chi_sim")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	Pix *image = pixRead(argv);
	api->SetImage(image);
	outText = api->GetUTF8Text();
	cout << UTF8ToGBK(outText) << endl;
	api->End();
	delete[] outText;
	pixDestroy(&image);
}
void test2(char *argv)
{
	char *outText;
	tesseract::TessBaseAPI  *api = new tesseract::TessBaseAPI();
	if (api->Init(NULL, "chi_sim")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	Pix *image = pixRead(argv);
	api->SetImage(image);
	Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
	for (int i = 0; i < boxes->n; i++)
	{
		BOX* box = boxaGetBox(boxes, i, L_CLONE);
		api->SetRectangle(box->x, box->y, box->w, box->h);

		char* ocrResult = api->GetUTF8Text();
		int conf = api->MeanTextConf();
		fprintf(stdout, "\n识别结果:索引[%d]--坐标[%d,%d]--区域[%d,%d]--概率[%d]--结果[%s]", i, box->x, box->y, box->w, box->h, conf, UTF8ToGBK(ocrResult));
	}
	api->End();
	pixDestroy(&image);
}
int main(int argc, char *argv[])
{
	if (strcmp(argv[2], "true")==0) {
		test2(argv[1]);
	}else{
		test(argv[1]);
	}
	return 0;
}
