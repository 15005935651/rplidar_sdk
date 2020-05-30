#pragma once


#include "rplidar.h"

#include<vector>

//#define _DLLEXPORT __declspec(dllexport)

using  namespace  rp::standalone::rplidar;

class RplidarInterfaceClass
{
public:
	RplidarInterfaceClass();
	~RplidarInterfaceClass();

	//����������(������������)
    bool ConnectRplidars();

	//�������
	bool StartMotors();

	//��ʼɨ��
	bool StartScans();


	//��ȡ����
	void GetDatas();

	//ֹͣɨ��
	bool StopScans();

	//ֹͣת��
	bool StopMotors();

	//�Ͽ������ͷ�����
	bool DisConnectRplidars();

private:
	//�˿ں�
	const char * opt_com_path = "\\\\.\\com3";

	//������
	_u32         opt_com_baudrate = 115200;

	//����
	RPlidarDriver * drv;

	//�豸��Ϣ
	rplidar_response_device_info_t devinfo;

	//�Ƿ��Ѿ�����
	bool isConnected = false;

	//����Ƿ��Ѿ�����
	bool isMotor = false;

	//�Ƿ��Ѿ���ʼɨ��
	bool isScan = false;


};




