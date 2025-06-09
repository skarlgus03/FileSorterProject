#pragma once
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "Ui/RootBlockArea.h"
#include <Block.h>
class BlockWidget : public QFrame {
    Q_OBJECT
public:
    BlockWidget(QWidget* canvas, BlockWidget* parent, int depth, int y);
    void setLogicBlock(const std::shared_ptr<Block>& block);
    int getTotalHeight() const;
    int getMaxRight() const;
    void relayoutChildren();
    void updateLayoutFromChildGrowth();
    void removeChild(BlockWidget* child);

    QVector<BlockWidget*> getChildren() const { return children; }
    int getCurrentDepth() { return currentDepth; }
    std::shared_ptr<Block> getBlock() const { return logicBlock; }
    BlockWidget* getParentBlock() const { return parentBlock; }

   
    void performSelfDelete();
signals:
    void resized();
    void requestDelete(BlockWidget* self);

private slots:
    void addChild();
    void choosePath();
    void onFilterTypeChanged(int index);

private:
    void updateEnabledStates();

    QWidget* canvasRef;
    BlockWidget* parentBlock;
    
    QLabel* label = nullptr;
    QVector<BlockWidget*> children;

    QPushButton* addChildBtn;
    QComboBox* filterTypeBox;
    QLineEdit* conditionEdit;
    QLabel* movePathLabel;
    QPushButton* movePathBtn;
    QComboBox* comparisonBox;
    QPushButton* deleteBtn;

    int currentDepth;
    int nextChildY = 0;

    std::shared_ptr<Block> logicBlock;
};
