#pragma once


#include <string>
#include <QString>


struct FileInfo {
    QString fileName;      // 파일 이름
    QString filePath;      // 원본 파일 경로
    QString extension;     // 확장자
    QString moveToPath;    // 이동할 경로
    QString size;          // 파일 크기
    QString date;          // 파일 날짜

   
};


