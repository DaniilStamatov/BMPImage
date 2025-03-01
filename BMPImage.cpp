#include "BMPImage.h"

BMPImage::BMPImage(const std::string& filename) { LoadFromFile(filename); }

void BMPImage::printInfo() const {
    std::cout << "BMP File Information:" << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "File Type: " << (m_header.fileType == 0x4D42 ? "BM" : "Unknown") << std::endl;
    std::cout << "File Size: " << m_header.fileSize << " bytes" << std::endl;
    std::cout << "Reserved1: " << m_header.reserved1 << " bytes" << std::endl;
    std::cout << "Reserved1: " << m_header.reserved2 << " bytes" << std::endl;
    std::cout << "Data Offset: " << m_header.dataOffset << " bytes" << std::endl;
    std::cout << "Width: " << m_infoHeader.width << " pixels" << std::endl;
    std::cout << "Height: " << m_infoHeader.height << " pixels" << std::endl;
    std::cout << "Planes: " << m_infoHeader.planes << std::endl;
    std::cout << "Bit Count: " << m_infoHeader.bitCount << " bits per pixel" << std::endl;
    std::cout << "Compression: " << m_infoHeader.compression << std::endl;
    std::cout << "Image Size: " << m_infoHeader.imageSize << " bytes" << std::endl;
    std::cout << "X Pixels Per Meter: " << m_infoHeader.xPixelsPerM << std::endl;
    std::cout << "Y Pixels Per Meter: " << m_infoHeader.yPixelsPerM << std::endl;
    std::cout << "Colors Used: " << m_infoHeader.colorsUsed << std::endl;
    std::cout << "Important Colors: " << m_infoHeader.colorsImportant << std::endl;
    std::cout << "----------------------" << std::endl;
}

void BMPImage::display() const {
    int pixelSize = m_infoHeader.bitCount / 8;
    int rowSize = (m_width * pixelSize);
    for (int y = m_height - 1; y >= 0; --y) {
        for (size_t x = 0; x < m_width; ++x) {
            int index = (y * rowSize) + (x * pixelSize);
            uint8_t b = m_pixels[index];
            uint8_t g = m_pixels[index + 1];
            uint8_t r = m_pixels[index + 2];

            if (r == 0 && g == 0 && b == 0) {
                std::cout << '#';
            } else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
}

void BMPImage::drawLine(int x1, int y1, int x2, int y2) {
    int deltax = abs(x2 - x1);
    int deltay = abs(y2 - y1);
    int error = 0;
    int deltaerr = deltay + 1;
    int y = y1;
    int diry = y1 < y2 ? 1 : -1;
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    for (int x = x1; x <= x2; ++x) {
        plot(x, y, 0, 0, 0);
        error += deltaerr;
        if (error >= (deltax + 1)) {
            y += diry;
            error -= (deltax + 1);
        }
    }
}

void BMPImage::save(const std::string& filename) {
    std::ofstream os(filename, std::ios::binary);
    if (!os) {
        throw std::runtime_error("Could not open file " + filename);
    }

    os.put('B');
    os.put('M');
    os.write(reinterpret_cast<const char*>(&m_header.fileSize), sizeof(m_header.fileSize));
    os.write(reinterpret_cast<const char*>(&m_header.reserved1), sizeof(m_header.reserved1));
    os.write(reinterpret_cast<const char*>(&m_header.reserved2), sizeof(m_header.reserved2));
    os.write(reinterpret_cast<const char*>(&m_header.dataOffset), sizeof(m_header.dataOffset));

    os.write(reinterpret_cast<const char*>(&m_infoHeader.size), sizeof(m_infoHeader.size));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.width), sizeof(m_infoHeader.width));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.height), sizeof(m_infoHeader.height));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.planes), sizeof(m_infoHeader.planes));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.bitCount), sizeof(m_infoHeader.bitCount));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.compression), sizeof(m_infoHeader.compression));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.imageSize), sizeof(m_infoHeader.imageSize));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.xPixelsPerM), sizeof(m_infoHeader.xPixelsPerM));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.yPixelsPerM), sizeof(m_infoHeader.yPixelsPerM));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.colorsUsed), sizeof(m_infoHeader.colorsUsed));
    os.write(reinterpret_cast<const char*>(&m_infoHeader.colorsImportant),
             sizeof(m_infoHeader.colorsImportant));

    os.seekp(m_header.dataOffset, std::ios::beg);

    int pixelSize = m_infoHeader.bitCount / 8;
    int rowSize = (m_width * pixelSize + 3) & ~3;
    for (int y = 0; y < (int)m_height; ++y) {
        for (int x = 0; x < (int)m_width; ++x) {
            int index = (y * rowSize) + (x * pixelSize);
            m_pixels[index] = 255;
            m_pixels[index + 1] = 255;
            m_pixels[index + 2] = 255;
            if (pixelSize == 4) {
                m_pixels[index + 3] = 255;
            }
        }
    }
    for (int y = 0; y < (int)m_height; ++y) {
        os.write(reinterpret_cast<const char*>(&m_pixels[y * rowSize]), rowSize);
    }

    os.close();
}

void BMPImage::plot(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= 0 && x < (int)m_width && y >= 0 && y < (int)m_height) {
        int pixelSize = m_infoHeader.bitCount / 8;
        int rowSize = (m_width * pixelSize + 3) & ~3;
        int index = (y * rowSize) + (x * pixelSize);

        m_pixels[index] = b;
        m_pixels[index + 1] = g;
        m_pixels[index + 2] = r;
    }
}

void BMPImage::LoadFromFile(const std::string& filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is) {
        throw std::runtime_error("Could not open file " + filename);
    }

    is.read(reinterpret_cast<char*>(&m_header), sizeof(m_header));
    if (!is) {
        throw std::runtime_error("Could not read BMP header from file " + filename);
    }
    if (m_header.fileType != 0x4D42) {
        throw std::runtime_error("Not a valid BMP file " + filename);
    }

    is.read(reinterpret_cast<char*>(&m_infoHeader), sizeof(m_infoHeader));
    if (!is) {
        throw std::runtime_error("Could not read BMP info header from file " + filename);
    }
    if (m_infoHeader.planes != 1) throw std::runtime_error("Unknown format");
    if (m_infoHeader.bitCount < 24) throw std::runtime_error("Unknown format");
    m_width = m_infoHeader.width;
    m_height = abs(m_infoHeader.height);
    std::cout << m_width << " " << m_height << std::endl;

    int pixelSize = m_infoHeader.bitCount / 8;
    int rowSize = (m_width * pixelSize + 3) & ~3;
    int size = rowSize * m_height;
    m_pixels.resize(size);
    is.seekg(m_header.dataOffset, std::ios::beg);
    is.seekg(m_header.dataOffset, std::ios::beg);
    for (int y = 0; y < (int)m_height; ++y) {
        is.read(reinterpret_cast<char*>(&m_pixels[y * rowSize]), rowSize);
    }
    is.close();
}