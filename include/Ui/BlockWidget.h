#pragma once
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <memory>
#include <vector>

#include <Block.h>

class BlockWidget : public QFrame {
    Q_OBJECT
public:
    BlockWidget(QWidget* canvas, BlockWidget* parentBlock, int depth, int y);
    int getTotalHeight() const;
    void relayoutChildren();
    void updateLayoutFromChildGrowth();
    QVector<BlockWidget*> getChildren() const;
signals:
    void resized();

protected:

private slots:
    void addChild();

private:
    QWidget* canvasRef;
    BlockWidget* parentBlock;
    QPushButton* addChildBtn;
    QLabel* label;
    QVector<BlockWidget*> children;


    int currentDepth;
    int nextChildY = 0;

    static constexpr int BLOCK_WIDTH = 120;
    static constexpr int BLOCK_HEIGHT = 80;
    static constexpr int H_SPACING = 150;
    static constexpr int V_SPACING = 10;

    std::shared_ptr<Block> logicBlock;
};
