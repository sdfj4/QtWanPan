#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QList>
#include "mytcpsocket.h"
#include "protocol.h"
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer();
    static MyTcpServer &getinstance();
    void incomingConnection(qintptr socketDescriptor);
    bool forwardMsg(const QString caDesName, PDU *pdu); // 服务器转发给其他客户端消息
    QString getStrRootPath() const;   //获取文件路径
public slots:
    void deleteSocket(MytcpSocket *mySocket);
private:
    QList<MytcpSocket*> m_tcpSocketList;
    QString m_strRootPath; // 文件系统根目录
};

#endif // MYTCPSERVER_H
