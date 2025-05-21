#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include "FileInfo.h"


std::vector<FileInfo> FileManager::collectFileInfos(const std::string& directoryPath) {
    std::vector<FileInfo> files;

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            FileInfo info;
            info.fileName = entry.path().filename().string();
            info.extension = entry.path().extension().string();
            info.moveToPath = ""; // 추후 Classifier가 결정
            info.size = std::filesystem::file_size(entry.path());
            info.filePath = entry.path().string(); // 전체 경로 저장
            files.push_back(info);
        }
    }

    return files;
}

bool FileManager::moveFile(const FileInfo& fileInfo) {
    try {
        std::string toPath = fileInfo.moveToPath + "/" + fileInfo.fileName;
        std::filesystem::create_directories(fileInfo.moveToPath); // 폴더 없으면 생성
        std::filesystem::rename(fileInfo.filePath, toPath);
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "파일 이동 오류: " << e.what() << std::endl;
        return false;
    }
}
