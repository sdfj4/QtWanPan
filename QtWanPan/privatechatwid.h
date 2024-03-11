#ifndef PRIVATECHATWID_H
#define PRIVATECHATWID_H

#include <QWidget>
#include "protocol.h"
namespace Ui {
class PrivateChatWid;
}

class PrivateChatWid : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChatWid(QWidget *parent = nullptr);
    ~PrivateChatWid();
    static PrivateChatWid& getInstance();
    void setStrChatName(const QString &strChatName);
    void updateShowMsgTE(PDU* pdu); // 更新showMsgTE聊天消息窗口的消息

private slots:
    void on_sendMsg_pb_clicked();

private:
    Ui::PrivateChatWid *ui;
    QString m_strChatName;  // 聊天对象用户名
    QString m_strLoginName; // 请求用户名
};

#endif // PRIVATECHATWID_H
