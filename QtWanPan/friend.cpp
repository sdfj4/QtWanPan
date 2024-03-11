#include "friend.h"
#include "tcpclient.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

Friend::Friend(QWidget *parent)
    : QWidget{parent}
{
    m_pFriendLW = new QListWidget;
    m_pInputMsgLE = new QLineEdit;
    m_pShowMsgTE = new QTextEdit;
    m_pDelFriendPB = new QPushButton("删除好友");
    m_pFlushFriendPB = new QPushButton("刷新好友");
    m_pShowOnlineUserPB = new QPushButton("显示在线用户");
    m_pSearchUserPB = new QPushButton("查找用户");
    m_pSendMsgPB = new QPushButton("发送");
    m_pPrivateChatPB = new QPushButton("私聊");

    QVBoxLayout *pLeftRightVBL = new QVBoxLayout; // 左侧右部分好友操作按钮布局
    pLeftRightVBL -> addWidget(m_pPrivateChatPB);
    pLeftRightVBL -> addWidget(m_pDelFriendPB);
    pLeftRightVBL -> addWidget(m_pFlushFriendPB);
    pLeftRightVBL -> addWidget(m_pShowOnlineUserPB);
    pLeftRightVBL -> addWidget(m_pSearchUserPB);
    QHBoxLayout *pRightDownHBL = new QHBoxLayout; // 右侧下方发送消息布局
    pRightDownHBL -> addWidget(m_pInputMsgLE);
    pRightDownHBL -> addWidget(m_pSendMsgPB);
    QVBoxLayout *pRightVBL = new QVBoxLayout; // 右侧聊天布局
    pRightVBL -> addWidget(m_pShowMsgTE);
    pRightVBL -> addLayout(pRightDownHBL);
    QHBoxLayout *pAllHBL = new QHBoxLayout; // 整体水平布局
    pAllHBL -> addWidget(m_pFriendLW);      // 左侧左部分好友列表
    pAllHBL -> addLayout(pLeftRightVBL);    // 左侧右部分好友操作
    pAllHBL -> addLayout(pRightVBL);        // 右侧聊天布局

    setLayout(pAllHBL); // 将整体布局pAllHBL设置为页面布局

    m_pOnlineUserW = new OnlineUserWid;

    m_pOnlineUserW -> hide(); // 默认所有在线用户页面隐藏

    // 绑定打开所有在线用户按钮与对应事件
    connect(m_pShowOnlineUserPB, SIGNAL(clicked(bool)),
            this, SLOT(showOrHideOnlineUserW()));

    // 绑定查找用户按钮与对应事件
    connect(m_pSearchUserPB, SIGNAL(clicked(bool)),
            this, SLOT(searchUser()));

    // 绑定刷新好友列表按钮与对应事件
    connect(m_pFlushFriendPB, SIGNAL(clicked(bool)),
            this, SLOT(flushFriendList()));

    // 绑定删除好友按钮与对应事件
    connect(m_pDelFriendPB, SIGNAL(clicked(bool)),
            this, SLOT(deleteFriend()));

    // 私聊按钮
    connect(m_pPrivateChatPB, SIGNAL(clicked(bool)),
            this, SLOT(privateChat()));

    // 群聊发送按钮
    connect(m_pSendMsgPB, SIGNAL(clicked(bool)),
            this, SLOT(groupChatSendMsg()));
}

void Friend::showOrHideOnlineUserW()
{
    if(m_pOnlineUserW -> isHidden())
    {
        m_pOnlineUserW -> show();
        PDU *pdu = mkPDU(0);
        pdu -> uiMsgType = ENUM_MSG_TYPE_ONLINE_USERS_REQUEST;
        TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        m_pOnlineUserW -> hide();
    }
}

void Friend::setOnlineUsers(PDU* pdu)
{
    if(NULL == pdu)
    {
        return ;
    }
    m_pOnlineUserW->setOnlineUsers(pdu);
}

