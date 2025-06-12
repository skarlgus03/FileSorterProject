#pragma once
#include <QWidget>
#include <QPointer>
#include "Ui/blockwidget.h"
#include "Ui/rootblockarea.h"

class CanvasWidget : public QWidget {
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget* parent = nullptr);
    void addRootArea(RootBlockArea* area);
    void clearRootAreas();
    
    void removeRootArea(RootBlockArea* area);

protected:
    void paintEvent(QPaintEvent* event) override;

public slots:
    void repaintAll();
private:
    QVector<QPointer<RootBlockArea>> rootAreas;
    void drawLinesRecursive(QPainter* painter, BlockWidget* parent, QMap<BlockWidget*, int>& midXCache);
};
