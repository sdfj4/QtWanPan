#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>
#include "operatewidget.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class TcpClient;
}
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT
public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void loadConfig();
    static TcpClient& getInstance(); // 将TcpClient设置为单例模式，方便外部调用socket功能
    QTcpSocket& getTcpSocket();

    QString getStrName() const;
    void setStrName(const QString &strName);

    QString getStrCurPath() const;  //获取当前文件夹
    void setStrCurPath(const QString &strCurPath);

    QString getStrRootPath() const; //根目录
    void setStrRootPath(const QString &strRootPath);
public slots:
    void showconnection();
    void recvMsg();
private slots:
    void on_login_pb_clicked();

    void on_regist_pb_clicked();

    void on_delete_pb_clicked();

private:
    Ui::TcpClient *ui;
    QString m_strIP; // 存储配置文件读取到的IP地址
    quint16 m_usPort; // 无符号16位整型 存储配置文件的端口号
    QTcpSocket m_tcpsocket;

    QString m_strName;      // 该客户端用户名
    QString m_strCurPath;   // 当前目录
    QString m_strRootPath;  // 用户根目录
};
#endif // TCPCLIENT_H