void Friend::searchUser()
{
    m_strSearchName = QInputDialog::getText(this, "搜索", "用户名："); // 通过输入子页面来获取用户输入返回一个文本类型

    if(!m_strSearchName.isEmpty())
    {
        qDebug() << "查找：" << m_strSearchName;
        PDU *pdu = mkPDU(0);
        pdu -> uiMsgType = ENUM_MSG_TYPE_SEARCH_USER_REQUEST;
        memcpy((char*)pdu -> caData, m_strSearchName.toStdString().c_str(), m_strSearchName.size());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

QString Friend::getStrSearchName() const
{
    return m_strSearchName;
}

void Friend::flushFriendList()
{
    QString strName = TcpClient::getInstance().getStrName(); // 获取自己用户名
    PDU* pdu = mkPDU(0);
    pdu -> uiMsgType = ENUM_MSG_TYPE_FLSUH_FRIEND_REQUEST;
    strncpy(pdu -> caData, strName.toStdString().c_str(), strName.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Friend::updateFriendList(PDU *pdu)
{
    if(NULL == pdu)
    {
        return ;
    }
    uint uiSize = pdu -> uiMsgLen / 32; // 注意是36，32 name + 4 online
    char caName[32] = {'\0'};
    char caOnline[4] = {'\0'};

    m_pFriendLW -> clear(); // 清除好友列表原有数据
    for(uint i = 0; i < uiSize; i++)
    {
        memcpy(caName, (char*)(pdu -> caMsg) + i * 32, 32);
        // memcpy(caOnline, (char*)(pdu -> caMsg) + 32 + i * 36, 4);
        qDebug() << "客户端好友" << caName ;
        m_pFriendLW -> addItem(QString("%1\t%2").arg(caName)
                                 .arg(strcmp(caOnline, "1") == 0?"离线":"在线"));
    }
}

void Friend::deleteFriend()
{
    if(NULL == m_pFriendLW -> currentItem()) // 如果没有选中好友
    {
        return ;
    }
    QString friName = m_pFriendLW -> currentItem() -> text(); // 获得选中的好友用户名
    friName = friName.split("\t")[0];
    QString loginName = TcpClient::getInstance().getStrName(); // 登录用户用户名


    PDU* pdu = mkPDU(0);
    pdu -> uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
    strncpy(pdu -> caData, friName.toStdString().c_str(), 32);
    strncpy(pdu -> caData + 32, loginName.toStdString().c_str(), 32);
    qDebug() << friName<<loginName;
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Friend::groupChatSendMsg()
{
    QString strMsg = m_pInputMsgLE -> text();
    if(strMsg.isEmpty())
    {
        QMessageBox::warning(this, "群聊", "发送信息不能为空！");
        return ;
    }
    m_pInputMsgLE->clear(); // 清空输入框
    m_pShowMsgTE->append(QString("%1 : %2").arg(TcpClient::getInstance().getStrName()).arg(strMsg));

    PDU* pdu = mkPDU(strMsg.size() + 1);
    pdu -> uiMsgType = ENUM_MSG_TYPE_GROUP_CHAT_REQUEST;
    strncpy(pdu -> caData, TcpClient::getInstance().getStrName().toStdString().c_str(), 32);
    strncpy((char*)(pdu -> caMsg), strMsg.toStdString().c_str(), strMsg.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Friend::privateChat()
{
    if(NULL != m_pFriendLW -> currentItem()) // 如果没有选中好友
    {
        QString strChatName = m_pFriendLW->currentItem()->text();
        strChatName=strChatName.split("\t")[0];
        PrivateChatWid::getInstance().setStrChatName(strChatName);
        if(PrivateChatWid::getInstance().isHidden()){
            PrivateChatWid::getInstance().show();
        }
    }else{
        QMessageBox::warning(this,"警告","请选择私聊对象");
    }


}

void Friend::updateGroupShowMsgTE(PDU *pdu)
{
    QString strMsg = QString("%1 : %2").arg(pdu->caData).arg((char*)pdu->caMsg);
    m_pShowMsgTE -> append(strMsg);
}

QListWidget *Friend::getPFriendLW() const
{
    return m_pFriendLW;
}

void Friend::setStrSearchName(const QString &strSearchName)
{
    m_strSearchName = strSearchName;
}
