#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QGenericMatrix>
#include <QGridLayout>

class QPaintEvent;
class QMouseEvent;
class Map : public QWidget
{
    Q_OBJECT
public:
    explicit Map(QWidget *parent = 0);
    ~Map();

public slots:
    void increaseRows();
    void increaseCols();
    void decreaseRows();
    void decreaseCols();

signals:

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    // void keyPressEvent(QKeyEvent *) override;
    void paintGrid(QPainter* painter);


private:
    QGridLayout cellGrid;
    int rows;
    int columns;
    int visibleRows;
    int visibleCols;
};

#endif // MAP_H
