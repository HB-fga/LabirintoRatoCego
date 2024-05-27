#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QGenericMatrix>
#include <vector>
#include "cell.h"

class QPaintEvent;
class QMouseEvent;
class Map : public QWidget
{
    Q_OBJECT
public:
    explicit Map(QWidget *parent = 0);
    ~Map();

    void drawPixel(const QPoint pt);
    void setBrushSize(const int size);
    const int brushSize() const;

public slots:
    QColor drawColor();
    void setBrushColor(QColor color);
    void increase();
    void decrease();

signals:

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    //void keyPressEvent(QKeyEvent *) override;
    void resizeEvent(QResizeEvent *);
    void paintGrid(QPainter* painter);


private:
    QColor m_drawColor;
    QPoint m_last;
    std::vector<std::vector<Cell>> matrix;
    int m_brushSize;
    int size;
};

#endif // MAP_H
