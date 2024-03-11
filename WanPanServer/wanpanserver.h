#ifndef WANPANSERVER_H
#define WANPANSERVER_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui {
class WanPanServer;
}
QT_END_NAMESPACE

class WanPanServer : public QWidget
{
    Q_OBJECT

public:
    WanPanServer(QWidget *parent = nullptr);
    ~WanPanServer();
    void getload();
private:
    Ui::WanPanServer *ui;
    QString m_strIP; // 存储配置文件读取到的IP地址
    quint16 m_usPort;
};
#endif // WANPANSERVER_H
