#define halcon12//有这个定义时加载halcon，屏蔽掉则不加载，保存pcd文件
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using KSJ3DDemoCShape;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.IO;
using System.Collections.Concurrent;
using System.Threading;
using HalconDotNet;
using System.Diagnostics;

namespace KSJ3DDemoCShape
{
    public partial class Form1 : Form
    {
        IniFiles ini;//初始化配置文件
        public Form1()//界面初始化函数
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
            m_nDeviceCurSel = 0;
            m_one = false;
            m_3dDataType = 0;
            m_save = false;
            lpContext = new IntPtr(2121); 
            
            m_LiveCallback = new KSJApi3D.KSJ_LIVE_IMAGE_CALLBACK(LiveCallback);//声明回调函数
            m_PDCallback = new KSJApi3D.KSJ_PROFILE_DATA_CALLBACK(PDCallback);
            m_PICallback = new KSJApi3D.KSJ_PROFILE_IMAGE_CALLBACK(PICallback);
            m_PCDCallback = new KSJApi3D.KSJ_POINT_CLOUD_DATA_CALLBACK(PCDCallback);
            m_PCICallback = new KSJApi3D.KSJ_POINT_CLOUD_IMAGE_CALLBACK(PCICallback);

            m_open = false;
            high = 300;
            this.pictureBox_preview.MouseWheel += new MouseEventHandler(pictureBox_preview_MouseWheel);//控件滚轮事件
            this.pictureBox_zmap.MouseWheel += new MouseEventHandler(pictureBox_zmap_MouseWheel);
        }
        int high;//按钮的间隔高度
        int m_nDeviceNum;//相机数量
        int m_nDeviceCurSel;//相机序号，默认0
        IntPtr lpContext;//回调的上下文句柄
        int m_3dDataType;//测试类型
        bool m_one;//是否采集一次
        int m_nColStart;//视场参数
        int m_nRowStart;
        int m_nColSize;
        int m_nRowSize;
        float m_fExpTimeMS;//曝光
        int m_nGain;//增益
        KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE m_StartTriggerCondition;//启动触发模式
        int m_nStartTriggerDelay;//启动触发延时
        KSJApi3D.KSJ3D_DATA_TRIGGER_MODE m_DataTriggerMode;//数据触发模式
        int m_StartTriggerFilter;//启动触发滤波
        int m_nInternalFrequencyHz;//内触发频率
        int m_nDataTriggerDivider;//数据触发分频
        int m_nDataTriggerDelay;//数据触发延时
        int m_nDataTriggerFilter;//数据触发滤波
        KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE m_DataTriggerCondition;//数据 触发模式

        int m_nNumberOfProfiles;//采集轮廓数
        int m_nThreshold;

        float m_fZMin;//zmap值
        float m_fZMax;

        float m_fProfiley;//运动方向精度
        bool m_save;//是否保存
        bool m_open;//是否找到3d相机
        bool m_auto;//是否启动自动zmap
        int m_triggermode;//预览时记录触发模式，停止预览时恢复
        int m_startmode;
    
