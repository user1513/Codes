using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
namespace GPS_RTC
{
    public partial class MainControl : Form
    {
        public MainControl()
        {
            InitializeComponent();
        }
        private string string1;
        public string String1
        {
            set
            {
                string1 = value;
            }
        }
        public void SetValue()
        {
            try
            {
                SPdataControl.PortName = string1;
                SPdataControl.Open();
            }
            catch
            {
            }
        }

        private void MainControl_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("将要退出主控制界面，是否继续？", "询问", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                Application.Exit();
            }
            else
            {
                
            }
        }
        //读取日期
        public void ReadDate()
        {
            string[] Day = new string[] { "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六" };  
            string dateDate = DateTime.Now.ToLongDateString();
            labDate.Text = string.Format("{0} ", dateDate);
            string week = Day[Convert.ToInt32(DateTime.Now.DayOfWeek.ToString("d"))].ToString();
            labDate.Text += week;
        }
        //读取时间
        public void ReadTime()
        {
            string dateTime = DateTime.Now.ToLongTimeString();
            labTime.Text = string.Format("{0}  ", dateTime);
        }
        private void MainControl_Load(object sender, EventArgs e)
        {

            ReadDate();//读取日期
            ReadTime();//读取时间
            timer_update.Enabled = true;
            for (int i = 17; i <= 99; i++)
            {
                cboxmodifyyear.Items.Add("20" + i.ToString());
            }
            for (int i = 1; i < 13; i++)
            {
                cboxmodifymonth.Items.Add(i.ToString());
            }
            for (int i = 1; i < 32; i++)
            {
                cboxmodifyday.Items.Add(i.ToString());
            }
            for (int i = 0; i < 24; i++)
            {
                cboxmodifyhour.Items.Add(i.ToString());
            }
            for (int i = 0; i < 60; i++)
            {
                cboxmodifyminute.Items.Add(i.ToString());
            }
            for (int i = 0; i < 61; i++)
            {
                cboxmodifysecond.Items.Add(i.ToString());
            }
            for (int i = 1; i < 8; i++)
            {
                cboxmodifyweek.Items.Add(i.ToString());
            }
            for (int i = 0; i < 24; i++)
            {
                cboxClockHour.Items.Add(i.ToString());
            }
            for (int i = 0; i < 60; i++)
            {
                cboxClockMinute.Items.Add(i.ToString());
            }
            for (int i = 0; i < 60; i++)
            {
                cboxClockSecond.Items.Add(i.ToString());
            }
            for (int i = 17; i <= 99; i++)
            {
                cboxClockyear.Items.Add("20" + i.ToString());
            }
            for (int i = 1; i < 13; i++)
            {
                cboxClockmonth.Items.Add(i.ToString());
            }
            for (int i = 1; i < 32; i++)
            {
                cboxClockday.Items.Add(i.ToString());
            }
            for (int i = 0; i < 31; i++)
            {
                cboxvolume.Items.Add(i.ToString());
            }
            string dateData = DateTime.Now.ToLongDateString();//日期
            string dateTime = DateTime.Now.ToLongTimeString();//时间
            string week = Convert.ToInt32(DateTime.Now.DayOfWeek.ToString("d")).ToString();//周


            if (week == "0")
            {
                week = "7";
            }


            cboxmodifyyear.Text = dateData.Substring(0, 4);
            cboxmodifymonth.Text = dateData.Substring(dateData.IndexOf('年') + 1, dateData.IndexOf('月') - (dateData.IndexOf('年') + 1));
            cboxmodifyday.Text = dateData.Substring(dateData.IndexOf('月') + 1, dateData.IndexOf('日') - (dateData.IndexOf('月') + 1));
            cboxmodifyhour.Text = dateTime.Substring(0, dateTime.IndexOf(':'));//0:00:00
            cboxmodifyminute.Text = dateTime.Substring(dateTime.IndexOf(':') + 1, 2);
            cboxmodifysecond.Text = dateTime.Substring(dateTime.IndexOf(':') + 4, 2);
            cboxmodifyweek.Text = week;//xiugai

        }

        private void timer_update_Tick(object sender, EventArgs e)
        {
            ReadDate();
            ReadTime();
        }
        int  acceptance_status;//接受状态

