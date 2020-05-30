#include "RplidarInterfaceClass.h"

#include <iostream>


int main() {
	std::cout << "hello";


	RplidarInterfaceClass* s = new RplidarInterfaceClass;
	s->ConnectRplidars();
	s->StartMotors();
	s->StartScans();

	std::cin.get();
	for (int i = 0; i < 100; i++)
		s->GetDatas();
	s->StopMotors();
	s->StopScans();
	s->DisConnectRplidars();

	return 0;
}

RplidarInterfaceClass::RplidarInterfaceClass()
{
	drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
}

RplidarInterfaceClass::~RplidarInterfaceClass()
{
	StopScans();
	StopMotors();
	DisConnectRplidars();
}

//连接雷达
bool RplidarInterfaceClass::ConnectRplidars()
{
	if (isConnected)
	{
		//雷达已连接;
		return  true;
	}
	if (drv == nullptr) {
		drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
		//创建雷达驱动成功;
	}
	if (drv == nullptr) {
		printf("雷达驱动创建失败");
		return false;
	}
	if (IS_FAIL(drv->connect(opt_com_path, opt_com_baudrate)))
	{
		//打开串口失败，端口其他程序被占用;
		return  true;
	}
	u_result ans = drv->getDeviceInfo(devinfo);
	if (IS_FAIL(ans)) {
		return ans;
	}
	//连接雷达驱动成功;
	isConnected = true;
	return  true;
}
//启动转机
bool RplidarInterfaceClass::StartMotors()
{
	if (drv == nullptr)
	{
		return  false;
	}
	if (!isConnected)
	{
		return  false;
	}
	if (isMotor)
	{
		return  true;
	}
	drv->startMotor();
	isMotor = true;
	//启动转机成功
	return true;
}
//开始扫描
bool RplidarInterfaceClass::StartScans()
{
	if (isConnected&&isMotor)
	{
		if (isScan)
		{
			return false;
		}
		else
		{
			if (drv == nullptr)
			{
				return  false;
			}
			else
			{
				drv->startScan(0,1);
				//启动扫描成功
				isScan = true;
				return  true;
			}
		}
	}
	else
	{
		return  false;
	}
}
//采集数据
void RplidarInterfaceClass::GetDatas()
{

	
	//TempData.Init(FVector2D(0,0),720);
	if (!isScan) {
		return ;
	}
	rplidar_response_measurement_node_t nodes[360 * 2];
	size_t   count = _countof(nodes);
	if (IS_OK(drv->grabScanData(nodes, count))) {
		drv->ascendScanData(nodes, count);
		for (int pos = 0; pos < static_cast<int>(count); pos++) {
			float angle = (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0f;
			float distance = nodes[pos].distance_q2 / 4.0f;
			std::cout << angle << " " << distance<<std::endl;
			

			//if (angle < 90 || angle>270)
			//{
			//	float x = distance * cos(angle*pi / 180);
			//	float y = distance * sin(angle*pi / 180);

			//	if (x > LeftDownAreaPoints.X&&x < RightUpAreaPoints.X)
			//	{
			//		if (y > LeftDownAreaPoints.Y&&y < RightUpAreaPoints.Y)
			//		{
			//			TempData.AddUnique(FVector2D(x, y));
			//		}
			//	}
			//}
		}
	}

	
}
// 停止扫描
bool RplidarInterfaceClass::StopScans()
{

	if (!isConnected)
	{
		return false;
	}
	if (!isScan)
	{
		return  true;
	}
	drv->stop();
	isScan = false;
	//停止扫描成功
	return true;
}
//停止转机
bool RplidarInterfaceClass::StopMotors()
{
	if (!isConnected)
	{
		return  false;
	}
	if (!isMotor)
	{
		return true;
	}
	if (isScan)
	{
		StopScans();
		isScan = false;
	}
	if (drv == nullptr)
	{
		return false;
	}
	if (!drv->stopMotor())
	{
		return false;
	}
	isMotor = false;
	//停止转机成功
	return true;
}

//断开连接释放驱动
bool RplidarInterfaceClass::DisConnectRplidars()
{
	StopScans();
	StopMotors();
	if (isConnected) {
		if (drv != nullptr) {
			drv->disconnect();
			drv->DisposeDriver(drv);
			//断开连接成功
			isConnected = false;
			return true;
		}
		else {
			//未创建连接
			return  false;

		}
	}
	//断开连接成功
	return true;
}

//导出函数
static RplidarInterfaceClass rplidarInterface;

//extern "C" {
//
//	_DLLEXPORT bool ConnectRplidar() {
//		
//		printf("雷达连接成功");
//		return rplidarInterface.ConnectRplidar();
//	}
//
//	_DLLEXPORT bool DisConnectRplidar() {
//		printf("断开连接");
//		return rplidarInterface.DisConnectRplidar();
//	}
//
//	_DLLEXPORT bool StartMotor() {
//		return rplidarInterface.StartMotor();
//	}
//
//	_DLLEXPORT bool StartScan() {
//		return rplidarInterface.StartScan();
//	}
//
//	_DLLEXPORT bool StopScan() {
//		return rplidarInterface.StopScan();
//	}
//
//	_DLLEXPORT bool StopMotor() {
//		return rplidarInterface.StopMotor();
//	}
//
//	_DLLEXPORT bool DisConnectRplidar() {
//		return rplidarInterface.DisConnectRplidar();
//	}
//
//
//}

