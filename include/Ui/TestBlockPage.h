#pragma once
#include <QWidget>
#include <QVector>

#include "Ui/rootblockarea.h"
#include "Ui/CanvasWidget.h"
#include "Ui/BlockWidget.h"

class QPushButton;
class QScrollArea;
class CanvasWidget;
class RootBlockArea;

class TestBlockPage : public QWidget {
    Q_OBJECT
public:
    explicit TestBlockPage(QWidget* parent = nullptr);
    void recalculateAllLayout();
private slots:
    void createRootBlock();

private:
    QScrollArea* scrollArea;
    CanvasWidget* canvas;
    QPushButton* btnCreateRoot;

    QVector<RootBlockArea*> rootAreas;
    int nextRootY = 50;
};
