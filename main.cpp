#include "BMPImage.h"

int main() {
    BMPImage image("res/icon.bmp");
    image.Display();
    image.DrawLine(100, 100, 200, 200, COLOR_BLACK);
    image.DrawLine(100, 200, 200, 100, COLOR_BLACK);
    image.Save("result/icon.bmp");
    image.Display();
    return 0;
}