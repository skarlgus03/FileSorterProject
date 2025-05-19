#pragma once

#include "FileInfo.h"
#include <vector>
#include <string>

class FileManager {
public:
    static std::vector<FileInfo> collectFileInfos(const std::string& directoryPath);
    static bool moveFile(const FileInfo& fileInfo);
};
