#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
//#include <QDateTime>
#include <QCloseEvent>

#include "usb_device.h"
#include "usb2spi.h"

#include <string>
#include <QFileDialog>
#include "usb_user.h"
#include "qtimer.h"
#include <QMessageBox>

#include    <QFileDialog>
#include    <QFile>
#include    <QDataStream>
#include    <QByteArray>

#include "protocol.h"
#include "stdio.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include <stdlib.h>
#include <crcLib.h>




long            RecordBytes;
long            RecordItems;
long            BeginItems;
long            DeltaItem =1;

#define          TimerCell 1
//#define          TimerCell 5

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    connect(&m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));

     m_pTimer.start(TimerCell);

    UpdateRobotSlavesConnectionStatus();
    QStringList serialNamePort;

    //VersionCompare();

    serialPort = new QSerialPort(this);

    connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadRead_Slot()));
    //connect(timer, &QTimer::timeout, this, &MainWindow::timeoutSlot);

    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        serialNamePort<<info.portName();
    }
    ui->serialPortCb->addItems(serialNamePort);
}



void MainWindow::aboutclick()
{
    ;
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString path;

extern QString path_usb_all_globel;


char Rx_dat_buf[Rx_Lenth];
int Rx_Data_Cnt =0;     // USB 收到的字节总数
int Rx_Data_Frame=0;   // 单帧USB数据长度


int USB_Read_Start =0;   // 1： 启动USB接收  0 ： 关闭USB接收
int USB_Inerface_On=0;

extern unsigned short MY_VID ;
extern unsigned short MY_PID ;


/*struct iap_t
{
    uint64_t codeLen=0;
    uint32_t packetSum=0;
    uint32_t currentPacket=0;
    uint16_t currentPacketLen=0;
    bool     waitPacketResponseFlag=false;
    uint32_t waitPacketResponseTimeCount=0;
    uint8_t  currentPacketData[IAP_PACKET_MAX_DATA_LEN]={0};
    uint8_t  data[1024*1024]={0};

}iap;*/


QString byteArray2Text(QByteArray data)
{
  //  rmTextEdit->setText(data.toHex().data());
    QString str;
   // for(int i=0;i<data.length();i++)
   // {
   //   // str+=QByteArray::number((unsigned char)data[i],16);
   //    str+=QByteArray::number(0x00,16);
   // }

   /* for(int i=0;i<256;i++)
    {
      // str+=QByteArray::number((unsigned char)data[i],16);
      // str=+ " ";
      // str+=QByteArray::number(i,16);
        str += QString("%1 ").arg(i,2,16,QChar('0'));
    }*/


    //for(int i=0;i<data.length();i++)
    //{
     //   str += QString("%1 ").arg(data[i],2,16,QChar('0'));
    //}

   /* for(int i=0;i<data.length();i++)
    {
        //str += QString("%1 ").arg(data[i],2,16,QChar('0'));
        //str=+ " ";
        str+=QByteArray::number(data[i],16);
    }*/



    QString str3 = data.toHex().data();//以十六进制显示
    str3 = str3.toUpper ();//转换为大写
    for(int i = 0;i<str3.length ()+2;i+=2)//填加空格
    {
           QString st = str3.mid (i,2);
           str += st;
           str += " ";
     }

   //str = QByteArray::fromHex(QString(data).toUtf8());

    /*str="index ";
    for(int i=0;i<200;i++)
    {
       str += QString("%1 ").arg(0,2,16,QChar('0')) ;//+ " ";
    }

    int row =0;
    for (int i = 0; i < data.length(); i++)
    {
       // First nuber
        if(i%0x10==0)
        {
             str +="\n";
             str +=  QString::number(row,16) +"         " ;
             row++;
        }

          str+=  QString("%1 ").arg((unsigned char)data[i],2,16,QChar('0')).toUpper()  ;
        if (i % 64 == 63)
        {
            str += "\n";
        }

    }*/

    return str;
}

struct iap_t iap;

QByteArray IAP_DataAllArray;
QDateTime startTime;
QDateTime endTime;
/*=  QDateTime::currentDateTime();
            qint64 intervalTime = startTime.secsTo(endTime); //求时间差
            qint64 intervalTimeMS = startTime.msecsTo(endTime);
*/

//QString IAP_init()
QString MainWindow::IAP_init()
{
    //iap.codeLen=IAP_DataAllArray.length();
    iap.packetSum=iap.codeLen/IAP_PACKET_MAX_DATA_LEN;
    if(iap.codeLen%IAP_PACKET_MAX_DATA_LEN)
    {
        iap.packetSum++;
    }

     qDebug() << "IAP_DataAllArray.length()  is " << static_cast<int>(IAP_DataAllArray.length());
    memcpy(&iap.data[0],IAP_DataAllArray.data(),IAP_DataAllArray.length());

    QString str=byteArray2Text(QByteArray((char*)&iap.data[0],460));

    iap.currentPacket=0;
    iap.currentPacketLen=0;
    iap.waitPacketResponseFlag=true;
    iap.waitPacketResponseTimeCount=0;
    iap.currentPacketAddress=APP_ADDRESS;

    iap.crc32=CRC32_INIT_VALUE;
    iap.crc32Packet=0;

    startTime=QDateTime::currentDateTime();

    ui->iapProgressBar->setMaximum((iap.packetSum-1));
    ui->iapProgressBar->setValue(0);

    return str;
}

QByteArray MainWindow::IAP()
{
    QByteArray currentPacketData;
    uint8_t data[1024]={0};
    uint16_t dataLen=0;
    if(iap.currentPacket<iap.packetSum-1)
    {
        iap.currentPacketLen=IAP_PACKET_MAX_DATA_LEN;
    }else
    {
        iap.currentPacketLen=iap.codeLen-IAP_PACKET_MAX_DATA_LEN*iap.currentPacket;
    }
    //memcpy(&data[0],&iap.data[iap.currentPacket*1024],iap.currentPacketLen);
    memcpy(&iap.currentPacketData[0],&iap.data[iap.currentPacket*IAP_PACKET_MAX_DATA_LEN],iap.currentPacketLen);

    iap.currentPacketAddress=APP_ADDRESS+IAP_PACKET_MAX_DATA_LEN*iap.currentPacket;

    memcpy(&data[0],&iap.currentPacketAddress,sizeof (iap.currentPacketAddress ));
    dataLen+=sizeof (iap.currentPacketAddress);

    memcpy(&data[0+dataLen],&iap.packetSum,sizeof (iap.packetSum));
    dataLen+=sizeof (iap.packetSum);

    memcpy(&data[0+dataLen],&iap.currentPacket,sizeof (iap.currentPacket ));
    dataLen+=sizeof (iap.currentPacket);

    memcpy(&data[0+dataLen],&iap.currentPacketLen,sizeof (iap.currentPacketLen ));
    dataLen+=sizeof (iap.currentPacketLen);

    if(iap.currentPacket==0)
    {
        iap.crc32=CRC32_INIT_VALUE;
        iap.crc32Packet=0;
    }

    if(iap.currentPacket<(iap.packetSum))
    {
        if(iap.currentPacket>0)
        {
            if(iap.currentPacket>iap.crc32Packet)
            {
                iap.crc32 = crc32(iap.crc32, &iap.currentPacketData[0], iap.currentPacketLen);
                iap.crc32Packet = iap.currentPacket;
            }
        } else
        {
            iap.crc32 = crc32(iap.crc32,&iap.currentPacketData[0], iap.currentPacketLen);
            iap.crc32Packet = iap.currentPacket;
        }
    }


    memcpy(&data[0+dataLen],&iap.crc32,sizeof (iap.crc32 ));
    dataLen+=sizeof (iap.crc32);


    memcpy(&data[0+dataLen],&iap.currentPacketData[0],iap.currentPacketLen);
    dataLen+=iap.currentPacketLen;

    //currentPacketData=QByteArray((char *) &data[0],iap.currentPacketLen);
    currentPacketData=QByteArray((char *) &data[0],dataLen);



  //  QString recvText;
    //recvText = byteArray2Text(BinPacket);

    //QString str=byteArray2Text(currentPacketData);
    //ui->serialPortLogEdit->appendPlainText(recvText);


    //if(iap.currentPacket<=4)
   // {
   //     printfLog(str);
   // }


    QString str;
    str.sprintf("iap.currentPacket is %d, iap.crc32 is 0x%08x\r\n",iap.currentPacket,iap.crc32);
    printfDbugLog(str);

    //str.sprintf("currentPacketData=%d\r\n",currentPacketData.length());
    //printfLog(str);


    return currentPacketData;
}

// 字节数组转16进制
//QString byteArray2Text(const QByteArray &str)
//{
    //QByteArray hex, result;
    //hex = str.toHex().toUpper();
    //for(int i = 0; i < hex.length(); i += 2)
    //{
     //   result += hex.mid(i, 2) + ' ';
    //}
    //return result;


//}


/*
QString byteArray2Text(QByteArray data)
{
  //  rmTextEdit->setText(data.toHex().data());
    QString str;
    str="index ";
    for(int i=0;i<200;i++)
    {
       str += QString("%1 ").arg(i,2,16,QChar('0')) ;//+ " ";
    }

    int row =0;
    for (int i = 0; i < data.length(); i++)
    {
       // First nuber
        if(i%0x10==0)
        {
             str +="\n";
             str +=  QString::number(row,16) +"         " ;
             row++;
        }

          str+=  QString("%1 ").arg((unsigned char)data[i],2,16,QChar('0')).toUpper()  ;
        if (i % 64 == 63)
        {
            str += "\n";
        }

    }

    return str;
}
*/

// USB 打开关闭按钮
void MainWindow::on_pushButton_USB_Open_clicked()
{

    if(ui->pushButton_USB_Open->text() == "USB_Open")
    {
        Rx_Data_Cnt =0;
        int ret =0;
        bool ret_bool =0;

        QString VID_Str = ui->VID_Data->text();
        QString PID_Str = ui->PID_Data->text();

        MY_VID = VID_Str.toUShort(&ret_bool,10);
        MY_PID = PID_Str.toUShort(&ret_bool,10);

        USB_Inerface_On =1;

        ret = USB_Init_Open_Process();    // 打开USB
        // 打开一个文件
        if(ret == 0)   // USB 设备打开失败
        {
            printf("USB Device open Fail!\r\n");
            return;  //
        }

        ui->pushButton_USB_Open->setText("USB_Close");   // 按钮显示修改为USB_Close


        ui->VID_Data->setDisabled(true);
        ui->PID_Data->setDisabled(true);

        USB_Read_Start =1;
    }
    else
    {
        ui->pushButton_USB_Open->setText("USB_Open");   // 按钮显示修改为USB_Close
        ui->VID_Data->setDisabled(false);
        ui->PID_Data->setDisabled(false);
        USB_Read_Start =0;

        if(USB_Inerface_On = 1) // 关闭操作只执行一次
        {
            USB_Close_Process();  // 关闭USB接口
            USB_Inerface_On =0;
        }
    }
}







extern ProtocolItem_t ProtocolItem;
extern txbuf_t txbuf;
extern rxbuf_t rxbuf;
uint8_t sendTestBuf[100];



QByteArray MainWindow::iapframe(void)
{
   QByteArray Frame=IAP();
    QString address;
    address.sprintf("iap.currentPacketAddress=%00000000x ",iap.currentPacketAddress);
    //ui->addressLogEdit->appendPlainText(address);
   return Frame;
}

QByteArray MainWindow::writeBinPacketToProtocolBuf(QByteArray BinPacketData)
{

   // QDateTime current_date_time = QDateTime::currentDateTime();
   // QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
   // printfLog(QString(current_date));
    QByteArray reArry;
    QString str;
    str.sprintf("writeBinPacketToProtocolBuf");
    printfDbugLog(QString(str));

    str = ui->electionBotIdEdit->text();
    ProtocolItem.ElectronBotID = str.toInt();
    str.sprintf("ElectronBotID=%d",ProtocolItem.ElectronBotID);
    printfDbugLog(QString(str));

    str=ui->jointIdCb->currentText();
    ProtocolItem.jointID = str.toInt();
    str.sprintf("jointID=%d",ProtocolItem.jointID);
    printfDbugLog(QString(str));

     //int a=33;
     //ui->electionBotIdEdit->setText(testprintf.number(a,10));
    // str = ui->jointIdCb->currentText();
    // ProtocolItem.jointID = str.toInt();

     ProtocolItem.cmd =CMD_IAP;

     ProtocolItem.dataLen = BinPacketData.length();
     ProtocolItem.data = (uint8_t *)BinPacketData.data();

    // str.sprintf("ProtocolItem.dataLen=%d",ProtocolItem.dataLen);
    // printfLog(str);

    // memset(&txbuf.buf,0,sizeof (txbuf.buf));
    // txbuf.dataLen=0;
     ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);

     //str.sprintf("txbuf.dataLen=%d",txbuf.dataLen);
     //printfLog(str);

     reArry=QByteArray((char*)txbuf.buf,txbuf.dataLen);
     return reArry;
     //return QByteArray((char*)txbuf.buf,txbuf.dataLen);
}

