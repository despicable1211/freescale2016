
extern uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
extern uint8 img[CAMERA_H][CAMERA_W];

//��������
 void PORTB_IRQHandler();
 void DMA0_IRQHandler();
 void sendimg(void *imgaddr, uint32 imgsize);
 void ov7725_init(void);