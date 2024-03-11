#ifndef DBOPERATE_H
#define DBOPERATE_H
#include <QObject>
#include <QSqlDatabase> // 连接数据库
#include <QSqlQuery>    // 数据库操作
#include <QStringList>
class DBOperate:public QObject
{
    Q_OBJECT
public:
    explicit DBOperate(QObject *parent = nullptr);
    static DBOperate& getInstance();
    void init();
    ~DBOperate();

    bool handleRegist(const char *name, const char *pwd); //注册操作
    bool handleLogin(const char *name, const char *pwd);  // 登录操作
    bool handleOffline(const char *name);                 // 处理用户下线
    QStringList handleOnlineUsers();                      // 处理查询所有在线用户
    int handleSearchUser(const char *name);               // 处理查找用户
    int handleAddFriend(const char *addedName, const char *sourceName);       // 处理添加好友
    bool handleAddFriendAgree(const char *addedName, const char *sourceName);
    int getIdByUserName(const char *name);
    QStringList handleFlushFriend(const char *name); // 处理刷新好友列表
    bool handleDeleteFriend(const char *deletedName, const char *sourceName); // 处理删除好友
private:
    QSqlDatabase m_db; // 连接数据库
};

#endif // DBOPERATE_H
