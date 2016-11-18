
extern uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
extern uint8 img[CAMERA_H][CAMERA_W];

//函数声明
 void PORTB_IRQHandler();
 void DMA0_IRQHandler();
 void sendimg(void *imgaddr, uint32 imgsize);
 void ov7725_init(void);