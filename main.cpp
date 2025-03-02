#include "BMPImage.h"

int main() {
    std::string filename;
    std::cin >> filename;
    BMPImage image(filename);
    image.Display();
    image.DrawLine(100, 100, 200, 200, COLOR_BLACK);
    image.DrawLine(100, 200, 200, 100, COLOR_BLACK);
    std::cin >> filename;
    image.Save(filename);
    image.Display();
    return 0;
}