#pragma once

#include <fstream>
#include <string>
#include <sstream>

class TextLogger
{
public:
    TextLogger(std::string fileName);
    ~TextLogger();

    void AddLine(std::string line);

private:
    std::string fileName;
    std::ofstream fs;
};