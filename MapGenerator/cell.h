#ifndef CELL_H
#define CELL_H

#include "qwidget.h"
#include <QObject>
#include <QPixmap>
#include <QPushButton>

enum class cellType{
    Wall,
    Path,
    Decision,
    Start,
    End
};

class Cell
{

public:
    explicit Cell();
    cellType getCellType();
    void setCellType(cellType newType);
    QPixmap getCellImage();

private:
    cellType type;
    QPixmap img;

signals:
};


#endif // CELL_H
