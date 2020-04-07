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
    public partial class Form1 : Form//实现预览和zmap的缩放功能
    {
        Point m_ptCanvas;           //画布原点在设备上的坐标
        Point m_ptCanvasBuf;        //重置画布坐标计算时用的临时变量
        Point m_ptBmp;              //图像位于画布坐标系中的坐标
        float m_nScale = 1.0F;      //缩放比例
        Point m_ptMouseDown;        //鼠标点下是在设备坐标上的坐标
        private void pictureBox_preview_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {      //如果左键点下    初始化计算要用的临时数据
                m_ptMouseDown = e.Location;
                m_ptCanvasBuf = m_ptCanvas;
            }
            pictureBox_preview.Focus();
        }
        bool init = false;
        private void pictureBox_preview_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {      //移动过程中 左键点下 重置画布坐标系
                m_ptCanvas = (Point)((Size)m_ptCanvasBuf + ((Size)e.Location - (Size)m_ptMouseDown));
                pictureBox_preview.Invalidate();
            }
        }

        private void pictureBox_preview_Paint(object sender, PaintEventArgs e)
        {
            if (init)
            {
                Graphics g = e.Graphics;
                g.TranslateTransform(m_ptCanvas.X, m_ptCanvas.Y);       //设置坐标偏移
                g.ScaleTransform(m_nScale, m_nScale);                   //设置缩放比
                g.DrawImage(bitmap, m_ptBmp);
            }
        }

        private void pictureBox_preview_MouseWheel(object sender, MouseEventArgs e)
        {
            if (m_nScale <= 0.3 && e.Delta <= 0) return;        //缩小下线
            if (m_nScale >= 4.9 && e.Delta >= 0) return;        //放大上线
            //获取 当前点到画布坐标原点的距离
            SizeF szSub = (Size)m_ptCanvas - (Size)e.Location;
            //当前的距离差除以缩放比还原到未缩放长度
            float tempX = szSub.Width / m_nScale;           //这里
            float tempY = szSub.Height / m_nScale;          //将画布比例
            //还原上一次的偏移                               //按照当前缩放比还原到
            m_ptCanvas.X -= (int)(szSub.Width - tempX);     //没有缩放
            m_ptCanvas.Y -= (int)(szSub.Height - tempY);    //的状态
            //重置距离差为  未缩放状态                       
            szSub.Width = tempX;
            szSub.Height = tempY;
            m_nScale += e.Delta > 0 ? 0.2F : -0.2F;
            //重新计算 缩放并 重置画布原点坐标
            m_ptCanvas.X += (int)(szSub.Width * m_nScale - szSub.Width);
            m_ptCanvas.Y += (int)(szSub.Height * m_nScale - szSub.Height);
            pictureBox_preview.Invalidate();
        }

        Point m_ptCanvaszmap;           //画布原点在设备上的坐标
        Point m_ptCanvasBufzmap;        //重置画布坐标计算时用的临时变量
        Point m_ptBmpzmap;              //图像位于画布坐标系中的坐标
        float m_nScalezmap = 1.0F;      //缩放比例
        Point m_ptMouseDownzmap;        //鼠标点下是在设备坐标上的坐标
        bool initzmap = false;
        private void pictureBox_zmap_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {      //如果左键点下    初始化计算要用的临时数据
                m_ptMouseDownzmap = e.Location;
                m_ptCanvasBufzmap = m_ptCanvaszmap;
            }
            pictureBox_zmap.Focus();
        }

        private void pictureBox_zmap_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {      //移动过程中 左键点下 重置画布坐标系
                m_ptCanvaszmap = (Point)((Size)m_ptCanvasBufzmap + ((Size)e.Location - (Size)m_ptMouseDownzmap));
                pictureBox_zmap.Invalidate();
            }
        }

        private void pictureBox_zmap_Paint(object sender, PaintEventArgs e)
        {
            if (initzmap)
            {
                Graphics g = e.Graphics;
                g.TranslateTransform(m_ptCanvaszmap.X, m_ptCanvaszmap.Y);       //设置坐标偏移
                g.ScaleTransform(m_nScalezmap, m_nScalezmap);                   //设置缩放比
                g.DrawImage(bitmap2, m_ptBmpzmap);
            }
        }

        private void pictureBox_zmap_MouseWheel(object sender, MouseEventArgs e)
        {
            if (m_nScalezmap <= 0.3 && e.Delta <= 0) return;        //缩小下线
            if (m_nScalezmap >= 4.9 && e.Delta >= 0) return;        //放大上线
            //获取 当前点到画布坐标原点的距离
            SizeF szSub = (Size)m_ptCanvaszmap - (Size)e.Location;
            //当前的距离差除以缩放比还原到未缩放长度
            float tempX = szSub.Width / m_nScalezmap;           //这里
            float tempY = szSub.Height / m_nScalezmap;          //将画布比例
            //还原上一次的偏移                               //按照当前缩放比还原到
            m_ptCanvaszmap.X -= (int)(szSub.Width - tempX);     //没有缩放
            m_ptCanvaszmap.Y -= (int)(szSub.Height - tempY);    //的状态
            //重置距离差为  未缩放状态                       
            szSub.Width = tempX;
            szSub.Height = tempY;
            m_nScalezmap += e.Delta > 0 ? 0.2F : -0.2F;
            //重新计算 缩放并 重置画布原点坐标
            m_ptCanvaszmap.X += (int)(szSub.Width * m_nScalezmap - szSub.Width);
            m_ptCanvaszmap.Y += (int)(szSub.Height * m_nScalezmap - szSub.Height);
            pictureBox_zmap.Invalidate();
        }

    }
}
