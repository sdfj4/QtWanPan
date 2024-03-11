#include "wanpanserver.h"
#include "ui_wanpanserver.h"
#include "mytcpserver.h"
#include <QDebug>
#include <QByteArray>
#include <QMessageBox>
#include <QHostAddress>

WanPanServer::WanPanServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WanPanServer)
{
    ui->setupUi(this);
    getload();
    MyTcpServer::getinstance().listen(QHostAddress(m_strIP),m_usPort);
}

WanPanServer::~WanPanServer()
{
    delete ui;
}

void WanPanServer::getload()
{
    QFile file(":/new/prefix1/serverconfig.txt"); // 文件对象，读取资源文件 ':' + "前缀" + "文件名"
    if(file.open(QIODevice::ReadOnly)) // file.open() 参数：打开方式：只读（注意，这里只读是写在QIODevice下的枚举，所以调用要声明命名空间） 返回true则打开成功
    {
        QByteArray baData = file.readAll(); // 读出所有数据，返回字节数组QByteArray
        QString strData = baData.toStdString().c_str(); // 转换为字符串 注意std::string不能自动转为QString，还需转为char*
        file.close();

        strData.replace("\r\n", " "); // 替换IP地址、端口号与服务器文件系统根地址之间\r\n
        QStringList strList = strData.split(" ");
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort(); // 无符号短整型
    }
    else // 文件打开失败则弹出提示窗口
    {
        QMessageBox::critical(this, "open config", "open config failed"); // 严重
    }
}