QByteArray MainWindow::ReadRobotSlavesConnectionStatusProtocolBuf()
{
    ProtocolItem.cmd =CMD_ReadRobotSlavesConnectionStatus;

    ProtocolItem.dataLen = 0;
    ProtocolItem.data = 0;
    ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);
    return QByteArray((char*)txbuf.buf,txbuf.dataLen);
}

uint8_t  serialPortframebuf[2048*10]={0};
uint32_t serialPortframeDataLen=0;
void MainWindow::DealSerialPortData()
{
    QString str;

    QByteArray receivedData;
    uint8_t cmd=0;
    receivedData=QByteArray((char*)serialPortframebuf,serialPortframeDataLen);
    cmd=ProtocolProcessing((uint8_t*)receivedData.data(),receivedData.length());
    if(cmd)
    {
        if(cmd==CMD_IAP)
        {
            ui->iapProgressBar->setMaximum((iap.packetSum-1));
            ui->iapProgressBar->setValue(iap.currentPacket);

            if(iap.currentPacket<=iap.packetSum-1 && iap.packetSum>0 )
            {
                QString str;
                str.sprintf("bing write ok");
                printfDbugLog(str);

                str.sprintf("iap.currentPacket is %d",iap.currentPacket);
                printfDbugLog(str);


                QByteArray BinPacket;
                BinPacket.clear();
                BinPacket=writeBinPacketToProtocolBuf(IAP());
                //BinPacket=writeBinPacketToProtocolBuf(iapframe());


                str.sprintf("packetSum is %d current packet %d\r\n",iap.packetSum,iap.currentPacket);
                printfDbugLog(str);

                QDateTime current_date_time = QDateTime::currentDateTime();
                QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
                printfDbugLog(current_date);
              //  serialPort->clear();

               // str.sprintf("BinPacket.lenth()=%d",BinPacket.length());
               // printfLog(str);

                serialPort->write(BinPacket.data(),BinPacket.length());
            }
            else
            {
                iap.waitPacketResponseFlag=false;
                 memset((uint8_t*)&iap,0,sizeof (iap));

                endTime=  QDateTime::currentDateTime();
                qint64 intervalTime = startTime.secsTo(endTime); //求时间差
                qint64 intervalTimeMS = startTime.msecsTo(endTime);

                QString timeUse;
                timeUse.sprintf("Time for downloading APP is %lldms",intervalTimeMS);
                printfLog(QString(timeUse));
                timeUse.sprintf("Time for downloading APP is %llds",intervalTime);
                printfLog(QString(timeUse));

            }
        }

}
 else
{

    if(ui->timeStampCb->isChecked())
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        //QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
        QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
        ui->serialPortLogEdit->appendPlainText(QString(current_date));

        str.sprintf("receivedData.dataLen=%d",receivedData.length());
        ui->serialPortLogEdit->appendPlainText(str);
    }


    QString buf = QString(receivedData);
    ui->serialPortLogEdit->appendPlainText(buf);

    //buf.sprintf("serialPortframeDataLen=%d",serialPortframeDataLen);
    //ui->serialPortLogEdit->appendPlainText(buf);
}

//{
//    recvText = byteArray2Text(receivedData);
//    ui->serialPortLogEdit->appendPlainText(recvText);
//    printfLog(recvText);
//}
}
uint8_t  USB_recieve_time_count=0;
uint32_t  IAP_time_count=0;
uint32_t time_cout1000=0;
uint32_t ReadRobotSlavesConnection_time_count=0;
uint32_t SerialPortReceice_time_counter=0;
uint32_t SerialPortReceiceNextPacket_time_counter=0;

bool UpdateRobotSlavesConnectionStatus_flag=false;
bool SerialPortReceiceNextPacket_flag=false;

/******* 处理USB接收 **********/
void MainWindow::handleTimeout(void)   // 定时器超时1S一次
{

    QByteArray BinPacket;

    QString str;

    SerialPortReceice_time_counter++;
    SerialPortReceiceNextPacket_time_counter++;

    USB_recieve_time_count++;
    IAP_time_count++;
    ReadRobotSlavesConnection_time_count++;
    iap.waitPacketResponseTimeCount++;

    if(USB_recieve_time_count>=100)
    {
       USB_recieve_time_count=0;
       USB_recieve_process();
    }

    if(iap.waitPacketResponseTimeCount>(400/TimerCell) && iap.waitPacketResponseFlag==true)
    {
        IAP_time_count=0;
        iap.waitPacketResponseTimeCount=0;

          // QByteArray BinPacket;
        BinPacket=writeBinPacketToProtocolBuf(IAP());
         // BinPacket=writeBinPacketToProtocolBuf(iapframe());

        str.sprintf("packetSum is %d current packet %d\r\n",iap.packetSum,iap.currentPacket);
        printfDbugLog(str);

        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
        printfDbugLog(current_date);


        //str=byteArray2Text(BinPacket);
       // printfDbugLog(str);
       // if(iap.currentPacket<3)
       // printfLog(str);

       // str.sprintf("BinPacket.lenth()=%d",BinPacket.length());
       // printfLog(str);

        serialPort->write(BinPacket.data(),BinPacket.length());
       // serialPort->write(BinPacket.data(),512);
    }

   if(ReadRobotSlavesConnection_time_count>(1000/TimerCell)) //更新连接状态
   {
       QByteArray buf;
       ReadRobotSlavesConnection_time_count=0;

       buf=ReadRobotSlavesConnectionStatusProtocolBuf();

       //QString str;
       //str = byteArray2Text(buf);
       //printfLog(str);
       if(ui->jointStatusReadEn->isChecked())serialPort->write(buf.data(),buf.length());
   }

   if(SerialPortReceice_time_counter>(15/TimerCell) && SerialPortReceiceNextPacket_flag==true)
   {
       DealSerialPortData();
       SerialPortReceice_time_counter=0;
       SerialPortReceiceNextPacket_flag=false;
       serialPortframeDataLen=0;

        memset(&serialPortframebuf[0],0,sizeof (serialPortframebuf));
   }

   if(UpdateRobotSlavesConnectionStatus_flag)
   {
       UpdateRobotSlavesConnectionStatus_flag=false;
       UpdateRobotSlavesConnectionStatus();
   }

}

// 窗口关闭事件
void MainWindow::closeEvent(QCloseEvent *event)
{

}

void MainWindow::on_USB_clicked()
{

}

void MainWindow::on_Clear_clicked()
{
    printfLog(" ");
}

// USB 发送数据
void MainWindow::on_LED_ON_CMD_clicked()
{
    char dat[3]={0x17,0x18,0x19};
    int ret = USB_Write_Process(dat,3,100);
    ret = ret+1;
}

void MainWindow::on_LED_OFF_CMD_clicked()
{
    char dat[3]={7,8,9};
    int ret = USB_Write_Process(dat,3,100);
    ret = ret+1;
}

// 16进制转ASCII
QByteArray string2ByteArray(const QString &str)
{
    QByteArray result;
    for(int i = 0; i < str.length(); i += 2)
    {
        result += str.mid(i, 2).toInt(NULL, 16);
    }
    return result;
}

