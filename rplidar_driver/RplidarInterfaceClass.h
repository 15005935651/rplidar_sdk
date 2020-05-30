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

	//建立了连接(包括创建驱动)
    bool ConnectRplidars();

	//启动电机
	bool StartMotors();

	//开始扫描
	bool StartScans();


	//获取数据
	void GetDatas();

	//停止扫描
	bool StopScans();

	//停止转机
	bool StopMotors();

	//断开连接释放驱动
	bool DisConnectRplidars();

private:
	//端口号
	const char * opt_com_path = "\\\\.\\com3";

	//波特率
	_u32         opt_com_baudrate = 115200;

	//驱动
	RPlidarDriver * drv;

	//设备信息
	rplidar_response_device_info_t devinfo;

	//是否已经连接
	bool isConnected = false;

	//电机是否已经启动
	bool isMotor = false;

	//是否已经开始扫描
	bool isScan = false;


};




