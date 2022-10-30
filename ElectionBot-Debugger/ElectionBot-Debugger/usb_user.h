#ifndef USB_USER_H
#define USB_USER_H



int USB_Init_Open_Process(void);
int USB_Read_Process(char *dat, int lenth);
void USB_Close_Process(void);


int USB_Write_Process(char *dat, int lenth,int timeout);

#endif // USB_USER_H
