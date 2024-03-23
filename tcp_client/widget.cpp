#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("客户端");

    // 实例化tcpSocket
    tcpSocket = new QTcpSocket(this);

    // 信号槽连接
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveMessages()));
    connect(tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChange(QAbstractSocket::SocketState)));
}

Widget::~Widget()
{
    delete ui;
}

/* 已经连接 */
void Widget::connected()
{
    ui->textBrowser->append("已经连上服务端...");
}

/* 已经断开服务端 */
void Widget::disconnected()
{
    ui->textBrowser->append("已经断开服务端...");
}

/* 客户端接收消息 */
void Widget::receiveMessages()
{
    QString messages = tcpSocket->readAll();
    ui->textBrowser->append("服务端：" + messages);
}

/* 客户端状态改变 */
void Widget::socketStateChange(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        ui->textBrowser->append("socket 状态：UnconnectedState");
        break;
    case QAbstractSocket::ConnectedState:
        ui->textBrowser->append("socket 状态：ConnectedState");
        break;
    case QAbstractSocket::ConnectingState:
        ui->textBrowser->append("socket 状态：ConnectingState");
        break;
    case QAbstractSocket::HostLookupState:
        ui->textBrowser->append("socket 状态：HostLookupState");
        break;
    case QAbstractSocket::ClosingState:
        ui->textBrowser->append("socket 状态：ClosingState");
        break;
    case QAbstractSocket::ListeningState:
        ui->textBrowser->append("socket 状态：ListeningState");
        break;
    case QAbstractSocket::BoundState:
        ui->textBrowser->append("socket 状态：BoundState");
        break;
    }
}

void Widget::on_toConnectPushButton_clicked()
{
    // 还没有连接
    if(tcpSocket->state() != tcpSocket->ConnectedState) {
        QString ip = ui->ipLineEdit->text();
        quint16 port = ui->portLineEdit->text().toUInt();
        tcpSocket->connectToHost(QHostAddress(ip), port);
        ui->textBrowser->append("正在连接...");
    } else
        ui->textBrowser->append("请填写IP和端口...");
}

void Widget::on_toDisConnectPushButton_clicked()
{
    // 断开连接
    tcpSocket->disconnectFromHost();
    // 关闭socket
    tcpSocket->close();
}

void Widget::on_clearTextPushButton_clicked()
{
    ui->textBrowser->clear();
}

void Widget::on_sendMessagesPushButton_clicked()
{
    if(NULL == tcpSocket)
        return;

    // 已经连接
    if(tcpSocket->state() == tcpSocket->ConnectedState) {
        // 发送消息
        tcpSocket->write(ui->messageLineEdit->text().toUtf8().data());
        // 回显
        ui->textBrowser->append("客户端：" + ui->messageLineEdit->text());
    }
}