        KSJApi3D.KSJ_LIVE_IMAGE_CALLBACK m_LiveCallback;//回调函数
        KSJApi3D.KSJ_PROFILE_DATA_CALLBACK m_PDCallback;
        KSJApi3D.KSJ_PROFILE_IMAGE_CALLBACK m_PICallback;
        KSJApi3D.KSJ_POINT_CLOUD_DATA_CALLBACK m_PCDCallback;
        KSJApi3D.KSJ_POINT_CLOUD_IMAGE_CALLBACK m_PCICallback;
        byte[] ys;//显示预览的数组
        byte[] ys2;//显示3d或轮廓的数组
        Bitmap bitmap;//预览位图
        Bitmap bitmap2;//显示3d或轮廓位图
        public unsafe System.Drawing.Bitmap BytesToBitmap(byte[] bytes, int width, int height, int nBitCount)//将数组转换为位图
        {
            try
            {
                PixelFormat bitmaptype = PixelFormat.Format8bppIndexed;
                int nPixelBytes = nBitCount >> 3;

                if (3 == nPixelBytes)
                {
                    bitmaptype = PixelFormat.Format24bppRgb;
                }
                else if (4 == nPixelBytes)
                {
                    bitmaptype = PixelFormat.Format32bppPArgb;
                }

                Bitmap bitmap = new Bitmap(width, height, bitmaptype);
                //获取图像的BitmapData对像 
                BitmapData bmpData = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.ReadWrite, bitmaptype);

                byte* ptr = (byte*)(bmpData.Scan0);
                int nWidthBytes = width * nPixelBytes;
                int nSrcOffset = 0;
                int nDesOffset = 0;
                for (int i = 0; i < height; i++)
                {
                    nSrcOffset = (height - i - 1) * nWidthBytes;
                    nDesOffset = 0;
                    for (int j = 0; j < width; j++)
                    {
                        for (int k = 0; k < nPixelBytes; k++)
                        {
                            *(ptr + nDesOffset + k) = bytes[nSrcOffset + k];
                        }

                        nDesOffset += nPixelBytes;
                        nSrcOffset += nPixelBytes;
                    }

                    ptr = ptr + nWidthBytes;
                }

                bitmap.UnlockBits(bmpData);  // 解锁内存区域

                if (1 == nPixelBytes)
                {
                    // 修改生成位图的索引表，从伪彩修改为灰度
                    ColorPalette palette;
                    // 获取一个Format8bppIndexed格式图像的Palette对象
                    using (Bitmap bmp = new Bitmap(1, 1, PixelFormat.Format8bppIndexed))
                    {
                        palette = bmp.Palette;
                    }
                    for (int i = 0; i < 256; i++)
                    {
                        palette.Entries[i] = Color.FromArgb(i, i, i);
                    }
                    // 修改生成位图的索引表
                    bitmap.Palette = palette;
                }

                return bitmap;
            }
            catch (ArgumentNullException ex)
            {
                throw ex;
            }
            catch (ArgumentException ex)
            {
                throw ex;
            }
        }

        unsafe private void LiveCallback(int nChannel, IntPtr pData, int nWidth, int nHeight, int nBitCount, IntPtr lpContext)//预览回调函数
        {

            Marshal.Copy(pData, ys, 0, nWidth * nHeight);
            bitmap = BytesToBitmap(ys, nWidth, nHeight, nBitCount);//转换位图
            if (!init)//未进行过宽高比例计算
            {
                float proportion = (float)pictureBox_preview.Width / (float)pictureBox_preview.Height;
                float proportion_bmp = (float)nWidth / (float)nHeight;
                if (proportion < proportion_bmp)//比较控件和图片宽高比，根据不同情况设置位图在控件内初始显示位置
                {
                    m_nScale = (float)pictureBox_preview.Width / (float)m_nColSize;
                    float h = m_nRowSize * m_nScale;
                    m_ptCanvas = new Point(0, 0);
                    int offset = (pictureBox_preview.Height - (int)h) / 2;
                    m_ptBmp = new Point(0, (int)(offset / m_nScale));
                }
                else
                {
                    m_nScale = (float)pictureBox_preview.Height / (float)nHeight;
                    float w = nWidth * m_nScale;
                    m_ptCanvas = new Point(0, 0);
                    int offset = (pictureBox_preview.Width - (int)w) / 2;
                    m_ptBmp = new Point((int)(offset / m_nScale), 0);
                }
                init = true;//进行设置后标记

            }

        }


        private void PDCallback(int nChannel, int nPointNum, float fYMillimeters, int nProfileIndex, IntPtr x, IntPtr z, int nLostProfileNum, IntPtr lpContext)
        {

        }

        private void PICallback(int nChannel, IntPtr pData, int nWidth, int nHeight, int nBitCount, IntPtr lpContext)//轮廓显示
        {
            //System.Diagnostics.Debug.WriteLine("轮廓显示");
            Marshal.Copy(pData, ys2, 0, nWidth * nHeight);
            bitmap2 = BytesToBitmap(ys2, nWidth, nHeight, nBitCount);//转换位图
            if (!initzmap)//未进行过宽高比例计算
            {
                //System.Diagnostics.Debug.WriteLine("计算轮廓比例");
                float proportion = (float)pictureBox_zmap.Width / (float)pictureBox_zmap.Height;
                float proportion_bmp = (float)nWidth / (float)nHeight;
                if (proportion < proportion_bmp)
                {
                    m_nScalezmap = (float)pictureBox_zmap.Width / (float)nWidth;
                    float h = nHeight * m_nScalezmap;
                    m_ptCanvaszmap = new Point(0, 0);
                    int offset = (pictureBox_zmap.Height - (int)h) / 2;
                    m_ptBmpzmap = new Point(0, (int)(offset / m_nScalezmap));
                }
                else
                {
                    m_nScalezmap = (float)pictureBox_zmap.Height / (float)nHeight;
                    float w = nWidth * m_nScalezmap;
                    m_ptCanvaszmap = new Point(0, 0);
                    int offset = (pictureBox_zmap.Width - (int)w) / 2;
                    m_ptBmpzmap = new Point((int)(offset / m_nScalezmap), 0);
                }
                initzmap = true;
            }



            if (m_one)//如果是一次采集则停止采集
            {
                m_one = false;
                checkBox_one.BeginInvoke((MethodInvoker)(() => { checkBox_one.Checked = false; }));
            }
        }



        private void PCDCallback(int nChannel, int nTotalPointNum, int nProfileNum, IntPtr x, IntPtr y, IntPtr z, int nTotalLostProfileNum, IntPtr lpContext)
        {
            //System.Diagnostics.Debug.WriteLine("进入采集");

            if (nTotalLostProfileNum > 0)//判断丢帧数
            {
                //System.Diagnostics.Debug.WriteLine("丢帧");
                string err = string.Format("丢失{0}帧", nTotalLostProfileNum);
                label_lost.BeginInvoke((MethodInvoker)(() => { label_lost.Text = err; }));
            }
            else
            {
                //System.Diagnostics.Debug.WriteLine("未丢帧");
                label_lost.BeginInvoke((MethodInvoker)(() => { label_lost.Text = "未丢帧"; }));
            }

            if (m_auto)//自动zamp计算最大最小点
            {
                float min = 1000f, max = 0;
                bool ret = false;
                float[] tz = new float[nTotalPointNum];
                Marshal.Copy(z, tz, 0, nTotalPointNum);
                for (int i = 0; i < nTotalPointNum; i++)
                {
                    if (Math.Abs(tz[i] + 1000f) > 0.00001f)
                    {
                        if (min > tz[i]) min = tz[i];
                        if (max < tz[i]) max = tz[i];
                        ret = true;
                    }
                }

                if (ret)
                {
                    KSJApi3D.KSJ3D_SetZMap(m_nDeviceCurSel, min, max);
                }
                //else System.Diagnostics.Debug.WriteLine("无有效数据");
            }

            if (m_save)//保存
            {
                string t1, filename;
                t1 = DateTime.Now.ToString("yyyy-MM-dd");
                //System.Diagnostics.Debug.WriteLine("开始保存");
#if halcon12//使用halcon保存om3文件
                filename = Application.StartupPath + "\\" + t1 + "-" + DateTime.Now.Hour.ToString() + DateTime.Now.Minute.ToString() + DateTime.Now.Second.ToString() + ".om3";
                if(checkBox1.Checked)
                {
                    int count = 0;
                    float[] tx = new float[nTotalPointNum];
                    float[] ty = new float[nTotalPointNum];
                    float[] tz = new float[nTotalPointNum];
                    Marshal.Copy(x, tx, 0, nTotalPointNum);
                    Marshal.Copy(y, ty, 0, nTotalPointNum);
                    Marshal.Copy(z, tz, 0, nTotalPointNum);
                    HTuple hv_PointCoordX = new HTuple();
                    HTuple hv_PointCoordY = new HTuple();
                    HTuple hv_PointCoordZ = new HTuple();
                    for (int i = 0; i < nTotalPointNum; i++)
                    {
                        hv_PointCoordX[count] = tx[i];
                        hv_PointCoordY[count] = ty[i];
                        hv_PointCoordZ[count] = tz[i];
                        count++;
                    }


                    HTuple hv_ObjectModel3D;
                    HOperatorSet.GenObjectModel3dFromPoints(hv_PointCoordX, hv_PointCoordY, hv_PointCoordZ, out hv_ObjectModel3D);
                    HOperatorSet.WriteObjectModel3d(hv_ObjectModel3D, "om3",filename, "invert_normals", "false");
                    HOperatorSet.ClearObjectModel3d(hv_ObjectModel3D);
                    hv_ObjectModel3D.UnPinTuple();
                    System.Diagnostics.Debug.WriteLine("保存完毕");
                }
                else 
                {
                    HObject Hobjx = null, Hobjy = null, Hobjz = null;
                    HOperatorSet.GenImage1(out Hobjx, "real", m_nColSize, m_nNumberOfProfiles, x);
                    HOperatorSet.GenImage1(out Hobjy, "real", m_nColSize, m_nNumberOfProfiles, y);
                    HOperatorSet.GenImage1(out Hobjz, "real", m_nColSize, m_nNumberOfProfiles, z);
                    HTuple hv_ObjectModel3D;
                    HOperatorSet.XyzToObjectModel3d(Hobjx, Hobjy, Hobjz, out hv_ObjectModel3D);
                    HOperatorSet.WriteObjectModel3d(hv_ObjectModel3D, "om3", filename, "invert_normals", "false");
                    HOperatorSet.ClearObjectModel3d(hv_ObjectModel3D);
                    hv_ObjectModel3D.UnPinTuple();
                    System.Diagnostics.Debug.WriteLine("保存完毕");
                }

#else//不使用halcon保存pcd文件
                filename = Application.StartupPath + "\\" + t1 + "-" + DateTime.Now.Hour.ToString() + DateTime.Now.Minute.ToString() + DateTime.Now.Second.ToString() + ".pcd";
                KSJApi3D.KSJ3D_HelperSaveToPCD(m_nColSize, m_nNumberOfProfiles, x, y, z, filename);
                //System.Diagnostics.Debug.WriteLine("保存完毕");
#endif
            }

            if (m_one)//如果是一次采集则停止采集
            {
                m_one = false;
                checkBox_one.BeginInvoke((MethodInvoker)(() => { checkBox_one.Checked = false; }));
            }
        }

        private void PCICallback(int nChannel, IntPtr pData, int nWidth, int nHeight, int nBitCount, IntPtr lpContext)//显示3d图形
        {
            //System.Diagnostics.Debug.WriteLine("3d显示");
            Marshal.Copy(pData, ys2, 0, nWidth * nHeight);
            bitmap2 = BytesToBitmap(ys2, nWidth, nHeight, nBitCount);//转换位图
            if (!initzmap)//未进行过宽高比例计算
            {
                //System.Diagnostics.Debug.WriteLine("计算3d比例");
                float proportion = (float)pictureBox_zmap.Width / (float)pictureBox_zmap.Height;
                float proportion_bmp = (float)nWidth / (float)nHeight;
                if (proportion < proportion_bmp)
                {
                    m_nScalezmap = (float)pictureBox_zmap.Width / (float)nWidth;
                    float h = nHeight * m_nScalezmap;
                    m_ptCanvaszmap = new Point(0, 0);
                    int offset = (pictureBox_zmap.Height - (int)h) / 2;
                    m_ptBmpzmap = new Point(0, (int)(offset / m_nScalezmap));
                }
                else
                {
                    m_nScalezmap = (float)pictureBox_zmap.Height / (float)nHeight;
                    float w = nWidth * m_nScalezmap;
                    m_ptCanvaszmap = new Point(0, 0);
                    int offset = (pictureBox_zmap.Width - (int)w) / 2;
                    m_ptBmpzmap = new Point((int)(offset / m_nScalezmap), 0);
                }
                initzmap = true;
            }

        }

        private void readini()//读配置文件
        {
            KSJApi3D.KSJ3D_START_TRIGGER_SOURCE source = KSJApi3D.KSJ3D_START_TRIGGER_SOURCE.STS_INPUT_0;
            bool bCur = true;
            string temp;
            float fMin=0, fMax=0;
            int nMin=0, nMax=0;

            numericUpDown_delay.Minimum = 0;
            numericUpDown_delay.Maximum = 65535;
            numericUpDown_filter.Minimum = 0;
            numericUpDown_filter.Maximum = 65535;
            numericUpDown_line.Minimum = 0;
            numericUpDown_line.Maximum = 65535;
            numericUpDown_rate.Minimum = 0;
            numericUpDown_rate.Maximum = 20000;
            numericUpDown_period.Minimum = 1;
            numericUpDown_period.Maximum = 65535;
            numericUpDown_delay2.Minimum = 0;
            numericUpDown_delay2.Maximum = 65535;
            numericUpDown_filter2.Minimum = 0;
            numericUpDown_filter2.Maximum = 65535;
            KSJApi3D.KSJ3D_GetExposureTimeRange(m_nDeviceCurSel, ref fMin, ref fMax);
            numericUpDown_expouse.Minimum = 0;
            numericUpDown_expouse.Maximum = (decimal)fMax;
            KSJApi3D.KSJ3D_GetGainRange(m_nDeviceCurSel, ref nMin, ref nMax);
            numericUpDown_gain.Minimum = nMin;
            numericUpDown_gain.Maximum = nMax;
            numericUpDown_t.Minimum = 0;
            numericUpDown_t.Maximum = 255;
            numericUpDown_low.Minimum = 0;
            numericUpDown_low.Maximum = 255;
            numericUpDown_high.Minimum = 0;
            numericUpDown_high.Maximum = 255;
            if (ini.ExistINIFile())//有配置文件
            {
                temp = ini.IniReadValue("StartTrigger", "Mode");//启动触发
                comboBox_startmode.SelectedIndex = int.Parse(temp);
                if (comboBox_startmode.SelectedIndex != 0) m_StartTriggerCondition = (KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE)(comboBox_startmode.SelectedIndex - 1);
                else m_StartTriggerCondition = (KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE)comboBox_startmode.SelectedIndex;
                if (comboBox_startmode.SelectedIndex == 0) KSJApi3D.KSJ3D_SetStartTrigger(m_nDeviceCurSel, KSJApi3D.KSJ3D_START_TRIGGER_SOURCE.STS_INPUT_0, false, m_StartTriggerCondition);
                else KSJApi3D.KSJ3D_SetStartTrigger(m_nDeviceCurSel, KSJApi3D.KSJ3D_START_TRIGGER_SOURCE.STS_INPUT_0, true, m_StartTriggerCondition);
                
                temp = ini.IniReadValue("StartTrigger", "Delay");
                m_nStartTriggerDelay = int.Parse(temp);
                temp = ini.IniReadValue("StartTrigger", "Filter");
                m_StartTriggerFilter = int.Parse(temp);
                temp = ini.IniReadValue("StartTrigger", "MultiLine");
                m_nNumberOfProfiles = int.Parse(temp);
                temp = ini.IniReadValue("DataTrigger", "Mode");//数据触发
                m_DataTriggerMode = (KSJApi3D.KSJ3D_DATA_TRIGGER_MODE)int.Parse(temp);
                temp = ini.IniReadValue("DataTrigger", "FrameRate");
                m_nInternalFrequencyHz = int.Parse(temp);
                temp = ini.IniReadValue("DataTrigger", "IntervalOfEveryPulse");
                m_nDataTriggerDivider = int.Parse(temp);
                temp = ini.IniReadValue("DataTrigger", "Delay");
                m_nDataTriggerDelay = int.Parse(temp);
                temp = ini.IniReadValue("DataTrigger", "Filter");
                m_nDataTriggerFilter = int.Parse(temp);
                temp = ini.IniReadValue("DataTrigger", "Method");
                m_DataTriggerCondition = (KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE)int.Parse(temp);
                
                temp = ini.IniReadValue("Param", "Exposure");//增益曝光视场
                m_fExpTimeMS = float.Parse(temp);
                temp = ini.IniReadValue("Param", "Gain");
                m_nGain = int.Parse(temp);
                temp = ini.IniReadValue("Param", "ColStart");
                m_nColStart = int.Parse(temp);
                temp = ini.IniReadValue("Param", "RowStart");
                m_nRowStart = int.Parse(temp);
                temp = ini.IniReadValue("Param", "ColSize");
                m_nColSize = int.Parse(temp);
                temp = ini.IniReadValue("Param", "RowSize");
                m_nRowSize = int.Parse(temp);

                temp = ini.IniReadValue("3D", "T");//3D参数
                m_nThreshold = int.Parse(temp);
                temp = ini.IniReadValue("3D", "Y");
                m_fProfiley = float.Parse(temp);
                temp = ini.IniReadValue("3D", "LOW");
                m_fZMin = float.Parse(temp);
                temp = ini.IniReadValue("3D", "HIGH");
                m_fZMax = float.Parse(temp);
                temp = ini.IniReadValue("3D", "TYPE");
                m_3dDataType = int.Parse(temp);
            }
            else 
            {
                KSJApi3D.KSJ3D_GetStartTrigger(m_nDeviceCurSel, ref source, ref bCur, ref m_StartTriggerCondition);//启动触发
                if (!bCur) comboBox_startmode.SelectedIndex = 0;
                else comboBox_startmode.SelectedIndex = (int)(m_StartTriggerCondition) + 1;
                KSJApi3D.KSJ3D_GetStartTriggerParameters(m_nDeviceCurSel, ref m_StartTriggerFilter, ref m_nStartTriggerDelay);
                m_nNumberOfProfiles = 500;

                KSJApi3D.KSJ3D_GetDataTriggerMode(m_nDeviceCurSel, ref m_DataTriggerMode);//数据触发
                KSJApi3D.KSJ3D_GetDataTriggerInternalFrequency(m_nDeviceCurSel, ref m_nInternalFrequencyHz);
                KSJApi3D.KSJ3D_GetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, ref m_nDataTriggerDivider, ref m_nDataTriggerDelay, ref m_nDataTriggerFilter, ref m_DataTriggerCondition);

                KSJApi3D.KSJ3D_GetExposureTime(m_nDeviceCurSel, ref m_fExpTimeMS);
                KSJApi3D.KSJ3D_GetGain(m_nDeviceCurSel, ref m_nGain);
                KSJApi3D.KSJ3D_GetRoi(m_nDeviceCurSel, ref m_nColStart, ref m_nRowStart, ref m_nColSize, ref m_nRowSize);

                KSJApi3D.KSJ3D_Get3DLaserLineBrightnessThreshold(m_nDeviceCurSel, ref m_nThreshold);
                KSJApi3D.KSJ3D_GetYResolution(m_nDeviceCurSel, ref m_fProfiley);
                //KSJApi3D.KSJ3D_GetZMap(m_nDeviceCurSel, ref m_fZMin, ref m_fZMax);
                float fFovNear = 0, fFovFar = 0;
                KSJApi3D.KSJ3D_GetMeasurementRange(m_nDeviceCurSel, ref m_fZMin, ref m_fZMax, ref fFovNear, ref fFovFar);
                if (m_fZMin < 0) m_fZMin = 0;
                numericUpDown_low.Value = (decimal)m_fZMin;
                numericUpDown_high.Value = (decimal)m_fZMax;
                m_3dDataType = 1;
            }


            numericUpDown_delay.Value = m_nStartTriggerDelay;//控件设置
            numericUpDown_filter.Value = m_StartTriggerFilter;
            numericUpDown_line.Value = m_nNumberOfProfiles;
            comboBox_datamode.SelectedIndex = (int)m_DataTriggerMode;
            numericUpDown_rate.Value = m_nInternalFrequencyHz;
            numericUpDown_period.Value = m_nDataTriggerDivider;
            numericUpDown_delay2.Value = m_nDataTriggerDelay;
            numericUpDown_filter2.Value = m_nDataTriggerFilter;
            comboBox_datamothod.SelectedIndex = (int)m_DataTriggerCondition;
            numericUpDown_expouse.Value = (decimal)m_fExpTimeMS;
            numericUpDown_gain.Value = m_nGain;
            numericUpDown_width.Value = m_nColSize;
            numericUpDown_height.Value = m_nRowSize;
            numericUpDown_colstart.Value = m_nColStart;
            numericUpDown_rowstart.Value = m_nRowStart;
            numericUpDown_t.Value = m_nThreshold;
            numericUpDown_y.Value = (decimal)m_fProfiley;
            numericUpDown_low.Value = (decimal)m_fZMin;
            numericUpDown_high.Value = (decimal)m_fZMax;
            comboBox_type.SelectedIndex = m_3dDataType;
            comboBox_laser.SelectedIndex = 2;
            KSJApi3D.KSJ3D_LaserModeSet(m_nDeviceCurSel, KSJApi3D.KSJ_LASER_MODE.LM_FLASH);
            KSJApi3D.KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);//相机设置
            KSJApi3D.KSJ3D_SetMaxNumberOfProfilesToCapture(m_nDeviceCurSel, m_nNumberOfProfiles);
            KSJApi3D.KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, m_DataTriggerMode);
            KSJApi3D.KSJ3D_SetDataTriggerInternalFrequency(m_nDeviceCurSel, m_nInternalFrequencyHz);
            KSJApi3D.KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
            KSJApi3D.KSJ3D_SetExposureTime(m_nDeviceCurSel, m_fExpTimeMS);
            KSJApi3D.KSJ3D_SetGain(m_nDeviceCurSel, m_nGain);
            KSJApi3D.KSJ3D_SetRoi(m_nDeviceCurSel, m_nColStart, m_nRowStart, m_nColSize, m_nRowSize);

            KSJApi3D.KSJ3D_Set3DLaserLineBrightnessThreshold(m_nDeviceCurSel, m_nThreshold);
            KSJApi3D.KSJ3D_SetYResolution(m_nDeviceCurSel, m_fProfiley);
            KSJApi3D.KSJ3D_SetZMap(m_nDeviceCurSel, m_fZMin, m_fZMax);
        }

        private void writeini()//写配置文件
        {
            string temp = (comboBox_startmode.SelectedIndex).ToString();//启动触发
            ini.IniWriteValue("StartTrigger", "Mode", temp);
            temp = (m_nStartTriggerDelay).ToString();
            ini.IniWriteValue("StartTrigger", "Delay", temp);
            temp = (m_StartTriggerFilter).ToString();
            ini.IniWriteValue("StartTrigger", "Filter", temp);
            temp = (m_nNumberOfProfiles).ToString();
            ini.IniWriteValue("StartTrigger", "MultiLine", temp);

            temp = (comboBox_datamode.SelectedIndex).ToString();//数据触发
            ini.IniWriteValue("DataTrigger", "Mode", temp);
            temp = (comboBox_datamothod.SelectedIndex).ToString();
            ini.IniWriteValue("DataTrigger", "Method", temp);
            temp = (m_nInternalFrequencyHz).ToString();
            ini.IniWriteValue("DataTrigger", "FrameRate", temp);
            temp = (m_nDataTriggerDelay).ToString();
            ini.IniWriteValue("DataTrigger", "Delay", temp);
            temp = (m_nDataTriggerDivider).ToString();
            ini.IniWriteValue("DataTrigger", "IntervalOfEveryPulse", temp);
            temp = (m_nDataTriggerFilter).ToString();
            ini.IniWriteValue("DataTrigger", "Filter", temp);

            temp = (m_fExpTimeMS).ToString();//增益曝光视场
            ini.IniWriteValue("Param", "Exposure", temp);
            temp = (m_nGain).ToString();
            ini.IniWriteValue("Param", "Gain", temp);
            temp = (m_nColStart).ToString();
            ini.IniWriteValue("Param", "ColStart", temp);
            temp = (m_nColSize).ToString();
            ini.IniWriteValue("Param", "ColSize", temp);
            temp = (m_nRowStart).ToString();
            ini.IniWriteValue("Param", "RowStart", temp);
            temp = (m_nRowSize).ToString();
            ini.IniWriteValue("Param", "RowSize", temp);

            temp = (m_fProfiley).ToString();//3D参数
            ini.IniWriteValue("3D", "Y", temp);
            temp = (m_nThreshold).ToString();
            ini.IniWriteValue("3D", "T", temp);
            temp = (m_fZMin).ToString();
            ini.IniWriteValue("3D", "LOW", temp);
            temp = (m_fZMax).ToString();
            ini.IniWriteValue("3D", "HIGH", temp);
            temp = (m_3dDataType).ToString();
            ini.IniWriteValue("3D", "TYPE", temp);
        }

        private void Form1_Load(object sender, EventArgs e)//初始化界面
        {
            System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;
            this.Width = 1225;
            this.Height = 700;
            int i;
            string[] szStartEndMode = { "disable", "Rising Edge", "Falling Edge" };
            string[] szTriggerMode = { "Free Run", "Internal", "External" };
            string[] szTriggerMethod = { "Rising Edge", "Falling Edge" };
            string[] strtype = { "轮廓", "3D" };
            string[] szLaser = { "Open", "Close", "Flash" };
            for (i = 0; i < 3; i++) comboBox_startmode.Items.Insert(i, szStartEndMode[i]);
            for (i = 0; i < 3; i++) comboBox_datamode.Items.Insert(i, szTriggerMode[i]);
            for (i = 0; i < 2; i++) comboBox_datamothod.Items.Insert(i, szTriggerMethod[i]);
            for (i = 0; i < 2; i++) comboBox_type.Items.Insert(i, strtype[i]);
            for (i = 0; i < 3; i++) comboBox_laser.Items.Insert(i, szLaser[i]);
            ksjlog = new IniFiles(Application.StartupPath + "\\KSJApi.ini");
            m_init = 0;
            KSJApi3D.KSJ3D_Inital();
            m_nDeviceCurSel = 0;
            UpdateDeviceList();//获取相机的列表插入到界面上放下拉框
            UpdateInterface();
            m_init = 1;
            checkBox_starttrigger.SetBounds(40, 110, 170, 25);//初始化设置按钮
            checkBox_roi.SetBounds(40, 150, 170, 25);
            checkBox_param.SetBounds(40, 190, 170, 25);
            checkBox_datatrigger.SetBounds(40, 230, 170, 25);
            checkBox_log.SetBounds(40, 270, 170, 25);
        }

        private bool CheckCameraName(string name)
        {
            for (int i = 0; i < name.Length; i++)
	        {
		        if (name[i]<0x20 || name[i]>0x7F) return false;
	        }
	        return true;
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)//退出界面
        {
            if(m_open)
            {
                writeini();//如果打开则写配置文件
                Timer_GET_FRAME_RATE.Stop();
            }
                
            KSJApi3D.KSJ3D_UnInitial();//释放3d相机资源
        }

        public struct DEVICEINFO
        {
            public int nIndex;
            public int DeviceType;
            public int nSerials;
            public ushort wFirmwareVersion;
            public ushort wFpgaVersion;
        };

        public DEVICEINFO[] m_DeviceInfo = new DEVICEINFO[m_nMaxDevice];
        public const int m_nMaxDevice = 64;

        public void UpdateDeviceList()
        {
            KSJApi3D.KSJ3D_UnInitial();
            KSJApi3D.KSJ3D_Inital();
            KSJApi3D.KSJ3D_GetDeviceCount(ref m_nDeviceNum);
            if (m_nDeviceNum == 0)//未找到相机，界面控件不可用
            {
                m_nDeviceCurSel = -1;
                checkBox_starttrigger.Enabled = false;
                checkBox_roi.Enabled = false;
                checkBox_param.Enabled = false;
                checkBox_datatrigger.Enabled = false;
                checkBox_log.Enabled = false;
                checkBox_preview.Enabled = false;
                checkBox_one.Enabled = false;
                checkBox_capture.Enabled = false;
                numericUpDown_line.Enabled = false;
                comboBox_type.Enabled = false;
                checkBox_save.Enabled = false;
                label_device.Text = "未查询到相机";
                return;
            }

            if (m_nDeviceCurSel >= m_nDeviceNum)
            {
                m_nDeviceCurSel = 0;
            }
            bool bIs3DCamera = false;
            string CameraName = "";
            IntPtr pszCameraName = Marshal.AllocCoTaskMem(64);
            ComboBox_DEVICE_LIST.Items.Clear();
            for (int i = 0; i < m_nDeviceNum; i++)
            {
                m_DeviceInfo[i].nIndex = i;
                KSJApi3D.KSJ3D_GetDeviceInformation(i, ref m_DeviceInfo[i].DeviceType, ref m_DeviceInfo[i].nSerials, ref m_DeviceInfo[i].wFirmwareVersion, ref m_DeviceInfo[i].wFpgaVersion);
                byte btMajVersion = (byte)((m_DeviceInfo[i].wFirmwareVersion & 0xFF00) >> 8);		// 得到主版本号
                byte btMinVersion = (byte)(m_DeviceInfo[i].wFirmwareVersion & 0x00FF);				// 得到副版本号

                byte btFpgaMajVersion = (byte)((m_DeviceInfo[i].wFpgaVersion & 0xFF00) >> 8);		// 得到主版本号
                byte btFpgaMinVersion = (byte)(m_DeviceInfo[i].wFpgaVersion & 0x00FF);				// 得到副版本号
                
                KSJApi3D.KSJ3D_Is3DCamera(m_nDeviceCurSel, ref bIs3DCamera);
                if (bIs3DCamera)
                {
                    KSJApi3D.KSJ3D_GetCameraName(i, pszCameraName);
                    CameraName = Marshal.PtrToStringAnsi(pszCameraName);
                }
                else CameraName = "not 3DCamera";
                if (!CheckCameraName(CameraName)) CameraName = "3DCamera";
                string szText = String.Format("Type({0})-Serials({1})-FwVer({2}.{3})-FpgaVer({4}.{5})",
                                        CameraName, m_DeviceInfo[i].nSerials, btMajVersion, btMinVersion, btFpgaMajVersion, btFpgaMinVersion);
                ComboBox_DEVICE_LIST.Items.Insert(i, szText);
            }

            ComboBox_DEVICE_LIST.SelectedIndex = m_nDeviceCurSel;
        }

        private void UpdateInterface()//获取选中相机的数据显示在界面上
        {
            if (m_nDeviceCurSel == -1) return;
            for (int i = 0; i < m_nDeviceNum; i++)
            {
                if (i != m_nDeviceCurSel) KSJApi3D.KSJ3D_DeviceClose(i);
            }

            int nRet = KSJApi3D.KSJ3D_DeviceOpen(m_nDeviceCurSel);
	        if (nRet == 1)
            {
                bool bIs3DCamera = false;
                string CameraName = "";
                IntPtr pszCameraName = Marshal.AllocCoTaskMem(64);
                KSJApi3D.KSJ3D_Is3DCamera(m_nDeviceCurSel, ref bIs3DCamera);
                if (bIs3DCamera)//类型为3d相机，读配置文件
                {
                    KSJApi3D.KSJ3D_GetCameraName(m_nDeviceCurSel, pszCameraName);
                    CameraName = Marshal.PtrToStringAnsi(pszCameraName);
                    if (!CheckCameraName(CameraName)) CameraName = "3DCamera";
                    string para = String.Format("\\param_{0}_{1}.ini", CameraName, m_DeviceInfo[m_nDeviceCurSel].nSerials);
                    ini = new IniFiles(Application.StartupPath + para);
                    readini();
                    m_open = true;
                    m_auto = true;
                    string temp = ksjlog.IniReadValue("Manage", "LogEnable");
                    if (temp == "1") checkBox_ksjlog.Checked = true;
                    else checkBox_ksjlog.Checked = false;

                    
                    checkBox_auto.Checked = true;
#if halcon12//使用halcon可以设置有效点,默认打开
                checkBox1.Checked = true;
                KSJApi3D.KSJ3D_SetObliterateInvalidData(m_nDeviceCurSel, true);
#else//存pcd模式关闭有效点设置
                    checkBox1.Enabled = false;
                    KSJApi3D.KSJ3D_SetObliterateInvalidData(m_nDeviceCurSel, false);
#endif
                    KSJApi3D.KSJ3D_LaserModeSet(m_nDeviceCurSel, KSJApi3D.KSJ_LASER_MODE.LM_FLASH);
                    Timer_GET_FRAME_RATE.Interval = 50;
                    Timer_GET_FRAME_RATE.Start();
                    label_device.Text = "相机连接成功";
                    checkBox_starttrigger.Enabled = true;
                    checkBox_roi.Enabled = true;
                    checkBox_param.Enabled = true;
                    checkBox_datatrigger.Enabled = true;
                    checkBox_log.Enabled = true;
                    checkBox_preview.Enabled = true;
                    checkBox_one.Enabled = true;
                    checkBox_capture.Enabled = true;
                    numericUpDown_line.Enabled = true;
                    comboBox_type.Enabled = true;
                    checkBox_save.Enabled = true;
                    return;
                }
                else label_device.Text = "相机非3d相机";
            }
            else label_device.Text = "相机已被其他进程打开";

            checkBox_starttrigger.Enabled = false;
            checkBox_roi.Enabled = false;
            checkBox_param.Enabled = false;
            checkBox_datatrigger.Enabled = false;
            checkBox_log.Enabled = false;
            checkBox_preview.Enabled = false;
            checkBox_one.Enabled = false;
            checkBox_capture.Enabled = false;
            numericUpDown_line.Enabled = false;
            comboBox_type.Enabled = false;
            checkBox_save.Enabled = false;
        }

        private void checkBox_preview_CheckedChanged(object sender, EventArgs e)//预览开关
        {
            bool bCheck = checkBox_preview.Checked;
            if (bCheck)//启动预览
            {
                m_startmode = comboBox_startmode.SelectedIndex;//记录触发模式
                m_triggermode = comboBox_datamode.SelectedIndex;
                comboBox_startmode.SelectedIndex = 0;//改为自由驱动模式
                comboBox_datamode.SelectedIndex = 0;
                ys = new byte[m_nColSize * m_nRowSize];//分配数组
                KSJApi3D.KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, m_LiveCallback, lpContext);//启动回调函数
                KSJApi3D.KSJ3D_StartAcquisition(m_nDeviceCurSel);

                checkBox_one.Enabled = false;//预览时部分参数不可调
                checkBox_capture.Enabled = false;

                numericUpDown_line.Enabled = false;
                comboBox_type.Enabled = false;
                checkBox_save.Enabled = false;

                comboBox_startmode.Enabled = false;
                numericUpDown_delay.Enabled = false;
                numericUpDown_filter.Enabled = false;

                numericUpDown_width.Enabled = false;
                numericUpDown_height.Enabled = false;
                numericUpDown_colstart.Enabled = false;
                numericUpDown_rowstart.Enabled = false;

                comboBox_datamode.Enabled = false;
                numericUpDown_rate.Enabled = false;
                comboBox_datamothod.Enabled = false;
                numericUpDown_delay2.Enabled = false;
                numericUpDown_period.Enabled = false;
                numericUpDown_filter2.Enabled = false;
                comboBox_laser.Enabled = false;
                init = false;
            }
            else
            {
                KSJApi3D.KSJ3D_StopAcquisition(m_nDeviceCurSel);//停止回调函数
                KSJApi3D.KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, null, lpContext);

                checkBox_one.Enabled = true;//回复参数
                checkBox_capture.Enabled = true;

                numericUpDown_line.Enabled = true;
                comboBox_type.Enabled = true;
                checkBox_save.Enabled = true;

                comboBox_startmode.Enabled = true;
                numericUpDown_delay.Enabled = true;
                numericUpDown_filter.Enabled = true;

                numericUpDown_width.Enabled = true;
                numericUpDown_height.Enabled = true;
                numericUpDown_colstart.Enabled = true;
                numericUpDown_rowstart.Enabled = true;

                comboBox_datamode.Enabled = true;
                comboBox_laser.Enabled = true;
                comboBox_startmode.SelectedIndex = m_startmode;//回复触发模式
                comboBox_datamode.SelectedIndex = m_triggermode;
                datatriger(m_triggermode);//根据触发模式设置界面
            }
        }

        private void checkBox_one_CheckedChanged(object sender, EventArgs e)//一次采集开关
        {
            bool bCheck = checkBox_one.Checked;
            if (bCheck)//启动采集
            {
                m_one = true;//打开一次采集标记
                if (m_3dDataType == 1)//判断相机采集类型
                {
                    ys2 = new byte[m_nColSize * m_nNumberOfProfiles];
                    KSJApi3D.KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, m_PCDCallback, lpContext);//3d回调函数
                    KSJApi3D.KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, m_PCICallback, lpContext);
                }
                else
                {
                    ys2 = new byte[m_nColSize * 256];
                    KSJApi3D.KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, m_PDCallback, lpContext);//轮廓回调函数
                    KSJApi3D.KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, m_PICallback, lpContext);
                }

                ys = new byte[m_nColSize * m_nRowSize];//预览
                KSJApi3D.KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, m_LiveCallback, lpContext);
                KSJApi3D.KSJ3D_StartAcquisition(m_nDeviceCurSel);

                checkBox_capture.Enabled = false;//大部分控件不可用
                checkBox_preview.Enabled = false;

                numericUpDown_line.Enabled = false;
                comboBox_type.Enabled = false;
                checkBox_save.Enabled = false;

                comboBox_startmode.Enabled = false;
                numericUpDown_delay.Enabled = false;
                numericUpDown_filter.Enabled = false;

                numericUpDown_width.Enabled = false;
                numericUpDown_height.Enabled = false;
                numericUpDown_colstart.Enabled = false;
                numericUpDown_rowstart.Enabled = false;

                comboBox_datamode.Enabled = false;
                numericUpDown_rate.Enabled = false;
                comboBox_datamothod.Enabled = false;
                numericUpDown_delay2.Enabled = false;
                numericUpDown_period.Enabled = false;
                numericUpDown_filter2.Enabled = false;
                comboBox_laser.Enabled = false;
                init = false;
            }
            else
            {
                KSJApi3D.KSJ3D_StopAcquisition(m_nDeviceCurSel);//关闭采集
                m_one = false;
                if (m_3dDataType == 1)//判断采集类型关闭回调函数
                {
                    KSJApi3D.KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, null, lpContext);
                    KSJApi3D.KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, null, lpContext);
                }
                else
                {
                    KSJApi3D.KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, null, lpContext);
                    KSJApi3D.KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, null, lpContext);
                }

                KSJApi3D.KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, null, lpContext);//关闭预览

                checkBox_capture.Enabled = true;//恢复大部分控件可控
                checkBox_preview.Enabled = true;

                numericUpDown_line.Enabled = true;
                comboBox_type.Enabled = true;
                checkBox_save.Enabled = true;

                comboBox_startmode.Enabled = true;
                numericUpDown_delay.Enabled = true;
                numericUpDown_filter.Enabled = true;

                numericUpDown_width.Enabled = true;
                numericUpDown_height.Enabled = true;
                numericUpDown_colstart.Enabled = true;
                numericUpDown_rowstart.Enabled = true;

                comboBox_datamode.Enabled = true;
                numericUpDown_rate.Enabled = true;
                comboBox_datamothod.Enabled = true;
                numericUpDown_delay2.Enabled = true;
                numericUpDown_period.Enabled = true;
                numericUpDown_filter2.Enabled = true;
                comboBox_laser.Enabled = true;
                datatriger(comboBox_datamode.SelectedIndex);//根据触发模式设置界面
            }
        }
        

        private void checkBox_capture_CheckedChanged(object sender, EventArgs e)//相机采集开关
        {
            bool bCheck = checkBox_capture.Checked;
            if (bCheck)//启动采集
            {
                if (m_3dDataType == 1)//判断相机采集类型
                {
                    ys2 = new byte[m_nColSize * m_nNumberOfProfiles];
                    KSJApi3D.KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, m_PCDCallback, lpContext);//3d回调函数
                    KSJApi3D.KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, m_PCICallback, lpContext);
                }
                else
                {
                    ys2 = new byte[m_nColSize * 256];
                    KSJApi3D.KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, m_PDCallback, lpContext);//轮廓回调函数
                    KSJApi3D.KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, m_PICallback, lpContext);
                }

                initzmap = false;
                ys = new byte[m_nColSize * m_nRowSize];//预览
                KSJApi3D.KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, m_LiveCallback, lpContext);
                KSJApi3D.KSJ3D_StartAcquisition(m_nDeviceCurSel);

                checkBox_one.Enabled = false;//大部分控件不可用
                checkBox_preview.Enabled = false;

                numericUpDown_line.Enabled = false;
                comboBox_type.Enabled = false;
                checkBox_save.Enabled = false;

                comboBox_startmode.Enabled = false;
                numericUpDown_delay.Enabled = false;
                numericUpDown_filter.Enabled = false;

                numericUpDown_width.Enabled = false;
                numericUpDown_height.Enabled = false;
                numericUpDown_colstart.Enabled = false;
                numericUpDown_rowstart.Enabled = false;

                comboBox_datamode.Enabled = false;
                numericUpDown_rate.Enabled = false;
                comboBox_datamothod.Enabled = false;
                numericUpDown_delay2.Enabled = false;
                numericUpDown_period.Enabled = false;
                numericUpDown_filter2.Enabled = false;
                comboBox_laser.Enabled = false;
                init = false;
            }
            else
            {
                KSJApi3D.KSJ3D_StopAcquisition(m_nDeviceCurSel);//关闭采集
                if (m_3dDataType == 1)//判断采集类型关闭回调函数
                {
                    KSJApi3D.KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, null, lpContext);
                    KSJApi3D.KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, null, lpContext);
                }
                else
                {
                    KSJApi3D.KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, null, lpContext);
                    KSJApi3D.KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, null, lpContext);
                }

                KSJApi3D.KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, null, lpContext);//关闭预览

                checkBox_one.Enabled = true;//恢复大部分控件可控
                checkBox_preview.Enabled = true;

                numericUpDown_line.Enabled = true;
                comboBox_type.Enabled = true;
                checkBox_save.Enabled = true;

                comboBox_startmode.Enabled = true;
                numericUpDown_delay.Enabled = true;
                numericUpDown_filter.Enabled = true;

                numericUpDown_width.Enabled = true;
                numericUpDown_height.Enabled = true;
                numericUpDown_colstart.Enabled = true;
                numericUpDown_rowstart.Enabled = true;

                comboBox_datamode.Enabled = true;
                numericUpDown_rate.Enabled = true;
                comboBox_datamothod.Enabled = true;
                numericUpDown_delay2.Enabled = true;
                numericUpDown_period.Enabled = true;
                numericUpDown_filter2.Enabled = true;
                comboBox_laser.Enabled = true;
                datatriger(comboBox_datamode.SelectedIndex);//根据触发模式设置界面
            }
        }
        

        private void hidecontrol()//隐藏控件
        {
            comboBox_startmode.Visible = false;
            numericUpDown_delay.Visible = false;
            numericUpDown_filter.Visible = false;
            label4.Visible = false;
            label5.Visible = false;
            label6.Visible = false;
            label13.Visible = false;
            label14.Visible = false;

            numericUpDown_width.Visible = false;
            numericUpDown_height.Visible = false;
            numericUpDown_colstart.Visible = false;
            numericUpDown_rowstart.Visible = false;
            button_setfov.Visible = false;
            label8.Visible = false;
            label7.Visible = false;
            label9.Visible = false;
            label10.Visible = false;

            numericUpDown_gain.Visible = false;
            numericUpDown_expouse.Visible = false;
            label12.Visible = false;
            label11.Visible = false;

            comboBox_datamode.Visible = false;
            numericUpDown_rate.Visible = false;
            comboBox_datamothod.Visible = false;
            numericUpDown_delay2.Visible = false;
            numericUpDown_period.Visible = false;
            numericUpDown_filter2.Visible = false;
            label15.Visible = false;
            label17.Visible = false;
            label16.Visible = false;
            label21.Visible = false;
            label20.Visible = false;
            label23.Visible = false;
            label22.Visible = false;
            label19.Visible = false;
            label18.Visible = false;
            label32.Visible = false;
            comboBox_laser.Visible = false;

            numericUpDown_t.Visible = false;
            numericUpDown_y.Visible = false;
            label26.Visible = false;
            label25.Visible = false;
            label24.Visible = false;

            numericUpDown_low.Visible = false;
            numericUpDown_high.Visible = false;
            label27.Visible = false;
            label28.Visible = false;
            label29.Visible = false;
            label30.Visible = false;

            checkBox_ksjlog.Visible = false;
        }

        private void checkBox_starttrigger_CheckedChanged(object sender, EventArgs e)//启动触发模式设置
        {
            hidecontrol();
            bool bCheck = checkBox_starttrigger.Checked;
            if (bCheck)
            {
                checkBox_roi.Checked = false;
                checkBox_param.Checked = false;
                checkBox_datatrigger.Checked = false;
                checkBox_log.Checked = false;
                comboBox_startmode.Visible = true;
                numericUpDown_delay.Visible = true;
                numericUpDown_filter.Visible = true;
                label4.Visible = true;
                label5.Visible = true;
                label6.Visible = true;
                label13.Visible = true;
                label14.Visible = true;

                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150 + high, 170, 25);
                checkBox_param.SetBounds(40, 190 + high, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230 + high, 170, 25);
                checkBox_log.SetBounds(40, 270 + high, 170, 25);

                label4.SetBounds(40, 150, 60, 20);
                comboBox_startmode.SetBounds(110, 150, 130, 20);
                label5.SetBounds(40, 180, 60, 20);
                numericUpDown_delay.SetBounds(110, 180, 60, 20);
                label13.SetBounds(200, 180, 60, 20);
                label6.SetBounds(40, 210, 60, 20);
                numericUpDown_filter.SetBounds(110, 210, 60, 20);
                label14.SetBounds(200, 210, 60, 20);
            }
            else 
            {
                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150, 170, 25);
                checkBox_param.SetBounds(40, 190, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230, 170, 25);
                checkBox_log.SetBounds(40, 270, 170, 25);
            }
        }

        private void checkBox_roi_CheckedChanged(object sender, EventArgs e)//视场设置
        {
            hidecontrol();
            bool bCheck = checkBox_roi.Checked;
            if (bCheck)
            {
                checkBox_starttrigger.Checked = false;
                checkBox_param.Checked = false;
                checkBox_datatrigger.Checked = false;
                checkBox_log.Checked = false;
                numericUpDown_width.Visible = true;
                numericUpDown_height.Visible = true;
                numericUpDown_colstart.Visible = true;
                numericUpDown_rowstart.Visible = true;
                button_setfov.Visible = true;
                label8.Visible = true;
                label7.Visible = true;
                label9.Visible = true;
                label10.Visible = true;
                numericUpDown_low.Visible = true;
                numericUpDown_high.Visible = true;
                label27.Visible = true;
                label28.Visible = true;
                label29.Visible = true;
                label30.Visible = true;

                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150, 170, 25);
                checkBox_param.SetBounds(40, 190 + high, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230 + high, 170, 25);
                checkBox_log.SetBounds(40, 270 + high, 170, 25);

                label8.SetBounds(40, 190, 60, 20);
                numericUpDown_width.SetBounds(110, 190, 60, 20);
                label7.SetBounds(40, 230, 60, 20);
                numericUpDown_height.SetBounds(110, 230, 60, 20);
                label9.SetBounds(40, 270, 60, 20);
                numericUpDown_colstart.SetBounds(110, 270, 60, 20);
                label10.SetBounds(40, 310, 60, 20);
                numericUpDown_rowstart.SetBounds(110, 310, 60, 20);
                button_setfov.SetBounds(40, 350, 40, 25);

                label27.SetBounds(40, 390, 60, 20);
                numericUpDown_low.SetBounds(110, 390, 60, 20);
                label29.SetBounds(180, 390, 60, 20);
                label28.SetBounds(40, 430, 60, 20);
                numericUpDown_high.SetBounds(110, 430, 60, 20);
                label30.SetBounds(180, 430, 60, 20);
            }
            else
            {
                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150, 170, 25);
                checkBox_param.SetBounds(40, 190, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230, 170, 25);
                checkBox_log.SetBounds(40, 270, 170, 25);
            }
        }

        private void checkBox_param_CheckedChanged(object sender, EventArgs e)//增益曝光参数设置
        {
            hidecontrol();
            bool bCheck = checkBox_param.Checked;
            if (bCheck)
            {
                checkBox_starttrigger.Checked = false;
                checkBox_roi.Checked = false;
                checkBox_datatrigger.Checked = false;
                checkBox_log.Checked = false;
                numericUpDown_gain.Visible = true;
                numericUpDown_expouse.Visible = true;
                label12.Visible = true;
                label11.Visible = true;
                numericUpDown_t.Visible = true;
                label26.Visible = true;

                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150, 170, 25);
                checkBox_param.SetBounds(40, 190, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230 + high, 170, 25);
                checkBox_log.SetBounds(40, 270 + high, 170, 25);

                label12.SetBounds(40, 230, 60, 20);
                numericUpDown_gain.SetBounds(110, 230, 60, 20);
                label11.SetBounds(40, 260, 60, 20);
                numericUpDown_expouse.SetBounds(110, 260, 60, 20);

                label26.SetBounds(40, 290, 60, 20);
                numericUpDown_t.SetBounds(110, 290, 60, 20);
            }
            else
            {
                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150, 170, 25);
                checkBox_param.SetBounds(40, 190, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230, 170, 25);
                checkBox_log.SetBounds(40, 270, 170, 25);
            }
        }

        private void checkBox_datatrigger_CheckedChanged(object sender, EventArgs e)//数据触发模式设置
        {
            hidecontrol();
            bool bCheck = checkBox_datatrigger.Checked;
            if (bCheck)
            {
                checkBox_starttrigger.Checked = false;
                checkBox_roi.Checked = false;
                checkBox_param.Checked = false;
                checkBox_log.Checked = false;
                comboBox_datamode.Visible = true;
                numericUpDown_rate.Visible = true;
                comboBox_datamothod.Visible = true;
                numericUpDown_delay2.Visible = true;
                numericUpDown_period.Visible = true;
                numericUpDown_filter2.Visible = true;
                label15.Visible = true;
                label17.Visible = true;
                label16.Visible = true;
                label21.Visible = true;
                label20.Visible = true;
                label23.Visible = true;
                label22.Visible = true;
                label19.Visible = true;
                label18.Visible = true;
                numericUpDown_y.Visible = true;
                label25.Visible = true;
                label24.Visible = true;
                label32.Visible = true;
                comboBox_laser.Visible = true;
                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150, 170, 25);
                checkBox_param.SetBounds(40, 190, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230, 170, 25);
                checkBox_log.SetBounds(40, 270 + high, 170, 25);

                label15.SetBounds(40, 270, 60, 20);
                comboBox_datamode.SetBounds(110, 270, 150, 20);
                label17.SetBounds(30, 300, 70, 20);
                numericUpDown_rate.SetBounds(110, 300, 60, 20);
                label16.SetBounds(40, 330, 60, 20);
                comboBox_datamothod.SetBounds(110, 330, 150, 20);
                label21.SetBounds(30, 360, 70, 20);
                numericUpDown_delay2.SetBounds(110, 360, 60, 20);
                label19.SetBounds(190, 360, 60, 20);
                label23.SetBounds(30, 390, 70, 20);
                numericUpDown_period.SetBounds(110, 390, 60, 20);
                label22.SetBounds(190, 390, 60, 20);
                label20.SetBounds(30, 420, 70, 20);
                numericUpDown_filter2.SetBounds(110, 420, 60, 20);
                label18.SetBounds(190, 420, 60, 20);

                label25.SetBounds(20, 450, 80, 20);
                numericUpDown_y.SetBounds(110, 450, 60, 20);
                label24.SetBounds(170, 450, 80, 20);
                label32.SetBounds(30, 480, 70, 20);
                comboBox_laser.SetBounds(110, 480, 150, 20);
            }
            else
            {
                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150, 170, 25);
                checkBox_param.SetBounds(40, 190, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230, 170, 25);
                checkBox_log.SetBounds(40, 270, 170, 25);
            }
        }

        private void checkBox_log_CheckedChanged(object sender, EventArgs e)//LOG设置
        {
            hidecontrol();
            bool bCheck = checkBox_log.Checked;
            if (bCheck)
            {
                checkBox_starttrigger.Checked = false;
                checkBox_roi.Checked = false;
                checkBox_param.Checked = false;
                checkBox_datatrigger.Checked = false;
                checkBox_ksjlog.Visible = true;

                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150, 170, 25);
                checkBox_param.SetBounds(40, 190, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230, 170, 25);
                checkBox_log.SetBounds(40, 270, 170, 25);

                checkBox_ksjlog.SetBounds(40, 300, 80, 20);
            }
            else
            {
                checkBox_starttrigger.SetBounds(40, 110, 170, 25);
                checkBox_roi.SetBounds(40, 150, 170, 25);
                checkBox_param.SetBounds(40, 190, 170, 25);
                checkBox_datatrigger.SetBounds(40, 230, 170, 25);
                checkBox_log.SetBounds(40, 270, 170, 25);
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)//设置保存
        {
            m_save = checkBox_save.Checked;
        }

        private void comboBox_type_SelectedIndexChanged(object sender, EventArgs e)//设置3d采集类型
        {
            if (m_nDeviceCurSel == -1) return;
            m_3dDataType = comboBox_type.SelectedIndex;
            if (m_3dDataType==1)
            {
                pictureBox_preview.Location = new System.Drawing.Point(277, 82);
                pictureBox_preview.Size = new System.Drawing.Size(451, 546);
                pictureBox_zmap.Location = new System.Drawing.Point(750, 82);
                pictureBox_zmap.Size = new System.Drawing.Size(451, 546);
            }
            else
            {
                pictureBox_preview.Location = new System.Drawing.Point(277, 82);
                pictureBox_preview.Size = new System.Drawing.Size(924, 270);
                pictureBox_zmap.Location = new System.Drawing.Point(277, 355);
                pictureBox_zmap.Size = new System.Drawing.Size(924, 270);
            }
        }

        private void numericUpDown_line_ValueChanged(object sender, EventArgs e)//设置采集行数
        {
            if (m_nDeviceCurSel == -1) return;
            m_nNumberOfProfiles = (int)(numericUpDown_line.Value);
            KSJApi3D.KSJ3D_SetMaxNumberOfProfilesToCapture(m_nDeviceCurSel, m_nNumberOfProfiles);
            //initzmap = false;
        }

        private void comboBox_startmode_SelectedIndexChanged(object sender, EventArgs e)//启动触发模式设置
        {
            if (m_nDeviceCurSel == -1) return;
            int nIndex = comboBox_startmode.SelectedIndex;
            if (nIndex != 0) m_StartTriggerCondition = (KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE)(nIndex - 1);
            else m_StartTriggerCondition = (KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE)nIndex;

            if (nIndex == 0) KSJApi3D.KSJ3D_SetStartTrigger(m_nDeviceCurSel, KSJApi3D.KSJ3D_START_TRIGGER_SOURCE.STS_INPUT_0, false, m_StartTriggerCondition);
            else KSJApi3D.KSJ3D_SetStartTrigger(m_nDeviceCurSel, KSJApi3D.KSJ3D_START_TRIGGER_SOURCE.STS_INPUT_0, true, m_StartTriggerCondition);
        }

        private void numericUpDown_delay_ValueChanged(object sender, EventArgs e)//启动触发延时设置
        {
            if (m_nDeviceCurSel == -1) return;
            m_nStartTriggerDelay = (int)(numericUpDown_delay.Value);
            KSJApi3D.KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);
        }

        private void numericUpDown_filter_ValueChanged(object sender, EventArgs e)//启动触发滤波
        {
            if (m_nDeviceCurSel == -1) return;
            m_StartTriggerFilter = (int)(numericUpDown_filter.Value);
            KSJApi3D.KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);
        }

        private void numericUpDown_gain_ValueChanged(object sender, EventArgs e)//增益
        {
            if (m_nDeviceCurSel == -1) return;
            m_nGain = (int)(numericUpDown_gain.Value);
            KSJApi3D.KSJ3D_SetGain(m_nDeviceCurSel, m_nGain);
        }

        private void numericUpDown_expouse_ValueChanged(object sender, EventArgs e)//曝光
        {
            if (m_nDeviceCurSel == -1) return;
            m_fExpTimeMS = (float)(numericUpDown_expouse.Value);
            KSJApi3D.KSJ3D_SetExposureTime(m_nDeviceCurSel, m_fExpTimeMS);
        }

        private void datatriger(int nIndex)//数据触发模式
        {
            if (nIndex == 0)//自由驱动模式
            {
                numericUpDown_rate.Enabled = false;
                comboBox_datamothod.Enabled = false;
                numericUpDown_delay2.Enabled = false;
                numericUpDown_filter2.Enabled = false;
                numericUpDown_period.Enabled = false;
            }
            else if (nIndex == 1)//内触发模式，只能设置频率
            {
                numericUpDown_rate.Enabled = true;
                comboBox_datamothod.Enabled = false;
                numericUpDown_delay2.Enabled = false;
                numericUpDown_filter2.Enabled = false;
                numericUpDown_period.Enabled = false;
            }
            else if (nIndex == 2)//外触发模式，可以设置滤波、延时、分频
            {
                numericUpDown_rate.Enabled = false;
                comboBox_datamothod.Enabled = true;
                numericUpDown_delay2.Enabled = true;
                numericUpDown_filter2.Enabled = true;
                numericUpDown_period.Enabled = true;
            }
        }

        private void comboBox_datamode_SelectedIndexChanged(object sender, EventArgs e)//数据触发设置
        {
            if (m_nDeviceCurSel == -1) return;
            int nIndex = comboBox_datamode.SelectedIndex;
            m_DataTriggerMode = (KSJApi3D.KSJ3D_DATA_TRIGGER_MODE)nIndex;
            KSJApi3D.KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, m_DataTriggerMode);
            datatriger(nIndex);
        }

        private void numericUpDown_rate_ValueChanged(object sender, EventArgs e)//内触发频率设置
        {
            if (m_nDeviceCurSel == -1) return;
            m_nInternalFrequencyHz = (int)(numericUpDown_rate.Value);
            KSJApi3D.KSJ3D_SetDataTriggerInternalFrequency(m_nDeviceCurSel, m_nInternalFrequencyHz);
        }

        private void comboBox_datamothod_SelectedIndexChanged(object sender, EventArgs e)//数据触发方法设置
        {
            if (m_nDeviceCurSel == -1) return;
            int nIndex = comboBox_datamothod.SelectedIndex;
            m_DataTriggerCondition = (KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE)nIndex;
            KSJApi3D.KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
        }

        private void numericUpDown_delay2_ValueChanged(object sender, EventArgs e)//数据触发延时
        {
            if (m_nDeviceCurSel == -1) return;
            m_nDataTriggerDelay = (int)(numericUpDown_delay2.Value);
            KSJApi3D.KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
        }

        private void numericUpDown_filter2_ValueChanged(object sender, EventArgs e)//数据触发滤波
        {
            if (m_nDeviceCurSel == -1) return;
            m_nDataTriggerFilter = (int)(numericUpDown_filter2.Value);
            KSJApi3D.KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
        }

        private void numericUpDown_period_ValueChanged(object sender, EventArgs e)//数据触发分频
        {
            if (m_nDeviceCurSel == -1) return;
            m_nDataTriggerDivider = (int)(numericUpDown_period.Value);
            KSJApi3D.KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
        }

        private void numericUpDown_t_ValueChanged(object sender, EventArgs e)//阈值
        {
            if (m_nDeviceCurSel == -1) return;
            m_nThreshold = (int)(numericUpDown_t.Value);
            KSJApi3D.KSJ3D_Set3DLaserLineBrightnessThreshold(m_nDeviceCurSel, m_nThreshold);
        }

        private void numericUpDown_y_ValueChanged(object sender, EventArgs e)//运动方向精度
        {
            if (m_nDeviceCurSel == -1) return;
            m_fProfiley = (float)(numericUpDown_y.Value);
            KSJApi3D.KSJ3D_SetYResolution(m_nDeviceCurSel, m_fProfiley);
        }

        private void numericUpDown_low_ValueChanged(object sender, EventArgs e)//zamp最小值
        {
            if (m_nDeviceCurSel == -1) return;
            m_fZMin = (float)(numericUpDown_low.Value);
            KSJApi3D.KSJ3D_SetZMap(m_nDeviceCurSel, m_fZMin, m_fZMax);
        }

        private void numericUpDown_high_ValueChanged(object sender, EventArgs e)//zmap最大值
        {
            if (m_nDeviceCurSel == -1) return;
            m_fZMax = (float)(numericUpDown_high.Value);
            KSJApi3D.KSJ3D_SetZMap(m_nDeviceCurSel, m_fZMin, m_fZMax);
        }

        private void button_load_Click(object sender, EventArgs e)//加载配置文件按钮
        {
            string path = "";
            string temp;
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.Filter = "配置文件|*.ffc";
                if (ofd.ShowDialog() == DialogResult.OK)  //如果点击的是打开文件
                {
                    path = ofd.FileName;  //获取全路径文件名
                    IniFiles load = new IniFiles(path);
                    temp = load.IniReadValue("StartTrigger", "Mode");//启动触发
                    comboBox_startmode.SelectedIndex = int.Parse(temp);
                    if (comboBox_startmode.SelectedIndex != 0) m_StartTriggerCondition = (KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE)(comboBox_startmode.SelectedIndex - 1);
                    else m_StartTriggerCondition = (KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE)comboBox_startmode.SelectedIndex;
                    if (comboBox_startmode.SelectedIndex == 0) KSJApi3D.KSJ3D_SetStartTrigger(m_nDeviceCurSel, KSJApi3D.KSJ3D_START_TRIGGER_SOURCE.STS_INPUT_0, false, m_StartTriggerCondition);
                    else KSJApi3D.KSJ3D_SetStartTrigger(m_nDeviceCurSel, KSJApi3D.KSJ3D_START_TRIGGER_SOURCE.STS_INPUT_0, true, m_StartTriggerCondition);

                    temp = load.IniReadValue("StartTrigger", "Delay");
                    m_nStartTriggerDelay = int.Parse(temp);
                    temp = load.IniReadValue("StartTrigger", "Filter");
                    m_StartTriggerFilter = int.Parse(temp);
                    temp = load.IniReadValue("StartTrigger", "MultiLine");
                    m_nNumberOfProfiles = int.Parse(temp);
                    temp = load.IniReadValue("DataTrigger", "Mode");//数据触发
                    m_DataTriggerMode = (KSJApi3D.KSJ3D_DATA_TRIGGER_MODE)int.Parse(temp);
                    temp = load.IniReadValue("DataTrigger", "FrameRate");
                    m_nInternalFrequencyHz = int.Parse(temp);
                    temp = load.IniReadValue("DataTrigger", "IntervalOfEveryPulse");
                    m_nDataTriggerDivider = int.Parse(temp);
                    temp = load.IniReadValue("DataTrigger", "Delay");
                    m_nDataTriggerDelay = int.Parse(temp);
                    temp = load.IniReadValue("DataTrigger", "Filter");
                    m_nDataTriggerFilter = int.Parse(temp);
                    temp = load.IniReadValue("DataTrigger", "Method");
                    m_DataTriggerCondition = (KSJApi3D.KSJ3D_TRIGGER_EDGE_MODE)int.Parse(temp);

                    temp = load.IniReadValue("Param", "Exposure");//增益曝光视场
                    m_fExpTimeMS = float.Parse(temp);
                    temp = load.IniReadValue("Param", "Gain");
                    m_nGain = int.Parse(temp);
                    temp = load.IniReadValue("Param", "ColStart");
                    m_nColStart = int.Parse(temp);
                    temp = load.IniReadValue("Param", "RowStart");
                    m_nRowStart = int.Parse(temp);
                    temp = load.IniReadValue("Param", "ColSize");
                    m_nColSize = int.Parse(temp);
                    temp = load.IniReadValue("Param", "RowSize");
                    m_nRowSize = int.Parse(temp);

                    temp = load.IniReadValue("3D", "T");//3D参数
                    m_nThreshold = int.Parse(temp);
                    temp = load.IniReadValue("3D", "Y");
                    m_fProfiley = float.Parse(temp);
                    temp = load.IniReadValue("3D", "LOW");
                    m_fZMin = float.Parse(temp);
                    temp = load.IniReadValue("3D", "HIGH");
                    m_fZMax = float.Parse(temp);
                    temp = load.IniReadValue("3D", "TYPE");
                    m_3dDataType = int.Parse(temp);

                    numericUpDown_delay.Value = m_nStartTriggerDelay;
                    numericUpDown_filter.Value = m_StartTriggerFilter;
                    numericUpDown_line.Value = m_nNumberOfProfiles;
                    comboBox_datamode.SelectedIndex = (int)m_DataTriggerMode;
                    numericUpDown_rate.Value = m_nInternalFrequencyHz;
                    numericUpDown_period.Value = m_nDataTriggerDivider;
                    numericUpDown_delay2.Value = m_nDataTriggerDelay;
                    numericUpDown_filter2.Value = m_nDataTriggerFilter;
                    comboBox_datamothod.SelectedIndex = (int)m_DataTriggerCondition;
                    numericUpDown_expouse.Value = (decimal)m_fExpTimeMS;
                    numericUpDown_gain.Value = m_nGain;
                    numericUpDown_width.Value = m_nColSize;
                    numericUpDown_height.Value = m_nRowSize;
                    numericUpDown_colstart.Value = m_nColStart;
                    numericUpDown_rowstart.Value = m_nRowStart;
                    numericUpDown_t.Value = m_nThreshold;
                    numericUpDown_y.Value = (decimal)m_fProfiley;
                    numericUpDown_low.Value = (decimal)m_fZMin;
                    numericUpDown_high.Value = (decimal)m_fZMax;
                    comboBox_type.SelectedIndex = m_3dDataType;

                    KSJApi3D.KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);
                    KSJApi3D.KSJ3D_SetMaxNumberOfProfilesToCapture(m_nDeviceCurSel, m_nNumberOfProfiles);
                    KSJApi3D.KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, m_DataTriggerMode);
                    KSJApi3D.KSJ3D_SetDataTriggerInternalFrequency(m_nDeviceCurSel, m_nInternalFrequencyHz);
                    KSJApi3D.KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
                    KSJApi3D.KSJ3D_SetExposureTime(m_nDeviceCurSel, m_fExpTimeMS);
                    KSJApi3D.KSJ3D_SetGain(m_nDeviceCurSel, m_nGain);
                    KSJApi3D.KSJ3D_SetRoi(m_nDeviceCurSel, m_nColStart, m_nRowStart, m_nColSize, m_nRowSize);

                    KSJApi3D.KSJ3D_Set3DLaserLineBrightnessThreshold(m_nDeviceCurSel, m_nThreshold);
                    KSJApi3D.KSJ3D_SetYResolution(m_nDeviceCurSel, m_fProfiley);
                    KSJApi3D.KSJ3D_SetZMap(m_nDeviceCurSel, m_fZMin, m_fZMax);
                }
            }

        }

        private void button_setfov_Click(object sender, EventArgs e)//设置采集视场
        {
            if (m_nDeviceCurSel == -1) return;
            m_nColStart = (int)(numericUpDown_colstart.Value);
            m_nRowStart = (int)(numericUpDown_rowstart.Value);
            m_nColSize = (int)(numericUpDown_width.Value);
            m_nRowSize = (int)(numericUpDown_height.Value);
            KSJApi3D.KSJ3D_SetRoi(m_nDeviceCurSel, m_nColStart, m_nRowStart, m_nColSize, m_nRowSize);
            KSJApi3D.KSJ3D_GetRoi(m_nDeviceCurSel, ref m_nColStart, ref m_nRowStart, ref m_nColSize, ref m_nRowSize);
            numericUpDown_colstart.Value = m_nColStart;
            numericUpDown_rowstart.Value = m_nRowStart;
            numericUpDown_width.Value = m_nColSize;
            numericUpDown_height.Value = m_nRowSize;
            init = false;
            initzmap = false;
            float fFovNear = 0, fFovFar = 0;
            KSJApi3D.KSJ3D_GetMeasurementRange(m_nDeviceCurSel, ref m_fZMin, ref m_fZMax, ref fFovNear, ref fFovFar);
            if (m_fZMin < 0) m_fZMin = 0;
            numericUpDown_low.Value = (decimal)m_fZMin;
            numericUpDown_high.Value = (decimal)m_fZMax;
        }

        private void checkBox_auto_CheckedChanged(object sender, EventArgs e)//自动zmap设置
        {
            m_auto = checkBox_auto.Checked;
        }

        private void checkBox1_CheckedChanged_1(object sender, EventArgs e)//采集有效点设置
        {
            KSJApi3D.KSJ3D_SetObliterateInvalidData(m_nDeviceCurSel, checkBox1.Checked);
        }

        private void Timer_GET_FRAME_RATE_Tick(object sender, EventArgs e)
        {
            float fTransmissionRate = 0.0f;
            float fExternalTriggerRateHz = 0.0f;

            KSJApi3D.KSJ3D_GetTransmissionRate(m_nDeviceCurSel, ref fTransmissionRate);
            KSJApi3D.KSJ3D_GetExternalTriggerRate(m_nDeviceCurSel, ref fExternalTriggerRateHz);
            string rate = string.Format("帧率：{0}，触发帧率：{1}", fTransmissionRate, fExternalTriggerRateHz);
            //System.Diagnostics.Debug.WriteLine(rate);
            label_rate.BeginInvoke((MethodInvoker)(() => { label_rate.Text = rate; }));
            pictureBox_preview.BeginInvoke((MethodInvoker)(() => { pictureBox_preview.Invalidate(); }));
            pictureBox_zmap.BeginInvoke((MethodInvoker)(() => { pictureBox_zmap.Invalidate(); }));
        }

        private void button_save_Click(object sender, EventArgs e)
        {
            string path = textBox_save.Text;
            if(string.IsNullOrEmpty(path))
            {
                MessageBox.Show("配置文件名不能为空");
                return;
            }
            IniFiles save = new IniFiles(Application.StartupPath + "\\"+ path + ".ini");//初始化配置文件
            string temp = (comboBox_startmode.SelectedIndex).ToString();//启动触发
            save.IniWriteValue("StartTrigger", "Mode", temp);
            temp = (m_nStartTriggerDelay).ToString();
            save.IniWriteValue("StartTrigger", "Delay", temp);
            temp = (m_StartTriggerFilter).ToString();
            save.IniWriteValue("StartTrigger", "Filter", temp);
            temp = (m_nNumberOfProfiles).ToString();
            save.IniWriteValue("StartTrigger", "MultiLine", temp);

            temp = (comboBox_datamode.SelectedIndex).ToString();//数据触发
            save.IniWriteValue("DataTrigger", "Mode", temp);
            temp = (comboBox_datamothod.SelectedIndex).ToString();
            save.IniWriteValue("DataTrigger", "Method", temp);
            temp = (m_nInternalFrequencyHz).ToString();
            save.IniWriteValue("DataTrigger", "FrameRate", temp);
            temp = (m_nDataTriggerDelay).ToString();
            save.IniWriteValue("DataTrigger", "Delay", temp);
            temp = (m_nDataTriggerDivider).ToString();
            save.IniWriteValue("DataTrigger", "IntervalOfEveryPulse", temp);
            temp = (m_nDataTriggerFilter).ToString();
            save.IniWriteValue("DataTrigger", "Filter", temp);

            temp = (m_fExpTimeMS).ToString();//增益曝光视场
            save.IniWriteValue("Param", "Exposure", temp);
            temp = (m_nGain).ToString();
            save.IniWriteValue("Param", "Gain", temp);
            temp = (m_nColStart).ToString();
            save.IniWriteValue("Param", "ColStart", temp);
            temp = (m_nColSize).ToString();
            save.IniWriteValue("Param", "ColSize", temp);
            temp = (m_nRowStart).ToString();
            save.IniWriteValue("Param", "RowStart", temp);
            temp = (m_nRowSize).ToString();
            save.IniWriteValue("Param", "RowSize", temp);

            temp = (m_fProfiley).ToString();//3D参数
            save.IniWriteValue("3D", "Y", temp);
            temp = (m_nThreshold).ToString();
            save.IniWriteValue("3D", "T", temp);
            temp = (m_fZMin).ToString();
            save.IniWriteValue("3D", "LOW", temp);
            temp = (m_fZMax).ToString();
            save.IniWriteValue("3D", "HIGH", temp);
            temp = (m_3dDataType).ToString();
            save.IniWriteValue("3D", "TYPE", temp);
            MessageBox.Show("配置文件名保存完毕");
        }

        private void comboBox_laser_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_nDeviceCurSel == -1) return;
            int nIndex = comboBox_laser.SelectedIndex;
            KSJApi3D.KSJ3D_LaserModeSet(m_nDeviceCurSel, (KSJApi3D.KSJ_LASER_MODE)nIndex);
        }

        IniFiles ksjlog;
        int m_init;
        private void checkBox_ksjlog_CheckedChanged(object sender, EventArgs e)
        {
            bool bCheck = checkBox_ksjlog.Checked;
            if (bCheck)
            {
                if (m_init == 1)
                {
                    ksjlog.IniWriteValue("Manage", "LogEnable", "1");
                    MessageBox.Show("打开LOG记录，重启程序生效，生成信息保存到执行程序目录KSJApiLog文件夹！");
                }
            }
            else
            {
                ksjlog.IniWriteValue("Manage", "LogEnable", "0");
                MessageBox.Show("关闭LOG记录，重启程序生效！");
            }
        }

        private void ComboBox_DEVICE_LIST_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_init == 1)
            {
                m_init = 0;
                m_nDeviceCurSel = ComboBox_DEVICE_LIST.SelectedIndex;
                UpdateDeviceList();//获取相机的列表插入到界面上放下拉框
                UpdateInterface();
                m_init = 1;
            }
            
        }
    }
}
