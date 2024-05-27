#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPixmap>

enum class cellType{
    Wall,
    Path,
    Decision,
    Start,
    End
} ;

class Cell
{

public:
    explicit Cell();
    QPixmap img;

private:
    cellType type;

signals:
};


#endif // CELL_H
