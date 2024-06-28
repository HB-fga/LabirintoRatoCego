#ifndef MAP_H
#define MAP_H

#include "cell.h"

#include <QWidget>
#include <QGenericMatrix>
#include <QGridLayout>
#include <QJsonObject>

using Graph = QMap<QPair<int, int>, QList<QPair<int, int>>>;

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
    void clearGrid();
    void setVisibleCols(int w);
    void setVisibleRows(int h);
    int getVisibleCols();
    int getVisibleRows();
    QPoint getStartPos();
    QPoint getEndPos();
    void setStartPos(int x, int y);
    void setEndPos(int x, int y);
    void setCellAtGrid(int i, int j, cellType type);
    QJsonObject getJSON();
    bool checkConectivity(const Graph& graph);
    bool isConnected();

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
    void paintGrid(QPainter* painter);

private:
    void setGridCellType(Cell* cell);
    QString findMoves(int row, int col, Graph& graph);
    QGridLayout cellGrid;
    cellType selectedCellType;
    QPoint startPos;
    QPoint endPos;
    int rows;
    int columns;
    int visibleRows;
    int visibleCols;
    bool connected;
};

#endif // MAP_H
