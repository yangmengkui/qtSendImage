#include "imagestream.h"
#include <QtCore/QFile>
#include <QtCore/QByteArray>

#define IPADDERSS "127.0.0.1"  //"192.168.1.119"

ImageStream::ImageStream(/*QWidget *qw*/) /*: QWidget(qw)*/{
    initNetWork();
    this->setGeometry(0,0,1024,768);
    std::cout<<this<<std::endl;
    //this注释掉了，图片也不能显示出来
    ql_show=new QLabel(this);//  new int;//开辟一个存放整数的存储空间，返回一个指向该存储空间的地址(即指针)
    //new时传了一个地址，例如将QWidgit传了进来，继承了他，可以在上面显示了，如果没有继承QWidget，虽然也画出来了，但是没有画布，显示不出来
    //    ql_show.setGeometry(0,0,1024,768);
    //     ql_show.setAutoFillBackground(true);
    this->ql_show->setGeometry(0,0,1024,768);//this是我个人理解后添加的
    this->ql_show->setAutoFillBackground(true); //删了就不能显示了
    //If enabled, this property will cause Qt to fill the background of the widget before invoking the paint event. The color used is defined by the QPalette::Window color role from the widget's palette.
    hasReadHead=false;
    avalibleNum=0;
    //    QTimer timer; //这样为什么不行
    /*QTimer **/timer = new QTimer();
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(askConnect()));
    connect(&client,SIGNAL(disconnected()),this,SLOT(connectAgain()));
}

void ImageStream::connectAgain(){
     timer->start(1000);
    std::cout<<"initing network"<<std::endl;
    hostAddress.setAddress(IPADDERSS);
    client.setParent(this);
    client.connectToHost(hostAddress, 8867);//8867
}

void ImageStream::askConnect(){
    std::cout<<"askConnect"<<std::endl;
        std::cout<<"initing network"<<std::endl;
        hostAddress.setAddress(IPADDERSS);
        client.setParent(this);
        client.connectToHost(hostAddress, 8867);//8867
}

ImageStream::~ImageStream(){

}


//当有数据包发送来时，解析第一个数据包并获取所有数据包的总长度，获取的第二个以上的数据包不做此处理。
void ImageStream::getSocketImage(){
    timer->stop();
    std::cout<<"connection has etabiled"<<std::endl;
        if(!hasReadHead){
            imageData.clear();   //第一个数据包发来时，先清空存储图片数据的空间
            QByteArray by=client.readAll();
            avalibleNum=by.left(10).toLong();   //找出第一个数据包的前10个字节,由此获得数据总长度。
            by.remove(0,10);  //移除前10个字节
            imageData.append(by);
            hasReadHead=true;
            if(imageData.length()>=avalibleNum){   //判断数据是否接收完毕
                std::cout<<"receive a image,length="<<avalibleNum<<std::endl;
                changeFace();
                hasReadHead=false;
            }
        }else{
            QByteArray by=client.readAll();
            imageData.append(by);
            if(imageData.length()>=avalibleNum){   //判断数据是否接收完毕
                std::cout<<"receive a image,length="<<avalibleNum<<std::endl;
                changeFace();/*available*/
                        hasReadHead=false;
            }
        }
}


void ImageStream::changeFace(){
    QPalette palette;
    QImage copy;
    QImage image=QImage::fromData(imageData,"jpg");// this->imageData
    //  image.pixel()
    //  copy = image.scaled(1024,768);
    copy = image.scaled(1024,768,Qt::KeepAspectRatioByExpanding);//Qt::KeepAspectRatioByExpanding      Qt::KeepAspectRatio
    //  palette.setBrush(QPalette::Background, QBrush(image));
    palette.setBrush(QPalette::Background, QBrush(copy));
    ql_show->setPalette(palette);
    //  ql_show.setPalette(palette);
}

void ImageStream::initNetWork(){
    std::cout<<"initing network"<<std::endl;
    hostAddress.setAddress(IPADDERSS);
    client.setParent(this);
    client.connectToHost(hostAddress, 8867);//8867
    connect(&client,SIGNAL(readyRead()),this,SLOT(getSocketImage()));
    std::cout<<"network inited!"<<std::endl;
    client.setReadBufferSize(1024*1024);
}
