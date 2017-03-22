#include "serverstream.h"
#include <string>
#include <QtCore/QFile>
#include <QtCore/QThread>
#include<QTime>
#include<QDateTime>


ServerStream::ServerStream(QObject *parent) : QObject(parent)
{
    connectFlag =  false;
    seconds = 0;
    connect(&server,SIGNAL(newConnection()),this,SLOT(dealConnection()));
    while (1) {
    initNetWork();
    server.waitForNewConnection(1000);
    }
//    timer = new QTimer(this);
//    timer->start(1000);
//    connect(timer,SIGNAL(timeout()),this,SLOT(askConnect()));


//    connect(this,SIGNAL(overtime()),this,SLOT(connectTimeOut()));

}

ServerStream::~ServerStream(){

}

//void ServerStream::connectTimeOut(){
//    std::cout<<"链接超时，程序将在5s后关闭"<<std::endl;
//    QDateTime t2,now;
//    t2 = QDateTime::currentDateTime();
//    do{
//        now = QDateTime::currentDateTime();
//    }while(t2.secsTo(now)<6);
//    exit(1);
//}

//void ServerStream::askConnect(){
//    if(connectFlag == false)
//    {
//    std::cout<<"客户端请求链接"<<std::endl;
//     server.listen(QHostAddress::Any,8867);
//   seconds++;
//   if(seconds > 3)
//       emit overtime();
//    }
//}

//初始化网络
void ServerStream::initNetWork(){
    std::cout<<"initing network..."<<std::endl;
    server.setParent(this); //让该实例化的对象成为server 服务器的对象，这样对象实例化被销毁，其子全部被销毁
    //Tells the server to listen for incoming connections on name. If the server is currently listening then it will return false. Return true on success otherwise false.
    server.listen(QHostAddress::Any,8867);//8867
    std::cout<<"network inited!"<<std::endl;
}

//当有连接时，将图片发送过去
void ServerStream::dealConnection(){
    std::cout<<"dealConnection has taking"<<std::endl;
    int num=0;
    long len=0;

    //Returns the next pending connection as a connected QLocalSocket object.
    //The socket is created as a child of the server, which means that it is automatically deleted when the QLocalServer object is destroyed.

    socket=server.nextPendingConnection();//QLocalSocket * QLocalServer::nextPendingConnection()
    //相当于拿到已经建立链接的套接字，这样readyRead() 信号才能发送出来出来,这样就可以相互通信了
    connectFlag = true;

    for(num=1;num<=10;num++){
        QFile file(":/images/"+QString::number(num)+".jpg"); //数值转化为字符串性的，这样可以通过+号将字符串链接起来
        file.open(QIODevice::ReadOnly);   //在Qt中，文件需要打开了才能进行操作，这点与java不同
        QByteArray by;
        by=file.readAll();
        std::cout<<by.size()<<std::endl;
        len=by.length();    //获取数据的总长度
        std::cout<<len<<"head is exit , bytes have been written!"<<std::endl;
        //        int n = 63;
        //        QByteArray::number(n);              // returns "63"
        //        QByteArray::number(n, 16);          // returns "3f"
        by.prepend(formHead(QByteArray::number((qlonglong)len)));  //将要发送的数据总长度加在数据的最前10个字节中，不足的在前面补零。
        len=socket->write(by);
        socket->flush();
        std::cout<<len<<"bytes have been written!"<<std::endl;
//        delay(10000);//7000

//        QTime t;
//        t.start();
//        while(t.elapsed()<100)
//            QCoreApplication::processEvents();

        QDateTime t1,now;
        t1 = QDateTime::currentDateTime();
        do{
            now = QDateTime::currentDateTime();
        }while(t1.secsTo(now)<1);

        if(num==10) num=1;
        if(len == -1)
        {
            std::cout<<"客户端断开链接，需要重新连入"<<std::endl;
//            socket->close();
            server.close();
            break;
        }
    }
    std::cout<<"dealConnection has taken"<<std::endl;
    connectFlag = true;
    //    server.close();
}


//将数据长度封装在10个字节的范围内,不足的在前面补零
QByteArray ServerStream::formHead(QByteArray by){
    int len=by.length();
    int i=0;
    for(i=10-len;i>0;i--){
        by=by.prepend('0');
    }
    return by;
}

//线程延时不能用，只好用最原始的延时方法了
void ServerStream::delay(int len){
    volatile int m=0;
    volatile int n=0;
    for(m=0;m<len;m++){
        for(n=0;n<len;n++);
    }
}
