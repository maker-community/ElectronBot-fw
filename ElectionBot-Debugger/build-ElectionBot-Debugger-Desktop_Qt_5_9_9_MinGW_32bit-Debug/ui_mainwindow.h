/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *function;
    QAction *about;
    QAction *instructions;
    QAction *SourceCode;
    QWidget *centralWidget;
    QGroupBox *groupBox_8;
    QTextBrowser *USB_Send_Data;
    QPlainTextEdit *mainLogEdit;
    QPlainTextEdit *USB_Recieve_Data;
    QCheckBox *showDebugLogCb;
    QGroupBox *groupBox;
    QPushButton *pushButton_USB_Open;
    QLineEdit *PID_Data;
    QLineEdit *VID_Data;
    QLabel *label_22;
    QLabel *label_23;
    QGroupBox *groupBox_2;
    QPushButton *Clear;
    QRadioButton *radioButton_recv_ascii;
    QPushButton *pushButton_recv_save;
    QRadioButton *radioButton_recv_unit16;
    QGroupBox *groupBox_3;
    QRadioButton *radioButton_send_ascii;
    QRadioButton *radioButton_send_unit16;
    QLineEdit *lineEdit_send;
    QLabel *label_7;
    QCheckBox *checkBox_send;
    QPushButton *clear_send_data;
    QPushButton *Data_Send;
    QGroupBox *groupBox_5;
    QLabel *label_14;
    QComboBox *jointIdCb;
    QLineEdit *writeElectionBotIdEdit;
    QPushButton *writeJointStatus;
    QPushButton *writeJointIdBt;
    QLineEdit *electionBotIdEdit;
    QPushButton *writeElectionBotIdBt;
    QLabel *label;
    QComboBox *writeJointIdCb;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_4;
    QDoubleSpinBox *initAngleDsb;
    QLabel *label_8;
    QDoubleSpinBox *kvDsb;
    QLabel *label_12;
    QDoubleSpinBox *angleDsb;
    QLabel *label_9;
    QDoubleSpinBox *kiDsb;
    QLabel *label_11;
    QCheckBox *invertedCb;
    QDoubleSpinBox *kdDsb;
    QLabel *label_13;
    QCheckBox *enableCb;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout;
    QDoubleSpinBox *angleMinDsb;
    QLabel *label_3;
    QDoubleSpinBox *modelAngleMaxDsb;
    QLabel *label_6;
    QDoubleSpinBox *angleMaxDsb;
    QLabel *label_4;
    QDoubleSpinBox *modelAngleMinDsb;
    QLabel *label_5;
    QGroupBox *groupBox_13;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *torqueLimitDsb;
    QLabel *label_2;
    QDoubleSpinBox *kpDsb;
    QLabel *label_10;
    QGroupBox *groupBox_14;
    QGridLayout *gridLayout_5;
    QLineEdit *inipathEdit;
    QPushButton *iniBt;
    QPushButton *iniSaveBt;
    QPushButton *readJointStatusBt;
    QLabel *label_39;
    QLabel *label_40;
    QPushButton *writeJointStatus_2;
    QPushButton *importIniStatusBt;
    QGroupBox *groupBox_7;
    QPushButton *openBt;
    QLabel *label_16;
    QLineEdit *sendEdit;
    QLabel *label_15;
    QLabel *label_17;
    QLabel *label_18;
    QComboBox *checkBitCb;
    QPushButton *closeBt;
    QComboBox *stopBitCb;
    QPushButton *clearBt;
    QComboBox *baudRateCb;
    QLabel *label_19;
    QComboBox *dataBitCb;
    QComboBox *serialPortCb;
    QPushButton *sendBt;
    QPlainTextEdit *serialPortLogEdit;
    QCheckBox *timeStampCb;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_2;
    QCheckBox *joint2Cb;
    QCheckBox *joint4Cb;
    QCheckBox *joint6Cb;
    QCheckBox *joint8Cb;
    QCheckBox *joint10Cb;
    QCheckBox *joint12Cb;
    QCheckBox *mpu6050Cb;
    QCheckBox *paj7620Cb;
    QGroupBox *groupBox_15;
    QDoubleSpinBox *rotationTestStartAngleDsb;
    QLabel *label_41;
    QDoubleSpinBox *rotationTestEndAngleDsb;
    QLabel *label_42;
    QPushButton *jointRotationTestBt;
    QPushButton *jointRotationTestCloseBt;
    QGroupBox *groupBox_4;
    QGroupBox *groupBox_6;
    QPushButton *appBt;
    QLineEdit *apppathEdit;
    QLabel *label_21;
    QPushButton *pushButton;
    QGroupBox *groupBox_10;
    QLabel *label_25;
    QLineEdit *appUrlPathEdit;
    QGroupBox *groupBox_19;
    QRadioButton *useDefaultWebAddressRb;
    QRadioButton *useCustomWebAddressRb;
    QPushButton *pushButton_2;
    QLabel *label_27;
    QProgressBar *progressBar;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_20;
    QLineEdit *appsizeEdit;
    QLabel *label_24;
    QLineEdit *appoffsizeEdit;
    QLabel *label_30;
    QLineEdit *crc32ValueEdit;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QPushButton *interruptDownloadBt;
    QLabel *label_29;
    QProgressBar *iapProgressBar;
    QPushButton *resetMasterBt;
    QPushButton *jointRotationTestBt_2;
    QComboBox *writeJointIdCb_2;
    QPushButton *jointRotationTestBt_3;
    QComboBox *writeJointIdCb_3;
    QGroupBox *groupBox_20;
    QCheckBox *joint2Cb_2;
    QCheckBox *joint2Cb_3;
    QPushButton *clearAllBt;
    QPushButton *pushButton_recv_clear_4;
    QMenuBar *menuBar;
    QMenu *menu;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1227, 776);
        MainWindow->setToolTipDuration(-1);
        function = new QAction(MainWindow);
        function->setObjectName(QStringLiteral("function"));
        about = new QAction(MainWindow);
        about->setObjectName(QStringLiteral("about"));
        instructions = new QAction(MainWindow);
        instructions->setObjectName(QStringLiteral("instructions"));
        SourceCode = new QAction(MainWindow);
        SourceCode->setObjectName(QStringLiteral("SourceCode"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox_8 = new QGroupBox(centralWidget);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(120, 10, 361, 461));
        USB_Send_Data = new QTextBrowser(groupBox_8);
        USB_Send_Data->setObjectName(QStringLiteral("USB_Send_Data"));
        USB_Send_Data->setEnabled(false);
        USB_Send_Data->setGeometry(QRect(10, 410, 341, 31));
        USB_Send_Data->setReadOnly(false);
        mainLogEdit = new QPlainTextEdit(groupBox_8);
        mainLogEdit->setObjectName(QStringLiteral("mainLogEdit"));
        mainLogEdit->setEnabled(true);
        mainLogEdit->setGeometry(QRect(10, 20, 341, 271));
        USB_Recieve_Data = new QPlainTextEdit(groupBox_8);
        USB_Recieve_Data->setObjectName(QStringLiteral("USB_Recieve_Data"));
        USB_Recieve_Data->setEnabled(false);
        USB_Recieve_Data->setGeometry(QRect(10, 320, 341, 81));
        showDebugLogCb = new QCheckBox(groupBox_8);
        showDebugLogCb->setObjectName(QStringLiteral("showDebugLogCb"));
        showDebugLogCb->setGeometry(QRect(10, 290, 101, 21));
        showDebugLogCb->setChecked(false);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setEnabled(false);
        groupBox->setGeometry(QRect(10, 20, 111, 121));
        pushButton_USB_Open = new QPushButton(groupBox);
        pushButton_USB_Open->setObjectName(QStringLiteral("pushButton_USB_Open"));
        pushButton_USB_Open->setGeometry(QRect(10, 80, 91, 23));
        PID_Data = new QLineEdit(groupBox);
        PID_Data->setObjectName(QStringLiteral("PID_Data"));
        PID_Data->setGeometry(QRect(50, 50, 51, 20));
        VID_Data = new QLineEdit(groupBox);
        VID_Data->setObjectName(QStringLiteral("VID_Data"));
        VID_Data->setGeometry(QRect(50, 24, 51, 20));
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(14, 50, 30, 20));
        label_23 = new QLabel(groupBox);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(14, 24, 30, 20));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setEnabled(false);
        groupBox_2->setGeometry(QRect(10, 140, 111, 121));
        Clear = new QPushButton(groupBox_2);
        Clear->setObjectName(QStringLiteral("Clear"));
        Clear->setGeometry(QRect(10, 0, 71, 23));
        radioButton_recv_ascii = new QRadioButton(groupBox_2);
        radioButton_recv_ascii->setObjectName(QStringLiteral("radioButton_recv_ascii"));
        radioButton_recv_ascii->setGeometry(QRect(10, 70, 100, 16));
        radioButton_recv_ascii->setMinimumSize(QSize(100, 0));
        pushButton_recv_save = new QPushButton(groupBox_2);
        pushButton_recv_save->setObjectName(QStringLiteral("pushButton_recv_save"));
        pushButton_recv_save->setGeometry(QRect(10, 20, 71, 21));
        radioButton_recv_unit16 = new QRadioButton(groupBox_2);
        radioButton_recv_unit16->setObjectName(QStringLiteral("radioButton_recv_unit16"));
        radioButton_recv_unit16->setGeometry(QRect(10, 48, 100, 16));
        radioButton_recv_unit16->setChecked(true);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setEnabled(false);
        groupBox_3->setGeometry(QRect(10, 260, 111, 151));
        radioButton_send_ascii = new QRadioButton(groupBox_3);
        radioButton_send_ascii->setObjectName(QStringLiteral("radioButton_send_ascii"));
        radioButton_send_ascii->setGeometry(QRect(10, 30, 100, 16));
        radioButton_send_ascii->setMinimumSize(QSize(100, 0));
        radioButton_send_ascii->setMaximumSize(QSize(100, 16777215));
        radioButton_send_unit16 = new QRadioButton(groupBox_3);
        radioButton_send_unit16->setObjectName(QStringLiteral("radioButton_send_unit16"));
        radioButton_send_unit16->setGeometry(QRect(10, 10, 100, 16));
        radioButton_send_unit16->setChecked(true);
        lineEdit_send = new QLineEdit(groupBox_3);
        lineEdit_send->setObjectName(QStringLiteral("lineEdit_send"));
        lineEdit_send->setGeometry(QRect(10, 50, 60, 20));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit_send->sizePolicy().hasHeightForWidth());
        lineEdit_send->setSizePolicy(sizePolicy);
        lineEdit_send->setMaximumSize(QSize(60, 16777215));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(80, 50, 32, 20));
        checkBox_send = new QCheckBox(groupBox_3);
        checkBox_send->setObjectName(QStringLiteral("checkBox_send"));
        checkBox_send->setGeometry(QRect(10, 80, 100, 16));
        clear_send_data = new QPushButton(groupBox_3);
        clear_send_data->setObjectName(QStringLiteral("clear_send_data"));
        clear_send_data->setGeometry(QRect(10, 100, 71, 23));
        Data_Send = new QPushButton(groupBox_3);
        Data_Send->setObjectName(QStringLiteral("Data_Send"));
        Data_Send->setGeometry(QRect(10, 120, 71, 23));
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(490, 130, 321, 561));
        label_14 = new QLabel(groupBox_5);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(90, 20, 54, 12));
        jointIdCb = new QComboBox(groupBox_5);
        jointIdCb->setObjectName(QStringLiteral("jointIdCb"));
        jointIdCb->setGeometry(QRect(150, 20, 69, 22));
        jointIdCb->setModelColumn(0);
        writeElectionBotIdEdit = new QLineEdit(groupBox_5);
        writeElectionBotIdEdit->setObjectName(QStringLiteral("writeElectionBotIdEdit"));
        writeElectionBotIdEdit->setGeometry(QRect(10, 50, 71, 20));
        writeJointStatus = new QPushButton(groupBox_5);
        writeJointStatus->setObjectName(QStringLiteral("writeJointStatus"));
        writeJointStatus->setGeometry(QRect(140, 120, 151, 23));
        writeJointIdBt = new QPushButton(groupBox_5);
        writeJointIdBt->setObjectName(QStringLiteral("writeJointIdBt"));
        writeJointIdBt->setGeometry(QRect(140, 90, 111, 23));
        electionBotIdEdit = new QLineEdit(groupBox_5);
        electionBotIdEdit->setObjectName(QStringLiteral("electionBotIdEdit"));
        electionBotIdEdit->setGeometry(QRect(10, 20, 71, 20));
        writeElectionBotIdBt = new QPushButton(groupBox_5);
        writeElectionBotIdBt->setObjectName(QStringLiteral("writeElectionBotIdBt"));
        writeElectionBotIdBt->setEnabled(false);
        writeElectionBotIdBt->setGeometry(QRect(10, 90, 111, 23));
        label = new QLabel(groupBox_5);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(220, 20, 54, 12));
        writeJointIdCb = new QComboBox(groupBox_5);
        writeJointIdCb->setObjectName(QStringLiteral("writeJointIdCb"));
        writeJointIdCb->setGeometry(QRect(150, 50, 69, 22));
        groupBox_9 = new QGroupBox(groupBox_5);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setGeometry(QRect(0, 320, 229, 122));
        gridLayout_4 = new QGridLayout(groupBox_9);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        initAngleDsb = new QDoubleSpinBox(groupBox_9);
        initAngleDsb->setObjectName(QStringLiteral("initAngleDsb"));
        initAngleDsb->setDecimals(1);
        initAngleDsb->setMinimum(-180);
        initAngleDsb->setMaximum(180);
        initAngleDsb->setSingleStep(1);
        initAngleDsb->setValue(0);

        gridLayout_4->addWidget(initAngleDsb, 0, 0, 1, 1);

        label_8 = new QLabel(groupBox_9);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_4->addWidget(label_8, 0, 1, 1, 1);

        kvDsb = new QDoubleSpinBox(groupBox_9);
        kvDsb->setObjectName(QStringLiteral("kvDsb"));
        kvDsb->setDecimals(1);
        kvDsb->setMaximum(100);
        kvDsb->setSingleStep(1);
        kvDsb->setValue(0);

        gridLayout_4->addWidget(kvDsb, 0, 2, 1, 1);

        label_12 = new QLabel(groupBox_9);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_4->addWidget(label_12, 0, 3, 1, 1);

        angleDsb = new QDoubleSpinBox(groupBox_9);
        angleDsb->setObjectName(QStringLiteral("angleDsb"));
        angleDsb->setDecimals(1);
        angleDsb->setMinimum(-180);
        angleDsb->setMaximum(180);
        angleDsb->setSingleStep(1);
        angleDsb->setValue(0);

        gridLayout_4->addWidget(angleDsb, 1, 0, 1, 1);

        label_9 = new QLabel(groupBox_9);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_4->addWidget(label_9, 1, 1, 1, 1);

        kiDsb = new QDoubleSpinBox(groupBox_9);
        kiDsb->setObjectName(QStringLiteral("kiDsb"));
        kiDsb->setDecimals(1);
        kiDsb->setMaximum(100);
        kiDsb->setSingleStep(1);
        kiDsb->setValue(0);

        gridLayout_4->addWidget(kiDsb, 1, 2, 1, 1);

        label_11 = new QLabel(groupBox_9);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_4->addWidget(label_11, 1, 3, 1, 1);

        invertedCb = new QCheckBox(groupBox_9);
        invertedCb->setObjectName(QStringLiteral("invertedCb"));
        invertedCb->setCheckable(true);
        invertedCb->setChecked(false);
        invertedCb->setTristate(false);

        gridLayout_4->addWidget(invertedCb, 2, 0, 1, 1);

        kdDsb = new QDoubleSpinBox(groupBox_9);
        kdDsb->setObjectName(QStringLiteral("kdDsb"));
        kdDsb->setDecimals(1);
        kdDsb->setMaximum(100);
        kdDsb->setSingleStep(1);
        kdDsb->setValue(50);

        gridLayout_4->addWidget(kdDsb, 2, 2, 2, 1);

        label_13 = new QLabel(groupBox_9);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_4->addWidget(label_13, 2, 3, 2, 1);

        enableCb = new QCheckBox(groupBox_9);
        enableCb->setObjectName(QStringLiteral("enableCb"));

        gridLayout_4->addWidget(enableCb, 3, 0, 1, 1);

        groupBox_12 = new QGroupBox(groupBox_5);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        groupBox_12->setGeometry(QRect(0, 180, 290, 78));
        gridLayout = new QGridLayout(groupBox_12);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        angleMinDsb = new QDoubleSpinBox(groupBox_12);
        angleMinDsb->setObjectName(QStringLiteral("angleMinDsb"));
        angleMinDsb->setDecimals(1);
        angleMinDsb->setMinimum(-180);
        angleMinDsb->setMaximum(180);
        angleMinDsb->setSingleStep(1);
        angleMinDsb->setValue(10);

        gridLayout->addWidget(angleMinDsb, 0, 0, 1, 1);

        label_3 = new QLabel(groupBox_12);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 1, 1, 1);

        modelAngleMaxDsb = new QDoubleSpinBox(groupBox_12);
        modelAngleMaxDsb->setObjectName(QStringLiteral("modelAngleMaxDsb"));
        modelAngleMaxDsb->setDecimals(1);
        modelAngleMaxDsb->setMinimum(-180);
        modelAngleMaxDsb->setMaximum(180);
        modelAngleMaxDsb->setSingleStep(1);
        modelAngleMaxDsb->setValue(30);

        gridLayout->addWidget(modelAngleMaxDsb, 0, 2, 1, 1);

        label_6 = new QLabel(groupBox_12);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 0, 3, 1, 1);

        angleMaxDsb = new QDoubleSpinBox(groupBox_12);
        angleMaxDsb->setObjectName(QStringLiteral("angleMaxDsb"));
        angleMaxDsb->setDecimals(2);
        angleMaxDsb->setMinimum(-180);
        angleMaxDsb->setMaximum(180);
        angleMaxDsb->setSingleStep(1);
        angleMaxDsb->setValue(40);

        gridLayout->addWidget(angleMaxDsb, 1, 0, 1, 1);

        label_4 = new QLabel(groupBox_12);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 1, 1, 1);

        modelAngleMinDsb = new QDoubleSpinBox(groupBox_12);
        modelAngleMinDsb->setObjectName(QStringLiteral("modelAngleMinDsb"));
        modelAngleMinDsb->setDecimals(1);
        modelAngleMinDsb->setMinimum(-180);
        modelAngleMinDsb->setMaximum(180);
        modelAngleMinDsb->setSingleStep(1);
        modelAngleMinDsb->setValue(0);

        gridLayout->addWidget(modelAngleMinDsb, 1, 2, 1, 1);

        label_5 = new QLabel(groupBox_12);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 1, 3, 1, 1);

        groupBox_13 = new QGroupBox(groupBox_5);
        groupBox_13->setObjectName(QStringLiteral("groupBox_13"));
        groupBox_13->setGeometry(QRect(0, 260, 231, 52));
        gridLayout_3 = new QGridLayout(groupBox_13);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        torqueLimitDsb = new QDoubleSpinBox(groupBox_13);
        torqueLimitDsb->setObjectName(QStringLiteral("torqueLimitDsb"));
        torqueLimitDsb->setDecimals(1);
        torqueLimitDsb->setMaximum(1);
        torqueLimitDsb->setSingleStep(0.1);
        torqueLimitDsb->setValue(0.5);

        gridLayout_3->addWidget(torqueLimitDsb, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_13);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_3->addWidget(label_2, 0, 1, 1, 1);

        kpDsb = new QDoubleSpinBox(groupBox_13);
        kpDsb->setObjectName(QStringLiteral("kpDsb"));
        kpDsb->setDecimals(1);
        kpDsb->setMaximum(100);
        kpDsb->setSingleStep(1);
        kpDsb->setValue(10);

        gridLayout_3->addWidget(kpDsb, 0, 2, 1, 1);

        label_10 = new QLabel(groupBox_13);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_3->addWidget(label_10, 0, 3, 1, 1);

        groupBox_14 = new QGroupBox(groupBox_5);
        groupBox_14->setObjectName(QStringLiteral("groupBox_14"));
        groupBox_14->setGeometry(QRect(0, 445, 270, 121));
        gridLayout_5 = new QGridLayout(groupBox_14);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        inipathEdit = new QLineEdit(groupBox_14);
        inipathEdit->setObjectName(QStringLiteral("inipathEdit"));
        inipathEdit->setMinimumSize(QSize(200, 0));
        QFont font;
        font.setPointSize(15);
        inipathEdit->setFont(font);
        inipathEdit->setFrame(true);
        inipathEdit->setReadOnly(true);

        gridLayout_5->addWidget(inipathEdit, 2, 0, 1, 2);

        iniBt = new QPushButton(groupBox_14);
        iniBt->setObjectName(QStringLiteral("iniBt"));
        iniBt->setMinimumSize(QSize(100, 0));
        QFont font1;
        font1.setPointSize(9);
        iniBt->setFont(font1);

        gridLayout_5->addWidget(iniBt, 0, 0, 1, 2);

        iniSaveBt = new QPushButton(groupBox_14);
        iniSaveBt->setObjectName(QStringLiteral("iniSaveBt"));
        iniSaveBt->setMinimumSize(QSize(100, 0));
        iniSaveBt->setFont(font1);

        gridLayout_5->addWidget(iniSaveBt, 1, 0, 1, 2);

        readJointStatusBt = new QPushButton(groupBox_5);
        readJointStatusBt->setObjectName(QStringLiteral("readJointStatusBt"));
        readJointStatusBt->setEnabled(false);
        readJointStatusBt->setGeometry(QRect(10, 120, 121, 23));
        label_39 = new QLabel(groupBox_5);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setGeometry(QRect(90, 50, 54, 12));
        label_40 = new QLabel(groupBox_5);
        label_40->setObjectName(QStringLiteral("label_40"));
        label_40->setGeometry(QRect(220, 50, 54, 12));
        writeJointStatus_2 = new QPushButton(groupBox_5);
        writeJointStatus_2->setObjectName(QStringLiteral("writeJointStatus_2"));
        writeJointStatus_2->setGeometry(QRect(10, 150, 122, 23));
        importIniStatusBt = new QPushButton(groupBox_5);
        importIniStatusBt->setObjectName(QStringLiteral("importIniStatusBt"));
        importIniStatusBt->setGeometry(QRect(140, 150, 141, 23));
        groupBox_7 = new QGroupBox(centralWidget);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(820, 20, 371, 521));
        openBt = new QPushButton(groupBox_7);
        openBt->setObjectName(QStringLiteral("openBt"));
        openBt->setGeometry(QRect(230, 340, 75, 23));
        label_16 = new QLabel(groupBox_7);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(75, 452, 36, 20));
        sendEdit = new QLineEdit(groupBox_7);
        sendEdit->setObjectName(QStringLiteral("sendEdit"));
        sendEdit->setGeometry(QRect(0, 490, 341, 21));
        label_15 = new QLabel(groupBox_7);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(75, 390, 36, 20));
        label_17 = new QLabel(groupBox_7);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(75, 359, 36, 20));
        label_18 = new QLabel(groupBox_7);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(75, 421, 36, 20));
        checkBitCb = new QComboBox(groupBox_7);
        checkBitCb->setObjectName(QStringLiteral("checkBitCb"));
        checkBitCb->setGeometry(QRect(0, 452, 69, 20));
        closeBt = new QPushButton(groupBox_7);
        closeBt->setObjectName(QStringLiteral("closeBt"));
        closeBt->setGeometry(QRect(230, 370, 75, 23));
        stopBitCb = new QComboBox(groupBox_7);
        stopBitCb->setObjectName(QStringLiteral("stopBitCb"));
        stopBitCb->setGeometry(QRect(0, 421, 69, 20));
        clearBt = new QPushButton(groupBox_7);
        clearBt->setObjectName(QStringLiteral("clearBt"));
        clearBt->setGeometry(QRect(230, 430, 75, 23));
        baudRateCb = new QComboBox(groupBox_7);
        baudRateCb->setObjectName(QStringLiteral("baudRateCb"));
        baudRateCb->setGeometry(QRect(0, 359, 69, 20));
        label_19 = new QLabel(groupBox_7);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(75, 328, 36, 20));
        dataBitCb = new QComboBox(groupBox_7);
        dataBitCb->setObjectName(QStringLiteral("dataBitCb"));
        dataBitCb->setGeometry(QRect(0, 390, 69, 20));
        serialPortCb = new QComboBox(groupBox_7);
        serialPortCb->setObjectName(QStringLiteral("serialPortCb"));
        serialPortCb->setGeometry(QRect(0, 328, 69, 20));
        sendBt = new QPushButton(groupBox_7);
        sendBt->setObjectName(QStringLiteral("sendBt"));
        sendBt->setGeometry(QRect(230, 400, 75, 23));
        serialPortLogEdit = new QPlainTextEdit(groupBox_7);
        serialPortLogEdit->setObjectName(QStringLiteral("serialPortLogEdit"));
        serialPortLogEdit->setGeometry(QRect(10, 20, 341, 281));
        timeStampCb = new QCheckBox(groupBox_7);
        timeStampCb->setObjectName(QStringLiteral("timeStampCb"));
        timeStampCb->setGeometry(QRect(230, 310, 101, 21));
        timeStampCb->setChecked(false);
        groupBox_11 = new QGroupBox(centralWidget);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        groupBox_11->setGeometry(QRect(810, 540, 201, 202));
        gridLayout_2 = new QGridLayout(groupBox_11);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        joint2Cb = new QCheckBox(groupBox_11);
        joint2Cb->setObjectName(QStringLiteral("joint2Cb"));

        gridLayout_2->addWidget(joint2Cb, 0, 0, 1, 1);

        joint4Cb = new QCheckBox(groupBox_11);
        joint4Cb->setObjectName(QStringLiteral("joint4Cb"));

        gridLayout_2->addWidget(joint4Cb, 1, 0, 1, 1);

        joint6Cb = new QCheckBox(groupBox_11);
        joint6Cb->setObjectName(QStringLiteral("joint6Cb"));

        gridLayout_2->addWidget(joint6Cb, 2, 0, 1, 1);

        joint8Cb = new QCheckBox(groupBox_11);
        joint8Cb->setObjectName(QStringLiteral("joint8Cb"));

        gridLayout_2->addWidget(joint8Cb, 3, 0, 1, 1);

        joint10Cb = new QCheckBox(groupBox_11);
        joint10Cb->setObjectName(QStringLiteral("joint10Cb"));

        gridLayout_2->addWidget(joint10Cb, 4, 0, 1, 1);

        joint12Cb = new QCheckBox(groupBox_11);
        joint12Cb->setObjectName(QStringLiteral("joint12Cb"));

        gridLayout_2->addWidget(joint12Cb, 5, 0, 1, 1);

        mpu6050Cb = new QCheckBox(groupBox_11);
        mpu6050Cb->setObjectName(QStringLiteral("mpu6050Cb"));

        gridLayout_2->addWidget(mpu6050Cb, 6, 0, 1, 1);

        paj7620Cb = new QCheckBox(groupBox_11);
        paj7620Cb->setObjectName(QStringLiteral("paj7620Cb"));

        gridLayout_2->addWidget(paj7620Cb, 7, 0, 1, 1);

        groupBox_15 = new QGroupBox(centralWidget);
        groupBox_15->setObjectName(QStringLiteral("groupBox_15"));
        groupBox_15->setGeometry(QRect(490, 40, 271, 81));
        rotationTestStartAngleDsb = new QDoubleSpinBox(groupBox_15);
        rotationTestStartAngleDsb->setObjectName(QStringLiteral("rotationTestStartAngleDsb"));
        rotationTestStartAngleDsb->setGeometry(QRect(10, 20, 60, 20));
        rotationTestStartAngleDsb->setDecimals(1);
        rotationTestStartAngleDsb->setMinimum(-90);
        rotationTestStartAngleDsb->setMaximum(90);
        rotationTestStartAngleDsb->setSingleStep(1);
        rotationTestStartAngleDsb->setValue(-90);
        label_41 = new QLabel(groupBox_15);
        label_41->setObjectName(QStringLiteral("label_41"));
        label_41->setGeometry(QRect(80, 20, 54, 16));
        rotationTestEndAngleDsb = new QDoubleSpinBox(groupBox_15);
        rotationTestEndAngleDsb->setObjectName(QStringLiteral("rotationTestEndAngleDsb"));
        rotationTestEndAngleDsb->setGeometry(QRect(10, 50, 60, 20));
        rotationTestEndAngleDsb->setDecimals(1);
        rotationTestEndAngleDsb->setMinimum(-90);
        rotationTestEndAngleDsb->setMaximum(90);
        rotationTestEndAngleDsb->setSingleStep(1);
        rotationTestEndAngleDsb->setValue(90);
        label_42 = new QLabel(groupBox_15);
        label_42->setObjectName(QStringLiteral("label_42"));
        label_42->setGeometry(QRect(80, 50, 54, 16));
        jointRotationTestBt = new QPushButton(groupBox_15);
        jointRotationTestBt->setObjectName(QStringLiteral("jointRotationTestBt"));
        jointRotationTestBt->setGeometry(QRect(150, 20, 106, 23));
        jointRotationTestCloseBt = new QPushButton(groupBox_15);
        jointRotationTestCloseBt->setObjectName(QStringLiteral("jointRotationTestCloseBt"));
        jointRotationTestCloseBt->setGeometry(QRect(150, 50, 106, 23));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 470, 471, 271));
        groupBox_6 = new QGroupBox(groupBox_4);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 20, 461, 51));
        appBt = new QPushButton(groupBox_6);
        appBt->setObjectName(QStringLiteral("appBt"));
        appBt->setGeometry(QRect(0, 20, 100, 28));
        appBt->setMinimumSize(QSize(100, 0));
        appBt->setFont(font1);
        apppathEdit = new QLineEdit(groupBox_6);
        apppathEdit->setObjectName(QStringLiteral("apppathEdit"));
        apppathEdit->setGeometry(QRect(160, 20, 200, 27));
        apppathEdit->setMinimumSize(QSize(200, 0));
        apppathEdit->setFont(font);
        apppathEdit->setFrame(true);
        apppathEdit->setReadOnly(true);
        label_21 = new QLabel(groupBox_6);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(110, 20, 81, 27));
        label_21->setFont(font1);
        pushButton = new QPushButton(groupBox_6);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(370, 20, 80, 23));
        groupBox_10 = new QGroupBox(groupBox_4);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        groupBox_10->setGeometry(QRect(10, 80, 471, 101));
        label_25 = new QLabel(groupBox_10);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(10, 20, 41, 27));
        label_25->setFont(font);
        appUrlPathEdit = new QLineEdit(groupBox_10);
        appUrlPathEdit->setObjectName(QStringLiteral("appUrlPathEdit"));
        appUrlPathEdit->setGeometry(QRect(50, 20, 400, 27));
        appUrlPathEdit->setMinimumSize(QSize(400, 0));
        appUrlPathEdit->setFont(font);
        appUrlPathEdit->setFrame(true);
        appUrlPathEdit->setReadOnly(false);
        groupBox_19 = new QGroupBox(groupBox_10);
        groupBox_19->setObjectName(QStringLiteral("groupBox_19"));
        groupBox_19->setGeometry(QRect(100, 60, 231, 31));
        useDefaultWebAddressRb = new QRadioButton(groupBox_19);
        useDefaultWebAddressRb->setObjectName(QStringLiteral("useDefaultWebAddressRb"));
        useDefaultWebAddressRb->setGeometry(QRect(10, 10, 91, 16));
        useDefaultWebAddressRb->setChecked(true);
        useCustomWebAddressRb = new QRadioButton(groupBox_19);
        useCustomWebAddressRb->setObjectName(QStringLiteral("useCustomWebAddressRb"));
        useCustomWebAddressRb->setGeometry(QRect(110, 10, 121, 16));
        pushButton_2 = new QPushButton(groupBox_10);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 60, 81, 31));
        label_27 = new QLabel(groupBox_10);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(340, 50, 101, 20));
        progressBar = new QProgressBar(groupBox_10);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(340, 70, 121, 16));
        progressBar->setValue(0);
        layoutWidget = new QWidget(groupBox_4);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(12, 187, 451, 29));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_20 = new QLabel(layoutWidget);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setFont(font1);

        horizontalLayout_2->addWidget(label_20);

        appsizeEdit = new QLineEdit(layoutWidget);
        appsizeEdit->setObjectName(QStringLiteral("appsizeEdit"));
        appsizeEdit->setMinimumSize(QSize(100, 0));
        appsizeEdit->setMaximumSize(QSize(120, 16777215));
        appsizeEdit->setFont(font1);
        appsizeEdit->setReadOnly(true);

        horizontalLayout_2->addWidget(appsizeEdit);

        label_24 = new QLabel(layoutWidget);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setFont(font1);

        horizontalLayout_2->addWidget(label_24);

        appoffsizeEdit = new QLineEdit(layoutWidget);
        appoffsizeEdit->setObjectName(QStringLiteral("appoffsizeEdit"));
        appoffsizeEdit->setEnabled(true);
        appoffsizeEdit->setMinimumSize(QSize(50, 0));
        appoffsizeEdit->setMaximumSize(QSize(120, 16777215));
        appoffsizeEdit->setFont(font1);
        appoffsizeEdit->setReadOnly(true);
        appoffsizeEdit->setClearButtonEnabled(false);

        horizontalLayout_2->addWidget(appoffsizeEdit);

        label_30 = new QLabel(layoutWidget);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setFont(font1);

        horizontalLayout_2->addWidget(label_30);

        crc32ValueEdit = new QLineEdit(layoutWidget);
        crc32ValueEdit->setObjectName(QStringLiteral("crc32ValueEdit"));
        crc32ValueEdit->setEnabled(true);
        crc32ValueEdit->setMinimumSize(QSize(50, 0));
        crc32ValueEdit->setMaximumSize(QSize(120, 16777215));
        crc32ValueEdit->setFont(font1);
        crc32ValueEdit->setReadOnly(true);
        crc32ValueEdit->setClearButtonEnabled(false);

        horizontalLayout_2->addWidget(crc32ValueEdit);

        layoutWidget1 = new QWidget(groupBox_4);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 220, 451, 25));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        interruptDownloadBt = new QPushButton(layoutWidget1);
        interruptDownloadBt->setObjectName(QStringLiteral("interruptDownloadBt"));

        horizontalLayout->addWidget(interruptDownloadBt);

        label_29 = new QLabel(layoutWidget1);
        label_29->setObjectName(QStringLiteral("label_29"));

        horizontalLayout->addWidget(label_29);

        iapProgressBar = new QProgressBar(layoutWidget1);
        iapProgressBar->setObjectName(QStringLiteral("iapProgressBar"));
        iapProgressBar->setValue(0);

        horizontalLayout->addWidget(iapProgressBar);

        resetMasterBt = new QPushButton(centralWidget);
        resetMasterBt->setObjectName(QStringLiteral("resetMasterBt"));
        resetMasterBt->setGeometry(QRect(490, 10, 106, 23));
        jointRotationTestBt_2 = new QPushButton(centralWidget);
        jointRotationTestBt_2->setObjectName(QStringLiteral("jointRotationTestBt_2"));
        jointRotationTestBt_2->setEnabled(false);
        jointRotationTestBt_2->setGeometry(QRect(1100, 640, 106, 23));
        writeJointIdCb_2 = new QComboBox(centralWidget);
        writeJointIdCb_2->setObjectName(QStringLiteral("writeJointIdCb_2"));
        writeJointIdCb_2->setEnabled(false);
        writeJointIdCb_2->setGeometry(QRect(1020, 640, 69, 22));
        jointRotationTestBt_3 = new QPushButton(centralWidget);
        jointRotationTestBt_3->setObjectName(QStringLiteral("jointRotationTestBt_3"));
        jointRotationTestBt_3->setEnabled(false);
        jointRotationTestBt_3->setGeometry(QRect(1100, 670, 121, 23));
        writeJointIdCb_3 = new QComboBox(centralWidget);
        writeJointIdCb_3->setObjectName(QStringLiteral("writeJointIdCb_3"));
        writeJointIdCb_3->setEnabled(false);
        writeJointIdCb_3->setGeometry(QRect(1020, 670, 69, 22));
        groupBox_20 = new QGroupBox(centralWidget);
        groupBox_20->setObjectName(QStringLiteral("groupBox_20"));
        groupBox_20->setEnabled(false);
        groupBox_20->setGeometry(QRect(1020, 550, 120, 80));
        joint2Cb_2 = new QCheckBox(groupBox_20);
        joint2Cb_2->setObjectName(QStringLiteral("joint2Cb_2"));
        joint2Cb_2->setGeometry(QRect(10, 20, 101, 21));
        joint2Cb_3 = new QCheckBox(groupBox_20);
        joint2Cb_3->setObjectName(QStringLiteral("joint2Cb_3"));
        joint2Cb_3->setGeometry(QRect(10, 40, 101, 21));
        clearAllBt = new QPushButton(centralWidget);
        clearAllBt->setObjectName(QStringLiteral("clearAllBt"));
        clearAllBt->setGeometry(QRect(610, 10, 91, 23));
        pushButton_recv_clear_4 = new QPushButton(centralWidget);
        pushButton_recv_clear_4->setObjectName(QStringLiteral("pushButton_recv_clear_4"));
        pushButton_recv_clear_4->setGeometry(QRect(10, 420, 41, 23));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1227, 22));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(function);
        menu->addAction(about);
        menu->addAction(instructions);
        menu->addAction(SourceCode);

        retranslateUi(MainWindow);

        jointIdCb->setCurrentIndex(2);
        stopBitCb->setCurrentIndex(0);
        baudRateCb->setCurrentIndex(2);
        dataBitCb->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ElectionBot-Debugger", Q_NULLPTR));
        function->setText(QApplication::translate("MainWindow", "\345\212\237\350\203\275\344\273\213\347\273\215", Q_NULLPTR));
        about->setText(QApplication::translate("MainWindow", "\347\211\210\346\235\203\345\243\260\346\230\216", Q_NULLPTR));
        instructions->setText(QApplication::translate("MainWindow", "\344\275\277\347\224\250\350\257\264\346\230\216\344\271\246", Q_NULLPTR));
        SourceCode->setText(QApplication::translate("MainWindow", "\346\272\220\347\240\201", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        SourceCode->setToolTip(QApplication::translate("MainWindow", "\346\272\220\347\240\201", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        groupBox_8->setTitle(QApplication::translate("MainWindow", "LOG", Q_NULLPTR));
        showDebugLogCb->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\350\260\203\350\257\225\344\277\241\346\201\257", Q_NULLPTR));
        groupBox->setTitle(QString());
        pushButton_USB_Open->setText(QApplication::translate("MainWindow", "USB_Open", Q_NULLPTR));
        PID_Data->setText(QApplication::translate("MainWindow", "32803", Q_NULLPTR));
        VID_Data->setText(QApplication::translate("MainWindow", "4097", Q_NULLPTR));
        label_22->setText(QApplication::translate("MainWindow", "PID\357\274\232", Q_NULLPTR));
        label_23->setText(QApplication::translate("MainWindow", "VID\357\274\232", Q_NULLPTR));
        groupBox_2->setTitle(QString());
        Clear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\346\216\245\346\224\266", Q_NULLPTR));
        radioButton_recv_ascii->setText(QApplication::translate("MainWindow", "ASCII\347\240\201", Q_NULLPTR));
        pushButton_recv_save->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272\346\226\207\344\273\266", Q_NULLPTR));
        radioButton_recv_unit16->setText(QApplication::translate("MainWindow", "16\350\277\233\345\210\266", Q_NULLPTR));
        groupBox_3->setTitle(QString());
        radioButton_send_ascii->setText(QApplication::translate("MainWindow", "ASCII\347\240\201", Q_NULLPTR));
        radioButton_send_unit16->setText(QApplication::translate("MainWindow", "16\350\277\233\345\210\266", Q_NULLPTR));
        lineEdit_send->setText(QApplication::translate("MainWindow", "1000", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "\346\257\253\347\247\222", Q_NULLPTR));
        checkBox_send->setText(QApplication::translate("MainWindow", "\345\276\252\347\216\257\345\217\221\351\200\201", Q_NULLPTR));
        clear_send_data->setText(QApplication::translate("MainWindow", "\346\270\205\351\231\244\346\225\260\346\215\256", Q_NULLPTR));
        Data_Send->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "\350\210\265\346\234\272\345\217\202\346\225\260\350\260\203\350\257\225\347\252\227\345\217\243", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "\347\224\265\345\255\220ID", Q_NULLPTR));
        jointIdCb->clear();
        jointIdCb->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "0", Q_NULLPTR)
         << QApplication::translate("MainWindow", "2", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4", Q_NULLPTR)
         << QApplication::translate("MainWindow", "6", Q_NULLPTR)
         << QApplication::translate("MainWindow", "8", Q_NULLPTR)
         << QApplication::translate("MainWindow", "10", Q_NULLPTR)
         << QApplication::translate("MainWindow", "12", Q_NULLPTR)
        );
        jointIdCb->setCurrentText(QApplication::translate("MainWindow", "4", Q_NULLPTR));
        writeElectionBotIdEdit->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        writeJointStatus->setText(QApplication::translate("MainWindow", "\345\206\231\345\275\223\345\211\215ID\347\232\204\350\210\265\346\234\272\345\217\202\346\225\260\345\210\260\344\270\273\346\216\247", Q_NULLPTR));
        writeJointIdBt->setText(QApplication::translate("MainWindow", "\345\206\231\350\210\265\346\234\272\346\226\260ID\345\210\260\344\270\273\346\216\247", Q_NULLPTR));
        electionBotIdEdit->setInputMask(QString());
        electionBotIdEdit->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        writeElectionBotIdBt->setText(QApplication::translate("MainWindow", "\345\206\231\347\224\265\345\255\220\346\226\260ID\345\210\260\344\270\273\346\216\247", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\350\210\265\346\234\272ID", Q_NULLPTR));
        writeJointIdCb->clear();
        writeJointIdCb->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "2", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4", Q_NULLPTR)
         << QApplication::translate("MainWindow", "6", Q_NULLPTR)
         << QApplication::translate("MainWindow", "8", Q_NULLPTR)
         << QApplication::translate("MainWindow", "10", Q_NULLPTR)
         << QApplication::translate("MainWindow", "12", Q_NULLPTR)
        );
        groupBox_9->setTitle(QApplication::translate("MainWindow", "\344\270\215\350\246\201\351\232\217\344\276\277\346\224\271\345\212\250\350\277\231\344\270\252\347\252\227\345\217\243\351\207\214\347\232\204\344\270\234\350\245\277", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "initAngle", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "kv", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "angle", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "ki", Q_NULLPTR));
        invertedCb->setText(QApplication::translate("MainWindow", "inverted", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "kd", Q_NULLPTR));
        enableCb->setText(QApplication::translate("MainWindow", "enable", Q_NULLPTR));
        groupBox_12->setTitle(QApplication::translate("MainWindow", "\350\247\222\345\272\246\350\260\203\350\257\225\347\233\270\345\205\263", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "angleMin", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "mdoelAngleMax", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "angleMax", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "modelAngleMin", Q_NULLPTR));
        groupBox_13->setTitle(QApplication::translate("MainWindow", "\351\251\261\345\212\250\350\203\275\345\212\233\350\260\203\350\257\225\347\233\270\345\205\263", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "torqueLimit", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "kp", Q_NULLPTR));
        groupBox_14->setTitle(QApplication::translate("MainWindow", "\350\210\265\346\234\272\345\217\202\346\225\260\351\205\215\347\275\256\346\226\207\344\273\266\345\257\274\345\205\245\344\270\216\344\277\235\345\255\230", Q_NULLPTR));
        iniBt->setText(QApplication::translate("MainWindow", "\345\257\274\345\205\245\346\234\254\345\234\260\350\207\252\345\256\232\344\271\211\351\205\215\347\275\256\346\226\207\344\273\266", Q_NULLPTR));
        iniSaveBt->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\350\256\276\347\275\256\345\210\260\346\234\254\345\234\260\351\205\215\347\275\256\346\226\207\344\273\266", Q_NULLPTR));
        readJointStatusBt->setText(QApplication::translate("MainWindow", "\344\273\216\344\270\273\346\216\247\350\257\273\345\217\226\350\210\265\346\234\272\345\217\202\346\225\260", Q_NULLPTR));
        label_39->setText(QApplication::translate("MainWindow", "\347\224\265\345\255\220\346\226\260ID", Q_NULLPTR));
        label_40->setText(QApplication::translate("MainWindow", "\350\210\265\346\234\272\346\226\260ID", Q_NULLPTR));
        writeJointStatus_2->setText(QApplication::translate("MainWindow", "\345\257\274\345\205\245\351\273\230\350\256\244\345\217\202\346\225\260\345\210\260\351\235\242\346\235\277", Q_NULLPTR));
        importIniStatusBt->setText(QApplication::translate("MainWindow", "\345\257\274\345\205\245\351\205\215\347\275\256\346\226\207\344\273\266\345\217\202\346\225\260\345\210\260\351\235\242\346\235\277", Q_NULLPTR));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "\344\270\262\345\217\243", Q_NULLPTR));
        openBt->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", Q_NULLPTR));
        label_16->setText(QApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", Q_NULLPTR));
        label_17->setText(QApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", Q_NULLPTR));
        label_18->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", Q_NULLPTR));
        checkBitCb->clear();
        checkBitCb->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "none", Q_NULLPTR)
        );
        closeBt->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\344\270\262\345\217\243", Q_NULLPTR));
        stopBitCb->clear();
        stopBitCb->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1", Q_NULLPTR)
         << QApplication::translate("MainWindow", "1.5", Q_NULLPTR)
         << QApplication::translate("MainWindow", "2", Q_NULLPTR)
        );
        clearBt->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        baudRateCb->clear();
        baudRateCb->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "4800", Q_NULLPTR)
         << QApplication::translate("MainWindow", "9600", Q_NULLPTR)
         << QApplication::translate("MainWindow", "115200", Q_NULLPTR)
         << QApplication::translate("MainWindow", "256000", Q_NULLPTR)
        );
        label_19->setText(QApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\267", Q_NULLPTR));
        dataBitCb->clear();
        dataBitCb->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "5", Q_NULLPTR)
         << QApplication::translate("MainWindow", "6", Q_NULLPTR)
         << QApplication::translate("MainWindow", "7", Q_NULLPTR)
         << QApplication::translate("MainWindow", "8", Q_NULLPTR)
        );
        sendBt->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", Q_NULLPTR));
        timeStampCb->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\346\227\266\351\227\264\346\210\263", Q_NULLPTR));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "\344\273\216\346\234\272\350\277\236\346\216\245\347\212\266\346\200\201", Q_NULLPTR));
        joint2Cb->setText(QApplication::translate("MainWindow", "\345\244\264\351\203\250/joint2", Q_NULLPTR));
        joint4Cb->setText(QApplication::translate("MainWindow", "\345\267\246\350\207\202\345\244\271\350\247\222/joint4", Q_NULLPTR));
        joint6Cb->setText(QApplication::translate("MainWindow", "\345\267\246\350\207\202\344\270\212\344\270\213\346\227\213\350\275\254/joint6", Q_NULLPTR));
        joint8Cb->setText(QApplication::translate("MainWindow", "\345\217\263\350\207\202\345\244\271\350\247\222/joint8", Q_NULLPTR));
        joint10Cb->setText(QApplication::translate("MainWindow", "\345\217\263\350\207\202\344\270\212\344\270\213\346\227\213\350\275\254/joint10", Q_NULLPTR));
        joint12Cb->setText(QApplication::translate("MainWindow", "\350\272\253\344\275\223\345\267\246\345\217\263\346\227\213\350\275\254/joint12", Q_NULLPTR));
        mpu6050Cb->setText(QApplication::translate("MainWindow", "\345\205\255\350\275\264\344\274\240\346\204\237\345\231\250/mpu6050", Q_NULLPTR));
        paj7620Cb->setText(QApplication::translate("MainWindow", "\346\211\213\345\212\277\350\257\206\345\210\253/paj7620", Q_NULLPTR));
        groupBox_15->setTitle(QApplication::translate("MainWindow", "\350\210\265\346\234\272\345\276\200\345\244\215\350\275\254\345\212\250\346\265\213\350\257\225\357\274\210 \347\273\204\350\243\205\345\245\275\344\272\206\344\270\215\350\246\201\347\224\250\350\277\231\344\270\252\346\265\213\357\274\211", Q_NULLPTR));
        label_41->setText(QApplication::translate("MainWindow", "\350\265\267\345\247\213\350\247\222\345\272\246", Q_NULLPTR));
        label_42->setText(QApplication::translate("MainWindow", "\345\233\236\350\275\254\350\247\222\345\272\246", Q_NULLPTR));
        jointRotationTestBt->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\346\265\213\350\257\225", Q_NULLPTR));
        jointRotationTestCloseBt->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\346\265\213\350\257\225", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "IAP", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "\346\234\254\345\234\260APP\346\226\207\344\273\266\345\257\274\345\205\245\345\217\212\347\203\247\345\275\225", Q_NULLPTR));
        appBt->setText(QApplication::translate("MainWindow", "\345\257\274\345\205\245\346\234\254\345\234\260APP\346\226\207\344\273\266", Q_NULLPTR));
        label_21->setText(QApplication::translate("MainWindow", "APP\350\267\257\345\276\204", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "\347\203\247\345\275\225\346\234\254\345\234\260\346\226\207\344\273\266", Q_NULLPTR));
        groupBox_10->setTitle(QApplication::translate("MainWindow", "\347\275\221\347\273\234APP\346\226\207\344\273\266\345\257\274\345\205\245\345\217\212\347\203\247\345\275\225", Q_NULLPTR));
        label_25->setText(QApplication::translate("MainWindow", "URL", Q_NULLPTR));
        groupBox_19->setTitle(QString());
        useDefaultWebAddressRb->setText(QApplication::translate("MainWindow", "\344\275\277\347\224\250\351\273\230\350\256\244\347\275\221\345\235\200", Q_NULLPTR));
        useCustomWebAddressRb->setText(QApplication::translate("MainWindow", "\344\275\277\347\224\250\350\207\252\345\256\232\344\271\211\350\256\244\347\275\221\345\235\200", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\347\203\247\345\275\225\347\275\221\347\273\234\346\226\207\344\273\266", Q_NULLPTR));
        label_27->setText(QApplication::translate("MainWindow", "\347\275\221\347\273\234\346\226\207\344\273\266\344\270\213\350\275\275\350\277\233\345\272\246", Q_NULLPTR));
        label_20->setText(QApplication::translate("MainWindow", "APPsize", Q_NULLPTR));
        appsizeEdit->setText(QString());
        label_24->setText(QApplication::translate("MainWindow", "flashOffsize", Q_NULLPTR));
        label_30->setText(QApplication::translate("MainWindow", "crc32Value", Q_NULLPTR));
        interruptDownloadBt->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242IAP", Q_NULLPTR));
        label_29->setText(QApplication::translate("MainWindow", "IAP\350\277\233\345\272\246", Q_NULLPTR));
        resetMasterBt->setText(QApplication::translate("MainWindow", "\344\270\273\346\216\247\347\263\273\347\273\237\345\244\215\344\275\215", Q_NULLPTR));
        jointRotationTestBt_2->setText(QApplication::translate("MainWindow", "IIC\350\277\236\346\216\245\346\243\200\346\265\213", Q_NULLPTR));
        writeJointIdCb_2->clear();
        writeJointIdCb_2->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "2", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4", Q_NULLPTR)
         << QApplication::translate("MainWindow", "6", Q_NULLPTR)
         << QApplication::translate("MainWindow", "8", Q_NULLPTR)
         << QApplication::translate("MainWindow", "10", Q_NULLPTR)
         << QApplication::translate("MainWindow", "12", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\346\211\213\345\212\277/7620", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\345\205\255\350\275\264/6050", Q_NULLPTR)
        );
        jointRotationTestBt_3->setText(QApplication::translate("MainWindow", "\344\270\273\346\216\247\350\260\203\350\257\225\344\277\241\346\201\257\345\274\200/\345\205\263", Q_NULLPTR));
        writeJointIdCb_3->clear();
        writeJointIdCb_3->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\274\200", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\345\205\263", Q_NULLPTR)
        );
        groupBox_20->setTitle(QApplication::translate("MainWindow", "\345\233\272\344\273\266\350\277\220\350\241\214\345\234\260\345\235\200\346\230\276\347\244\272", Q_NULLPTR));
        joint2Cb_2->setText(QApplication::translate("MainWindow", "IAP\345\214\272", Q_NULLPTR));
        joint2Cb_3->setText(QApplication::translate("MainWindow", "APP\345\214\272", Q_NULLPTR));
        clearAllBt->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\346\211\200\346\234\211\347\252\227\345\217\243", Q_NULLPTR));
        pushButton_recv_clear_4->setText(QApplication::translate("MainWindow", "\346\270\205\351\231\244", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
