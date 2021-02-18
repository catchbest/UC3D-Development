using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace KSJ3DDemoCShape64
{
    class KSJApi3D64
    {
        [DllImport("KSJApi64.dll")]
        public extern static int KSJ3D_Inital();

        [DllImport("KSJApi64.dll")]
        public extern static int KSJ3D_UnInitial();

        [DllImport("KSJApi64.dll")]
        public extern static int KSJ3D_GetDeviceCount(ref int pnCount);

        [DllImport("KSJApi64.dll")]
        public extern static int KSJ3D_GetDeviceInformation(int nChannel, ref int pnDeviceType, ref int pnSerialNumber, ref ushort pwFirmwareVersion, ref ushort pwFpgaVersion);

        [DllImport("KSJApi64.dll")]
        public extern static int KSJ3D_Is3DCamera(int nChannel, ref bool pb3DCamera);

        public delegate void KSJ_LIVE_IMAGE_CALLBACK(int nChannel, IntPtr pData, int nWidth, int nHeight, int nBitCount, IntPtr lpContext);
        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_RegisterLiveImageCB(int nChannel, KSJ_LIVE_IMAGE_CALLBACK pfCallback, IntPtr lpContext);//取得实时图像回调接口设置

        public delegate void KSJ_PROFILE_DATA_CALLBACK(int nChannel, int nPointNum, float fYMillimeters, int nProfileIndex, IntPtr x, IntPtr z, int nLostProfileNum, IntPtr lpContext);
        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_RegisterProfileDataCB(int nChannel, KSJ_PROFILE_DATA_CALLBACK pfCallback, IntPtr lpContext);//获取到Profile数据，x，z

        public delegate void KSJ_PROFILE_IMAGE_CALLBACK(int nChannel, IntPtr pData, int nWidth, int nHeight, int nBitCount, IntPtr lpContext);
        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_RegisterProfileImageCB(int nChannel, KSJ_PROFILE_IMAGE_CALLBACK pfCallback, IntPtr lpContext);//通过zmap转换的profile图像

        public delegate void KSJ_POINT_CLOUD_DATA_CALLBACK(int nChannel, int nTotalPointNum, int nProfileNum, IntPtr x, IntPtr y, IntPtr z, int nTotalLostProfileNum, IntPtr lpContext);
        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_RegisterPointCloudDataCB(int nChannel, KSJ_POINT_CLOUD_DATA_CALLBACK pfCallback, IntPtr lpContext);//获取合成的profile 3d点云数据，x，y，z

        public delegate void KSJ_POINT_CLOUD_IMAGE_CALLBACK(int nChannel, IntPtr pData, int nWidth, int nHeight, int nBitCount, IntPtr lpContext);
        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_RegisterPointCloudImageCB(int nChannel, KSJ_POINT_CLOUD_IMAGE_CALLBACK pfCallback, IntPtr lpContext);// 获取zmap转换的3d图像

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetRoiMax(int nChannel, ref int pnColMax, ref int pnRowMax);//设置ROI

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetRoi(int nChannel, int nColStart, int nRowStart, int nColSize, int nRowSize);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetRoi(int nChannel, ref int pnColStart, ref int pnRowStart, ref int pnColSize, ref int pnRowSize);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetExposureTimeRange(int nChannel, ref float pfExpTimeMSMin, ref float pfExpTimeMSMax);//设置曝光

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetExposureTime(int nChannel, float fExpTimeMS);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetExposureTime(int nChannel, ref float pfExpTimeMS);

         [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetGainRange(int nChannel, ref int pnGainMin, ref int pnGainMax);//设置曝光

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetGain(int nChannel, int nGain);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetGain(int nChannel, ref int pnGain);

        public enum KSJ3D_START_TRIGGER_SOURCE
        {
            STS_INPUT_0 = 0,     // 目前只有一个start trigger输入源
        };

        public enum KSJ3D_TRIGGER_EDGE_MODE
        {
            TEM_RISING_EDGE = 0,
            TEM_FALLING_EDGE = 1
        };

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetStartTrigger(int nChannel, KSJ3D_START_TRIGGER_SOURCE StartTriggerSource, bool bEnable, KSJ3D_TRIGGER_EDGE_MODE StartTriggerCondition);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetStartTrigger(int nChannel, ref KSJ3D_START_TRIGGER_SOURCE pStartTriggerSource, ref bool pbEnable, ref KSJ3D_TRIGGER_EDGE_MODE pStartTriggerCondition);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetStartTriggerParameters(int nChannel, int StartTriggerFilter, int nStartTriggerDelay);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetStartTriggerParameters(int nChannel, ref int pStartTriggerFilter, ref int pnStartTriggerDelay);

        // 设置数据触发模式
        public enum KSJ3D_DATA_TRIGGER_MODE
        {
            DTM_FREE_RUN = 0,            // 自由触发，以最快的帧率进行触发
            DTM_INTERNAL = 1,            // 内部根据触发频率设置进行自动触发
            DTM_EXTERNAL = 2             // 根据外部触发信号进行触发
        };


        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetDataTriggerMode(int nChannel, KSJ3D_DATA_TRIGGER_MODE DataTriggerMode);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetDataTriggerMode(int nChannel, ref KSJ3D_DATA_TRIGGER_MODE pDataTriggerMode);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetDataTriggerInternalFrequency(int nChannel, int nInternalFrequencyHz);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetDataTriggerInternalFrequency(int nChannel, ref int pnInternalFrequencyHz);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetDataTriggerExternalTriggerParameters(int nChannel, int nDataTriggerDivider, int nDataTriggerDelay, int nDataTriggerFilter, KSJ3D_TRIGGER_EDGE_MODE DataTriggerCondition);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetDataTriggerExternalTriggerParameters(int nChannel, ref int pnDataTriggerDivider, ref int pnDataTriggerDelay, ref int pnDataTriggerFilter, ref KSJ3D_TRIGGER_EDGE_MODE pDataTriggerCondition);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetMaxNumberOfProfilesToCapture(int nChannel, int nNumberOfProfiles);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetMaxNumberOfProfilesToCapture(int nChannel, ref int pNumberOfProfiles);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_StartAcquisition(int nChannel);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_StopAcquisition(int nChannel);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetObliterateInvalidData(int nChannel, bool bObliterate);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetObliterateInvalidData(int nChannel, ref bool bObliterate);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_Set3DLaserLineBrightnessThreshold(int nChannel, int nThreshold);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_Get3DLaserLineBrightnessThreshold(int nChannel, ref int pnThreshold);

        public enum KSJ_LASER_MODE
        {
            LM_NORMAL_OPEN = 0,
            LM_NORMAL_CLOSE = 1,
            LM_FLASH
        };

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_LaserModeSet(int nChannel, KSJ_LASER_MODE LaserMode);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_LaserModeGet(int nChannel, ref KSJ_LASER_MODE pLaserMode);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetMeasurementRange(int nChannel, ref float pfZMin, ref float pfZMax, ref float pfFovNear, ref float pfFovFar);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetZMap(int nChannel, float fZMin, float fZMax);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetZMap(int nChannel, ref float pfZMin, ref float pfZMax);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_SetYResolution(int nChannel, float fYResolution);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetYResolution(int nChannel, ref float pfYResolution);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetTransmissionRate(int nChannel, ref float pfTransmissionRate);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetExternalTriggerRate(int nChannel, ref float pfExternalTriggerRateHz);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_HelperSaveToPCD(int nWidth, int nHeight, IntPtr pfX, IntPtr pfY, IntPtr pfZ, string pszFileName);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_GetCameraName(int nChannel, IntPtr pcszName);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_DeviceOpen(int nChannel);

        [DllImport("KSJApi64.dll")]
        public static extern int KSJ3D_DeviceClose(int nChanne);

    }
}
