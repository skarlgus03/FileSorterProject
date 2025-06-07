#include "Ui/canvaswidget.h"
#include <QPainter>
#include <QtMath>

CanvasWidget::CanvasWidget(QWidget* parent)
    : QWidget(parent) {
    setMinimumSize(2000, 2000);
}

void CanvasWidget::addRootArea(RootBlockArea* area) {
    rootAreas.append(area);
}

void CanvasWidget::clearRootAreas() {
    rootAreas.clear();
}

void CanvasWidget::repaintAll() {
    update();
}

void CanvasWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    for (auto* area : rootAreas) {
        drawLinesRecursive(&painter, area->getRootBlock());
    }
}

void CanvasWidget::drawLinesRecursive(QPainter* painter, BlockWidget* parent) {
    for (BlockWidget* child : parent->getChildren()) {
        QPoint start = parent->mapTo(this, parent->rect().center());

        QRect childRect = child->rect();
        QPoint end = child->mapTo(this, QPoint(childRect.left(), childRect.center().y()));

        // 선 그리기
        painter->drawLine(start, end);

        // 방향 벡터 (정규화)
        QLineF line(start, end);
        QPointF unit = line.unitVector().p2() - line.unitVector().p1();

        // 각도 보정 및 화살머리 그리기
        constexpr int arrowSize = 10;
        QPointF arrowP1 = end - unit * arrowSize + QPointF(-unit.y(), unit.x()) * (arrowSize / 2.5);
        QPointF arrowP2 = end - unit * arrowSize - QPointF(-unit.y(), unit.x()) * (arrowSize / 2.5);

        QPolygonF arrowHead;
        arrowHead << end << arrowP1 << arrowP2;

        painter->setBrush(Qt::white);
        painter->drawPolygon(arrowHead);

        drawLinesRecursive(painter, child);
    }
}

