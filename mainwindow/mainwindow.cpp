#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QPixmap pix("X:\Desktop\FacultateII\PAC\TwitterRepo\TwitterBackground");
    int w = ui.background->width();
    int h = ui.background->height();
    ui.background->setPixmap(pix.scaled(w, h,Qt::KeepAspectRatio));

}
