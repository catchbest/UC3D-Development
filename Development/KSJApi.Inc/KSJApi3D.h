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
	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_Inital
	/// @brief     初始化KSJApi动态库
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 一般在程序初始化时调用
	///          \li 相机在这个执行这个函数之前与电脑连接好
	///          \li 如果需要更新设备，请执行反初始化KSJ_UnInit()后再执行KSJ3D_Inital()，需要特别注意的是：重新这样操作以后，具体物理相机所对应的索引号可能会改变。
	///          \li 如果需要操作指定的相机，需要给物理相机分配不同的序号Serials（KSJ_SetSerials()），根据不同的序号关联到当前的索引号
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_Inital();

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_UnInitial
	/// @brief     反初始化KSJApi动态库
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后，释放分配资源并关闭本进程中所有已经打开的设备
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_UnInitial();

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_GetVersion
	/// @brief     获取KSJAPI库的版本号
	/// @param     pnMaj1[out] 返回主版本号1，一般做重要整体结构调整时会改变，增加1；主版本号1不同的API库不兼容
	/// @param     pnMaj2[out] 返回主版本号2，一般做主要功能增加时会改变，增加1
	/// @param     pnMin1[out] 返回次版本号1，一般做重要整体结构调整或者修改主要逻辑时会改变，增加1
	/// @param     pnMin2[out] 返回次版本号2，一般在小的改动、修改Bug时会改变，增加1
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetAPIVersion(int *pnMaj1, int *pnMaj2, int *pnMin1, int *pnMin2);
	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_DeviceGetCount
	/// @brief     得到设备数目
	/// @return    连接到主机上的KSJ设备数目
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetDeviceCount(int *pnCount);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_DeviceGetInformationEx
	/// @brief     得到设备信息（型号，序号，固件版本号）
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pusDeviceType[out] 返回设备类型的地址指针
	/// @param     pnSerialNumber[out] 返回设备序号的地址指针，通过设个序号可以区别不同的物理相机，这个序号可以通过KSJ3D_SetSerialNumber()修改
	/// @param     pusFirmwareVersion[out] 返回设备固件版本号的地址指针
	/// @param     pusFpgaVersion[out] 返回设备FPGA版本号的地址指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetDeviceInformation(int nChannel, int* pnDeviceType, int* pnSerialNumber, unsigned short *pwFirmwareVersion, unsigned short *pwFpgaVersion);
	
	///-----------------------------------------------------------------------------
	/// @brief     KSJ3D_SetSerialNumber
	/// @brief     设置相机序号
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     btSerials [in] 将序号设置到当前相机
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	/// @attention 序号即KSJ3D_GetDeviceInformation所获取的pnSerialNumber
	///-----------------------------------------------------------------------------
	KSJ_API int __stdcall  KSJ3D_SetSerialNumber(int nChannel, unsigned char btSerials);

	///-----------------------------------------------------------------------------
	/// @brief     KSJ3D_Is3DCamera
	/// @brief     判断一个KSJ相机是否是3D相机
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pb3DCamera[out] 返回相机是否是3D相机
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_Is3DCamera(int nChannel, bool* pb3DCamera);

	///-----------------------------------------------------------------------------
	/// @brief     KSJ3D_GetCameraName
	/// @brief     判断一个KSJ相机是否是3D相机
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pcszName[out] 返回相机是否是3D相机
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetCameraName(int nChannel, char* pcszName);

	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_LIVE_IMAGE_CALLBACK
	/// @brief     实时图像回调函数定义
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pData [out] 指向当前图像数据的内存指针
	/// @param     nWidth [out] 图像数据的宽度
	/// @param     nHeight [out] 图像数据的高度
	/// @param     nBitCount [out] 图像数据的位图深度
	/// @param     lpContext [out] 用户上下文指针，这个指针是用户调用KSJ3D_RegisterLiveImageCB时传入的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	typedef void(__stdcall *KSJ_LIVE_IMAGE_CALLBACK)(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_RegisterLiveImageCB
	/// @brief     设置实时图像的回调函数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfCallback [in] 用户定义的实时图像回调函数指针
	/// @param     lpContext [in] 回调函数的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_RegisterLiveImageCB(int nChannel, KSJ_LIVE_IMAGE_CALLBACK pfCallback, void *lpContext);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_PROFILE_DATA_CALLBACK
	/// @brief     轮廓数据回调函数定义
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nPointNum [out] 当前轮廓数据的点数
	/// @param     fYMillimeters [out] 当前轮廓数据在运动方向相对于开始轮廓的位置，单位：毫米
	/// @param     nProfileIndex [out] 当前轮廓的序号
	/// @param     x [out] 指向当前轮廓数据的x方向内存指针
	/// @param     z [out] 指向当前轮廓数据的z方向内存指针
	/// @param     nLostProfileNum [out] 当前轮廓与上一个轮廓之间丢失的轮廓数
	/// @param     lpContext [out] 用户上下文指针，这个指针是用户调用KSJ3D_RegisterProfileDataCB时传入的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	typedef void(__stdcall *KSJ_PROFILE_DATA_CALLBACK)(int nChannel, int nPointNum, float fYMillimeters, int nProfileIndex, float *x, float *z, int nLostProfileNum, void *lpContext);
	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_RegisterProfileDataCB
	/// @brief     设置轮廓数据的回调函数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfCallback [in] 用户定义的轮廓数据回调函数指针
	/// @param     lpContext [in] 回调函数的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_RegisterProfileDataCB(int nChannel, KSJ_PROFILE_DATA_CALLBACK pfCallback, void *lpContext);

	/// 轮廓数据参数
	typedef struct _tag_PROFILE_DATA_PARAM
	{
		int     nPointNum;                  ///< 轮廓数据的点数
		float   fYMillimeters;              ///< 轮廓数据在运动方向相对于开始轮廓的位置，单位：毫米
		int     nProfileIndex;              ///< 轮廓的序号
		int     nLostProfileNum;            ///< 轮廓与上一个轮廓之间丢失的轮廓数
		unsigned long long  ullTimesTamp;   ///< 轮廓的时间戳
	}PROFILE_DATA_PARAM, *PPROFILE_DATA_PARAM;

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_PROFILE_DATA_CALLBACKEX
	/// @brief     轮廓数据回调函数定义
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     x [out] 指向当前轮廓数据的x方向内存指针
	/// @param     z [out] 指向当前轮廓数据的z方向内存指针
	/// @param     pParam [out] 指向当前轮廓数据参数的内存指针
	/// @param     lpContext [out] 用户上下文指针，这个指针是用户调用KSJ3D_RegisterProfileDataCBEx时传入的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	typedef void(__stdcall *KSJ_PROFILE_DATA_CALLBACKEX)(int nChannel, float *x, float *z, PROFILE_DATA_PARAM* pParam, void *lpContext);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_RegisterProfileDataCBEx
	/// @brief     设置轮廓数据的回调函数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfCallback [in] 用户定义的轮廓数据回调函数指针
	/// @param     lpContext [in] 回调函数的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_RegisterProfileDataCBEx(int nChannel, KSJ_PROFILE_DATA_CALLBACKEX pfCallback, void *lpContext);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_PROFILE_IMAGE_CALLBACK
	/// @brief     轮廓图像回调函数定义
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pData [out] 指向轮廓图像数据的内存指针
	/// @param     nWidth [out] 图像数据的宽度
	/// @param     nHeight [out] 图像数据的高度
	/// @param     nBitCount [out] 图像数据的位图深度
	/// @param     lpContext [out] 用户上下文指针，这个指针是用户调用KSJ3D_RegisterProfileImageCB时传入的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	typedef void(__stdcall *KSJ_PROFILE_IMAGE_CALLBACK)(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext);
	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_RegisterProfileImageCB
	/// @brief     设置轮廓图像的回调函数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfCallback [in] 用户定义的轮廓图像回调函数指针
	/// @param     lpContext [in] 回调函数的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_RegisterProfileImageCB(int nChannel, KSJ_PROFILE_IMAGE_CALLBACK pfCallback, void *lpContext);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_POINT_CLOUD_DATA_CALLBACK
	/// @brief     点云数据回调函数定义
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nTotalPointNum [out] 点云数据总点数
	/// @param     nProfileNum [out] 轮廓的数量
	/// @param     x [out] 指向当前点云数据的x方向内存指针
	/// @param     y [out] 指向当前点云数据的y方向内存指针
	/// @param     z [out] 指向当前点云数据的z方向内存指针
	/// @param     nTotalLostProfileNum [out] 总共丢失的轮廓数
	/// @param     lpContext [out] 用户上下文指针，这个指针是用户调用KSJ3D_RegisterPointCloudDataCB时传入的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	typedef void(__stdcall *KSJ_POINT_CLOUD_DATA_CALLBACK)(int nChannel, int nTotalPointNum, int nProfileNum, float *x, float *y, float *z, int nTotalLostProfileNum, void *lpContext);
	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_RegisterPointCloudDataCB
	/// @brief     设置点云数据的回调函数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfCallback [in] 用户定义的点云数据回调函数指针
	/// @param     lpContext [in] 回调函数的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_RegisterPointCloudDataCB(int nChannel, KSJ_POINT_CLOUD_DATA_CALLBACK pfCallback, void *lpContext);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_POINT_CLOUD_IMAGE_CALLBACK
	/// @brief     点云数据Zmap图像回调函数定义
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pData [out] 指向轮廓图像数据的内存指针
	/// @param     nWidth [out] 图像数据的宽度
	/// @param     nHeight [out] 图像数据的高度
	/// @param     nBitCount [out] 图像数据的位图深度
	/// @param     lpContext [out] 用户上下文指针，这个指针是用户调用KSJ3D_RegisterProfileImageCB时传入的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	typedef void(__stdcall *KSJ_POINT_CLOUD_IMAGE_CALLBACK)(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext);
	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_RegisterPointCloudImageCB
	/// @brief     设置点云数据Zmap图像的回调函数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfCallback [in] 用户定义的点云数据回调函数指针
	/// @param     lpContext [in] 回调函数的上下文指针
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_RegisterPointCloudImageCB(int nChannel, KSJ_POINT_CLOUD_IMAGE_CALLBACK pfCallback, void *lpContext);
	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetRoiMax
	/// @brief     获取3D ROI的范围
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pnColMax [out] 水平的最大设置范围
	/// @param     pnRowMax [out] 垂直行的最大设置范围
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetRoiMax(int nChannel, int* pnColMax, int* pnRowMax);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetRoi
	/// @brief     设置视场范围
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nColStart [in] 水平像素起始位置
	/// @param     nRowStart [in] 垂直行起始位置
	/// @param     nColSize [in] 水平像素数
	/// @param     nRowSize [in] 垂直行数
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetRoi(int nChannel, int nColStart, int nRowStart, int nColSize, int nRowSize);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetRoi
	/// @brief     获取视场范围
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pnColStart [out] 水平像素起始位置
	/// @param     pnRowStart [out] 垂直行起始位置
	/// @param     pnColSize [out] 水平像素数
	/// @param     pnRowSize [out] 垂直行数
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetRoi(int nChannel, int* pnColStart, int* pnRowStart, int* pnColSize, int* pnRowSize);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetExposureTimeRange
	/// @brief     获取相机的曝光范围
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfExpTimeMSMin [out] 最小曝光，单位：毫秒
	/// @param     pfExpTimeMSMax [out] 最大曝光，单位：毫秒
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetExposureTimeRange(int nChannel, float* pfExpTimeMSMin, float* pfExpTimeMSMax);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetExposureTime
	/// @brief     设置相机曝光
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     fExpTimeMS [in] 曝光值，单位：毫秒
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetExposureTime(int nChannel, float fExpTimeMS);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetExposureTime
	/// @brief     获取相机曝光
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     fExpTimeMS [out] 当前相机的曝光值，单位：毫秒
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetExposureTime(int nChannel, float* pfExpTimeMS);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetGainRange
	/// @brief     获取相机的增益范围
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pnGainMin [out] 最小增益
	/// @param     pnGainMax [out] 最大增益
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetGainRange(int nChannel, int* pnGainMin, int* pnGainMax);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetGain
	/// @brief     设置相机增益
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nGain [in] 增益值
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetGain(int nChannel, int nGain);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetGain
	/// @brief     获取相机增益
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     fExpTimeMS [out] 当前相机的增益值
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetGain(int nChannel, int* pnGain);

	enum  	KSJ3D_START_TRIGGER_SOURCE {
		STS_INPUT_0 = 0,     ///< 开始采集信号源 INPUT0
	};

	/// 开始采集信号模式
	enum  	KSJ3D_TRIGGER_EDGE_MODE {
		TEM_RISING_EDGE = 0,   ///< 上升沿
		TEM_FALLING_EDGE = 1,  ///< 下降沿
		TEM_HIGHLEVEL = 2,	   ///< 高电平有效，高电平的时候相机开始进入采集图像状态
		TEM_LOWLEVEL = 3       ///< 低电平有效，低电平的时候相机开始进入采集图像状态
	};
	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetStartTrigger
	/// @brief     设置相机开始采集信号
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     StartTriggerSource [in] 信号输入接口
	/// @param     bEnable [in] 是否启用开始采集信号，如果设置为false，那么相机将会一直处于工作状态，只有有Data Trigger信号就会采集图像
	/// @param     StartTriggerCondition [in] 开始采集信号模式，参考KSJ3D_TRIGGER_EDGE_MODE定义
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetStartTrigger(int nChannel, KSJ3D_START_TRIGGER_SOURCE  StartTriggerSource,  bool bEnable,  KSJ3D_TRIGGER_EDGE_MODE  StartTriggerCondition);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetStartTrigger
	/// @brief     获取开始采集信号
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pStartTriggerSource [out] 信号输入接口
	/// @param     pbEnable [out] 是否启用开始采集信号
	/// @param     pStartTriggerCondition [out] 触发条件，参考KSJ3D_TRIGGER_EDGE_MODE定义
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetStartTrigger(int nChannel, KSJ3D_START_TRIGGER_SOURCE *pStartTriggerSource, bool *pbEnable, KSJ3D_TRIGGER_EDGE_MODE *pStartTriggerCondition);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetStartTriggerParameters
	/// @brief     设置相机开始采集信号参数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     StartTriggerFilter [in] 信号滤波时间，单位：微秒
	/// @param     nStartTriggerDelay [in] 信号延迟；该参数表示在收到Start Trigger信号后，将会忽略掉nStartTriggerDelay个Data Trigger信号之后，才开始采集
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetStartTriggerParameters(int nChannel, int StartTriggerFilter, int nStartTriggerDelay);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetStartTriggerParameters
	/// @brief     获取相机开始采集信号参数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pStartTriggerFilter [out] 信号滤波时间，单位：微秒
	/// @param     pnStartTriggerDelay [out] 信号延迟；该参数表示在收到Start Trigger信号后，将会忽略掉nStartTriggerDelay个Data Trigger信号之后，才开始采集
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetStartTriggerParameters(int nChannel, int* pStartTriggerFilter, int *pnStartTriggerDelay);

	// 设置数据触发模式
	enum  	KSJ3D_DATA_TRIGGER_MODE
	{
		DTM_FREE_RUN = 0,            ///< 自由触发，以最快的帧率进行触发
		DTM_INTERNAL = 1,            ///< 内部根据触发频率设置进行自动触发
		DTM_EXTERNAL = 2             ///< 根据外部触发信号进行触发
	};

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetDataTriggerMode
	/// @brief     设置相机数据触发信号模式
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     DataTriggerMode [in] 数据触发信号模式，参考KSJ3D_DATA_TRIGGER_MODE定义
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetDataTriggerMode(int nChannel, KSJ3D_DATA_TRIGGER_MODE DataTriggerMode);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetDataTriggerMode
	/// @brief     获取相机数据触发信号模式
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pDataTriggerMode [out] 数据触发信号模式，参考KSJ3D_DATA_TRIGGER_MODE定义
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetDataTriggerMode(int nChannel, KSJ3D_DATA_TRIGGER_MODE *pDataTriggerMode);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetDataTriggerInternalFrequency
	/// @brief     设置内部触发模式（DTM_INTERNAL）时的触发频率
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nInternalFrequencyHz [in] 触发频率
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetDataTriggerInternalFrequency(int nChannel, int nInternalFrequencyHz);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetDataTriggerInternalFrequency
	/// @brief     获取当前内部触发模式时的触发频率
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pnInternalFrequencyHz [out] 触发频率
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetDataTriggerInternalFrequency(int nChannel, int *pnInternalFrequencyHz);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetDataTriggerExternalTriggerParameters
	/// @brief     设置外触发模式（DTM_EXTERNAL）的参数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nDataTriggerDivider [in] 触发信号分频参数，即多个信号触发一次
	/// @param     nDataTriggerDelay [in] 触发延迟，当前不使用
	/// @param     nDataTriggerFilter [in] 触发信号滤波，单位：微秒
	/// @param     DataTriggerCondition [in] 触发信号模式，参考KSJ3D_DATA_TRIGGER_MODE的定义
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetDataTriggerExternalTriggerParameters(int nChannel, int nDataTriggerDivider, int nDataTriggerDelay, int nDataTriggerFilter, KSJ3D_TRIGGER_EDGE_MODE DataTriggerCondition);
	
	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetDataTriggerExternalTriggerParameters
	/// @brief     获取外触发模式（DTM_EXTERNAL）的参数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pnDataTriggerDivider [out] 触发信号分频参数，即多个信号触发一次
	/// @param     pnDataTriggerDelay [out] 触发延迟，当前不使用
	/// @param     pnDataTriggerFilter [out] 触发信号滤波，单位：微秒
	/// @param     pDataTriggerCondition [out] 触发信号模式，参考KSJ3D_DATA_TRIGGER_MODE的定义
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetDataTriggerExternalTriggerParameters(int nChannel, int *pnDataTriggerDivider, int *pnDataTriggerDelay, int* pnDataTriggerFilter, KSJ3D_TRIGGER_EDGE_MODE *pDataTriggerCondition);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetMaxNumberOfProfilesToCapture
	/// @brief     设置一帧3D图像的最大轮廓数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nNumberOfProfiles [in] 轮廓数
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetMaxNumberOfProfilesToCapture(int nChannel, int nNumberOfProfiles);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetMaxNumberOfProfilesToCapture
	/// @brief     获取一帧3D图像的最大轮廓数
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pNumberOfProfiles [out] 轮廓数
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetMaxNumberOfProfilesToCapture(int nChannel, int* pNumberOfProfiles);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_StartAcquisition
	/// @brief     启动3D处理(采集)线程
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_StartAcquisition(int nChannel);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_StartAcquisition
	/// @brief     停止3D处理(采集)线程
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_StopAcquisition(int nChannel);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetObliterateInvalidData
	/// @brief     设置数据回调是否返回无效的点（Z值为-1000的时候，为无效）
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     bObliterate [in] 是否自动删除无效的点；如果为true，那么点云数据的长度将是变化的，如果为false，那么无效的点的Z值设定为-1000
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API int __stdcall KSJ3D_SetObliterateInvalidData(int nChannel, bool bObliterate);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetObliterateInvalidData
	/// @brief     获取数据回调是否返回无效的点
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     bObliterate [out] 是否自动删除无效的点
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API int __stdcall KSJ3D_GetObliterateInvalidData(int nChannel, bool* bObliterate);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_Set3DLaserLineBrightnessThreshold
	/// @brief     设置激光线的阈值
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     nThreshold [in] 阈值范围[0-255], 灰度值下限，用于计算profile的阈值
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_Set3DLaserLineBrightnessThreshold(int nChannel, int nThreshold);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_Get3DLaserLineBrightnessThreshold
	/// @brief     获取激光线的阈值
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pnThreshold [in] 阈值
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_Get3DLaserLineBrightnessThreshold(int nChannel, int *pnThreshold);

	/// 设置线激光工作模式
	enum KSJ_LASER_MODE
	{
		LM_NORMAL_OPEN = 0,      ///< 常开
		LM_NORMAL_CLOSE = 1,     ///< 常关
		LM_FLASH                 ///< 频闪
	};

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_LaserModeSet
	/// @brief     设置线激光工作模式
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     LaserMode [in] 线激光工作模式， 参考KSJ_LASER_MODE定义
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_LaserModeSet(int nChannel, KSJ_LASER_MODE LaserMode);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_LaserModeGet
	/// @brief     获取线激光工作模式
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pLaserMode [out] 线激光工作模式， 参考KSJ_LASER_MODE定义
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_LaserModeGet(int nChannel, KSJ_LASER_MODE *pLaserMode);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetMeasurementRangeMax
	/// @brief     获取相机的最大测量范围
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfZ [out] 高度测量范围；单位：毫米
	/// @param     pfFovNear [out] 近端视野；单位：毫米
	/// @param     pfFovFar [out] 远端视野；单位：毫米
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetMeasurementRangeMax(int nChannel, float *pfZ, float *pfFovNear, float *pfFovFar);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetMeasurementRangeMax
	/// @brief     获取当前相机的测量范围
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfZMin [out] 高度相对零点最小测量位置；单位：毫米
	/// @param     pfZMax [out] 高度相对零点最大测量位置；单位：毫米
	/// @param     pfFovNear [out] 近端视野；单位：毫米
	/// @param     pfFovFar [out] 远端视野；单位：毫米
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetMeasurementRange(int nChannel, float *pfZMin, float *pfZMax, float *pfFovNear, float *pfFovFar);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetZMap
	/// @brief     设置点云数据转到到Zmap图像的高度映射关系
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     fZMin [in] 映射为灰度值0的高度值
	/// @param     fZMax [in] 映射为灰度值255的高度值
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetZMap(int nChannel, float fZMin, float fZMax);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetZMap
	/// @brief     获取点云数据转到到Zmap图像的高度映射关系
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfZMin [out] 映射为灰度值0的高度值
	/// @param     pfZMax [out] 映射为灰度值255的高度值
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetZMap(int nChannel, float *pfZMin, float *pfZMax);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_SetYResolution
	/// @brief     设置Y方向（运动方向）的分辨率
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     fYResolution [in] Y方向（运动方向）的分辨率；单位：毫米
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetYResolution(int nChannel, float fYResolution);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetYResolution
	/// @brief     获取Y方向（运动方向）的分辨率
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfYResolution [out] Y方向（运动方向）的分辨率；单位：毫米
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetYResolution(int nChannel, float *pfYResolution);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetTransmissionRate
	/// @brief     获取传输帧率
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfTransmissionRate [out] 传输帧率
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetTransmissionRate(int nChannel, float *pfTransmissionRate);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetExternalTriggerRate
	/// @brief     获取外触发信号的频率
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pfExternalTriggerRateHz [out] 外触发信号的频率
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetExternalTriggerRate(int nChannel, float *pfExternalTriggerRateHz);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_HelperSaveToPCD
	/// @brief     将3D点云数据保存为PCD数据文件
	/// @param     nWidth [in] 3D数据帧的宽度
	/// @param     nHeight [in] 3D数据帧的高度，profile数
	/// @param     pfX [in] 保存3D数据帧的X方向的数据地址指针
	/// @param     pfY [in] 保存3D数据帧的Y方向的数据地址指针
	/// @param     pfZ [in] 保存3D数据帧的Z方向的数据地址指针
	/// @param     pszFileName [in] PCD文件路径
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 可以任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_HelperSaveToPCD(int nWidth, int nHeight, float *pfX, float *pfY, float *pfZ, const TCHAR *pszFileName);

	// KSJ3D_StartAcquisition启动后不允许调用
	// KSJ_API  int __stdcall KSJ3D_CaptureLiveImage(int nChannel, unsigned char *pLiveImageData, int *pnWidth, int *pnHeight, int *pnBitCount);
	// 传入image数据，计算profile标定后的测量数据，KSJ3D_StartAcquisition启动后不允许调用
	// KSJ_API  int __stdcall KSJ3D_CalculateProfileDataFromImage(int nChannel, unsigned char *pLiveImageData, float *pfX, float *pfZ);
	// KSJ_API  int __stdcall KSJ3D_MapProfileDataToImage(int nChannel, float *pfX, float *pfZ, int nPointNum);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_DeviceOpen
	/// @brief     打开指定索引的设备
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 可以任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_DeviceOpen(int nChannel);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_DeviceOpen
	/// @brief     关闭指定索引的设备，如果不关闭指定相机，那么在多进程下是不能操作这台相机
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_DeviceClose(int nChannel);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_DeviceGetStatus
	/// @brief     获取设备在当前进程中的开启关闭状态
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pbOpen [out] 设备的开启状态
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_DeviceGetStatus(int nChannel, bool* pbOpen);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetDriveVersion
	/// @brief     获取当前设备的驱动版本号
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @param     pnMaj1[out] 返回主版本号1
	/// @param     pnMaj2[out] 返回主版本号2
	/// @param     pnMin1[out] 返回次版本号1
	/// @param     pnMin2[out] 返回次版本号2
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 调用KSJ3D_Inital函数初始化后调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetDriveVersion(int nChannel, int *pnMaj1, int *pnMaj2, int *pnMin1, int *pnMin2);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_LogSet
	/// @brief     设置Log日志是否输出及输出的保存目录
	/// @param     bEnable [in] 是否打开Log日志的输出功能
	/// @param     pszFolder [in] 输出的Log日志所保存的目录；可以为NULL或空字符串。
	///				\li 如果为NULL或空字符串，则Log输出目录会自动创建，目录位置在KSJAPI库所在目录下创建名称为KSJApiLog的目录
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_LogSet(bool bEnable, const TCHAR *pszFolder);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetLastRetCode
	/// @brief     获取最后的错误码
	/// @param     nChannel [in] 设备索引（从0开始，最大索引数为:连接到主机的设备数目减一）
	/// @attention 可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetLastRetCode(int nChannel);

#ifdef __cplusplus
}
#endif

#endif
