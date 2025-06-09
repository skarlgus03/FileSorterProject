#pragma once
#include <QWidget>
#include "Ui/blockwidget.h"
#include "Ui/rootblockarea.h"

class CanvasWidget : public QWidget {
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget* parent = nullptr);
    void addRootArea(RootBlockArea* area);
    void clearRootAreas();
    void repaintAll();
    void removeRootArea(RootBlockArea* area);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<RootBlockArea*> rootAreas;
    void drawLinesRecursive(QPainter* painter, BlockWidget* parent, QMap<BlockWidget*, int>& midXCache);
};