// 文本转ASCII
QByteArray text2ByteArray(const QString &str)
{
    QString data = str.simplified();
    QStringList hexlist = data.split(" ");
    QByteArray result;
    for(int i = 0; i < hexlist.length(); ++i)
    {
        if(!hexlist[i].isEmpty())
        {
            result += string2ByteArray(hexlist[i]);
        }
    }
    return result;
}


// 数据发送
void MainWindow::on_Data_Send_clicked()
{
    // 发送数据
        if(ui->pushButton_USB_Open->text() == "USB_Open")
        {
            return;
        }
        if(ui->USB_Send_Data->toPlainText().isEmpty())
        {
            return;
        }

        QByteArray sendData;
        if(ui->radioButton_send_ascii->isChecked())
        {
            sendData = ui->USB_Recieve_Data->toPlainText().toLatin1();
        }
        else
        {
            sendData = text2ByteArray(ui->USB_Send_Data->toPlainText());
        }



        char *SendDataChar;
        SendDataChar = sendData.data();

        if(ui->checkBox_send->isChecked())
        {
            if(ui->Data_Send->text() == "发送")
            {
                int ret = USB_Write_Process(SendDataChar,sendData.length(),100);
                int time_interval =ui->lineEdit_send->text().toInt();
                timer_Send.start( time_interval );
                ui->Data_Send->setText("停止发送");
                ui->checkBox_send->setEnabled(false);
            }
            else
            {
                timer_Send.stop();
                ui->Data_Send->setText("发送");
                ui->checkBox_send->setEnabled(true);
            }
        }
        else
        {
               int ret = USB_Write_Process(SendDataChar,sendData.length(),100);
        }

}

void MainWindow::on_clear_send_data_clicked()
{
    ui->USB_Send_Data->clear();
}

void MainWindow::on_checkBox_send_toggled(bool checked)
{
    if(checked)
    {
        if(ui->lineEdit_send->text().toInt() > 0)
        {
            ui->lineEdit_send->setEnabled(false);
        }
        else
        {
            ui->checkBox_send->setChecked(false);
            QMessageBox::critical(this, "错误", "循环发送周期错误", "确定");
        }
    }
    else
    {
        ui->lineEdit_send->setEnabled(true);
    }
}


void MainWindow::sendData()
{
    QByteArray sendData;
    char *SendDataChar;
    if(ui->radioButton_send_ascii->isChecked())
    {
        sendData = ui->USB_Recieve_Data->toPlainText().toLatin1();
    }
    else
    {
        sendData = text2ByteArray(ui->USB_Send_Data->toPlainText());
    }
    SendDataChar = sendData.data();
    int ret = USB_Write_Process(SendDataChar,sendData.length(),100);
}



void MainWindow::on_pushButton_recv_save_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "请输入保存文件名", "", "文本文件 (*.txt)");
    if(filePath.isEmpty())
    {
        return;
    }

    QFile file(filePath);
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        file.write(ui->USB_Recieve_Data->toPlainText().toUtf8());
        file.close();
        QMessageBox::information(this, "提示", "文件保存成功", "确定");
    }
    else
    {
        QMessageBox::critical(this, "错误", "文件保存失败", "确定");
    }
}



void MainWindow::USB_recieve_process()
{
    if( USB_Read_Start ==1 )   // USB 开始接收数据
    {
        QString recvText;
        int ret = USB_Read_Process(Rx_dat_buf, 300);

        if(ret >0)
        {
            QByteArray receivedData = QByteArray(Rx_dat_buf,ret);

            Rx_Data_Frame = ret;
            Rx_Data_Cnt += ret;
            if(ui->radioButton_recv_ascii->isChecked())
            {
                recvText = QString::fromLatin1(receivedData,ret);
                printfLog(QString("接收：%1").arg(recvText));
            }
            else
            {
                recvText = byteArray2Text(receivedData);
                printfLog(QString(recvText));
            }
            USB_Close_Process();
            //open_dev();
        }
    }
}

//// 接收数据
//void MainWindow::getUsbData()
//{
//    QByteArray receivedData = serialport->readAll();

//    QString recvText;

//}

void MainWindow::on_about_triggered()
{
    QMessageBox::information(this, "提示", "本软件为免费软件，禁止任何组合和个人利用本软件盈利！"
                            );
}
// 功能描述
void MainWindow::on_function_triggered()
{
    QMessageBox::information(this, "提示", "打开串口后，可以与电子进行串口通讯"
            );
}



void MainWindow::myPrintf(const char* format, ...)
{
 //   QString str;
 //   QByteArray buf;
  //  sprintf(buf.data(),format);

 //   str=QString(buf);
    //str.sprintf( (char*) format);
 //   printfLog(str);
}

void MainWindow::PrintfElectronBotJointStatus(ElectronBotJointStatus_t * Status,uint8_t id)
{

       QString str;
       str.sprintf("*******joint%dStatus******\r\n",id);
       printfLog(str);

       str.sprintf("angleMin=%f\r\n", Status->angleMin);
       printfLog(str);

       str.sprintf("angleMax=%f\r\n", Status->angleMax);
       printfLog(str);

       str.sprintf("modelAngelMin=%f\r\n", Status->modelAngelMin);
       printfLog(str);

       str.sprintf("modelAngelMax=%f\r\n", Status->modelAngelMax);
       printfLog(str);

       str.sprintf("torqueLimit=%f\r\n", Status->torqueLimit);
       printfLog(str);

       str.sprintf("kp=%f\r\n", Status->kp);
       printfLog(str);

       /*str.sprintf("\r\n");


       str.sprintf("initAngle=%f\r\n", Status->initAngle);
       printfLog(str);

       str.sprintf("angle=%f\r\n", Status->angle);
       printfLog(str);

       str.sprintf("ki=%f\r\n", Status->ki);
       printfLog(str);

       str.sprintf("kv=%f\r\n", Status->kv);
       printfLog(str);

       str.sprintf("kd=%f\r\n", Status->kd);
       printfLog(str);

       str.sprintf("enable=%s\r\n", Status->enable ? "true" : "false");
       printfLog(str);

       str.sprintf("inverted=%s\r\n", Status->inverted ? "true" : "false");
        printfLog(str);
*/
}

QByteArray MainWindow::writeJointStatusToBuf(void)
{
   // QTime current_time =QTime::currentTime();
   // int hour = current_time.hour();
   // int minute = current_time.minute();
   // int second = current_time.second();
   // int msec = current_time.msec();

   // QString timestr;
   // timestr.sprintf("%d:%d:%d:%d",hour,minute,second,msec);
   // printfLog(QString(timestr));

    QDateTime current_date_time = QDateTime::currentDateTime();
    //QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
    printfLog(QString(current_date));

    QString testprintf;
    testprintf.sprintf("writeJointStatus");
    printfLog(QString(testprintf));

    QString str = ui->electionBotIdEdit->text();
    ProtocolItem.ElectronBotID = str.toInt();
    str.sprintf("ElectronBotID=%d",ProtocolItem.ElectronBotID);
    printfLog(QString(str));

    str=ui->jointIdCb->currentText();
    ProtocolItem.jointID = str.toInt();
    str.sprintf("jointID=%d",ProtocolItem.jointID);
    printfLog(QString(str));

     //int a=33;
     //ui->electionBotIdEdit->setText(testprintf.number(a,10));
    // str = ui->jointIdCb->currentText();
    // ProtocolItem.jointID = str.toInt();

     ProtocolItem.cmd =CMD_WriteAllJointStatus;

     ElectronBotJointStatus_t JointStatus;

     JointStatus.angleMin = ui->angleMinDsb->value();
     str.sprintf("angleMin=%lf",JointStatus.angleMin);
    // printfLog(QString(str));

     JointStatus.angleMax = ui->angleMaxDsb->value();
     str.sprintf("angleMax=%lf",JointStatus.angleMax);
    // printfLog(QString(str));

     JointStatus.modelAngelMin = ui->modelAngleMinDsb->value();
     str.sprintf("angleMin=%lf",JointStatus.modelAngelMin);
    // printfLog(QString(str));

     JointStatus.modelAngelMax = ui->modelAngleMaxDsb->value();
     str.sprintf("angleMax=%lf",JointStatus.modelAngelMax);
   //  printfLog(QString(str));


     JointStatus.torqueLimit = ui->torqueLimitDsb->value();
     str.sprintf("torqueLimit=%lf",JointStatus.torqueLimit);
    // printfLog(QString(str));

     JointStatus.kp = ui->kpDsb->value();
     str.sprintf("kp=%lf",JointStatus.kp);
   //  printfLog(QString(str));

  //   str.sprintf("\r\n");
  //   printfLog(QString(str));


     JointStatus.initAngle = ui->initAngleDsb->value();
     str.sprintf("initAngle=%lf",JointStatus.initAngle);
  //   printfLog(QString(str));

     JointStatus.angle = ui->angleDsb->value();
     str.sprintf("angle=%lf",JointStatus.angle);
 //    printfLog(QString(str));

     JointStatus.ki = ui->kiDsb->value();
     str.sprintf("ki=%lf",JointStatus.ki);
   //  printfLog(QString(str));

     JointStatus.kv = ui->kvDsb->value();
     str.sprintf("kv=%lf",JointStatus.kv);
   //  printfLog(QString(str));

     JointStatus.kd = ui->kdDsb->value();
     str.sprintf("kd=%lf",JointStatus.kd);
  //   printfLog(QString(str));

     if(ui->enableCb->isChecked())
     {
        JointStatus.enable = 1;
     }
     else
     {
        JointStatus.enable = 0;
     }
     str.sprintf("enable=%s",JointStatus.enable?"true" :"false");
   //  printfLog(QString(str));

     if(ui->invertedCb->isChecked())
     {
        JointStatus.inverted = 1;
     }
     else
     {
        JointStatus.inverted = 0;
     }
     str.sprintf("inverted=%s",JointStatus.inverted?"true" :"false");
   //  printfLog(QString(str));

     PrintfElectronBotJointStatus(&JointStatus,ProtocolItem.jointID);

     ProtocolItem.dataLen = sizeof(JointStatus);
     ProtocolItem.data = (uint8_t *)&JointStatus;

    // memset(&txbuf.buf,0,sizeof (txbuf.buf));
    // txbuf.dataLen=0;
     ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);


     return QByteArray((char*)txbuf.buf,txbuf.dataLen);
}


