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
    void increase();
    void decrease();

signals:

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    // void keyPressEvent(QKeyEvent *) override;
    void paintGrid(QPainter* painter);


private:
    QGridLayout cellGrid;
    int size;
};

#endif // MAP_H
