#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);

    MainWindow win;
    win.show();

    return app.exec();
}
