#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "FileInfo.h"
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>

class FileManager {
public:
    FileManager() = delete; // 객체 생성 금지

    // 파일 정보 수집
    static std::vector<FileInfo> collectFileInfos(const std::string& directoryPath) {
        std::vector<FileInfo> files;

        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                FileInfo info;
                info.keyword = entry.path().filename().string();
                info.extension = entry.path().extension().string();
                info.moveToPath = ""; // 분류는 Classifier에서 설정
                info.size = std::filesystem::file_size(entry.path());
                info.originalFullPath = entry.path().string();
                files.push_back(info);
            }
        }

        return files;
    }

    // 파일 이동
    static bool moveFileToDestination(const FileInfo& fileInfo) {
        try {
            const std::string& destinationDir = fileInfo.moveToPath;
            const std::string& fileName = fileInfo.keyword;
            std::string fullDestinationPath = destinationDir + "/" + fileName;

            std::filesystem::create_directories(destinationDir); // 폴더 없으면 생성
            std::filesystem::rename(fileInfo.originalFullPath, fullDestinationPath); // 이동
            return true;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "파일 이동 실패: " << e.what() << std::endl;
            return false;
        }
    }
};

#endif // FILE_MANAGER_H
