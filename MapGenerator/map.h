#ifndef MAP_H
#define MAP_H

#include "cell.h"

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

    Cell* getCell(int x, int y);
    Cell* getCell(QMouseEvent *event);
    void setVisibleCols(int w);
    void setVisibleRows(int h);
    void setCellAtGrid(int i, int j, cellType type);

public slots:
    void increaseRows();
    void increaseCols();
    void decreaseRows();
    void decreaseCols();
    void changeCellTypeWall();
    void changeCellTypeStart();
    void changeCellTypeEnd();
    void changeCellTypeDecision();
    void changeCellTypePath();


signals:

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    // void keyPressEvent(QKeyEvent *) override;
    void paintGrid(QPainter* painter);


private:
    void setGridCellType(Cell* cell);
    QGridLayout cellGrid;
    cellType selectedCell;
    QPoint startPos;
    QPoint endPos;
    int rows;
    int columns;
    int visibleRows;
    int visibleCols;
};

#endif // MAP_H