void MainWindow::on_writeJointStatus_clicked()
{
    QByteArray sendData = writeJointStatusToBuf();

     serialPort->write(sendData.data(),sendData.length());
}




extern struct RobotSlavesConnectionStatus_t  RobotSlavesConnectionStatus;
void MainWindow::UpdateRobotSlavesConnectionStatus()
{
    if(RobotSlavesConnectionStatus.joint2==true)
    {
       // ui->joint2cb->
         ui->joint2Cb->setChecked(true);
    }
    else
    {
        ui->joint2Cb->setChecked(false);
    }

    if(RobotSlavesConnectionStatus.joint4==true)
    {
       // ui->joint2cb->
         ui->joint4Cb->setChecked(true);
    }
    else
    {
        ui->joint4Cb->setChecked(false);
    }

    if(RobotSlavesConnectionStatus.joint6==true)
    {
       // ui->joint2cb->
         ui->joint6Cb->setChecked(true);
    }
    else
    {
        ui->joint6Cb->setChecked(false);
    }


    if(RobotSlavesConnectionStatus.joint8==true)
    {
       // ui->joint2cb->
         ui->joint8Cb->setChecked(true);
    }
    else
    {
        ui->joint8Cb->setChecked(false);
    }


    if(RobotSlavesConnectionStatus.joint10==true)
    {
       // ui->joint2cb->
         ui->joint10Cb->setChecked(true);
    }
    else
    {
        ui->joint10Cb->setChecked(false);
    }

    if(RobotSlavesConnectionStatus.joint12==true)
    {
       // ui->joint2cb->
         ui->joint12Cb->setChecked(true);
    }
    else
    {
        ui->joint12Cb->setChecked(false);
    }

    if(RobotSlavesConnectionStatus.mpu6050Init==true)
    {
       // ui->joint2cb->
         ui->mpu6050Cb->setChecked(true);
    }
    else
    {
        ui->mpu6050Cb->setChecked(false);
    }

    if(RobotSlavesConnectionStatus.paj7620Init==true)
    {
       // ui->joint2cb->
         ui->paj7620Cb->setChecked(true);
    }
    else
    {
        ui->paj7620Cb->setChecked(false);
    }



}

void MainWindow::serialPortReadRead_Slot()
{
    QString buf;
    //buf = QString(serialPort->readAll());
    //ui->serialPortLogEdit->appendPlainText(buf);

    QByteArray receivedData= serialPort->readAll();

    if(SerialPortReceiceNextPacket_flag==false)
    {
        SerialPortReceice_time_counter=0;
        serialPortframeDataLen=0;

         memcpy(&serialPortframebuf[serialPortframeDataLen],receivedData.data(),receivedData.length());
         serialPortframeDataLen+=receivedData.length();
         SerialPortReceiceNextPacket_flag=true;
    }
    else if(SerialPortReceiceNextPacket_flag==true)
    {
        SerialPortReceice_time_counter=0;
        //memcpy(serialPortframebuf,receivedData.data(),receivedData.length());
        memcpy(&serialPortframebuf[serialPortframeDataLen],receivedData.data(),receivedData.length());
        serialPortframeDataLen+=receivedData.length();
    }


   /* QString recvText;

    uint8_t cmd=0;
    //if(ProtocolLookUp(framebuf,(uint8_t*)receivedData.data(),receivedData.length()))
    //if(ProtocolProcessing((uint8_t*)receivedData.data(),receivedData.length()))
    cmd=ProtocolProcessing((uint8_t*)receivedData.data(),receivedData.length());
    {
       // if(iap.currentPacket<=iap.packetSum-1 && iap.currentPacket<10)
       if((iap.currentPacket<=iap.packetSum-1) && cmd==CMD_IAP)
       {
            QString str;
            str.sprintf("bing write ok");
            printfLog(QString(str));

            str.sprintf("iap.currentPacket is %d",iap.currentPacket);
            printfLog(QString(str));


            QByteArray BinPacket;
           // BinPacket=writeBinPacketToProtocolBuf(IAP());
            BinPacket=writeBinPacketToProtocolBuf(iapframe());

            str.sprintf("packetSum is %d current packet %d\r\n",iap.packetSum,iap.currentPacket);
             printfLog(QString(str));

            QDateTime current_date_time = QDateTime::currentDateTime();
            QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
             printfLog(QString(current_date));

            QString recvText;
             recvText = byteArray2Text(BinPacket);
             ui->serialPortLogEdit->appendPlainText(recvText);
    // ui->serialPortLogEdit->appendPlainText(recvText);
    // iap.waitPacketResponseFlag=false;
            serialPort->write(BinPacket.data(),BinPacket.length());
       }
        else{
            iap.waitPacketResponseFlag=false;
        }

    }

    {
        recvText = byteArray2Text(receivedData);
        ui->serialPortLogEdit->appendPlainText(recvText);
        printfLog(recvText);
    }
   // else
    {
        buf = QString(receivedData);
        ui->serialPortLogEdit->appendPlainText(buf);
    }*/

   // QByteArray receivedData = QByteArray(Rx_dat_buf,ret);

  //  Rx_Data_Frame = ret;
 //   Rx_Data_Cnt += ret;
  //  if(ui->radioButton_recv_ascii->isChecked())
  //  {
  //      recvText = QString::fromLatin1(receivedData,ret);
  //      printfLog(QString("接收：%1").arg(recvText));
  //  }
  //  else
  //  {
  //      recvText = byteArray2Text(receivedData);
  //      printfLog(QString(recvText));
  //  }
  //  USB_Close_Process();*/

}


void MainWindow::uiprintf(QString str)
{
    printfLog(QString(str));
}

void MainWindow::on_openBt_clicked()
{
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity  checkBits;

    if(ui->baudRateCb->currentText() == "4800")
    {
        baudRate = QSerialPort::Baud4800;
    }else if(ui->baudRateCb->currentText() == "9600")
    {
        baudRate = QSerialPort::Baud9600;
    }else if(ui->baudRateCb->currentText() == "115200")
    {
        baudRate = QSerialPort::Baud115200;
    }

    if(ui->dataBitCb->currentText() == "5")
    {
        dataBits = QSerialPort::Data5;
    }else if(ui->dataBitCb->currentText() == "6")
    {
        dataBits = QSerialPort::Data6;
    }else if(ui->dataBitCb->currentText() == "7")
    {
        dataBits = QSerialPort::Data7;
    }else if(ui->dataBitCb->currentText() == "8")
    {
        dataBits = QSerialPort::Data8;
    }

    if(ui->stopBitCb->currentText() == "1")
    {
        stopBits = QSerialPort::OneStop;
    }else if(ui->stopBitCb->currentText() == "1.5")
    {
        stopBits = QSerialPort::OneAndHalfStop;
    }else if(ui->stopBitCb->currentText() == "2")
    {
        stopBits = QSerialPort::TwoStop;
    }

    if(ui->checkBitCb->currentText() == "none")
    {
        checkBits = QSerialPort::NoParity;
    }

   // QSerialPortInfo *p;
   // p=&ui->serialPortCb->currentText();
   // serialPort->setPort((QSerialPortInfo)ui->serialPortCb->currentText());
    serialPort->setPortName(ui->serialPortCb->currentText());
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(checkBits);

    if(serialPort->open(QIODevice::ReadWrite) == true)
    {
        QMessageBox::information(this,"提示","成功");
    }
    else
    {
        QMessageBox::critical(this,"提示","失败");
    }
}

void MainWindow::on_closeBt_clicked()
{
    serialPort->close();
}

void MainWindow::on_sendBt_clicked()
{

    QByteArray sendData;
    uint8_t testBuf[1024]={0};
    for(uint16_t i=0;i<1024;i++)
    {
        testBuf[i]=i;
    }

    sendData=QByteArray((char*)testBuf,300);

   // serialPort->write(ui->sendEdit->text().toLocal8Bit().data());
   // serialPort->write(sendData.data(),sendData.length());
    serialPort->write((char*)testBuf,255);
}

void MainWindow::on_clearBt_clicked()
{
    ui->serialPortLogEdit->clear();
}

void MainWindow::on_writeJointIdBt_clicked()
{
    uint8_t NewJointId=2;
    QString str = ui->electionBotIdEdit->text();
    ProtocolItem.ElectronBotID = str.toInt();
    str.sprintf("ElectronBotID=%d",ProtocolItem.ElectronBotID);
    printfLog(QString(str));

    str=ui->jointIdCb->currentText();
    ProtocolItem.jointID = str.toInt();
    str.sprintf("jointID=%d",ProtocolItem.jointID);
    printfLog(QString(str));

     //int a=33;
     //ui->electionBotIdEdit->setText(testprintf.number(a,10));
    // str = ui->jointIdCb->currentText();
    // ProtocolItem.jointID = str.toInt();


    str=ui->writeJointIdCb->currentText();
    NewJointId = str.toInt();
    str.sprintf("NewJointID=%d",NewJointId);
    printfLog(QString(str));

     ProtocolItem.cmd =CMD_WriteJointID;

     ProtocolItem.dataLen = 1;
     ProtocolItem.data = (uint8_t *)&NewJointId;

    // memset(&txbuf.buf,0,sizeof (txbuf.buf));
    // txbuf.dataLen=0;
     ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);

        QByteArray sendData;
     sendData=QByteArray((char*)txbuf.buf,txbuf.dataLen);
      serialPort->write(sendData.data(),sendData.length());

}


struct RotationTestAngle_t
{
    double  StartAngle;
    double  EndAngle;
}     RotationTestAngle;

