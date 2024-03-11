#ifndef OPERATEWIDGET_H
#define OPERATEWIDGET_H
#include "friend.h"
#include <QWidget>
#include <QListWidget>
#include "filesystem.h"
#include <QStackedWidget>
#include <QLabel>

class OperateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OperateWidget(QWidget *parent = nullptr);
    static OperateWidget &getInstance(); // 将operatewidget设计为单例模式
    Friend *getPFriend() const;
    FileSystem *getPFileSystem() const;
private:
    QListWidget *m_pListWidget; // 组织主页面左侧常用功能（好友、文件按钮等）
    Friend *m_pFriend;
    FileSystem *m_pFileSystem;  // 文件页面
    QStackedWidget *m_pSW;      // 容器，每次显示一个页面（好友or文件）
signals:
};

#endif // OPERATEWIDGET_H