        private void btnSearch_Click(object sender, EventArgs e)
        {
            int password = Convert.ToInt32(tboxpassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }

            cboxNO.Items.Clear();
            cboxNO.Items.Add("ALL");
            btnSearch.Enabled = false;
            btnSearch.Text = "搜素中";
            btnSetPassword.Enabled = false;
            btnManualSetRTC.Enabled = false;
            btnAutoSetRTC.Enabled = false;
            btnReadalarmNumber.Enabled = false;
            btnNewalarm.Enabled = false;
            btndeletenowalarm.Enabled = false;
            btndeleteallalarm.Enabled = false;
            btnListenalarm.Enabled = false;
            btnCloseListenalarm.Enabled = false;
            btnclockOK.Enabled = false;
            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            DATE[1] = 0x00;//地址位
            DATE[2] = Convert.ToByte(password / 256);//密码高位
            DATE[3] = Convert.ToByte(password % 256);//密码低位
            DATE[4] = 0x01;//命令
            DATE[5] = 0x00;//数据位一
            DATE[6] = 0x00;//数据位二
            DATE[7] = 0x00;//数据位三
            DATE[8] = 0x00;//数据位四
            DATE[9] = 0x00;//数据位五
            DATE[10] = 0x00;//数据位六
            DATE[11] = 0x00;//数据位七
            DATE[12] = 0x00;//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 3000;//搜索3000毫秒
            timer_usart.Enabled = true;//按键使能关闭
            timer_usart.Start();    //定时器打开
            acceptance_status = 1;  //接收状态打开
                   
            SPdataControl.Write(DATE, 0, 15);//发送串口数据
        }
        private void btnManualSetRTC_Click(object sender, EventArgs e)
        {
            int password = Convert.ToInt32(tboxpassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }

            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(password / 256);//密码高位
            DATE[3] = Convert.ToByte(password % 256);//密码低位
            DATE[4] = 0x02;//命令
            DATE[5] = Convert.ToByte(cboxmodifyyear.Text.Substring(2, 2));//数据位一
            DATE[6] = Convert.ToByte(cboxmodifymonth.Text);//数据位二
            DATE[7] = Convert.ToByte(cboxmodifyday.Text);//数据位三
            DATE[8] = Convert.ToByte(cboxmodifyhour.Text);//数据位四
            DATE[9] = Convert.ToByte(cboxmodifyminute.Text);//数据位五
            DATE[10] = Convert.ToByte(cboxmodifysecond.Text);//数据位六
            DATE[11] = Convert.ToByte(cboxmodifyweek.Text);//数据位七
            DATE[12] = Convert.ToByte(cboxmodifytweek.Text);//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 1000;//1000毫秒
            timer_usart.Enabled = true;//按键使能关闭
            timer_usart.Start();    //定时器打开
            acceptance_status = 2;  //接收状态打开
                   
            SPdataControl.Write(DATE, 0, 15);//发送串口数据
        }
        private void btnAutoSetRTC_Click(object sender, EventArgs e)
        {
            string dateData = DateTime.Now.ToLongDateString();//日期
            string dateTime = DateTime.Now.ToLongTimeString();//时间
            string week = Convert.ToInt32(DateTime.Now.DayOfWeek.ToString("d")).ToString();//周

            if (week == "0")
            {
                week = "7";
            }

            byte _year = Convert.ToByte(dateData.Substring(2, 2));
            byte _month = Convert.ToByte(dateData.Substring(dateData.IndexOf('年') + 1, dateData.IndexOf('月') - (dateData.IndexOf('年') + 1)));
            byte _day = Convert.ToByte(dateData.Substring(dateData.IndexOf('月') + 1, dateData.IndexOf('日') - (dateData.IndexOf('月') + 1)));

            byte _hour = Convert.ToByte(dateTime.Substring(0, dateTime.IndexOf(':')));//0:00:00
            byte _min = Convert.ToByte(dateTime.Substring(dateTime.IndexOf(':') + 1, 2));
            byte _sec = Convert.ToByte(dateTime.Substring(dateTime.IndexOf(':') + 4, 2));
            byte _week = Convert.ToByte(week);

            cboxmodifyyear.Text = dateData.Substring(0, 4);
            cboxmodifymonth.Text = dateData.Substring(dateData.IndexOf('年') + 1, dateData.IndexOf('月') - (dateData.IndexOf('年') + 1));
            cboxmodifyday.Text = dateData.Substring(dateData.IndexOf('月') + 1, dateData.IndexOf('日') - (dateData.IndexOf('月') + 1));
            cboxmodifyhour.Text = dateTime.Substring(0, dateTime.IndexOf(':'));//0:00:00
            cboxmodifyminute.Text = dateTime.Substring(dateTime.IndexOf(':') + 1, 2);
            cboxmodifysecond.Text = dateTime.Substring(dateTime.IndexOf(':') + 4, 2);
            cboxmodifyweek.Text = week;//xiugai
            //0代表日
            int password = Convert.ToInt32(tboxpassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }

            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(password / 256);//密码高位
            DATE[3] = Convert.ToByte(password % 256);//密码低位
            DATE[4] = 0x03;//命令
            DATE[5] = _year;//数据位一
            DATE[6] = _month;//数据位二
            DATE[7] = _day;//数据位三
            DATE[8] = _hour;//数据位四
            DATE[9] = _min;//数据位五
            DATE[10] = _sec;//数据位六
            DATE[11] = _week;//数据位七
            DATE[12] = 0x00;//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 1000;//1000毫秒
            timer_usart.Start();    //定时器打开
            acceptance_status = 3;  //接收状态打开

            SPdataControl.Write(DATE, 0, 15);//发送串口数据
        }
        private void btnclockOK_Click(object sender, EventArgs e)
        {
            byte[] DATE = new byte[15];
            if (cboxClockNo.Text == "")
            {
                MessageBox.Show("闹钟编号不能为空");
                return;
            }
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(tboxpassword.Text.Substring(0, 2));//密码高位
            DATE[3] = Convert.ToByte(tboxpassword.Text.Substring(2, 2));//密码低位
            if (tabControl1.SelectedIndex == 0)//周次
            {
                DATE[4] = 0x04;//命令
                DATE[5] = Convert.ToByte(cboxClockNo.Text);//数据位一
                DATE[6] = Convert.ToByte(cboxClockHour.Text);//数据位二
                DATE[7] = Convert.ToByte(cboxClockMinute.Text);//数据位三
                DATE[8] = Convert.ToByte(cboxClockSecond.Text);//数据位四
                DATE[9] = Convert.ToByte(cboxClockbellNo.Text);//数据位五
                byte repeat = 0x00;
                if (cboxOne.Checked == true)
                {
                    repeat |= 0x01;
                }
                if (cboxTwo.Checked == true)
                {
                    repeat |= 0x02;
                }
                if (cboxThree.Checked == true)
                {
                    repeat |= 0x04;
                }
                if (cboxFour.Checked == true)
                {
                    repeat |= 0x08;
                }
                if (cboxFive.Checked == true)
                {
                    repeat |= 0x10;
                }
                if (cboxSix.Checked == true)
                {
                    repeat |= 0x20;
                }
                if (cboxSeven.Checked == true)
                {
                    repeat |= 0x40;
                }
                if (repeat == 0x00)
                {
                    repeat = 0x80;//一次哦
                }
                DATE[10] = repeat;//数据位六
                DATE[11] = 0x00;//数据位七
                DATE[12] = 0x00;//数据位八
                int checknumber = 0;
                for (int i = 1; i < 13; i++)
                {
                    checknumber += DATE[i];
                }
                DATE[13] = (byte)(checknumber & 0xFF);//校验位
                DATE[14] = 0xAA;//包尾

                timer_usart.Interval = 1000;//1000毫秒
                timer_usart.Enabled = true;//按键使能关闭
                timer_usart.Start();    //定时器打开
                acceptance_status = 4;  //接收状态打开

                SPdataControl.Write(DATE, 0, 15);
            }
            else if (tabControl1.SelectedIndex == 1)//日期
            {
                DATE[4] = 0x05;//命令
                DATE[5] = Convert.ToByte(cboxClockNo.Text);//数据位一
                DATE[6] = Convert.ToByte(cboxClockHour.Text);//数据位二
                DATE[7] = Convert.ToByte(cboxClockMinute.Text);//数据位三
                DATE[8] = Convert.ToByte(cboxClockSecond.Text);//数据位四
                DATE[9] = Convert.ToByte(cboxClockbellNo.Text);//数据位五
                DATE[10] = Convert.ToByte(cboxClockyear.Text.Substring(2, 2));//数据位六
                DATE[11] = Convert.ToByte(cboxClockmonth.Text);//数据位七
                DATE[12] = Convert.ToByte(cboxClockday.Text);//数据位八
                int checknumber = 0;
                for (int i = 1; i < 13; i++)
                {
                    checknumber += DATE[i];
                }
                DATE[13] = (byte)(checknumber & 0xFF);//校验位
                DATE[14] = 0xAA;//包尾

                timer_usart.Interval = 1000;//1000毫秒
                timer_usart.Enabled = true;//按键使能关闭
                timer_usart.Start();    //定时器打开
                acceptance_status = 5;  //接收状态打开

                SPdataControl.Write(DATE, 0, 15);
            }
        }
        private void btnSetPassword_Click(object sender, EventArgs e)
        {
            int password = Convert.ToInt32(tboxpassword.Text);
            int SetPassword = Convert.ToInt32(tboxSetPassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }
            if ((SetPassword < 0) || (SetPassword > 9999))
            {
                MessageBox.Show("新密码输入范围超限");
                return;
            }

            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(password / 256);//密码高位
            DATE[3] = Convert.ToByte(password % 256);//密码低位
            DATE[4] = 0x06;//命令
            DATE[5] = Convert.ToByte(SetPassword / 256);//数据位一
            DATE[6] = Convert.ToByte(SetPassword % 256);//数据位二
            DATE[7] = 0x00;//数据位三
            DATE[8] = 0x00;//数据位四
            DATE[9] = 0x00;//数据位五
            DATE[10] = 0x00;//数据位六
            DATE[11] = 0x00;//数据位七
            DATE[12] = 0x00;//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 1000;//1000毫秒
            timer_usart.Enabled = true;//按键使能关闭
            timer_usart.Start();    //定时器打开
            acceptance_status = 6;  //接收状态打开

            SPdataControl.Write(DATE, 0, 15);
        }

