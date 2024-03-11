#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include "protocol.h"
#include <QFile>
#include <QTimer>
class MytcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MytcpSocket();
    QString getStrName();
public slots:
    void recMsg();
    void handleClientOffline(); // 接收客户端下线信号并处理
    void handledownloadFileData(); // 向客户端实际传输要下载的文件数据
signals:
     void offline(MytcpSocket *socket);
private:
    QString m_strName;   //用户的名字
    TransFile* m_uploadFile; // 上传文件的信息

    QFile *m_pDownloadFile; // 客户端要下载的文件
    QTimer *m_pTimer; // 计时器
};

#endif // MYTCPSOCKET_H
