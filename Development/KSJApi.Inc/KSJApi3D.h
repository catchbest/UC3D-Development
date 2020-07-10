#ifndef __KSJ_API_3D_H__
#define __KSJ_API_3D_H__


#ifdef KSJAPI_EXPORTS
#define KSJ_API __declspec(dllexport)
#elif defined KSJAPI_STATIC
#define KSJ_API
#else
#define KSJ_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"{
#endif
	
	/*
	初始化和反初始化
	*/
	KSJ_API  int __stdcall KSJ3D_Inital();
	KSJ_API  int __stdcall KSJ3D_UnInitial();


	KSJ_API  int __stdcall KSJ_WrEEPROMExEx(int nChannel, unsigned long ulRomAdress, unsigned char *pData, int nBytes);

	/*
	取得库版本
	*/
	KSJ_API  int __stdcall KSJ3D_GetAPIVersion(int *pnMaj1, int *pnMaj2, int *pnMin1, int *pnMin2);
	
	/*
	取得相机的个数
	*/
	KSJ_API  int __stdcall KSJ3D_GetDeviceCount(int *pnCount);

	/*
	取得相机的信息
	*/
	KSJ_API  int __stdcall KSJ3D_GetDeviceInformation(int nChannel, int* pnDeviceType, int* pnSerialNumber, unsigned short *pwFirmwareVersion, unsigned short *pwFpgaVersion);
	
	/*
	相机是否是3D相机
	*/
	KSJ_API  int __stdcall KSJ3D_Is3DCamera(int nChannel, bool* pb3DCamera);

	/*
	取得相机的名称，pcszName需要64Byte的空间
	*/
	KSJ_API  int __stdcall KSJ3D_GetCameraName(int nChannel, char* pcszName);
	
	/*
	取得实时图像回调接口设置
	*/
	typedef void(__stdcall *KSJ_LIVE_IMAGE_CALLBACK)(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext);
	KSJ_API  int __stdcall KSJ3D_RegisterLiveImageCB(int nChannel, KSJ_LIVE_IMAGE_CALLBACK pfCallback, void *lpContext);

	// nLostProfileNum当前profile与上一个profile之间丢失了多少个profile。nProfileIndex水印序号
	typedef void(__stdcall *KSJ_PROFILE_DATA_CALLBACK)(int nChannel, int nPointNum, float fYMillimeters, int nProfileIndex, float *x, float *z, int nLostProfileNum, void *lpContext);
	// 获取到Profile数据，x，z
	KSJ_API  int __stdcall KSJ3D_RegisterProfileDataCB(int nChannel, KSJ_PROFILE_DATA_CALLBACK pfCallback, void *lpContext);

	/*
	取得profile图像回调接口设置
	*/
	typedef void(__stdcall *KSJ_PROFILE_IMAGE_CALLBACK)(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext);
	// 通过zmap转换的profile图像
	KSJ_API  int __stdcall KSJ3D_RegisterProfileImageCB(int nChannel, KSJ_PROFILE_IMAGE_CALLBACK pfCallback, void *lpContext);

	// nTotalLostProfileNum最后的点云数据共丢失了多少个profile，nProfileNum表示实际采集的profile，x,y,z数字元素个数需要为nPointNum。
	typedef void(__stdcall *KSJ_POINT_CLOUD_DATA_CALLBACK)(int nChannel, int nTotalPointNum, int nProfileNum, float *x, float *y, float *z, int nTotalLostProfileNum, void *lpContext);
	// 获取合成的profile 3d点云数据，x，y，z
	KSJ_API  int __stdcall KSJ3D_RegisterPointCloudDataCB(int nChannel, KSJ_POINT_CLOUD_DATA_CALLBACK pfCallback, void *lpContext);

	/*
	取得点云数据回调接口设置
	*/
	typedef void(__stdcall *KSJ_POINT_CLOUD_IMAGE_CALLBACK)(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext);
	// 获取zmap转换的3d图像
	KSJ_API  int __stdcall KSJ3D_RegisterPointCloudImageCB(int nChannel, KSJ_POINT_CLOUD_IMAGE_CALLBACK pfCallback, void *lpContext);
	
	/*
	设置ROI
	*/
	KSJ_API  int __stdcall KSJ3D_GetRoiMax(int nChannel, int* pnColMax, int* pnRowMax);
	KSJ_API  int __stdcall KSJ3D_SetRoi(int nChannel, int nColStart, int nRowStart, int nColSize, int nRowSize);
	KSJ_API  int __stdcall KSJ3D_GetRoi(int nChannel, int* pnColStart, int* pnRowStart, int* pnColSize, int* pnRowSize);

	/*
	设置相机曝光
	*/
	KSJ_API  int __stdcall KSJ3D_GetExposureTimeRange(int nChannel, float* pfExpTimeMSMin, float* pfExpTimeMSMax);
	KSJ_API  int __stdcall KSJ3D_SetExposureTime(int nChannel, float fExpTimeMS);
	KSJ_API  int __stdcall KSJ3D_GetExposureTime(int nChannel, float* pfExpTimeMS);

	/*
	设置相机增益
	*/
	KSJ_API  int __stdcall KSJ3D_GetGainRange(int nChannel, int* pnGainMin, int* pnGainMax);
	KSJ_API  int __stdcall KSJ3D_SetGain(int nChannel, int nGain);
	KSJ_API  int __stdcall KSJ3D_GetGain(int nChannel, int* pnGain);

	/*
	设置工作开始的触发模式
	StartTriggerSource - 这个暂时先保留，目前还不需要设置，使用默认的就行
	enable - 是否启用开始采集信号
	TriggerEdgeMode - 开始信号的模式
	*/
	enum  	KSJ3D_START_TRIGGER_SOURCE {
		STS_INPUT_0 = 0,     // 目前只有一个start trigger输入源
	};

	enum  	KSJ3D_TRIGGER_EDGE_MODE {
		TEM_RISING_EDGE = 0,
		TEM_FALLING_EDGE = 1
	};
	
	KSJ_API  int __stdcall KSJ3D_SetStartTrigger(int nChannel, KSJ3D_START_TRIGGER_SOURCE  StartTriggerSource,  bool bEnable,  KSJ3D_TRIGGER_EDGE_MODE  StartTriggerCondition);
	KSJ_API  int __stdcall KSJ3D_GetStartTrigger(int nChannel, KSJ3D_START_TRIGGER_SOURCE *pStartTriggerSource, bool *pbEnable, KSJ3D_TRIGGER_EDGE_MODE *pStartTriggerCondition);

	/*
	设置Start触发信号参数
	triggerFilter - 信号滤波
	*/
	KSJ_API  int __stdcall KSJ3D_SetStartTriggerParameters(int nChannel, int StartTriggerFilter, int nStartTriggerDelay);
	KSJ_API  int __stdcall KSJ3D_GetStartTriggerParameters(int nChannel, int* pStartTriggerFilter, int *pnStartTriggerDelay);

	// 设置数据触发模式
	enum  	KSJ3D_DATA_TRIGGER_MODE
	{
		DTM_FREE_RUN = 0,            // 自由触发，以最快的帧率进行触发
		DTM_INTERNAL = 1,            // 内部根据触发频率设置进行自动触发
		DTM_EXTERNAL = 2             // 根据外部触发信号进行触发
	};
	KSJ_API  int __stdcall KSJ3D_SetDataTriggerMode(int nChannel, KSJ3D_DATA_TRIGGER_MODE DataTriggerMode);
	KSJ_API  int __stdcall KSJ3D_GetDataTriggerMode(int nChannel, KSJ3D_DATA_TRIGGER_MODE *pDataTriggerMode);

	// 设置在DTM_INTERNAL数据触发模式下的触发频率
	KSJ_API  int __stdcall KSJ3D_SetDataTriggerInternalFrequency(int nChannel, int nInternalFrequencyHz);
	KSJ_API  int __stdcall KSJ3D_GetDataTriggerInternalFrequency(int nChannel, int *pnInternalFrequencyHz);

	/*
	设置数据外触发信号参数
	nTriggerDivider - 多个信号触发一次
	nTriggerDelay - 开始丢弃信号数
	nTriggerFilter - 信号滤波
	DataTriggerCondition - 信号Method：上升沿/下降沿
	*/
	// 设置在DTM_EXTERNAL数据触发模式下的相关设置（分频、延时、滤波、触发模式）
	KSJ_API  int __stdcall KSJ3D_SetDataTriggerExternalTriggerParameters(int nChannel, int nDataTriggerDivider, int nDataTriggerDelay, int nDataTriggerFilter, KSJ3D_TRIGGER_EDGE_MODE DataTriggerCondition);
	KSJ_API  int __stdcall KSJ3D_GetDataTriggerExternalTriggerParameters(int nChannel, int *pnDataTriggerDivider, int *pnDataTriggerDelay, int* pnDataTriggerFilter, KSJ3D_TRIGGER_EDGE_MODE *pDataTriggerCondition);


	/*
	这是3D点云图像的profiles数（目前对应咱们的multi line）
	*/
	KSJ_API  int __stdcall KSJ3D_SetMaxNumberOfProfilesToCapture(int nChannel, int nNumberOfProfiles);
	KSJ_API  int __stdcall KSJ3D_GetMaxNumberOfProfilesToCapture(int nChannel, int* pNumberOfProfiles);


	/*
	开始和停止3D处理(采集)线程
	*/
	KSJ_API  int __stdcall KSJ3D_StartAcquisition(int nChannel);
	KSJ_API  int __stdcall KSJ3D_StopAcquisition(int nChannel);


	// 是否仅返回有效点（Z值为-1000的时候，为无效）
	KSJ_API int __stdcall KSJ3D_SetObliterateInvalidData(int nChannel, bool bObliterate);
	KSJ_API int __stdcall KSJ3D_GetObliterateInvalidData(int nChannel, bool* bObliterate);

	// nThreadhold [0-255], 灰度值下限，用于计算profile的阈值
	KSJ_API  int __stdcall KSJ3D_Set3DLaserLineBrightnessThreshold(int nChannel, int nThreshold);
	KSJ_API  int __stdcall KSJ3D_Get3DLaserLineBrightnessThreshold(int nChannel, int *pnThreshold);

	enum KSJ_LASER_MODE
	{
		LM_NORMAL_OPEN = 0,
		LM_NORMAL_CLOSE = 1,
		LM_FLASH
	};
	// 设置线激光工作模式，常开、常关、频闪
	KSJ_API  int __stdcall KSJ3D_LaserModeSet(int nChannel, KSJ_LASER_MODE LaserMode);
	KSJ_API  int __stdcall KSJ3D_LaserModeGet(int nChannel, KSJ_LASER_MODE *pLaserMode);

	
	// 得到高度测量范围，近端视野，远端视野
	KSJ_API  int __stdcall KSJ3D_GetMeasurementRange(int nChannel, float *pfZMin, float *pfZMax, float *pfFovNear, float *pfFovFar);

	// 决定高度测量数据转成图像灰度数据的显示效果，fZMin(mm)映射为灰度值0，fZMax（mm）映射为灰度值255
	KSJ_API  int __stdcall KSJ3D_SetZMap(int nChannel, float fZMin, float fZMax);
	KSJ_API  int __stdcall KSJ3D_GetZMap(int nChannel, float *pfZMin, float *pfZMax);

	// Y方向（运动方向）的分辨率
	KSJ_API  int __stdcall KSJ3D_SetYResolution(int nChannel, float fYResolution);
	KSJ_API  int __stdcall KSJ3D_GetYResolution(int nChannel, float *pfYResolution);

	// 传输帧率，根据水印时间计算，实际获取的帧数
	KSJ_API  int __stdcall KSJ3D_GetTransmissionRate(int nChannel, float *pfTransmissionRate);

	// 不考虑丢帧，根据水印的计数和时间计算
	KSJ_API  int __stdcall KSJ3D_GetExternalTriggerRate(int nChannel, float *pfExternalTriggerRateHz);

	KSJ_API  int __stdcall KSJ3D_HelperSaveToPCD(int nWidth, int nHeight, float *pfX, float *pfY, float *pfZ, const TCHAR *pszFileName);

	// KSJ3D_StartAcquisition启动后不允许调用
	// KSJ_API  int __stdcall KSJ3D_CaptureLiveImage(int nChannel, unsigned char *pLiveImageData, int *pnWidth, int *pnHeight, int *pnBitCount);
	// 传入image数据，计算profile标定后的测量数据，KSJ3D_StartAcquisition启动后不允许调用
	// KSJ_API  int __stdcall KSJ3D_CalculateProfileDataFromImage(int nChannel, unsigned char *pLiveImageData, float *pfX, float *pfZ);
	// KSJ_API  int __stdcall KSJ3D_MapProfileDataToImage(int nChannel, float *pfX, float *pfZ, int nPointNum);



#ifdef __cplusplus
}
#endif

#endif
