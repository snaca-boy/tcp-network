#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpServer *tcpServer;  // tcp服务器
    QTcpSocket *tcpSocket;  // 通信套接字

private slots:
    void clientConnected();     // 客户端连接处理槽函数
    void receiveMessages();     // 接收到消息
    void socketStateChange(QAbstractSocket::SocketState);   // 连接状态改变槽函数
    void on_startListenPushButton_clicked();
    void on_stopListenPushButton_clicked();
    void on_clearTextPushButton_clicked();
    void on_sendMessagesPushButton_clicked();
};
#endif // WIDGET_H
