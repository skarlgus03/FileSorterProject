#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include "FileInfo.h"


std::vector<FileInfo> FileManager::collectFileInfos(const std::string& directoryPath) {
    std::vector<FileInfo> files; //지정한 폴더 안의 모든 일반 파일들을 찾아서, 각각의 정보를 FileInfo에 담아 리스트로 반환하는 함수

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) { //directroyPath 폴더를 하나씩 검사
        if (entry.is_regular_file()) {         //폴더가 링크가 아닌 일반 파일들만 처리함
            FileInfo info;
            info.fileName = entry.path().filename().string();         //파일 이름만 추출
            info.extension = entry.path().extension().string();         //파일 확장자만 추출함.
            info.moveToPath = "";         // 파일을 이동할 경로는 아직 비워둠 추후 Classifier가 분류함. 
            info.size = std::filesystem::file_size(entry.path());         //파일 크기 저장
            info.filePath = entry.path().string();         // 전체 경로 저장
            files.push_back(info);         //완성된 FileInfo 객체를 백터에 추가함.
        }
    }

    return files;
}

bool FileManager::moveFile(const FileInfo& fileInfo) { //파일을 실제로 이동시키는 함수
    try { //이동 대상 전체 경로 생성
        std::string toPath = fileInfo.moveToPath + "/" + fileInfo.fileName;         //이동할 폴더가 없다면 생성함
        std::filesystem::create_directories(fileInfo.moveToPath);         // 폴더 없으면 생성
        std::filesystem::rename(fileInfo.filePath, toPath);         //파일을 이동시킴
        return true;          //성공적으로 이동했으면 true 반환함.
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "파일 이동 오류: " << e.what() << std::endl; 
        return false;         //에러가 나면 false를 반환함
    }
}
