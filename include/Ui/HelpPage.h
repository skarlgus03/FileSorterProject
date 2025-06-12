#pragma once

#include <QWidget>

class QLabel;
class QTextEdit;
class QVBoxLayout;

class HelpPage : public QWidget {
	Q_OBJECT
public:
	explicit HelpPage(QWidget* parent = nullptr);

private:
	QLabel* titleLabel;
	QTextEdit* descriptionText;
	QVBoxLayout* layout;
};