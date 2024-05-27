#include "map.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgb>
#include <QDebug>
#include <QPainterPath>
#include <iostream>

static void clearCanvas(QImage &canvas, int width, int height)
{
    canvas = QImage(width,height,QImage::Format_RGB888);
    canvas.fill(QColor(Qt::white));
}

Map::Map(QWidget *parent) : QWidget(parent)
{
    m_drawColor = QColor(Qt::black);
    //clearCanvas(m_canvas, width(), height());
    size = 10;
    setBrushSize(20);


    for(int i=0; i<size;i++){
        std::vector<Cell> line;
        for(int j=0; j<size;j++){
            Cell c;
            line.push_back(c);
        }
        matrix.push_back(line);
    }
}

Map::~Map()
{

}


void Map::setBrushSize(const int bSize)
{
    qDebug() << "setBrushSize: " << bSize;
    m_brushSize = bSize;
}

const int Map::brushSize() const{
    return m_brushSize;
}

void Map::mousePressEvent(QMouseEvent *event){
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

void Map::resizeEvent(QResizeEvent *event)
{

}

QColor Map::drawColor()
{
    return m_drawColor;
}

void Map::setBrushColor(QColor color)
{
    m_drawColor = color;
}

void Map::increase(){
    size++;
    std::vector<Cell> line;

    Cell cell;
    for(auto &c : matrix)
        c.push_back(cell);

    for(int j=0; j<size;j++){
        line.push_back(cell);
    }
    matrix.push_back(line);

    repaint();
}

void Map::decrease(){
    size--;
    matrix.pop_back();

    repaint();
}

void Map::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    //clearCanvas(m_canvas, width(), height());
    //painter.drawPixmap(0,0,QPixmap::fromImage(m_canvas));
    paintGrid(&painter);

}

void Map::paintGrid(QPainter* painter){
    // Draw Grid
    int cellSize = 30;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            //QRect rect(10+i*cellSize, 10+j*cellSize, cellSize, cellSize);
            painter->drawPixmap(10+i*cellSize, 10+j*cellSize, matrix[i][j].img);
            //painter->drawRect(rect);
        }
    }
}

