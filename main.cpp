#include "BMPImage.h"
#include <filesystem>
std::string GetFullPath() {
    std::string path;
    while (true) {
        std::cout << "Введите полный путь к BMP файлу: ";
        std::getline(std::cin, path);

        if (std::filesystem::path(path).is_absolute()) {
            return path;
        } else {
            std::cout << "Ошибка: Пожалуйста, введите полный путь к файлу." << std::endl;
        }
    }
}

int main() {
    std::string filename = GetFullPath();
    BMPImage image(filename);
    image.Display();
    image.DrawLine(100, 100, 200, 200, COLOR_BLACK);
    image.DrawLine(100, 200, 200, 100, COLOR_BLACK);
    filename = GetFullPath();
    image.Save(filename);
    image.Display();
    return 0;
}