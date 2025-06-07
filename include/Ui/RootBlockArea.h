#pragma once
#include <QFrame>
#include "Ui/BlockWidget.h"

class RootBlockArea : public QFrame {
    Q_OBJECT
public:
    explicit RootBlockArea(QWidget* parent, int y);
    BlockWidget* getRootBlock() const;

    int getTotalHeight() const;
    void updateHeight();

private:
    BlockWidget* rootBlock;
    static constexpr int WIDTH = 1000;
    static constexpr int HEIGHT_PADDING = 20;
    static constexpr int BLOCK_HEIGHT = 80;

};