        private void btnListenalarm_Click(object sender, EventArgs e)
        {
            int password = Convert.ToInt32(tboxpassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }

            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(password / 256);//密码高位
            DATE[3] = Convert.ToByte(password % 256);//密码低位
            DATE[4] = 0x07;//命令
            DATE[5] = Convert.ToByte(cboxClockbellNo.Text);//数据位一
            DATE[6] = 0x00;//数据位二
            DATE[7] = 0x00;//数据位三
            DATE[8] = 0x00;//数据位四
            DATE[9] = 0x00;//数据位五
            DATE[10] = 0x00;//数据位六
            DATE[11] = 0x00;//数据位七
            DATE[12] = 0x00;//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 1000;//1000毫秒
            timer_usart.Enabled = true;//按键使能关闭
            timer_usart.Start();    //定时器打开
            acceptance_status = 7;  //接收状态打开

            SPdataControl.Write(DATE, 0, 15);
        }

        private void btnCloseListenalarm_Click(object sender, EventArgs e)
        {
            int password = Convert.ToInt32(tboxpassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }
            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(password / 256);//密码高位
            DATE[3] = Convert.ToByte(password % 256);//密码低位
            DATE[4] = 0x08;//命令
            DATE[5] = 0x00;//数据位一
            DATE[6] = 0x00;//数据位二
            DATE[7] = 0x00;//数据位三
            DATE[8] = 0x00;//数据位四
            DATE[9] = 0x00;//数据位五
            DATE[10] = 0x00;//数据位六
            DATE[11] = 0x00;//数据位七
            DATE[12] = 0x00;//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 1000;//1000毫秒
            timer_usart.Enabled = true;//按键使能关闭
            timer_usart.Start();    //定时器打开
            acceptance_status = 8;  //接收状态打开

            SPdataControl.Write(DATE, 0, 15);
        }
        private void btnReadalarmNumber_Click(object sender, EventArgs e)
        {
            int password = Convert.ToInt32(tboxpassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }
            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(password / 256);//密码高位
            DATE[3] = Convert.ToByte(password % 256);//密码低位
            DATE[4] = 0x09;//命令
            DATE[5] = 0x00;//数据位一
            DATE[6] = 0x00;//数据位二
            DATE[7] = 0x00;//数据位三
            DATE[8] = 0x00;//数据位四
            DATE[9] = 0x00;//数据位五
            DATE[10] = 0x00;//数据位六
            DATE[11] = 0x00;//数据位七
            DATE[12] = 0x00;//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 3000;//1000毫秒
            timer_usart.Enabled = true;//按键使能关闭
            timer_usart.Start();    //定时器打开
            acceptance_status = 9;  //接收状态打开

            SPdataControl.Write(DATE, 0, 15);

            //
        }