void MainWindow::on_jointRotationTestBt_clicked()
{
    QString str = ui->electionBotIdEdit->text();
    ProtocolItem.ElectronBotID = str.toInt();
    str.sprintf("ElectronBotID=%d",ProtocolItem.ElectronBotID);
    printfLog(QString(str));

    str=ui->jointIdCb->currentText();
    ProtocolItem.jointID = str.toInt();
    str.sprintf("jointID=%d",ProtocolItem.jointID);
    printfLog(QString(str));


    RotationTestAngle.StartAngle = ui->rotationTestStartAngleDsb->value();
    RotationTestAngle.EndAngle = ui->rotationTestEndAngleDsb->value();

   // str.sprintf("kp=%lf",JointStatus.kp);
   // printfLog(QString(str));
     //int a=33;
     //ui->electionBotIdEdit->setText(testprintf.number(a,10));
    // str = ui->jointIdCb->currentText();
    // ProtocolItem.jointID = str.toInt();

     ProtocolItem.cmd =CMD_JointRotationTest;

     ProtocolItem.dataLen = sizeof(RotationTestAngle_t);
     ProtocolItem.data = (uint8_t*)&RotationTestAngle;

    // memset(&txbuf.buf,0,sizeof (txbuf.buf));
    // txbuf.dataLen=0;
     ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);

     QByteArray sendData;
     sendData=QByteArray((char*)txbuf.buf,txbuf.dataLen);
     serialPort->write(sendData.data(),sendData.length());
}

void MainWindow::on_jointRotationTestCloseBt_clicked()
{
    QString str = ui->electionBotIdEdit->text();
    ProtocolItem.ElectronBotID = str.toInt();
    str.sprintf("ElectronBotID=%d",ProtocolItem.ElectronBotID);
    printfLog(QString(str));

    str=ui->jointIdCb->currentText();
    ProtocolItem.jointID = str.toInt();
    str.sprintf("jointID=%d",ProtocolItem.jointID);
    printfLog(QString(str));



   // str.sprintf("kp=%lf",JointStatus.kp);
   // printfLog(QString(str));
     //int a=33;
     //ui->electionBotIdEdit->setText(testprintf.number(a,10));
    // str = ui->jointIdCb->currentText();
    // ProtocolItem.jointID = str.toInt();

     ProtocolItem.cmd =CMD_JointRotationTestClose;

     ProtocolItem.dataLen = 0;
     ProtocolItem.data = 0;

    // memset(&txbuf.buf,0,sizeof (txbuf.buf));
    // txbuf.dataLen=0;
     ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);

     QByteArray sendData;
     sendData=QByteArray((char*)txbuf.buf,txbuf.dataLen);
     serialPort->write(sendData.data(),sendData.length());
}

void MainWindow::on_resetMasterBt_clicked()
{
    cleanJointStatusCb();
    QString str = ui->electionBotIdEdit->text();
    ProtocolItem.ElectronBotID = str.toInt();
    str.sprintf("ElectronBotID=%d",ProtocolItem.ElectronBotID);
    printfLog(QString(str));

    str=ui->jointIdCb->currentText();
    ProtocolItem.jointID = str.toInt();
    str.sprintf("jointID=%d",ProtocolItem.jointID);
    printfLog(QString(str));

     //int a=33;
     //ui->electionBotIdEdit->setText(testprintf.number(a,10));
    // str = ui->jointIdCb->currentText();
    // ProtocolItem.jointID = str.toInt();



     ProtocolItem.cmd =CMD_ResetMaster;

     ProtocolItem.dataLen = 0;
     ProtocolItem.data = 0;

    // memset(&txbuf.buf,0,sizeof (txbuf.buf));
    // txbuf.dataLen=0;
     ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);

     QByteArray sendData;
     sendData=QByteArray((char*)txbuf.buf,txbuf.dataLen);
     serialPort->write(sendData.data(),sendData.length());
}


void MainWindow::on_writeElectionBotIdBt_clicked()
{
    uint32_t NewElectronBotID=0;
    QString str = ui->electionBotIdEdit->text();
    ProtocolItem.ElectronBotID = str.toInt();
    str.sprintf("ElectronBotID=%d",ProtocolItem.ElectronBotID);
    printfLog(QString(str));

    str=ui->jointIdCb->currentText();
    ProtocolItem.jointID = str.toInt();
    str.sprintf("jointID=%d",ProtocolItem.jointID);
    printfLog(QString(str));

     //int a=33;
     //ui->electionBotIdEdit->setText(testprintf.number(a,10));
    // str = ui->jointIdCb->currentText();
    // ProtocolItem.jointID = str.toInt();


    str=ui->writeElectionBotIdEdit->text();
    NewElectronBotID = str.toInt();
    str.sprintf("NewElectronBotID=%d",NewElectronBotID);
    printfLog(QString(str));

     ProtocolItem.cmd =CMD_WriteJointID;

     ProtocolItem.dataLen = 1;
     ProtocolItem.data = (uint8_t *)&NewElectronBotID;

    // memset(&txbuf.buf,0,sizeof (txbuf.buf));
    // txbuf.dataLen=0;
     ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);

        QByteArray sendData;
     sendData=QByteArray((char*)txbuf.buf,txbuf.dataLen);
      serialPort->write(sendData.data(),sendData.length());
}

void MainWindow::on_writeJointStatus_2_clicked()
{
    //settings = new QSettings();


    //如果不存在Config.ini，便生成一个Config.ini。如果已经存在了，则略过。
    //if(!QFile::exists("setting.ini"))
    //{
    //     QSettings configIniWrite("setting.ini",QSettings::IniFormat);
       // 输出setting.ini得文件路径，就可以找到配置文件了
    //   qDebug() << QCoreApplication::applicationDirPath();
   // }


    settings = new QSettings("defaultJointStauts.ini",QSettings::IniFormat);


       //通过setValue函数将键值对放在相对于的节下面
       // 格式： settings->setValue("节名/键名","键值");

       //settings->setValue("student/name","zs");
      // settings->setValue("student/age","18");
      // settings->setValue("student/sex","male");

       // 通过value获取值
       // 格式： settings->value(""节名/键名"");
       //settings->value("student/name");
      // settings->value("student/age");
      // settings->value("student/sex");

       //qDebug() << settings->value("student/name");
      // qDebug() << settings->value("student/age");
      // qDebug() << settings->value("student/sex").toString();

       char path[][8] = {"joint0","joint2","joint4","joint6","joint8","joint10","joint12"};
       char path2[]="joint4";
       uint8_t jointID=0;;
       QString str;


       str=ui->jointIdCb->currentText();
       jointID = str.toInt()/2;

       //str.sprintf("torqueLimit=%lf",JointStatus.torqueLimit);
       //QString value;
       double angleMin=0;
       double torqueLimit=0.5;
       double value=0;
       //settings->value("joint4/angleMin");
       //settings->value("%s/angleMin",path);
       //settings->value("joint4/angleMax");
       //angleMin=settings->value("%s/angleMin",path).toDouble();

       value=0;
       str.sprintf("%s/angleMin",&path[jointID][0]);
       //value=settings->value("joint4/angleMin").toDouble();
        value =settings->value(QString(str)).toDouble();
       //value
       //ui->angleMinDsb->value()=angleMin;
        ui->angleMinDsb->setValue(value);

        //value=0;
        //value =settings->value("joint4/angleMax").toDouble();
        //ui->angleMaxDsb->setValue(value);

        value=0;
        str.sprintf("%s/angleMax",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->angleMaxDsb->setValue(value);

        // value=0;
        // str.sprintf("%s/angleMax",path2);
        // value =settings->value(QString(str)).toDouble();
        // ui->angleMaxDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/modelAngleMin").toDouble();
        str.sprintf("%s/modelAngleMin",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->modelAngleMinDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/modelAngleMax").toDouble();
        str.sprintf("%s/modelAngleMax",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->modelAngleMaxDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/initAngle").toDouble();
        str.sprintf("%s/initAngle",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->initAngleDsb->setValue(value);


        value=0;
        str.sprintf("%s/angle",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        //value=2;
        ui->angleDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/torqueLimit").toDouble();
        str.sprintf("%s/torqueLimit",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->torqueLimitDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/kp").toDouble();
        str.sprintf("%s/kp",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->kpDsb->setValue(value);

        value=0;
       // value =settings->value("joint4/ki").toDouble();
        str.sprintf("%s/ki",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->kiDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/kv").toDouble();
        str.sprintf("%s/kv",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->kvDsb->setValue(value);

        value=0;
       // value =settings->value("joint4/kd").toDouble();
        str.sprintf("%s/kd",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->kdDsb->setValue(value);

        bool b_value=false;
       // b_value =settings->value("joint4/inverted").toBool();
        str.sprintf("%s/inverted",&path[jointID][0]);
        b_value =settings->value(QString(str)).toBool();
        ui->invertedCb->setChecked(b_value);

       // b_value =settings->value("joint4/enable").toBool();
        str.sprintf("%s/enable",&path[jointID][0]);
        b_value =settings->value(QString(str)).toBool();
        ui->enableCb->setChecked(b_value);


       //qDebug() << settings->value("%s/angleMin",path);
       //qDebug() << settings->value("joint4/angleMax");
}

void MainWindow::on_appBt_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
        tr("Open File"),//控件的objectName
         ".",           //对话框显示时默认打开的目录，"." 代表程序运行目录
        tr("Text Files(*.bin)"));   //对话框的后缀名过滤器，将txt文件改为bin文件。
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, tr("Read File"),
                tr("Cannot open file:\n%1").arg(path));
            return;
        }

        QString buf;
        QFileInfo info(file);

        buf = QString(info.absolutePath());
        buf.append("/");
        buf.append(info.baseName());    //直接获得文件名
        buf.append(".bin");
        qDebug("%s",qPrintable(buf));


         ui->apppathEdit->setText(buf);
        ui->appsizeEdit->setText(QString::number(info.size(),16));

        //QString appsize = QString("0x%1").arg(info.size(), 8, 16, QLatin1Char('0'));
        //appsize = appsize.toUpper();

        //QString appsize;
        //appsize.sprintf("0x%08llx",info.size());
        //ui->appsizeEdit->setText(appsize);
        //ui->appoffsizeEdit->setText(tr("0x00010000"));  //默认偏移量

    } else {
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
    }
}

void MainWindow::on_pushButton_clicked()
{
    QFile readfile(ui->apppathEdit->text());    //要读取的bin文件
    QFileInfo read_file_info(readfile);         //要读取的bin文件的信息
    if (!readfile.open(QIODevice::ReadOnly)) {  //只读方式打开bin文件
        QMessageBox::warning(this, tr("Read File"),
            tr("Cannot open file:\n%1").arg(ui->apppathEdit->text()));
        return;
    }
    QDataStream BinFileData(&readfile);  //将刚刚创建的 file 对象的指针传递给一个QDataStream实例 BinFileData
    char *pBuff = new char[1024*1024];    //修改为1M大小，以便读取APP.bin时复用。修改为动态分配？
    BinFileData.readRawData(pBuff,static_cast<int>(read_file_info.size()));//要读取bin文件的大小
    qDebug() << "IAP.bin size is " << static_cast<int>(read_file_info.size());    //打印文件长度

    IAP_DataAllArray.clear();
    IAP_DataAllArray = QByteArray(pBuff,static_cast<int>(read_file_info.size()));

    QString recvText;
    recvText = byteArray2Text(IAP_DataAllArray);
  //  ui->serialPortLogEdit->appendPlainText(recvText);
    printfDbugLog(recvText);

    //uint8_t testBuf[1024]={0};
    //for(uint16_t i=0;i<1024;i++)
    //{
    //    testBuf[i]=i;
    //}


    iap.codeLen=read_file_info.size();
   // uint32_t crc32Value=0xffffffff;

   // crc32Value=crc32(crc32Value,(uint8_t*)IAP_DataAllArray.data(),IAP_DataAllArray.length());

   // QString str;

  //  str.sprintf("0x%08x",crc32Value);
  //  ui->crc32ValueEdit->setText(str);
    //printfLog(str);

    IAP_init();

    showBinFileInfo();

 //   QString str;
 //   uint32_t crc32Value=0xffffffff;
 //   crc32Value=0xffffffff;

 //   crc32Value=crc32(crc32Value,(uint8_t*)iap.data,iap.codeLen);
    //crc32Value=crc32(crc32Value,(uint8_t*)iap.data,IAP_PACKET_MAX_DATA_LEN*2);
 //   str.sprintf("0x%08x",crc32Value);
//    ui->crc32ValueEdit->setText(str);
//    printfLog(str);

    //IAP_init();

   // str=IAP_init();
   // printfLog(str);

   // sendData=IAP();
   // sendData=QByteArray((char*)testBuf,1024);
   // recvText = byteArray2Text(sendData);
   // ui->serialPortLogEdit->appendPlainText(recvText);

    //serialPort->write(sendData.data(),sendData.length());
   // serialPort->write(sendData.data(),128);

   // timer->start(1000);

    readfile.close();
}

void MainWindow::timeoutSlot()
{
    QString str;
    str.sprintf("timeout1000");
    printfLog(QString(str));
}

void MainWindow::on_iniBt_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
        tr("Open File"),//控件的objectName
         ".",           //对话框显示时默认打开的目录，"." 代表程序运行目录
        tr("Text Files(*.ini)"));   //对话框的后缀名过滤器，将txt文件改为bin文件。
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, tr("Read File"),
                tr("Cannot open file:\n%1").arg(path));
            return;
        }

        QString buf;
        QFileInfo info(file);

        buf = QString(info.absolutePath());
        buf.append("/");
        buf.append(info.baseName());    //直接获得文件名
        buf.append(".ini");
        qDebug("%s",qPrintable(buf));

        ui->inipathEdit->setText(buf);

    } else {
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
    }
}

