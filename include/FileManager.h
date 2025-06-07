// FileManager.h
#pragma once

#include <QString>

struct FileInfo;

class FileManager {
public:
    static QString moveFile(const FileInfo& fileInfo);
};

