#pragma once
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>
#include <memory>
#include <vector>
#include <Block.h>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "Ui/RootBlockArea.h"

class BlockWidget : public QFrame {
    Q_OBJECT
public:
    BlockWidget(QWidget* canvas, BlockWidget* parent, int depth, int y);
    void setLogicBlock(const std::shared_ptr<Block>& block);
    int getTotalHeight() const;
    int getMaxRight() const;
    void relayoutChildren();
    void updateLayoutFromChildGrowth();

    QVector<BlockWidget*> getChildren() const { return children; }
    int getCurrentDepth() { return currentDepth; }

signals:
    void resized();

private slots:
    void addChild();
    void choosePath();
    void onFilterTypeChanged(int index);

private:
    void updateEnabledStates();

    QWidget* canvasRef;
    BlockWidget* parentBlock;
    QPushButton* addChildBtn;
    QLabel* label = nullptr;
    QVector<BlockWidget*> children;

    QComboBox* filterTypeBox;
    QLineEdit* conditionEdit;
    QLabel* movePathLabel;
    QPushButton* movePathBtn;
    QComboBox* comparisonBox;

    int currentDepth;
    int nextChildY = 0;

    std::shared_ptr<Block> logicBlock;
};
