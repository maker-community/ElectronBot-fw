#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>

#include    <QNetworkAccessManager>
#include    <QNetworkReply>
#include    <QFile>
#include    <QUrl>

//#include <QDateTime>

// 引入配置文件的头文件
#include <QtCore/QCoreApplication>
#include <QSettings>

#define Rx_Lenth 32*1024   // USB 接收缓冲区大小


//class MyThread : public QThread {
//    public:
//        virtual void run();
//    };


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     QSerialPort *serialPort;
     QTimer* timer;
     // 配置文件对象
     QSettings *settings;
    void uiprintf(QString);

    QNetworkAccessManager networkManager;//网络管理
    QNetworkReply *reply;   //网络响应
    QFile *downloadedFile;//下载保存的临时文件

    void printfLog(QString str);
    void printfDbugLog(QString str);

private:
    Ui::MainWindow *ui;
    QSerialPort *serialport;
    QTimer timer_Send;   // 周期发送
    QTimer m_pTimer;     // 定时处理USB接收

//    void        RecordDisplay();
//    long        Record_Seek(long Item, int direction);


private slots:

        void handleTimeout(void);

    void on_pushButton_USB_Open_clicked();
    void on_USB_clicked();
    void on_Clear_clicked();

    void on_LED_ON_CMD_clicked();

    void on_LED_OFF_CMD_clicked();

    void on_Data_Send_clicked();

    void on_clear_send_data_clicked();

    void on_checkBox_send_toggled(bool checked);

    void on_pushButton_recv_save_clicked();
    void sendData(void);
    void aboutclick();

    void on_about_triggered();

    void on_function_triggered();

    void on_writeJointStatus_clicked();

    void on_openBt_clicked();

    void on_closeBt_clicked();

    void on_sendBt_clicked();

    void on_clearBt_clicked();

    void serialPortReadRead_Slot();

    void on_writeJointIdBt_clicked();

    void on_writeElectionBotIdBt_clicked();

    void on_writeJointStatus_2_clicked();

    void on_appBt_clicked();

    void on_pushButton_clicked();

    void timeoutSlot();

    //void QTimer::start(std::chrono::milliseconds msec);

    void on_iniBt_clicked();

    void on_importIniStatusBt_clicked();

    void on_iniSaveBt_clicked();
    QByteArray ReadRobotSlavesConnectionStatusProtocolBuf();
    void UpdateRobotSlavesConnectionStatus();
    void DealSerialPortData();


    void on_resetMasterBt_clicked();

    void on_writeJointStatus_5_clicked();

    void on_jointRotationTestBt_clicked();

    void on_jointRotationTestCloseBt_clicked();

    void on_instructions_triggered();

    void on_pushButton_2_clicked();




//http----------------------------------------------
    void on_finished();
    void on_readyRead();
    void on_downloadProgress(qint64 bytesRead, qint64 totalBytes);


    void on_btnDefaultPath_clicked();

    void on_btnDownload_clicked();

    void on_editURL_textChanged(const QString &arg1);
//-----------------------------------------------

// VersionCompare--------------------------------
    int VersionCompareStr(const QString &strVer1, const QString &strVer2);
    void VersionCompare();

//__________________________

    void on_interruptDownloadBt_clicked();


    void on_clearAllBt_clicked();

    void on_readJointStatusBt_clicked();


    void on_SourceCode_triggered();

    void on_Guide_triggered();

    void on_version_triggered();

private:
//    void initComm();
//    bool isOpen();
//    void  SD_Data_Read(int blcok_num,int Data_Len);
//    void  SD_Data_Read_To_Ram(void);

    void Timer_Start(int TIMER_TIMEOUT);
    void closeEvent(QCloseEvent *event);
    void USB_recieve_process(void);
    QByteArray writeJointStatusToBuf(void);
    QByteArray writeBinPacketToProtocolBuf(QByteArray BinPacketData);
    QByteArray iapframe(void);
    QString IAP_init();
    QByteArray IAP();
    void showBinFileInfo();
};

#endif // MAINWINDOW_H
