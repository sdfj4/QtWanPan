#include "tcpclient.h"

#include <QApplication>
// #include "operatewidget.h"
// #include "onlineuserwid.h"
// #include "friend.h"
// #include "operatewidget.h"
// #include "privatechatwid.h"
// #include "filesystem.h"
#include "sharedfilefriendlist.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TcpClient::getInstance().show();

    // TcpClient w;
    // w.show();

    // OperateWidget w;
    // w.show();

    // OnlineUserWid w;
    // w.show();

    // Friend w;
    // w.show();

    // PrivateChatWid w;
    // w.show();

    // FileSystem w;
    // w.show();

    // sharedFileFriendList w;
    // w.show();
    return a.exec();
}
