/*
 * Delay.h
 *
 *  Created on: 2018年2月24日
 *      Author: mqvista
 */

#ifndef DELAY_H_
#define DELAY_H_

#define  SYSCLK   72.0               ///ָ��CPU����Ƶ��Ϊ72MHz
#define  A             6.0           //һ��ѭ��������������
#define  B             3.0           //���á���ʼ���������ܹ����õ�������
#define  delay_us(nus)   wait(((nus)*(SYSCLK)-(B))/(A))
#define  delay_ms(nms)   delay_us((nms)*1000)
#define  delay_s(ns)     delay_ms((ns)*1000)

class Delay {
public:
	Delay();
	void wait(unsigned long n);
};

#endif /* DELAY_H_ */
