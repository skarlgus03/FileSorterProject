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

    std::vector<std::shared_ptr<Block>> getRootBlocks() const { return rootLogicBlocks;}
    std::shared_ptr<Block> getExceptionBlock() const { return exceptionBlock;}
    QString getExceptionPath() const { return exceptionPath; }

    void onDeleteBlock(BlockWidget* widget);
    void removeRootBlockArea(RootBlockArea* area);



private slots:
    void createRootBlock();

private:
    QScrollArea* scrollArea;
    CanvasWidget* canvas;
    
    // buttons
    QPushButton* createRootBtn;
    QPushButton* exceptionBtn;
    QPushButton* settingSaveBtn;

    // path Label
    QLabel* exceptionPathLabel = nullptr;

    std::vector<std::shared_ptr<Block>> rootLogicBlocks;
    std::shared_ptr<Block> exceptionBlock;

    QString exceptionPath;

    QVector<RootBlockArea*> rootAreas;
    int nextRootY = 50;
};
