#include "BMPImage.h"

int main() {
    BMPImage image("icon.bmp");
    BMPImage image1("new_icon.bmp");
    image1.save("new_icon.bmp");
    return 0;
}