#include "RplidarInterfaceClass.h"

#include <iostream>


//
//int main() {
//	std::cout << "hello";
//
//
//	RplidarInterfaceClass* s = new RplidarInterfaceClass;
//	s->ConnectRplidars();
//	s->StartMotors();
//	s->StartScans();
//
//	std::cin.get();
//	
//
//	vector<RplidarData> rplidarDatas ;
//	rplidarDatas = s->GetDatas();
//	for(RplidarData rpd : rplidarDatas)
//	{
//		cout << rpd.angle;
//		cout << rpd.distance;
//
//	}
//	rplidarDatas.clear();
//
//	s->StopMotors();
//	s->StopScans();
//	s->DisConnectRplidars();
//	delete s;
//	s = nullptr;
//
//	return 0;
//}

RplidarInterfaceClass::RplidarInterfaceClass()
{
	drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
}

RplidarInterfaceClass::~RplidarInterfaceClass()
{
	/*StopScans();
	StopMotors();
	DisConnectRplidars();*/
}

//�����״�
bool RplidarInterfaceClass::ConnectRplidars()
{
	if (isConnected)
	{
		//�״�������;
		return  true;
	}
	if (drv == nullptr) {
		drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
		//�����״������ɹ�;
	}
	if (drv == nullptr) {
		printf("�״���������ʧ��");
		return false;
	}
	if (IS_FAIL(drv->connect(opt_com_path, opt_com_baudrate)))
	{
		//�򿪴���ʧ�ܣ��˿���������ռ��;
		return  true;
	}
	u_result ans = drv->getDeviceInfo(devinfo);
	if (IS_FAIL(ans)) {
		return ans;
	}
	//�����״������ɹ�;
	isConnected = true;
	return  true;
}
//����ת��
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
	//����ת���ɹ�
	return true;
}
//��ʼɨ��
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
				//����ɨ��ɹ�
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

//�ɼ�����
bool RplidarInterfaceClass::GetDatas(RplidarData *temp)
{
	if (!isScan) {
		return false;
	}
	rplidar_response_measurement_node_t nodes[360 * 2];
	size_t   count = _countof(nodes);
	if (IS_OK(drv->grabScanData(nodes, count))) {
		drv->ascendScanData(nodes, count);
		for (int pos = 0; pos < static_cast<int>(count); pos++) {
			temp[pos].angle = (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0f;
			temp[pos].distance = nodes[pos].distance_q2 / 4.0f;
		}
		return true;
	}
	else
	{
		return false;
	}

	
}
// ֹͣɨ��
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
	//ֹͣɨ��ɹ�
	return true;
}
//ֹͣת��
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
	//ֹͣת���ɹ�
	return true;
}

//�Ͽ������ͷ�����
bool RplidarInterfaceClass::DisConnectRplidars()
{
	StopScans();
	StopMotors();
	if (isConnected) {
		if (drv != nullptr) {
			drv->disconnect();
			drv->DisposeDriver(drv);
			//drv = nullptr;
			//�Ͽ����ӳɹ�
			isConnected = false;
			return true;
		}
		else {
			//δ��������
			return  false;

		}
	}
	//�Ͽ����ӳɹ�
	return true;
}

//��������
RplidarInterfaceClass *rplidarInterface;

extern "C" {

	_DLLEXPORT bool ConnectRplidar() {
		
		printf("�״����ӳɹ�");
		rplidarInterface = new RplidarInterfaceClass;
		return rplidarInterface->ConnectRplidars();
	}

	_DLLEXPORT bool DisConnectRplidar() {
		printf("�Ͽ�����");
		rplidarInterface->DisConnectRplidars();
		delete rplidarInterface;
		rplidarInterface = nullptr;
		return true;
	}

	_DLLEXPORT bool StartMotor() {
		return rplidarInterface->StartMotors();
	}

	_DLLEXPORT bool StartScan() {
		return rplidarInterface->StartScans();
	}

	_DLLEXPORT bool StopScan() {
		return rplidarInterface->StopScans();
	}

	_DLLEXPORT bool StopMotor() {
		return rplidarInterface->StopMotors();
	}

	_DLLEXPORT bool GetData(RplidarData * data) {

		return rplidarInterface->GetDatas(data);
		
	}



}

