#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

struct __attribute__((__packed__)) BMPHeader {
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
};

struct __attribute__((__packed__)) BMPInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerM;
    int32_t yPixelsPerM;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};

class BMPImage {
   public:
    BMPImage(const std::string& filename);
    void printInfo() const;
    void display() const;
    void save(const std::string& filename);
    void plot(int x, int y, uint8_t r, uint8_t g, uint8_t b);

   private:
    void LoadFromFile(const std::string& filename);
    void drawLine(int x1, int y1, int x2, int y2);

   private:
    BMPHeader m_header;
    BMPInfoHeader m_infoHeader;
    std::vector<uint8_t> m_pixels;
    int m_width;
    int m_height;
};