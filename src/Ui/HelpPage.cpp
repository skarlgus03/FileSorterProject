#include "Ui/HelpPage.h"
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

HelpPage::HelpPage(QWidget* parent) : QWidget(parent) {

    titleLabel = new QLabel("📘 사용 설명서", this);
    QFont titleFont("Noto Sans KR", 16, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("margin-bottom: 10px;");

    descriptionText = new QTextEdit(this);
    descriptionText->setReadOnly(true);
    QFont bodyFont("Noto Sans KR", 12);
    descriptionText->setFont(bodyFont);
    descriptionText->setText(R"(
이 프로그램은 블럭을 이용하여 파일을 분류하는 프로그램입니다.


🧩 블럭 구성방법

- '부모 생성' 버튼을 눌러 루트 블럭을 추가합니다.
- 각 블럭의 필터타입, 조건, 이동될 경로 등을 설정합니다.
- '+' 을 눌러 블럭의 자식블럭을 추가할 수 있습니다.


💾 저장 및 불러오기

- '설정 저장' 버튼으로, JSON형식으로 블럭의 구조를 저장합니다
- '설정 불러오기' 버튼으로, JSON 파일을 선택해 블럭을 불러올 수 있습니다.


📂 예외 경로

- '예외 경로' 버튼을 눌러 모든 조건에 해당되지 않는 파일이 이동될 경로를 지정할 수 있습니다.


📅 날짜 필터 형식
- YYYY-MM-DD~YYYY-MM-DD 형식의 날짜 범위를 입력할 수 있습니다. (예: 2025-06-12~2025-06-30)
- ~2025-06-30 -> 2025 6월 30일 까지 전부
- 2025-06-30~ -> 2025 6월 30일 부터 전부
)");
	layout = new QVBoxLayout(this);
	layout->addWidget(titleLabel);
	layout->addWidget(descriptionText);
	setLayout(layout);
}
