#include "BMPImage.h"

int main() {
    std::string filename;
    std::cin >> filename;
    BMPImage image(filename);
    image.DrawLine(100, 100, 200, 200);
    image.DrawLine(100, 200, 200, 100);
    std::cin >> filename;
    image.Save(filename);
    return 0;
}