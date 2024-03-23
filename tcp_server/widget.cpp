#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("服务端");

    // 实例化tcpServer和tcpSocket
    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);

    // 设置停止监听状态不可用
    // ui->stopListenPushButton->setEnabled(false);

    // 信号槽连接
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(clientConnected()));
}

Widget::~Widget()
{
    delete ui;
}

/* 新的客户端连接 */
void Widget::clientConnected()
{
    // 获取客户端的套接字
    tcpSocket = tcpServer->nextPendingConnection();
    // 客户端的 ip 信息
    QString ip = tcpSocket->peerAddress().toString();
    // 客户端的端口信息
    quint16 port = tcpSocket->peerPort();
    // 在文本浏览框里显示出客户端的连接信息
    ui->textBrowser->append("客户端已连接");
    ui->textBrowser->append("客户端 ip 地址:" + ip);
    ui->textBrowser->append("客户端端口:" + QString::number(port));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveMessages()));
    connect(tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChange(QAbstractSocket::SocketState)));
}

/* 服务端接收消息 */
void Widget::receiveMessages()
{
    QString messages = "客户端：" + tcpSocket->readAll();
    ui->textBrowser->append(messages);
}

/* 服务端状态改变 */
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

void Widget::on_startListenPushButton_clicked()
{
    qDebug() << "start listen" << endl;
    if(!ui->ipLineEdit->text().isEmpty() && !ui->portLineEdit->text().isEmpty()) {
        QString ip = ui->ipLineEdit->text();
        quint16 port = ui->portLineEdit->text().toUInt();
        tcpServer->listen(QHostAddress(ip), port);
        ui->textBrowser->append("正在监听...");
    } else
        ui->textBrowser->append("请填写IP和端口...");
}

void Widget::on_stopListenPushButton_clicked()
{
    qDebug() << "stop listen" << endl;
    tcpServer->close();
    /* 如果是连接上了也应该断开，如果不断开客户端还能继续发送信息，因为 socket 未断开，还在监听上一次端口 */
    if(tcpSocket->state() == tcpSocket->ConnectedState)
        tcpSocket->disconnectFromHost();
    ui->textBrowser->append("停止监听...");
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
        ui->textBrowser->append("服务端：" + ui->messageLineEdit->text());
    }
}
