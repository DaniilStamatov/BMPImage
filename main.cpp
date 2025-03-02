#include "BMPImage.h"

int main() {
    BMPImage image("new_icon2.bmp");
    image.display();
    image.save("new_icon2.bmp");
    return 0;
}