        private void btndeletenowalarm_Click(object sender, EventArgs e)
        {
            int password = Convert.ToInt32(tboxpassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }
            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(password / 256);//密码高位
            DATE[3] = Convert.ToByte(password % 256);//密码低位
            DATE[4] = 0x0A;//命令
            DATE[5] = Convert.ToByte(cboxClockNo.Text);//数据位一
            DATE[6] = 0x00;//数据位二
            DATE[7] = 0x00;//数据位三
            DATE[8] = 0x00;//数据位四
            DATE[9] = 0x00;//数据位五
            DATE[10] = 0x00;//数据位六
            DATE[11] = 0x00;//数据位七
            DATE[12] = 0x00;//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 1000;//1000毫秒
            timer_usart.Enabled = true;//按键使能关闭
            timer_usart.Start();    //定时器打开
            acceptance_status = 10;  //接收状态打开

            SPdataControl.Write(DATE, 0, 15);
        }

        private void btndeleteallalarm_Click(object sender, EventArgs e)
        {
            int password = Convert.ToInt32(tboxpassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }
            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(tboxpassword.Text.Substring(0, 2));//密码高位
            DATE[3] = Convert.ToByte(tboxpassword.Text.Substring(2, 2));//密码低位
            DATE[4] = 0x0B;//命令
            DATE[5] = 0x00;//数据位一
            DATE[6] = 0x00;//数据位二
            DATE[7] = 0x00;//数据位三
            DATE[8] = 0x00;//数据位四
            DATE[9] = 0x00;//数据位五
            DATE[10] = 0x00;//数据位六
            DATE[11] = 0x00;//数据位七
            DATE[12] = 0x00;//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 1000;//1000毫秒
            timer_usart.Enabled = true;//按键使能关闭
            timer_usart.Start();    //定时器打开
            acceptance_status = 11;  //接收状态打开

            SPdataControl.Write(DATE, 0, 15);
        }

