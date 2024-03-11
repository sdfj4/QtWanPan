#include "mytcpserver.h"
#include <QDebug>


MyTcpServer::MyTcpServer()
{}

MyTcpServer &MyTcpServer::getinstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"new connetion";
    MytcpSocket *pTcpSocket= new MytcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);
    connect(pTcpSocket, SIGNAL(offline(MytcpSocket *)), this, SLOT(deleteSocket(MytcpSocket *)));
}

bool MyTcpServer::forwardMsg(const QString caDesName, PDU *pdu)
{
    if(caDesName == NULL || pdu == NULL)
    {
        return false;
    }
    // 查找目标用户名的Socket
    for(int i = 0; i < m_tcpSocketList.size(); ++ i)
    {
        if(caDesName == m_tcpSocketList.at(i) -> getStrName()) // 查找到
        {
            m_tcpSocketList.at(i)->write((char*)pdu, pdu -> uiPDULen); // 转发消息
            return true;
        }
    }

    return false;
}

void MyTcpServer::deleteSocket(MytcpSocket *mySocket)
{
    // 遍历m_tcpSocketList并删除socket
    QList<MytcpSocket*>::iterator iter = m_tcpSocketList.begin();
    for(; iter != m_tcpSocketList.end(); iter ++)
    {
        if(mySocket == *iter)
        {
            (*iter) -> deleteLater(); // 延迟释放空间，使用delete会报错！！！
            *iter = NULL;
            m_tcpSocketList.erase(iter); // 删除列表中指针
            break;
        }
    }
}

QString MyTcpServer::getStrRootPath() const
{
    return m_strRootPath;
}
