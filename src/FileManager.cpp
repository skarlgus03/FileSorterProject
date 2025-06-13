#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include "FileInfo.h"
#include <sstream>




//파일을 실제로 이동시키는 함수

QString FileManager::moveFile(const FileInfo& fileInfo) {
    QString log;

    try {
        if (fileInfo.moveToPath.isEmpty()) {
            log = QStringLiteral("❌ [이동 실패] moveToPath가 비어있습니다: %1").arg(fileInfo.fileName);
            return log;
        }

        std::filesystem::path from(fileInfo.filePath.toStdWString());
        std::filesystem::path to(fileInfo.moveToPath.toStdWString());
        to /= from.filename();

        if (!std::filesystem::exists(from)) {
            log = QStringLiteral("❌ [이동 실패] 원본 파일이 존재하지 않습니다: %1").arg(QString::fromStdWString(from.wstring()));
            return log;
        }

        if (from == to) {
            log = QStringLiteral("⚠️ [이동 생략] 원본과 대상이 동일 경로입니다: %1").arg(QString::fromStdWString(to.wstring()));
            return log;
        }

        if (std::filesystem::exists(to)) {
            log = QStringLiteral("❌ [이동 실패] 대상 경로에 이미 존재합니다: %1").arg(QString::fromStdWString(to.wstring()));
            return log;
        }

        std::filesystem::create_directories(to.parent_path());
        std::filesystem::rename(from, to);

        log = QStringLiteral("✅ [이동 성공] %1 → %2")
            .arg(QString::fromStdWString(from.wstring()))
            .arg(QString::fromStdWString(to.wstring()));
        return log;
    }
    catch (const std::filesystem::filesystem_error& e) {
        log = QStringLiteral("🔥 [예외 발생] %1").arg(e.what());
        return log;
    }
}