        private int Count = 0;
        int[,] Alarm_Data = new int[50,8];
        private int sys_clocknumber = 0;
        private void SPdataControl_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {


            byte[] array = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
            int n = SPdataControl.BytesToRead;
            byte[] buf = new byte[n];
            SPdataControl.Read(buf, 0, n);

            for (int i = 0; i < n; i++)
            {
                array[Count] = buf[i];
                if (acceptance_status != 255)
                {
                    switch (Count)
                    {
                        case 0:
                            if (array[Count] == 0x55)
                            {
                                Count++;
                            }
                            else
                            {
                                Count = 0;
                            }
                            break;
                        case 1:
                            Count++;
                            break;
                        case 2:
                            Count++;
                            break;
                        case 3:
                            Count++;
                            break;
                        case 4:
                            if (array[Count] == (byte)(array[1] + array[2] + array[3]))
                            {
                                Count++;
                            }
                            else
                            {
                                Count = 0;
                            }

                            break;
                        case 5:
                            if (array[Count] == 0xAA)
                            {
                                Count = 0;
                                switch (array[2])//判断命令
                                {
                                    case 0x01:
                                        if (acceptance_status == 1)
                                        {
                                            if (array[3] != 0x00)
                                            {
                                                string str = Convert.ToString(array[3], 16).ToUpper();
                                                txtRecieve.AppendText("发现地址编号为：" + (str.Length == 1 ? "0" + str : str) + "的设备\r\n");
                                                cboxNO.Items.Add(str);
                                            }
                                        }
                                        break;
                                    case 0x02:
                                        if (acceptance_status == 2)
                                        {
                                            if (array[3] == 0x01)//正确
                                            {
                                                string str = Convert.ToString(array[1], 16).ToUpper();
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "的时间设置成功\r\n");
                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }

                                            }
                                        }
                                        break;
                                    case 0x03:
                                        if (acceptance_status == 3)
                                        {
                                            if (array[3] == 0x01)//正确
                                            {
                                                string str = Convert.ToString(array[1], 16).ToUpper();
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "的时间同步成功\r\n");

                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }
                                            }
                                        }
                                        break;
                                    case 0x04:
                                        if (acceptance_status == 4)//正确
                                        {
                                            if (array[3] == 0x01)//正确
                                            {
                                                string str = Convert.ToString(array[1], 16).ToUpper();
                                                string str1 = "设备编号为" + (str.Length == 1 ? "0" + str : str) + "的闹铃" + cboxClockNo.Text + "设置成功,闹铃时间为" + cboxClockHour.Text + "时" + cboxClockMinute.Text + "分" + cboxClockSecond.Text + "秒，铃声为：" + cboxClockbellNo.Text + ",重复响铃";
                                                byte flag = 0;
                                                if (cboxOne.Checked == true)
                                                {
                                                    str1 += " 星期一";
                                                    flag = 1;
                                                }
                                                if (cboxTwo.Checked == true)
                                                {
                                                    str1 += " 星期二";
                                                    flag = 1;
                                                }
                                                if (cboxThree.Checked == true)
                                                {
                                                    str1 += " 星期三";
                                                    flag = 1;
                                                }
                                                if (cboxFour.Checked == true)
                                                {
                                                    str1 += " 星期四";
                                                    flag = 1;
                                                }
                                                if (cboxFive.Checked == true)
                                                {
                                                    str1 += " 星期五";
                                                    flag = 1;
                                                }
                                                if (cboxSix.Checked == true)
                                                {
                                                    str1 += " 星期六";
                                                    flag = 1;
                                                }
                                                if (cboxSeven.Checked == true)
                                                {
                                                    str1 += " 星期日";
                                                    flag = 1;
                                                }
                                                if (flag == 0)
                                                {
                                                    str1 += "无";
                                                }
                                                str1 += "\r\n";
                                                txtRecieve.AppendText(str1);
                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }
                                            }
                                        }
                                        break;
                                    case 0x05:
                                        if (acceptance_status == 5)//正确
                                        {
                                            if (array[3] == 0x01)//正确
                                            {
                                                string str = Convert.ToString(array[1], 16).ToUpper();
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "的闹铃" + cboxClockNo.Text + "设置成功,闹铃时间为" + cboxClockyear.Text + "年" + cboxClockmonth.Text + "月" + cboxClockday.Text + "日" + cboxClockHour.Text + "时" + cboxClockMinute.Text + "分" + cboxClockSecond.Text + "秒，铃声为：" + cboxClockbellNo.Text + "。\r\n");

                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }
                                            }
                                        }
                                        break;
                                    case 0x06:
                                        if (acceptance_status == 6)
                                        {
                                            if (array[3] == 0x01)//正确
                                            {
                                                string str = Convert.ToString(array[1], 16).ToUpper();
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "的密码修改成功\r\n");
                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }
                                            }
                                        }
                                        break;
                                    case 0x07:
                                        if (acceptance_status == 7)
                                        {
                                            if (array[3] == 0x01)//正确
                                            {
                                                string str = Convert.ToString(array[1], 16).ToUpper();
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "的闹铃" + cboxClockbellNo.Text + "试听中……\r\n");

                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }
                                            }
                                        }
                                        break;
                                    case 0x08:
                                        if (acceptance_status == 8)
                                        {
                                            if (array[3] == 0x01)//正确
                                            {
                                                string str = Convert.ToString(array[1], 16).ToUpper();
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "的闹铃取消试听成功\r\n");
                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }
                                            }
                                        }
                                        break;
                                    case 0x09:
                                        if (acceptance_status == 9)
                                        {
                                            string str = Convert.ToString(array[1], 16).ToUpper();
                                            sys_clocknumber = Convert.ToByte(array[3]);
                                            if (sys_clocknumber != 0)
                                            {
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "的闹铃共有" + array[3] + "个，正在读取中……\r\n");

                                                timer_usart.Interval = 3000;
                                                acceptance_status = 255;
                                            }
                                            else
                                            {
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "的闹铃共有" + array[3] + "个，不进行读取操作\r\n");
                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }
                                            }
                                            cboxClockNo.Items.Clear();
                                        }
                                        break;
                                    case 0x0A:
                                        if (acceptance_status == 10)
                                        {
                                            if (array[3] == 0x01)//正确
                                            {
                                                string str = Convert.ToString(array[1], 16).ToUpper();
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "删除当前闹铃成功\r\n");

                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }
                                            }
                                        }
                                        break;
                                    case 0x0B:
                                        if (acceptance_status == 11)
                                        {
                                            if (array[3] == 0x01)//正确
                                            {
                                                string str = Convert.ToString(array[1], 16).ToUpper();
                                                txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "删除所有闹铃成功\r\n");

                                                if (cboxNO.Text != "ALL")
                                                {
                                                    acceptance_status = 0;
                                                    timer_usart.Stop();
                                                    timer_usart.Enabled = false;
                                                }
                                            }
                                        }
                                        break;
                                    case 0x0c:
                                        if (array[3] == 0x01)//正确
                                        {
                                            string str = Convert.ToString(array[1], 16).ToUpper();
                                            txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "密码输入错误\r\n");

                                            //acceptance_status = 0;
                                            //timer_usart.Stop();
                                            //timer_usart.Enabled = false;
                                        }
                                        break;
                                    case 0x0D://设置音量
                                        if (array[3] == 0x01)//正确
                                        {
                                            string str = Convert.ToString(array[1], 16).ToUpper();
                                            txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "设置音量成功\r\n");

                                            if (cboxNO.Text != "ALL")
                                            {
                                                acceptance_status = 0;
                                                timer_usart.Stop();
                                                timer_usart.Enabled = false;
                                            }
                                        }
                                        break;
                                    default:
                                        break;
                                }
                            }
                            else
                            {
                                Count = 0;
                            }
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    switch (Count)
                    {
                        case 0:
                            if (array[Count] == 0x66)
                            {
                                Count++;
                            }
                            else
                            {
                                Count = 0;
                            }
                            break;
                        case 1:
                            //if (array[Count] == 0x00)//地址判断
                            //{
                                Count++;
//                            }
                            //else
                            //{
                            //    Count = 0;
                            //}

                            break;
                        case 2:
                            if (array[Count] < 50)//个数判断
                            {
                                Count++;
                            }
                            else
                            {
                                Count = 0;
                            }
                            break;
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 7:
                        case 8:
                        case 9:
                        case 10:
                            Count++;
                            break;
                        case 11:
                            if (array[Count] == (byte)(array[1] + array[2] + array[3] + array[4] + array[5] + array[6] + array[7] + array[8] + array[9] + array[10]))
                            {
                                Count++;
                            }
                            else
                            {
                                Count = 0;
                            }
                            break;
                        case 12:
                            if (array[Count] == 0x99)
                            {


                                for (int j = 3; j < 11; j++)
                                {
                                    Alarm_Data[array[2] - 1, j - 3] = array[j] / 16 * 10 + array[j] % 16;
                                }
                                string str = Convert.ToString(array[1], 16).ToUpper();
                                if (Alarm_Data[array[2] - 1, 0] == 0x01)
                                {
                                    string str1 = "设备编号为" + str + "的闹钟" + array[2].ToString() + "数据为" + Alarm_Data[array[2] - 1, 1] + "时" + Alarm_Data[array[2] - 1, 2] + "分" + Alarm_Data[array[2] - 1, 3] + "秒，铃声为" + Alarm_Data[array[2] - 1, 4] + ",重复响铃";
                                    byte flag = 0;
                                    if (cboxOne.Checked == true)
                                    {
                                        str1 += " 星期一";
                                        flag = 1;
                                    }
                                    if (cboxTwo.Checked == true)
                                    {
                                        str1 += " 星期二";
                                        flag = 1;
                                    }
                                    if (cboxThree.Checked == true)
                                    {
                                        str1 += " 星期三";
                                        flag = 1;
                                    }
                                    if (cboxFour.Checked == true)
                                    {
                                        str1 += " 星期四";
                                        flag = 1;
                                    }
                                    if (cboxFive.Checked == true)
                                    {
                                        str1 += " 星期五";
                                        flag = 1;
                                    }
                                    if (cboxSix.Checked == true)
                                    {
                                        str1 += " 星期六";
                                        flag = 1;
                                    }
                                    if (cboxSeven.Checked == true)
                                    {
                                        str1 += " 星期日";
                                        flag = 1;
                                    }
                                    if (flag == 0)
                                    {
                                        str1 += "无";
                                    }
                                    str1 += "\r\n";
                                    txtRecieve.AppendText(str1);
                                }
                                else if (Alarm_Data[array[2] - 1, 0] == 0x02)
                                {
                                    txtRecieve.AppendText("设备编号为" + (str.Length == 1 ? "0" + str : str) + "的数据为" + Alarm_Data[array[2] - 1, 5] + "年" + Alarm_Data[array[2] - 1, 6] + "月" + Alarm_Data[array[2] - 1, 7] + "日" + Alarm_Data[array[2] - 1, 1] + "时" + Alarm_Data[array[2] - 1, 2] + "分" + Alarm_Data[array[2] - 1, 3] + "秒，铃声为" + Alarm_Data[array[2] - 1, 4] + "。\r\n");

                                }
                                cboxClockNo.Items.Add(array[2]);
                                if (sys_clocknumber == array[2])
                                {
                                    txtRecieve.AppendText("读取结束\r\n");
                                    acceptance_status = 0;
                                    timer_usart.Stop();
                                    timer_usart.Enabled = false;
                                    cboxClockNo.SelectedIndex = 0;
                                }
                                else
                                {
                                    timer_usart.Interval = 3000;
                                }
                                

                            }
                            Count = 0;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        private void timer_usart_Tick(object sender, EventArgs e)
        {
            timer_usart.Stop();
            timer_usart.Enabled = false;
            if (cboxNO.Text == "ALL")
            {
                if (acceptance_status != 1)
                {
                    acceptance_status = 0;
                    timer_usart.Stop();
                    timer_usart.Enabled = false;
                    return;
                }

            }
            switch (acceptance_status)
            {
                case 1:
                    btnSearch.Enabled = true;
                    btnSetPassword.Enabled = true;
                    btnManualSetRTC.Enabled = true;
                    btnAutoSetRTC.Enabled = true;
                    btnReadalarmNumber.Enabled = true;
                    btnNewalarm.Enabled = true;
                    btndeletenowalarm.Enabled = true;
                    btndeleteallalarm.Enabled = true;
                    btnListenalarm.Enabled = true;
                    btnCloseListenalarm.Enabled = true;
                    btnclockOK.Enabled = true;
                    btnSearch.Text = "搜索在线设备";
                    cboxNO.SelectedIndex = 0;
                    txtRecieve.AppendText("搜索结束\r\n");
                    break;
                case 2:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "的时间设置失败\r\n");
                    break;
                case 3:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "的时间同步失败\r\n");
                    break;
                case 4:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "的闹铃设置失败\r\n");
                    break;
                case 5:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "的闹铃设置失败\r\n");
                    break;
                case 6:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "的密码修改失败\r\n");
                    break;
                case 7:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "的闹铃" + cboxClockbellNo.Text + "试听错误\r\n");
                    break;
                case 8:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "的闹铃取消试听失败\r\n");
                    break;
                case 9:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "的闹铃个数读取失败\r\n");
                    break;
                case 10:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "删除当前闹铃失败\r\n");
                    break;
                case 11:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "删除所有闹铃失败\r\n");
                    break;
                case 12:
                    txtRecieve.AppendText("设备编号为" + cboxNO.Text + "设置音量失败\r\n");
                    break;
                case 255:
                    txtRecieve.AppendText("读取闹钟超时\r\n");
                    break;
                default:
                    break;
            }
            acceptance_status = 0;
        }

        private void cboxClockNo_SelectedIndexChanged(object sender, EventArgs e)
        {
            cboxClockHour.Text = Alarm_Data[cboxClockNo.SelectedIndex, 1].ToString();
            cboxClockMinute.Text = Alarm_Data[cboxClockNo.SelectedIndex, 2].ToString();
            cboxClockSecond.Text = Alarm_Data[cboxClockNo.SelectedIndex, 3].ToString();
            cboxClockbellNo.Text = Alarm_Data[cboxClockNo.SelectedIndex, 4].ToString();

            if (Alarm_Data[cboxClockNo.SelectedIndex, 0] == 0x01)
            {
                //tabControl1.SelectTab(0);
                if (Alarm_Data[cboxClockNo.SelectedIndex, 5] / 1 == 1)
                {
                    cboxOne.Checked = true;
                }else
                {
                    cboxOne.Checked = false;
                }
                if (Alarm_Data[cboxClockNo.SelectedIndex, 5] / 2 == 1)
                {
                    cboxTwo.Checked = true;
                }else
                {
                    cboxTwo.Checked = false;
                }
                if (Alarm_Data[cboxClockNo.SelectedIndex, 5] / 4 == 1)
                {
                    cboxThree.Checked = true; 
                }else
                {
                    cboxThree.Checked = false;
                }
                if (Alarm_Data[cboxClockNo.SelectedIndex, 5] / 8 == 1)
                {
                    cboxFour.Checked = true;
                }else
                {
                    cboxFour.Checked = false;
                }
                if (Alarm_Data[cboxClockNo.SelectedIndex, 5] / 16 == 1)
                {
                    cboxFive.Checked = true;
                }else
                {
                    cboxFive.Checked = false;
                }
                if (Alarm_Data[cboxClockNo.SelectedIndex, 5] / 32 == 1)
                {
                    cboxSix.Checked = true;
                }else
                {
                    cboxSix.Checked = false;
                }
                if (Alarm_Data[cboxClockNo.SelectedIndex, 5] / 64 == 1)
                {
                    cboxSeven.Checked = true;
                }
                else
                {
                    cboxSeven.Checked = false;
                }
            }
            else if (Alarm_Data[cboxClockNo.SelectedIndex, 0] == 0x02)
            {
                //tabControl1.SelectTab(1);
                cboxClockyear.Text = Alarm_Data[cboxClockNo.SelectedIndex, 5].ToString();
                cboxClockmonth.Text = Alarm_Data[cboxClockNo.SelectedIndex, 6].ToString();
                cboxClockday.Text = Alarm_Data[cboxClockNo.SelectedIndex, 7].ToString();
                
            }
        }

        private void btnNewalarm_Click(object sender, EventArgs e)
        {
            sys_clocknumber++;
            cboxClockNo.Text = sys_clocknumber.ToString();
            cboxClockNo.Items.Add(sys_clocknumber);
        }

        private void btnsetvolume_Click(object sender, EventArgs e)
        {
            int password = Convert.ToInt32(tboxpassword.Text);
            if ((password < 0) || (password > 9999))
            {
                MessageBox.Show("设备密码输入范围超限");
                return;
            }
            byte[] DATE = new byte[15];
            DATE[0] = 0x55;//包头
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
            DATE[2] = Convert.ToByte(password / 256);//密码高位
            DATE[3] = Convert.ToByte(password % 256);//密码低位
            DATE[4] = 0x0C;//命令
            DATE[5] = Convert.ToByte(cboxvolume.Text);//数据位一
            DATE[6] = 0x00;//数据位二
            DATE[7] = 0x00;//数据位三
            DATE[8] = 0x00;//数据位四
            DATE[9] = 0x00;//数据位五
            DATE[10] = 0x00;//数据位六
            DATE[11] = 0x00;//数据位七
            DATE[12] = 0x00;//数据位八
            int checknumber = 0;
            for (int i = 1; i < 13; i++)
            {
                checknumber += DATE[i];
            }
            DATE[13] = (byte)(checknumber & 0xFF);//校验位
            DATE[14] = 0xAA;//包尾

            timer_usart.Interval = 1000;//1000毫秒
            timer_usart.Enabled = true;//按键使能关闭
            timer_usart.Start();    //定时器打开
            acceptance_status = 12;  //接收状态打开

            SPdataControl.Write(DATE, 0, 15);
        }
    }
}
