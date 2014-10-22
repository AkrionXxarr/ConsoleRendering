#include "TextLogger.h"

using namespace std;

TextLogger::TextLogger(string fileName)
{
    this->fileName = fileName.append(".txt");
    fs.open(this->fileName, ofstream::out | ofstream::trunc);
    fs.close();
}

TextLogger::~TextLogger()
{
    if (fs.is_open())
        fs.close();
}

void TextLogger::AddLine(string line)
{
    fs.open(fileName, ofstream::out | ofstream::app);
    fs << line;
    fs.close();
}