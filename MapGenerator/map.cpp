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
    size = 10;

    for(int i=0; i<size;i++)
        for(int j=0; j<size;j++)
            cellGrid.addWidget(new Cell, i, j);

    this->cellGrid.setSpacing(0);
    this->setLayout(&cellGrid);
}

Map::~Map()
{

}

void Map::mousePressEvent(QMouseEvent *event){
    // qDebug() << "mouse pos:" << this->mapFromGlobal(QCursor::pos());

    // if(event->buttons() & Qt::LeftButton){
    //     m_last = event->pos();
    // }
}

void Map::mouseMoveEvent(QMouseEvent *event){
    // if(event->buttons() & Qt::LeftButton){
        // drawPixel(event->pos());
    //     repaint();
    // }
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

void Map::increase(){
    // size++;
    // std::vector<Cell> line;

    // Cell cell;
    // for(auto &c : matrix)
    //     c.push_back(cell);

    // for(int j=0; j<size;j++){
    //     line.push_back(cell);
    // }
    // matrix.push_back(line);

    // repaint();
}

void Map::decrease(){
    // size--;
    // matrix.pop_back();

    // repaint();
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
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            Cell* cell = qobject_cast<Cell*>(cellGrid.itemAtPosition(i, j)->widget());

            if(cell->isVisible())
                painter->drawPixmap(cell->pos(), cell->getCellImage());

            // qDebug() << "global pos" << cell->pos();
            // qDebug() << "local pos" << this->mapFromGlobal(cell->pos());
        }
    }
}
