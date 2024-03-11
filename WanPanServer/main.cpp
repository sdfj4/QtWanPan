#include "wanpanserver.h"

#include <QApplication>
#include "dboperate.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WanPanServer w;
    w.show();

    DBOperate::getInstance().init();
    return a.exec();
}
