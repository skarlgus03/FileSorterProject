#pragma once

#include "FileInfo.h"
#include <vector>
#include <string>

class FileManager {
public:

    // directoryPath(폴더 경로)에 있는 파일들을 검사해서, 각각의 정보를 담은 FileInfo 객체들의 벡터를 반환 
    static std::vector<FileInfo> collectFileInfos(const std::string& directoryPath);

    // 전달받은 FileInfo 객체의 정보에 따라 파일을 실제로 이동시키는 함수
    static bool moveFile(const FileInfo& fileInfo);
};
