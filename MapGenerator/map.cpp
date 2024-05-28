#include "map.h"
#include "cell.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgb>
#include <QDebug>
#include <QPainterPath>
#include <QCursor>

Map::Map(QWidget *parent) : QWidget(parent)
{
    rows = 20;
    columns = 20;
    visibleRows = 10;
    visibleCols = 10;

    for(int i=0; i<rows;i++)
        for(int j=0; j<columns;j++){
            Cell* cell = new Cell;
            if(i >= visibleRows || j >= visibleCols)
                cell->setVisible(false);
            cellGrid.addWidget(cell, i, j);
        }

    this->cellGrid.setSpacing(0);
    this->setLayout(&cellGrid);
}

Map::~Map()
{

}

void Map::mousePressEvent(QMouseEvent *event)
{
    QWidget * const widget = childAt(event->pos());
    if (widget) {
        Cell* cell = qobject_cast<Cell*>(widget);
        cell->setCellType(cellType::Start);
        repaint();
    }
}

void Map::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        QWidget * const widget = childAt(event->pos());
        if (widget) {
            Cell* cell = qobject_cast<Cell*>(widget);
            cell->setCellType(cellType::Start);
            repaint();
        }
    }
}

// void Map::keyPressEvent( QKeyEvent* event ) {
//     std::cout << "KeyEvent Enter\n";
//     switch ( event->key() ) {
//     case Qt::Key_Right:
//         std::cout << "KeyEvent Right\n";
//         size++;
//         break;
//     case Qt::Key_Left:
//         std::cout << "KeyEvent Left\n";
//         size--;
//         break;
//     default:
//         event->ignore();
//         break;
//     }
// }

void Map::increaseRows(){
    if(visibleRows+1 > rows) return;
    for(int j=0;j<visibleCols;j++){
        Cell* cell = qobject_cast<Cell*>(cellGrid.itemAtPosition(visibleRows, j)->widget());
        cell->setVisible(true);
    }
    visibleRows++;

    repaint();
}

void Map::increaseCols(){
    if(visibleCols+1 > columns) return;
    for(int i=0;i<visibleRows;i++){
        Cell* cell = qobject_cast<Cell*>(cellGrid.itemAtPosition(i, visibleCols)->widget());
        cell->setVisible(true);
    }
    visibleCols++;

    repaint();
}

void Map::decreaseRows(){
    if(visibleRows-1 < 2) return;
    for(int j=0;j<visibleCols;j++){
        Cell* cell = qobject_cast<Cell*>(cellGrid.itemAtPosition(visibleRows-1, j)->widget());
        cell->setVisible(false);
    }
    visibleRows--;

    repaint();
}

void Map::decreaseCols(){
    if(visibleCols-1 < 2) return;
    for(int i=0;i<visibleRows;i++){
        Cell* cell = qobject_cast<Cell*>(cellGrid.itemAtPosition(i, visibleCols-1)->widget());
        cell->setVisible(false);
    }
    visibleCols--;

    repaint();
}

void Map::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    paintGrid(&painter);

}

void Map::paintGrid(QPainter* painter){
    // Draw Grid
    int cellSize = 30;
    for(int i=0;i<rows;i++){
        for(int j=0;j<columns;j++){
            Cell* cell = qobject_cast<Cell*>(cellGrid.itemAtPosition(i, j)->widget());

            if(cell->isVisible())
                painter->drawPixmap(cell->pos(), cell->getCellImage());

            // qDebug() << "global pos" << cell->pos();
            // qDebug() << "local pos" << this->mapFromGlobal(cell->pos());
        }
    }
}