void MainWindow::on_importIniStatusBt_clicked()
{
     //settings = new QSettings("setting.ini",QSettings::IniFormat);

     //  QFile readfile(ui->apppathEdit->text());    //要读取的bin文件

     //  settings = new QSettings("setting.ini",QSettings::IniFormat);

       // QSettings settings(ui->apppathEdit->text());    //要读取的bin文件
        settings = new QSettings(ui->inipathEdit->text(),QSettings::IniFormat);

    //通过setValue函数将键值对放在相对于的节下面
       // 格式： settings->setValue("节名/键名","键值");

       //settings->setValue("student/name","zs");
      // settings->setValue("student/age","18");
      // settings->setValue("student/sex","male");

       // 通过value获取值
       // 格式： settings->value(""节名/键名"");
       //settings->value("student/name");
      // settings->value("student/age");
      // settings->value("student/sex");

       //qDebug() << settings->value("student/name");
      // qDebug() << settings->value("student/age");
      // qDebug() << settings->value("student/sex").toString();

       char path[][8] = {"joint0","joint2","joint4","joint6","joint8","joint10","joint12"};
       char path2[]="joint4";
       uint8_t jointID=0;;
       QString str;


       str=ui->jointIdCb->currentText();
       jointID = str.toInt()/2;

       //str.sprintf("torqueLimit=%lf",JointStatus.torqueLimit);
       //QString value;
       double angleMin=0;
       double torqueLimit=0.5;
       double value=0;
       //settings->value("joint4/angleMin");
       //settings->value("%s/angleMin",path);
       //settings->value("joint4/angleMax");
       //angleMin=settings->value("%s/angleMin",path).toDouble();

       value=0;
       str.sprintf("%s/angleMin",&path[jointID][0]);
       //value=settings->value("joint4/angleMin").toDouble();
        value =settings->value(QString(str)).toDouble();
       //value
       //ui->angleMinDsb->value()=angleMin;
        ui->angleMinDsb->setValue(value);

        //value=0;
        //value =settings->value("joint4/angleMax").toDouble();
        //ui->angleMaxDsb->setValue(value);

        value=0;
        str.sprintf("%s/angleMax",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->angleMaxDsb->setValue(value);

        // value=0;
        // str.sprintf("%s/angleMax",path2);
        // value =settings->value(QString(str)).toDouble();
        // ui->angleMaxDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/modelAngleMin").toDouble();
        str.sprintf("%s/modelAngleMin",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->modelAngleMinDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/modelAngleMax").toDouble();
        str.sprintf("%s/modelAngleMax",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->modelAngleMaxDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/initAngle").toDouble();
        str.sprintf("%s/initAngle",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->initAngleDsb->setValue(value);

        value=0;
        str.sprintf("%s/angle",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        //value=2;
        ui->angleDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/torqueLimit").toDouble();
        str.sprintf("%s/torqueLimit",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->torqueLimitDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/kp").toDouble();
        str.sprintf("%s/kp",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->kpDsb->setValue(value);

        value=0;
       // value =settings->value("joint4/ki").toDouble();
        str.sprintf("%s/ki",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->kiDsb->setValue(value);

        value=0;
        //value =settings->value("joint4/kv").toDouble();
        str.sprintf("%s/kv",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->kvDsb->setValue(value);

        value=0;
       // value =settings->value("joint4/kd").toDouble();
        str.sprintf("%s/kd",&path[jointID][0]);
        value =settings->value(QString(str)).toDouble();
        ui->kdDsb->setValue(value);

        bool b_value=false;
       // b_value =settings->value("joint4/inverted").toBool();
        str.sprintf("%s/inverted",&path[jointID][0]);
        b_value =settings->value(QString(str)).toBool();
        ui->invertedCb->setChecked(b_value);

       // b_value =settings->value("joint4/enable").toBool();
        str.sprintf("%s/enable",&path[jointID][0]);
        b_value =settings->value(QString(str)).toBool();
        ui->enableCb->setChecked(b_value);
}

void MainWindow::on_iniSaveBt_clicked()
{
    // QSettings settings(ui->apppathEdit->text());    //要读取的bin文件
     settings = new QSettings(ui->inipathEdit->text(),QSettings::IniFormat);

 //通过setValue函数将键值对放在相对于的节下面
    // 格式： settings->setValue("节名/键名","键值");

    //settings->setValue("student/name","zs");
   // settings->setValue("student/age","18");
   // settings->setValue("student/sex","male");

    // 通过value获取值
    // 格式： settings->value(""节名/键名"");
    //settings->value("student/name");
   // settings->value("student/age");
   // settings->value("student/sex");

    //qDebug() << settings->value("student/name");
   // qDebug() << settings->value("student/age");
   // qDebug() << settings->value("student/sex").toString();

    char path[][8] = {"joint0","joint2","joint4","joint6","joint8","joint10","joint12"};
    char path2[]="joint4";
    uint8_t jointID=0;;
    QString str;


    str=ui->jointIdCb->currentText();
    jointID = str.toInt()/2;

    //str.sprintf("torqueLimit=%lf",JointStatus.torqueLimit);
    //QString value;
    double angleMin=0;
    double torqueLimit=0.5;
    double value=0;
    //settings->value("joint4/angleMin");
    //settings->value("%s/angleMin",path);
    //settings->value("joint4/angleMax");
    //angleMin=settings->value("%s/angleMin",path).toDouble();

   // value=0;
   // str.sprintf("%s/angleMin",&path[jointID][0]);
    //value=settings->value("joint4/angleMin").toDouble();
    value=0;
    str.sprintf("%s/angleMin",&path[jointID][0]);
     value=ui->angleMinDsb->value();
     settings->setValue(QString(str), value);
    // value =settings->value(QString(str)).toDouble();
    //value
    //ui->angleMinDsb->value()=angleMin;
    // ui->angleMinDsb->setValue(value);

     //value=0;
     //value =settings->value("joint4/angleMax").toDouble();
     //ui->angleMaxDsb->setValue(value);

    // value=0;
    // str.sprintf("%s/angleMax",&path[jointID][0]);
    //value =settings->value(QString(str)).toDouble();
    // ui->angleMaxDsb->setValue(value);


     value=0;
     str.sprintf("%s/angleMax",&path[jointID][0]);
      value=ui->angleMaxDsb->value();
      settings->setValue(QString(str), value);

     // value=0;
     // str.sprintf("%s/angleMax",path2);
     // value =settings->value(QString(str)).toDouble();
     // ui->angleMaxDsb->setValue(value);

    // value=0;
     //value =settings->value("joint4/modelAngleMin").toDouble();
    // str.sprintf("%s/modelAngleMin",&path[jointID][0]);
    // value =settings->value(QString(str)).toDouble();
    // ui->modelAngleMinDsb->setValue(value);



     value=0;
     str.sprintf("%s/modelAngleMin",&path[jointID][0]);
      value=ui->modelAngleMinDsb->value();
      settings->setValue(QString(str), value);



     //value=0;
    // str.sprintf("%s/modelAngleMax",&path[jointID][0]);
    // value =settings->value(QString(str)).toDouble();
    // ui->modelAngleMaxDsb->setValue(value);


      value=0;
      str.sprintf("%s/modelAngleMax",&path[jointID][0]);
      value=ui->modelAngleMaxDsb->value();
      settings->setValue(QString(str), value);



    // value=0;
    // str.sprintf("%s/initAngle",&path[jointID][0]);
    // value =settings->value(QString(str)).toDouble();
    // ui->initAngleDsb->setValue(value);


     value=0;
     str.sprintf("%s/initAngle",&path[jointID][0]);
     value=ui->initAngleDsb->value();
     settings->setValue(QString(str), value);


   //  value=0;
   //  str.sprintf("%s/angle",&path[jointID][0]);
   //  value =settings->value(QString(str)).toDouble();
     //value=2;
   //  ui->angleDsb->setValue(value);


     value=0;
     str.sprintf("%s/angle",&path[jointID][0]);
     value=ui->angleDsb->value();
     settings->setValue(QString(str), value);


     //value=0;
     //str.sprintf("%s/torqueLimit",&path[jointID][0]);
    // value =settings->value(QString(str)).toDouble();
    // ui->torqueLimitDsb->setValue(value);


     value=0;
     str.sprintf("%s/torqueLimit",&path[jointID][0]);
     value=ui->torqueLimitDsb->value();
     settings->setValue(QString(str), value);


    // value=0;
     //value =settings->value("joint4/kp").toDouble();
    // str.sprintf("%s/kp",&path[jointID][0]);
    // value =settings->value(QString(str)).toDouble();
    // ui->kpDsb->setValue(value);

     value=0;
     str.sprintf("%s/kp",&path[jointID][0]);
     value=ui->kpDsb->value();
     settings->setValue(QString(str), value);



    // value=0;
    // value =settings->value("joint4/ki").toDouble();
     //str.sprintf("%s/ki",&path[jointID][0]);
     //value =settings->value(QString(str)).toDouble();
     //ui->kiDsb->setValue(value);


     value=0;
     str.sprintf("%s/ki",&path[jointID][0]);
     value=ui->kiDsb->value();
     settings->setValue(QString(str), value);


    // value=0;
     //value =settings->value("joint4/kv").toDouble();
    // str.sprintf("%s/kv",&path[jointID][0]);
   //  value =settings->value(QString(str)).toDouble();
   //  ui->kvDsb->setValue(value);


     value=0;
     str.sprintf("%s/kv",&path[jointID][0]);
     value=ui->kvDsb->value();
     settings->setValue(QString(str), value);


     //value=0;
    // value =settings->value("joint4/kd").toDouble();
     //str.sprintf("%s/kd",&path[jointID][0]);
     //value =settings->value(QString(str)).toDouble();
     //ui->kdDsb->setValue(value);

     value=0;
     str.sprintf("%s/kd",&path[jointID][0]);
     value=ui->kdDsb->value();
     settings->setValue(QString(str), value);

    // bool b_value=false;
    // b_value =settings->value("joint4/inverted").toBool();
   //  str.sprintf("%s/inverted",&path[jointID][0]);
   //  b_value =settings->value(QString(str)).toBool();
   //  ui->invertedCb->setChecked(b_value);


     bool b_value=false;
     str.sprintf("%s/inverted",&path[jointID][0]);
     if(ui->invertedCb->isChecked())
     {
         b_value=true;
     }
     else{
         b_value=false;
     }
     settings->setValue(QString(str), b_value);

    // b_value =settings->value("joint4/enable").toBool();
   //  str.sprintf("%s/enable",&path[jointID][0]);
   //  b_value =settings->value(QString(str)).toBool();
   //  ui->enableCb->setChecked(b_value);


     b_value=false;
     str.sprintf("%s/enable",&path[jointID][0]);
     if(ui->enableCb->isChecked())
     {
         b_value=true;
     }
     else{
         b_value=false;
     }
     settings->setValue(QString(str), b_value);

}



void MainWindow::on_writeJointStatus_5_clicked()
{

}





void MainWindow::on_instructions_triggered()
{
   // QDesktopServices::openUrl(QUrl("https://docs.qq.com/doc/DRENCQkVMRm9HTE1m?ADPUBNO=27255&ADSESSION=1666151976&ADTAG=CLIENT.QQ.5929_.0&ADUIN=1580723783&tdsourcetag=s_pcqq_send_grpfile&u=c5d89916e0794e2b8e93e8e746fdd6ff"));
   // QDesktopServices::openUrl(QUrl("https://pan.baidu.com/s/1e5PZKQ_tphVfA2Pt378HgA?pwd=nt6k"));
   // QDesktopServices::openUrl(QUrl("https://gitee.com/xiao-xiaoming95/election-bot-firmware/raw/master/ElectronBot-fw/app/bin/app.bin"));
    //QDesktopServices::openUrl(QUrl("https://docs.qq.com/doc/DRENCQkVMRm9HTE1m?ADPUBNO=27255&ADSESSION=1667225696&ADTAG=CLIENT.QQ.5929_.0&ADUIN=1580723783&tdsourcetag=s_pcqq_send_grpfile&u=c5d89916e0794e2b8e93e8e746fdd6ff"));
    QDesktopServices::openUrl(QUrl("https://docs.qq.com/doc/DRHRuTFhYaWJETU5s?groupUin=VRFNT55LSO%252BEJoEkMPpVUw%253D%253D&ADUIN=1580723783&ADSESSION=1667377579&ADTAG=CLIENT.QQ.5929_.0&ADPUBNO=27255&jumpuin=1580723783&u=c5d89916e0794e2b8e93e8e746fdd6ff"));
}



void MainWindow::on_finished()
{//网络响应结束
    QFileInfo fileInfo;
    fileInfo.setFile(downloadedFile->fileName());

    downloadedFile->close();
    delete downloadedFile;
    downloadedFile = Q_NULLPTR;

    reply->deleteLater(); //
    reply = Q_NULLPTR;


    QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));





   // QFile readfile(ui->apppathEdit->text());    //要读取的bin文件

     QFile readfile(fileInfo.absoluteFilePath());    //要读取的bin文件
    QFileInfo read_file_info(readfile);         //要读取的bin文件的信息
    if (!readfile.open(QIODevice::ReadOnly)) {  //只读方式打开bin文件
        QMessageBox::warning(this, tr("Read File"),
            tr("Cannot open file:\n%1").arg(ui->apppathEdit->text()));
        return;
    }
    QDataStream BinFileData(&readfile);  //将刚刚创建的 file 对象的指针传递给一个QDataStream实例 BinFileData
    char *pBuff = new char[1024*1024];    //修改为1M大小，以便读取APP.bin时复用。修改为动态分配？
    BinFileData.readRawData(pBuff,static_cast<int>(read_file_info.size()));//要读取bin文件的大小
    qDebug() << "IAP.bin size is " << static_cast<int>(read_file_info.size());    //打印文件长度


   // QString appsize;
   // appsize.sprintf("0x%08llx",read_file_info.size());
   // ui->appsizeEdit->setText(appsize);
   // ui->appoffsizeEdit->setText(tr("0x00010000"));  //默认偏移量


    IAP_DataAllArray.clear();
    IAP_DataAllArray = QByteArray(pBuff,static_cast<int>(read_file_info.size()));

   // QString recvText;
   // recvText = byteArray2Text(IAP_DataAllArray);
   // printfDbugLog(recvText);

  //  QByteArray sendData;
  //  uint8_t testBuf[1024]={0};
//    for(uint16_t i=0;i<1024;i++)
//    {
 //       testBuf[i]=i;
 //   }

    iap.codeLen=read_file_info.size();
    IAP_init();

    showBinFileInfo();

   // sendData=IAP();
   // sendData=QByteArray((char*)testBuf,1024);
   // recvText = byteArray2Text(sendData);
   // ui->serialPortLogEdit->appendPlainText(recvText);

    //serialPort->write(sendData.data(),sendData.length());
   // serialPort->write(sendData.data(),128);

   // timer->start(1000);

    readfile.close();


}

void MainWindow::on_readyRead()
{//读取下载的数据
    downloadedFile->write(reply->readAll());
}

void MainWindow::on_downloadProgress(qint64 bytesRead, qint64 totalBytes)
{//下载进程
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}

void MainWindow::on_btnDefaultPath_clicked()
{//缺省路径  按钮

}

void MainWindow::on_btnDownload_clicked()
{//开始下载 按钮

}

void MainWindow::on_editURL_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
}




void MainWindow::on_pushButton_2_clicked()
{
    //QString textfile=QFileDialog::getOpenFileName(this,tr("文件"),addrStr,tr("text(*.txt)"));
   // QFile file("https://gitee.com/xiao-xiaoming95/election-bot-firmware/blob/master/ElectronBot-fw/app/bin/setting.ini");
   // file.open(QIODevice::ReadOnly|QIODevice::Text);
   // printfLog(file.readAll());

   // QDesktopServices::openUrl(QUrl("https://gitee.com/xiao-xiaoming95/election-bot-firmware/blob/master/ElectronBot-fw/app/bin/setting.ini", QUrl::TolerantMode));

  //QString runPath = QCoreApplication::applicationDirPath();
    //QUrl file_name = QFileDialog::getOpenFileUrl(this,QStringLiteral("选择路径"),runPath,"Text Files(*.txt *.png)",nullptr,QFileDialog::DontUseCustomDirectoryIcons);
  //  QUrl file_name = QFileDialog::getOpenFileUrl(this,"https://gitee.com/xiao-xiaoming95/election-bot-firmware/blob/master/ElectronBot-fw/app/bin/setting.ini",runPath,"Text Files(*.txt *.png)",nullptr,QFileDialog::DontUseCustomDirectoryIcons);



    //QFile readfile(ui->apppathEdit->text());    //要读取的bin文件

    /*QFile readfile(ui->apppathEdit->text());    //要读取的bin文件
    QFileInfo read_file_info(readfile);         //要读取的bin文件的信息
    if (!readfile.open(QIODevice::ReadOnly)) {  //只读方式打开bin文件
        QMessageBox::warning(this, tr("Read File"),
            tr("Cannot open file:\n%1").arg(ui->apppathEdit->text()));
        return;
    }
    QDataStream BinFileData(&readfile);  //将刚刚创建的 file 对象的指针传递给一个QDataStream实例 BinFileData
    char *pBuff = new char[1024*1024];    //修改为1M大小，以便读取APP.bin时复用。修改为动态分配？
    BinFileData.readRawData(pBuff,static_cast<int>(read_file_info.size()));//要读取bin文件的大小
    qDebug() << "IAP.bin size is " << static_cast<int>(read_file_info.size());    //打印文件长度

    IAP_DataAllArray.clear();
    IAP_DataAllArray = QByteArray(pBuff,static_cast<int>(read_file_info.size()));

    QString recvText;
    recvText = byteArray2Text(IAP_DataAllArray);
    ui->serialPortLogEdit->appendPlainText(recvText);

    QByteArray sendData;
    uint8_t testBuf[1024]={0};
    for(uint16_t i=0;i<1024;i++)
    {
        testBuf[i]=i;
    }

    iap.codeLen=read_file_info.size();
    IAP_init();

   // sendData=IAP();
   // sendData=QByteArray((char*)testBuf,1024);
   // recvText = byteArray2Text(sendData);
   // ui->serialPortLogEdit->appendPlainText(recvText);

    //serialPort->write(sendData.data(),sendData.length());
   // serialPort->write(sendData.data(),128);

   // timer->start(1000);

    readfile.close();*/


   /* QString urlSpec = ui->editURL->text().trimmed();
    if (urlSpec.isEmpty())
    {
        QMessageBox::information(this, "错误",
          "请指定需要下载的URL");
        return;
    }*/
    QString urlSpec;
   // urlSpec.sprintf("https://gitee.com/xiao-xiaoming95/election-bot-firmware/raw/master/ElectronBot-fw/app/bin/setting.ini");

    urlSpec.sprintf("https://gitee.com/xiao-xiaoming95/election-bot-firmware/raw/master/ElectronBot-fw/app/bin/app.bin");
    //urlSpec.sprintf("https://github.com/maker-community/ElectronBot-fw/raw/master/Release/ElectionBot-fw/app.bin");

    if(ui->useCustomWebAddressRb->isChecked())
    {
        urlSpec =  ui->appUrlPathEdit->text();
    }

    QUrl newUrl = QUrl::fromUserInput(urlSpec);//URL地址
    if (!newUrl.isValid())
    {
        QMessageBox::information(this, "错误",
          QString("无效URL: %1 \n 错误信息: %2").arg(urlSpec, newUrl.errorString()));
        return;
    }


    QString  curPath=QDir::currentPath();
    QDir    dir(curPath);

    QString  sub="temp";
    dir.mkdir(sub);

    //ui->editPath->setText(curPath+"/"+sub+"/");

    //QString tempDir =ui->editPath->text().trimmed();//临时目录

    QString tempDir ={curPath+"/"+sub+"/"};//临时目录
    if (tempDir.isEmpty())
    {
        QMessageBox::information(this, tr("错误"), "请指定保存下载文件的目录");
        return;
    }

    QString fullFileName =tempDir+newUrl.fileName(); //文件名

    if (QFile::exists(fullFileName))
        QFile::remove(fullFileName);

    downloadedFile =new QFile(fullFileName);//创建临时文件
    if (!downloadedFile->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("错误"),"临时文件打开错误");
        return;
    }



//发送玩过请求，创建网络响应
    reply = networkManager.get(QNetworkRequest(newUrl));
    connect(reply, SIGNAL(finished()), this, SLOT(on_finished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(on_downloadProgress(qint64,qint64)));
}


void MainWindow::on_interruptDownloadBt_clicked()
{
    //iap.waitPacketResponseFlag=false;
    memset((uint8_t*)&iap,0,sizeof (iap));
}


void MainWindow::printfLog(QString str)
{
    //printfLog(str);
    ui->mainLogEdit->appendPlainText(str);
}


void MainWindow::printfDbugLog(QString str)
{
    if(ui->showDebugLogCb->isChecked())
    {
        ui->mainLogEdit->appendPlainText(str);
    }
//#ifdef DbugLog
   // printfLog(str);
   // printfLog(str);
//#endif
}

void MainWindow::on_clearAllBt_clicked()
{
    cleanJointStatusCb();
    ui->serialPortLogEdit->clear();
     ui->USB_Recieve_Data->clear();
      ui->mainLogEdit->clear();
     //  ui->addressLogEdit->clear();
}

/*
int MainWindow::VersionCompare(const QString &strVer1, const QString &strVer2)
{
    if(!strVer1.compare(strVer2))
    {
        return 0;
    }

    QStringList verList1 = strVer1.split(".");
    QStringList verList2 = strVer2.split(".");

    int listCount1 = verList1.count();
    int listCount2 = verList2.count();
    int nCount = listCount1 > listCount2 ? listCount2 : listCount1;

    for(int i = 0; i < nCount; ++i)
    {
        bool bOK1 = false;//判断是否转换成成功
        bool bOK2 = false;
        int num1 = verList1[i].toInt(&bOK1);
        int num2 = verList2[i].toInt(&bOK2);
        if(!bOK1 || !bOK2)//转换失败返回0
        {
            return -2;
        }
        if(num1 == num2)
        {
            continue;
        }
        if(num1 > num2)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}

void MainWindow::SlotVersionCompare()
{
    QString strVer1 = ui->lineEdit_Ver1->text();
    QString strVer2 = ui->lineEdit_Ver2->text();
    int nResult = VersionCompare(strVer1, strVer2);
    QString strResult = tr("result: ");
    switch (nResult)
    {
        case -2:
            strResult = QString("版本号格式错误");
            break;
        case -1:
            strResult += QString("%1 小于 %2").arg(strVer1).arg(strVer2);
            break;
        case 0:
            strResult += QString("%1 等于 %2").arg(strVer1).arg(strVer2);
            break;
        case 1:
            strResult += QString("%1 大于 %2").arg(strVer1).arg(strVer2);
            break;
        default:
            break;
    }

    ui->label_Result->setText(strResult);
}*/



int MainWindow::VersionCompareStr(const QString &strVer1, const QString &strVer2)
{
    if(!strVer1.compare(strVer2))
    {
        return 0;
    }

    QStringList verList1 = strVer1.split(".");
    QStringList verList2 = strVer2.split(".");

    int listCount1 = verList1.count();
    int listCount2 = verList2.count();
    int nCount = listCount1 > listCount2 ? listCount2 : listCount1;

    for(int i = 0; i < nCount; ++i)
    {
        bool bOK1 = false;//判断是否转换成成功
        bool bOK2 = false;
        int num1 = verList1[i].toInt(&bOK1);
        int num2 = verList2[i].toInt(&bOK2);
        if(!bOK1 || !bOK2)//转换失败返回0
        {
            return -2;
        }
        if(num1 == num2)
        {
            continue;
        }
        if(num1 > num2)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}

void MainWindow::VersionCompare()
{
    QString strVer1 = {"1.0.0.0"};
    QString strVer2 = {"1.0.0.1"};
    int nResult = VersionCompareStr(strVer1, strVer2);
    QString strResult = tr("result: ");
    switch (nResult)
    {
        case -2:
            strResult = QString("版本号格式错误");
            break;
        case -1:
            strResult += QString("%1 小于 %2").arg(strVer1).arg(strVer2);
            break;
        case 0:
            strResult += QString("%1 等于 %2").arg(strVer1).arg(strVer2);
            break;
        case 1:
            strResult += QString("%1 大于 %2").arg(strVer1).arg(strVer2);
            break;
        default:
            break;
    }

    //ui->label_Result->setText(strResult);
    printfLog(strResult);
}

void MainWindow::showBinFileInfo()
{
    QString appsize;
    appsize.sprintf("0x%08lx",iap.codeLen);
    ui->appsizeEdit->setText(appsize);
    ui->appoffsizeEdit->setText(tr("0x00010000"));  //默认偏移量

    QString str;
    uint32_t crc32Value=0xffffffff;
    crc32Value=0xffffffff;

    crc32Value=crc32(crc32Value,(uint8_t*)iap.data,iap.codeLen);
    //crc32Value=crc32(crc32Value,(uint8_t*)iap.data,IAP_PACKET_MAX_DATA_LEN*2);
    str.sprintf("0x%08x",crc32Value);
    ui->crc32ValueEdit->setText(str);
    printfLog(str);
}

void MainWindow::on_readJointStatusBt_clicked()
{

}



void MainWindow::on_SourceCode_triggered()
{
     QDesktopServices::openUrl(QUrl("https://github.com/maker-community/ElectronBot-fw"));
}

void MainWindow::on_Guide_triggered()
{
      QDesktopServices::openUrl(QUrl("https://docs.qq.com/doc/DRENCQkVMRm9HTE1m?ADPUBNO=27255&ADSESSION=1667225696&ADTAG=CLIENT.QQ.5929_.0&ADUIN=1580723783&tdsourcetag=s_pcqq_send_grpfile&u=c5d89916e0794e2b8e93e8e746fdd6ff"));
}

void MainWindow::on_version_triggered()
{
    QString str;
    str.sprintf("%s\r\r",APP_VERSION);
    QMessageBox::information(this, "版本", str
            );
}


void MainWindow::cleanJointStatusCb(void)
{
    ui->joint2Cb->setChecked(false);
    ui->joint4Cb->setChecked(false);
    ui->joint6Cb->setChecked(false);
    ui->joint8Cb->setChecked(false);
    ui->joint10Cb->setChecked(false);
    ui->joint12Cb->setChecked(false);

    ui->mpu6050Cb->setChecked(false);
    ui->paj7620Cb->setChecked(false);
}
