
#include "Ui/canvaswidget.h"
#include <QPainter>
#include <QtMath>
#include <QPainterPath>
#include <QMap>
#include <QDebug>

CanvasWidget::CanvasWidget(QWidget* parent)
    : QWidget(parent) {
    setMinimumSize(2000, 2000);
}

void CanvasWidget::addRootArea(RootBlockArea* area) {
    rootAreas.append(area);
}

void CanvasWidget::removeRootArea(RootBlockArea* area) {
    rootAreas.removeAll(area);
}

void CanvasWidget::clearRootAreas() {
    rootAreas.clear();
}

void CanvasWidget::repaintAll() {
    update();
}

void CanvasWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    static QMap<BlockWidget*, int> parentToMidX;
    parentToMidX.clear();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);

    for (auto* area : rootAreas) {
        if (!area || area->parent() == nullptr || !area->isVisible()) continue;

        BlockWidget* root = area->getRootBlock();
        if (!root || root->parent() == nullptr || !root->isVisible()) continue;

        drawLinesRecursive(&painter, root, parentToMidX);
    }
}

void CanvasWidget::drawLinesRecursive(QPainter* painter, BlockWidget* parent, QMap<BlockWidget*, int>& midXCache) {
    if (!parent || parent->parent() == nullptr || !parent->isVisible()) return;

    for (BlockWidget* child : parent->getChildren()) {
        if (!child || child->parent() == nullptr || !child->isVisible()) continue;

        QRect parentRect = parent->rect();
        QPointF start = parent->mapTo(this, QPoint(parentRect.right(), parentRect.center().y()));

        QRect childRect = child->rect();
        QPointF end = child->mapTo(this, QPoint(childRect.left(), childRect.center().y()));

        int midX = (start.x() + end.x()) / 2;
        midXCache[parent] = midX;

        QPainterPath path(start);
        path.lineTo(midX, start.y());
        path.lineTo(midX, end.y());
        path.lineTo(end);
        painter->drawPath(path);

        QLineF arrowLine(QPointF(midX, end.y()), end);
        QPointF unit = arrowLine.unitVector().p2() - arrowLine.unitVector().p1();
        QPointF arrowP1 = end - unit * 8 + QPointF(-unit.y(), unit.x()) * 4;
        QPointF arrowP2 = end - unit * 8 - QPointF(-unit.y(), unit.x()) * 4;

        painter->drawLine(end, arrowP1);
        painter->drawLine(end, arrowP2);

        drawLinesRecursive(painter, child, midXCache);
    }
}
