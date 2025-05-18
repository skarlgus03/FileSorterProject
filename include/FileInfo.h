#ifndef FILEINFO_H
#define FILEINFO_H

#include <string>

struct FileInfo {
    std::string fileName;      // 파일 이름
    std::string filePath;      // 원본 파일 경로
    std::string extension;     // 확장자
    std::string moveToPath;    // 이동할 경로
    std::string date;          // 파일 날짜
   
};

#endif 
