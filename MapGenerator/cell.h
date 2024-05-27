#ifndef CELL_H
#define CELL_H

#include <QWidget>
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

class Cell : public QWidget
{
    Q_OBJECT
public:
    explicit Cell();
    cellType getCellType();
    void setCellType(cellType newType);
    QPixmap getCellImage();
    void mousePressEvent(QMouseEvent *event);

private:
    cellType type;
    QPixmap img;

signals:
};


#endif // CELL_H
