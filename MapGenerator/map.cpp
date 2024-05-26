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
    clearCanvas(m_canvas, width(), height());
    size = 10;
}

Map::~Map()
{

}

void Map::drawPixel(const QPoint pt){
    QRgb value = m_drawColor.rgb();

    QPainterPath ctx;

    ctx.moveTo(m_last.x(), m_last.y());
    ctx.lineTo(pt.x(), pt.y());

    QPainter painter(&m_canvas);
    painter.setBrush(m_drawColor);
    painter.setPen(QPen(m_drawColor, m_brushSize, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(ctx);
    painter.end();
    m_last = pt;

}

void Map::setBrushSize(const int size)
{
    qDebug() << "setBrushSize: " << size;
    m_brushSize = size;
}

const int Map::brushSize() const{
    return m_brushSize;
}

void Map::mousePressEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        m_last = event->pos();
    }
}

void Map::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        drawPixel(event->pos());
        repaint();
    }
}

void Map::keyPressEvent( QKeyEvent* event ) {
    std::cout << "KeyEvent Enter\n";
    switch ( event->key() ) {
    case Qt::Key_Right:
        std::cout << "KeyEvent Right\n";
        size++;
        break;
    case Qt::Key_Left:
        std::cout << "KeyEvent Left\n";
        size--;
        break;
    default:
        event->ignore();
        break;
    }
}

void Map::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    QImage newCanvas;
    clearCanvas(newCanvas, width(), height());

    QPainter p(&newCanvas);
    p.drawImage(0,0,m_canvas);

    m_canvas = newCanvas;

    update();
}

QColor Map::drawColor()
{
    return m_drawColor;
}

void Map::setBrushColor(QColor color)
{
    m_drawColor = color;
}

void Map::clear()
{
    clearCanvas(m_canvas, width(), height());
    update();
}

void Map::increase(){
    size++;
    std::cout << "Aumentou: " << size << std::endl;
    this->update();
}

void Map::decrease(){
    size--;
    std::cout << "Diminuiu: " << size << std::endl;
    this->update();
}

void Map::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    //clearCanvas(m_canvas, width(), height());
    paintGrid(&painter);
    painter.drawPixmap(0,0,QPixmap::fromImage(m_canvas));
}

void Map::paintGrid(QPainter* painter){
    // Draw Grid
    int cellSize = 30;
    std::cout << "Painted: " << size << std::endl;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            QRect rect(10+i*cellSize, 10+j*cellSize, cellSize, cellSize);
            painter->drawRect(rect);
        }
    }
}

