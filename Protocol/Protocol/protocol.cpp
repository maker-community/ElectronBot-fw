#include "protocol.h"
 
ElectronBotJointStatus_t ElectronBotjoint[6];
JointStatus_t joint[7];
ProtocolItem_t ProtocolItem;
txbuf_t txbuf;
rxbuf_t rxbuf;

void myprintf(const char* format, ...)
{
    printf(format);
}

void BufClear(uint8_t* buf, uint8_t value, uint16_t len)
{
    memset(buf, value, len);
}

uint8_t CheckSum(uint8_t* buf, uint16_t dataAreaLen)
{
    uint16_t i = 0, checkLen = 0;
    uint8_t* checkBuf;
    uint8_t checkValue = 0;
    uint32_t sum = 0;

    checkBuf = &buf[1];
    checkLen = dataAreaLen + 8;
    //i=dataAreaLen+8;
    for (i = 0; i < checkLen; i++)
    {
        sum = sum + checkBuf[i];
    }
    checkValue = sum % 256;
    return checkValue;
}


void ComposeProtocolFrame(uint8_t* buf ,uint16_t *dataLen, ProtocolItem_t* items)
{
    uint16_t len = 0, itemLen = 0;
    uint8_t checkValue = 0;

    buf[0] = FrameHead;
    len++;

    itemLen = sizeof(items->ElectronBotID);
    memcpy(&buf[len], &items->ElectronBotID, itemLen);
    len += itemLen;

    if (items->frame == ResponseFrame)
    {
        items->cmd = items->cmd | 0x80;
    }
    itemLen = sizeof(items->cmd);
    memcpy(&buf[len], &items->cmd, itemLen);
    len += itemLen;

    itemLen = sizeof(items->jointID);
    memcpy(&buf[len], &items->jointID, itemLen);
    len += itemLen;

    itemLen = sizeof(items->dataLen);
    memcpy(&buf[len], &items->dataLen, itemLen);
    len += itemLen;

    //itemLen=sizeof(items->dataLen);
    if (items->dataLen > 0)
    {
        memcpy(&buf[len], items->data, items->dataLen);
        len += items->dataLen;
    }

    checkValue = CheckSum((uint8_t*)&txbuf.buf[0], len - 1);
    buf[len] = checkValue;
    len++;

    buf[len] = FrameTail;
    len++;
    
    *dataLen = len;
    // memcpy(txbuf,&items->ElectronBotID,sizeof(items->ElectronBotID));
}


//查找inBuf里是否有协议帧,有则将协议帧拷贝到outBuf
bool ProtocolLookUp(uint8_t* outBuf, uint8_t* inBuf, uint16_t inlen)
{
    uint16_t i;
    uint16_t datalen = 0;
    uint8_t checkValue = 0;
    if (inlen < 11)
    {
        return false;
    }
    for (i = 0; i < inlen; i++)
    {
        if (inBuf[i] == FrameHead && inlen - i >= 11)
        {
            datalen = inBuf[i + 7] + inBuf[i + 8] * 256;
            if (inBuf[i + 7 + 2 + datalen + 2 - 1] == FrameTail)
            {
                checkValue = inBuf[i + 7 + 2 + datalen + 1 - 1];
                if (checkValue == CheckSum(&inBuf[i], datalen))
                {
                    memcpy(outBuf, &inBuf[i], 7 + 2 + datalen + 2);
                   // *outframeHead = i;
                    return true;
                    // ProtocolProcessing(rxbuf);
                }
            }
        }
    }
    return false;
}

//此函数可以自己重新写
void ProtocolProcessing(uint8_t* inbuf, uint16_t len)
{

    BufClear((uint8_t*)&rxbuf, 0, sizeof(rxbuf));
    if (ProtocolLookUp(rxbuf.buf, inbuf, len) == false)
    {
        myprintf("\r\n  No Protocol!!!!!!! \r\n");
        return;
    }

    ElectronBotJointStatus_t *test;

    uint8_t idbuf = 0;
    uint8_t* buf;

    buf = rxbuf.buf;
    ProtocolItem.ElectronBotID = buf[1] + buf[2] * 256 + buf[3] * 256 * 256 + buf[4] * 256 * 256 * 256;
    ProtocolItem.cmd = buf[5];
    ProtocolItem.jointID = buf[6];
    ProtocolItem.dataLen = buf[7] + buf[8] * 256;
    ProtocolItem.data = &buf[9];

    myprintf("ElectronBotID=%d\r\n", ProtocolItem.ElectronBotID);
    myprintf("jointID=%d\r\n", ProtocolItem.jointID);
    myprintf("dataLen=%d\r\n", ProtocolItem.dataLen);

    test = (ElectronBotJointStatus_t*)ProtocolItem.data;

    idbuf = ProtocolItem.jointID / 2;
    if (ProtocolItem.cmd == CMD_WriteAllJointStatus || ProtocolItem.cmd == CMD_WriteAllJointStatus |0x80)
    {
        myprintf("cmd=CMD_WriteAllJointStatus\r\n");
        myprintf("angleMax=%f\r\n", test->angleMax);
        myprintf("angle=%f\r\n", test->angle);
        myprintf("modelAngelMin=%f\r\n", test->modelAngelMin);
        myprintf("modelAngelMax=%f\r\n", test->modelAngelMax);
        myprintf("inverted=%s\r\n", test->inverted ? "true" : "false");
        myprintf("initAngle=%f\r\n", test->initAngle);
        myprintf("torqueLimit=%f\r\n", test->torqueLimit);
        myprintf("kp=%f\r\n", test->kp);
        myprintf("ki=%f\r\n", test->ki);
        myprintf("kv=%f\r\n", test->kv);
        myprintf("kd=%f\r\n", test->kd);
        myprintf("enable=%s\r\n", test->enable ? "true" : "false");
    }
}


uint8_t testbuf[300] = { 0 };
uint8_t testbuf2[300] = { 0 };

void testComposeProtocolFrame()
{
    ElectronBotJointStatus_t test, * p;

    p = &test;
    BufClear((uint8_t*)p, 0, sizeof(ElectronBotJointStatus_t));

    test.angleMin = 10;
    test.angleMax = 30;
    test.angle = 0;
    test.modelAngelMin = 12;
    test.modelAngelMax = 32;
    test.inverted = true;
    test.initAngle = 15;
    test.torqueLimit = 0.5;
    test.kp = 20;
    test.ki = 1;
    test.kv = 2;
    test.kd = 3;
    test.enable = true;

   // p = &test;
    memcpy((uint8_t*)testbuf, (uint8_t*)p, sizeof(ElectronBotJointStatus_t));

    ProtocolItem_t* p2;
    p2 = &ProtocolItem;
    BufClear((uint8_t*)p2, 0, sizeof(ProtocolItem_t));

    ProtocolItem.ElectronBotID = 12345678;
    ProtocolItem.cmd = CMD_WriteAllJointStatus;
    ProtocolItem.jointID = 12;
    ProtocolItem.dataLen = sizeof(ElectronBotJointStatus_t);
    ProtocolItem.data = testbuf;
    ProtocolItem.frame = CommandFrame; 

    ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen ,&ProtocolItem);//组帧
    //memcpy(&testbuf2[100], txbuf.buf, txbuf.dataLen);
    //ProtocolProcessing(testbuf2, sizeof(testbuf2));//解帧
    ProtocolProcessing(txbuf.buf, txbuf.dataLen);//解帧

    BufClear((uint8_t*)&txbuf, 0, sizeof(txbuf));
}
