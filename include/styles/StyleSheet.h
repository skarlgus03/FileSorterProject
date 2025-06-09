#pragma once
#include <QString>


//스타일 시트

inline QString defaultButtonStyle() {
    return R"(
        QPushButton{
        background - color: #16162b;
        color: #ffffff;
        border: 2px solid #464964;
        border - radius: 6px;
        }
        QPushButton:hover{
            background - color: #3B82F6;
            color: white;
        }
    )";
}

inline QString comboBoxStyle() {
    return R"(
        QComboBox {
            background-color: #2b2f40;
            color: #dcdfe4;
            border: 1px solid #3b4c6e;
            border-radius: 6px;
            padding-left: 6px;
        }
        QComboBox:hover {
            border-color: #3b78ff;
        }
        QComboBox::drop-down {
            border: none;
        }
    )";
}

inline QString smallButtonStyle() {
    return R"(
        QPushButton {
            background-color: #2e3440;
            color: white;
            border: 1px solid #4c566a;
            border-radius: 4px;      
            min-width: 16px;
            min-height: 16px;
            max-width: 20px;
            max-height: 20px;
            padding: 0;
        }
        QPushButton:hover {
            background-color: #3b4252;
            border-color: #88c0d0;
        }
    )";
}