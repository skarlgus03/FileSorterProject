#pragma once
#include <QFrame>

class BlockWidget;

class RootBlockArea : public QFrame {
    Q_OBJECT
public:
    explicit RootBlockArea(QWidget* parent, int y);
    BlockWidget* getRootBlock() const;


    int getTotalHeight() const;
    void updateSize();
    int calculateMaxDepth(BlockWidget* node) const;
    int getTotalWidth() const;


private:
    BlockWidget* rootBlock;
    static constexpr int WIDTH = 1000;
    static constexpr int HEIGHT_PADDING = 20;

};
