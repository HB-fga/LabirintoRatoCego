#ifndef CELL_H
#define CELL_H

#include <QObject>

enum class cellType{
    Wall,
    Path,
    Decision,
    Start,
    End
} ;

class Cell : public QObject
{
    Q_OBJECT
public:
    explicit Cell(QObject *parent = nullptr);

private:
    cellType type;
    //QColor color;
signals:
};


#endif // CELL_H
