#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include "FileInfo.h"


//지정한 폴더 안의 모든 일반 파일들을 찾아서, 각각의 정보를 FileInfo에 담아 리스트로 반환하는 함수
std::vector<FileInfo> FileManager::collectFileInfos(const std::string& directoryPath) {
    std::vector<FileInfo> files; 

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) { //directroyPath 폴더를 하나씩 검사
        if (entry.is_regular_file()) {          //폴더가 링크가 아닌 일반 파일들만 처리함
            FileInfo info;
            info.fileName = entry.path().filename().string();         // 파일 이름만 추출
            info.extension = entry.path().extension().string();       // 파일 확장자만 추출함.
            info.moveToPath = "";                                     // 파일을 이동할 경로는 아직 비워둠 추후 Classifier가 분류함. 
            info.size = std::filesystem::file_size(entry.path());     // 파일 크기 저장
            info.filePath = entry.path().string();                    // 전체 경로 저장
            
            // 해당 파일의 마지막 수정시간을 가지고 (ftime은 c++ 시간 표현 방식 중 하나)
            auto ftime = std::filesystem::last_write_time(entry);     
            
            // ftime을 현재 system_clock의 시간 포맷으로 바꾸는 과정
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                            ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
            
            //변환된 system_clock 시간을 time_t 타입(일반적인 시간 표현)으로 바꿈
            std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

            //time_t 값을 srttime 형식으로 포맷함 (예시: 2025-05-21 xx:xx:xx 형태로 info.date에 저장)
            std::stringstream ss;
            ss << std::put_time(std::localtime(&cftime), "%Y-%m-%d %H:%M:%S");
            info.date = ss.str();

            //완성된 FileInfo 객체를 백터에 추가함.
            files.push_back(info);         

            
        }
    }

    return files;
}


//파일을 실제로 이동시키는 함수
bool FileManager::moveFile(const FileInfo& fileInfo) { 
    try { 
        std::string toPath = fileInfo.moveToPath + "/" + fileInfo.fileName;         //이동할 폴더가 없다면 생성함
        std::filesystem::create_directories(fileInfo.moveToPath);         // 폴더 없으면 생성
        std::filesystem::rename(fileInfo.filePath, toPath);         //파일을 이동시킴
        return true;          
    
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "파일 이동 오류: " << e.what() << std::endl; 
        return false;         
    }
}
