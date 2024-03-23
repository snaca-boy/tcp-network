#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>

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
    QTcpSocket *tcpSocket;  // 通信套接字

private slots:
    void connected();       // 已连接
    void disconnected();    // 已断开连接
    void receiveMessages(); // 接收到消息
    void socketStateChange(QAbstractSocket::SocketState);   // 连接状态改变槽函数
    void on_toConnectPushButton_clicked();
    void on_toDisConnectPushButton_clicked();
    void on_clearTextPushButton_clicked();
    void on_sendMessagesPushButton_clicked();
};
#endif // WIDGET_H
