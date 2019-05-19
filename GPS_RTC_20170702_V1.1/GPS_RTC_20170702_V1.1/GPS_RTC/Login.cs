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
    public partial class Login : Form
    {
        public Login()
        {
            InitializeComponent();
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            cboxCOM.Text = "搜索中";
            SearchAndAddSerialToComBox(SPdataLogin, cboxCOM);
        }
        //搜索串口
        private void SearchAndAddSerialToComBox(SerialPort Myport, ComboBox MyBox)
        {
            String[] MyString = new string[10];
            string Buffer;
            MyBox.Items.Clear();//移除所有COM口
            int count = 0;
            for (int i = 0; i <= 10; i++)
            {
                try
                {
                    Buffer = "COM" + i.ToString();
                    Myport.PortName = Buffer;
                    Myport.Open();
                    MyString[count++] = Buffer;
                    MyBox.Items.Add(Buffer);
                    Myport.Close();
                }
                catch
                {
                }
            }
            MyBox.Text = MyString[0];
            //搜索没有找到可用串口则会提示，屏蔽没有提示
            if (MyBox.Text == "")
            {
                MessageBox.Show("没有找到可用串口,请重试！");
            }
        }

        private void Login_Load(object sender, EventArgs e)
        {
            SearchAndAddSerialToComBox(SPdataLogin, cboxCOM);//搜索可用串口
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            if (cboxCOM.Text == "")
            {
                MessageBox.Show("没有检测到上位机组件，请重试！");
                return;
            }
            if (txtuser.Text == "")
            {
                MessageBox.Show("账号不能为空");
                return;
            }
            if (txtpassword.Text == "")
            {
                MessageBox.Show("密码不能为空");
                return;
            }

            try
            {
                SPdataLogin.PortName = cboxCOM.Text;
                SPdataLogin.Open();
                SPdataLogin.Close();
            }
            catch
            {
                MessageBox.Show("登陆失败！！！串口设备打开错误");
                return;
            }

            if ((txtuser.Text == "admin") && (txtpassword.Text == "admin"))
            {

                
                MessageBox.Show("登陆成功");

                Login Login = new Login();
                this.Hide();


                MainControl MainControl = new MainControl();//实例化一个MainControl窗口
                
                
                MainControl.String1 = cboxCOM.Text;//设置MainControl中String1的值
                MainControl.SetValue();//设置Form2中Label1的

                
                MainControl.ShowDialog();
                
                
            }
            else
            {
                MessageBox.Show("用户名或密码错误，请重试");
            }

        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {

        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void txtpassword_TextChanged(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void txtuser_TextChanged(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void cboxCOM_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }



    }
}
