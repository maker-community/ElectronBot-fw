/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ElectionBot-Debugger/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[57];
    char stringdata0[1182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "handleTimeout"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 30), // "on_pushButton_USB_Open_clicked"
QT_MOC_LITERAL(4, 57, 14), // "on_USB_clicked"
QT_MOC_LITERAL(5, 72, 16), // "on_Clear_clicked"
QT_MOC_LITERAL(6, 89, 21), // "on_LED_ON_CMD_clicked"
QT_MOC_LITERAL(7, 111, 22), // "on_LED_OFF_CMD_clicked"
QT_MOC_LITERAL(8, 134, 20), // "on_Data_Send_clicked"
QT_MOC_LITERAL(9, 155, 26), // "on_clear_send_data_clicked"
QT_MOC_LITERAL(10, 182, 24), // "on_checkBox_send_toggled"
QT_MOC_LITERAL(11, 207, 7), // "checked"
QT_MOC_LITERAL(12, 215, 31), // "on_pushButton_recv_save_clicked"
QT_MOC_LITERAL(13, 247, 8), // "sendData"
QT_MOC_LITERAL(14, 256, 10), // "aboutclick"
QT_MOC_LITERAL(15, 267, 18), // "on_about_triggered"
QT_MOC_LITERAL(16, 286, 21), // "on_function_triggered"
QT_MOC_LITERAL(17, 308, 27), // "on_writeJointStatus_clicked"
QT_MOC_LITERAL(18, 336, 17), // "on_openBt_clicked"
QT_MOC_LITERAL(19, 354, 18), // "on_closeBt_clicked"
QT_MOC_LITERAL(20, 373, 17), // "on_sendBt_clicked"
QT_MOC_LITERAL(21, 391, 18), // "on_clearBt_clicked"
QT_MOC_LITERAL(22, 410, 23), // "serialPortReadRead_Slot"
QT_MOC_LITERAL(23, 434, 25), // "on_writeJointIdBt_clicked"
QT_MOC_LITERAL(24, 460, 31), // "on_writeElectionBotIdBt_clicked"
QT_MOC_LITERAL(25, 492, 29), // "on_writeJointStatus_2_clicked"
QT_MOC_LITERAL(26, 522, 16), // "on_appBt_clicked"
QT_MOC_LITERAL(27, 539, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(28, 561, 11), // "timeoutSlot"
QT_MOC_LITERAL(29, 573, 16), // "on_iniBt_clicked"
QT_MOC_LITERAL(30, 590, 28), // "on_importIniStatusBt_clicked"
QT_MOC_LITERAL(31, 619, 20), // "on_iniSaveBt_clicked"
QT_MOC_LITERAL(32, 640, 42), // "ReadRobotSlavesConnectionStat..."
QT_MOC_LITERAL(33, 683, 33), // "UpdateRobotSlavesConnectionSt..."
QT_MOC_LITERAL(34, 717, 18), // "DealSerialPortData"
QT_MOC_LITERAL(35, 736, 24), // "on_resetMasterBt_clicked"
QT_MOC_LITERAL(36, 761, 29), // "on_writeJointStatus_5_clicked"
QT_MOC_LITERAL(37, 791, 30), // "on_jointRotationTestBt_clicked"
QT_MOC_LITERAL(38, 822, 35), // "on_jointRotationTestCloseBt_c..."
QT_MOC_LITERAL(39, 858, 25), // "on_instructions_triggered"
QT_MOC_LITERAL(40, 884, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(41, 908, 11), // "on_finished"
QT_MOC_LITERAL(42, 920, 12), // "on_readyRead"
QT_MOC_LITERAL(43, 933, 19), // "on_downloadProgress"
QT_MOC_LITERAL(44, 953, 9), // "bytesRead"
QT_MOC_LITERAL(45, 963, 10), // "totalBytes"
QT_MOC_LITERAL(46, 974, 25), // "on_btnDefaultPath_clicked"
QT_MOC_LITERAL(47, 1000, 22), // "on_btnDownload_clicked"
QT_MOC_LITERAL(48, 1023, 22), // "on_editURL_textChanged"
QT_MOC_LITERAL(49, 1046, 4), // "arg1"
QT_MOC_LITERAL(50, 1051, 17), // "VersionCompareStr"
QT_MOC_LITERAL(51, 1069, 7), // "strVer1"
QT_MOC_LITERAL(52, 1077, 7), // "strVer2"
QT_MOC_LITERAL(53, 1085, 14), // "VersionCompare"
QT_MOC_LITERAL(54, 1100, 30), // "on_interruptDownloadBt_clicked"
QT_MOC_LITERAL(55, 1131, 21), // "on_clearAllBt_clicked"
QT_MOC_LITERAL(56, 1153, 28) // "on_readJointStatusBt_clicked"

    },
    "MainWindow\0handleTimeout\0\0"
    "on_pushButton_USB_Open_clicked\0"
    "on_USB_clicked\0on_Clear_clicked\0"
    "on_LED_ON_CMD_clicked\0on_LED_OFF_CMD_clicked\0"
    "on_Data_Send_clicked\0on_clear_send_data_clicked\0"
    "on_checkBox_send_toggled\0checked\0"
    "on_pushButton_recv_save_clicked\0"
    "sendData\0aboutclick\0on_about_triggered\0"
    "on_function_triggered\0on_writeJointStatus_clicked\0"
    "on_openBt_clicked\0on_closeBt_clicked\0"
    "on_sendBt_clicked\0on_clearBt_clicked\0"
    "serialPortReadRead_Slot\0"
    "on_writeJointIdBt_clicked\0"
    "on_writeElectionBotIdBt_clicked\0"
    "on_writeJointStatus_2_clicked\0"
    "on_appBt_clicked\0on_pushButton_clicked\0"
    "timeoutSlot\0on_iniBt_clicked\0"
    "on_importIniStatusBt_clicked\0"
    "on_iniSaveBt_clicked\0"
    "ReadRobotSlavesConnectionStatusProtocolBuf\0"
    "UpdateRobotSlavesConnectionStatus\0"
    "DealSerialPortData\0on_resetMasterBt_clicked\0"
    "on_writeJointStatus_5_clicked\0"
    "on_jointRotationTestBt_clicked\0"
    "on_jointRotationTestCloseBt_clicked\0"
    "on_instructions_triggered\0"
    "on_pushButton_2_clicked\0on_finished\0"
    "on_readyRead\0on_downloadProgress\0"
    "bytesRead\0totalBytes\0on_btnDefaultPath_clicked\0"
    "on_btnDownload_clicked\0on_editURL_textChanged\0"
    "arg1\0VersionCompareStr\0strVer1\0strVer2\0"
    "VersionCompare\0on_interruptDownloadBt_clicked\0"
    "on_clearAllBt_clicked\0"
    "on_readJointStatusBt_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      49,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  259,    2, 0x08 /* Private */,
       3,    0,  260,    2, 0x08 /* Private */,
       4,    0,  261,    2, 0x08 /* Private */,
       5,    0,  262,    2, 0x08 /* Private */,
       6,    0,  263,    2, 0x08 /* Private */,
       7,    0,  264,    2, 0x08 /* Private */,
       8,    0,  265,    2, 0x08 /* Private */,
       9,    0,  266,    2, 0x08 /* Private */,
      10,    1,  267,    2, 0x08 /* Private */,
      12,    0,  270,    2, 0x08 /* Private */,
      13,    0,  271,    2, 0x08 /* Private */,
      14,    0,  272,    2, 0x08 /* Private */,
      15,    0,  273,    2, 0x08 /* Private */,
      16,    0,  274,    2, 0x08 /* Private */,
      17,    0,  275,    2, 0x08 /* Private */,
      18,    0,  276,    2, 0x08 /* Private */,
      19,    0,  277,    2, 0x08 /* Private */,
      20,    0,  278,    2, 0x08 /* Private */,
      21,    0,  279,    2, 0x08 /* Private */,
      22,    0,  280,    2, 0x08 /* Private */,
      23,    0,  281,    2, 0x08 /* Private */,
      24,    0,  282,    2, 0x08 /* Private */,
      25,    0,  283,    2, 0x08 /* Private */,
      26,    0,  284,    2, 0x08 /* Private */,
      27,    0,  285,    2, 0x08 /* Private */,
      28,    0,  286,    2, 0x08 /* Private */,
      29,    0,  287,    2, 0x08 /* Private */,
      30,    0,  288,    2, 0x08 /* Private */,
      31,    0,  289,    2, 0x08 /* Private */,
      32,    0,  290,    2, 0x08 /* Private */,
      33,    0,  291,    2, 0x08 /* Private */,
      34,    0,  292,    2, 0x08 /* Private */,
      35,    0,  293,    2, 0x08 /* Private */,
      36,    0,  294,    2, 0x08 /* Private */,
      37,    0,  295,    2, 0x08 /* Private */,
      38,    0,  296,    2, 0x08 /* Private */,
      39,    0,  297,    2, 0x08 /* Private */,
      40,    0,  298,    2, 0x08 /* Private */,
      41,    0,  299,    2, 0x08 /* Private */,
      42,    0,  300,    2, 0x08 /* Private */,
      43,    2,  301,    2, 0x08 /* Private */,
      46,    0,  306,    2, 0x08 /* Private */,
      47,    0,  307,    2, 0x08 /* Private */,
      48,    1,  308,    2, 0x08 /* Private */,
      50,    2,  311,    2, 0x08 /* Private */,
      53,    0,  316,    2, 0x08 /* Private */,
      54,    0,  317,    2, 0x08 /* Private */,
      55,    0,  318,    2, 0x08 /* Private */,
      56,    0,  319,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QByteArray,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   44,   45,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   49,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,   51,   52,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleTimeout(); break;
        case 1: _t->on_pushButton_USB_Open_clicked(); break;
        case 2: _t->on_USB_clicked(); break;
        case 3: _t->on_Clear_clicked(); break;
        case 4: _t->on_LED_ON_CMD_clicked(); break;
        case 5: _t->on_LED_OFF_CMD_clicked(); break;
        case 6: _t->on_Data_Send_clicked(); break;
        case 7: _t->on_clear_send_data_clicked(); break;
        case 8: _t->on_checkBox_send_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_pushButton_recv_save_clicked(); break;
        case 10: _t->sendData(); break;
        case 11: _t->aboutclick(); break;
        case 12: _t->on_about_triggered(); break;
        case 13: _t->on_function_triggered(); break;
        case 14: _t->on_writeJointStatus_clicked(); break;
        case 15: _t->on_openBt_clicked(); break;
        case 16: _t->on_closeBt_clicked(); break;
        case 17: _t->on_sendBt_clicked(); break;
        case 18: _t->on_clearBt_clicked(); break;
        case 19: _t->serialPortReadRead_Slot(); break;
        case 20: _t->on_writeJointIdBt_clicked(); break;
        case 21: _t->on_writeElectionBotIdBt_clicked(); break;
        case 22: _t->on_writeJointStatus_2_clicked(); break;
        case 23: _t->on_appBt_clicked(); break;
        case 24: _t->on_pushButton_clicked(); break;
        case 25: _t->timeoutSlot(); break;
        case 26: _t->on_iniBt_clicked(); break;
        case 27: _t->on_importIniStatusBt_clicked(); break;
        case 28: _t->on_iniSaveBt_clicked(); break;
        case 29: { QByteArray _r = _t->ReadRobotSlavesConnectionStatusProtocolBuf();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = std::move(_r); }  break;
        case 30: _t->UpdateRobotSlavesConnectionStatus(); break;
        case 31: _t->DealSerialPortData(); break;
        case 32: _t->on_resetMasterBt_clicked(); break;
        case 33: _t->on_writeJointStatus_5_clicked(); break;
        case 34: _t->on_jointRotationTestBt_clicked(); break;
        case 35: _t->on_jointRotationTestCloseBt_clicked(); break;
        case 36: _t->on_instructions_triggered(); break;
        case 37: _t->on_pushButton_2_clicked(); break;
        case 38: _t->on_finished(); break;
        case 39: _t->on_readyRead(); break;
        case 40: _t->on_downloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 41: _t->on_btnDefaultPath_clicked(); break;
        case 42: _t->on_btnDownload_clicked(); break;
        case 43: _t->on_editURL_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 44: { int _r = _t->VersionCompareStr((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 45: _t->VersionCompare(); break;
        case 46: _t->on_interruptDownloadBt_clicked(); break;
        case 47: _t->on_clearAllBt_clicked(); break;
        case 48: _t->on_readJointStatusBt_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 49)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 49;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
