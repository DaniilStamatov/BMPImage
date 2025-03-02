#include "BMPImage.h"

int main() {
    BMPImage image("icon.bmp");
    image.display();
    image.save("new_icon2.bmp");
    return 0;
}