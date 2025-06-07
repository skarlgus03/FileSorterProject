#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include "FileInfo.h"
#include <sstream>




//파일을 실제로 이동시키는 함수

QString FileManager::moveFile(const FileInfo& fileInfo) {
    std::ostringstream log;

    try {
        if (fileInfo.moveToPath.empty()) {
            log << "[이동 실패] moveToPath가 비어있습니다: " << fileInfo.fileName;
            return QString::fromStdString(log.str());
        }

        if (!std::filesystem::exists(fileInfo.filePath)) {
            log << "[이동 실패] 원본 파일이 존재하지 않습니다: " << fileInfo.filePath;
            return QString::fromStdString(log.str());
        }

        std::filesystem::path from(fileInfo.filePath);
        std::filesystem::path to(fileInfo.moveToPath);
        to /= from.filename();

        if (from == to) {
            log << "[이동 생략] 원본과 대상이 동일 경로입니다: " << to.string();
            return QString::fromStdString(log.str());
        }

        if (std::filesystem::exists(to)) {
            log << "[이동 실패] 대상 경로에 이미 존재합니다: " << to.string();
            return QString::fromStdString(log.str());
        }

        std::filesystem::create_directories(fileInfo.moveToPath);
        std::filesystem::rename(from, to);

        log << "[이동 성공] " << from.string() << " → " << to.string();
        return QString::fromStdString(log.str());

    }
    catch (const std::filesystem::filesystem_error& e) {
        log << "[예외 발생] " << e.what();
        return QString::fromStdString(log.str());
    }
}
