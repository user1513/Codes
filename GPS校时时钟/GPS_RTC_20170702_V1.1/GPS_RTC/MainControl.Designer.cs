namespace GPS_RTC
{
    partial class MainControl
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainControl));
            this.SPdataControl = new System.IO.Ports.SerialPort(this.components);
            this.btnSearch = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.cboxNO = new System.Windows.Forms.ComboBox();
            this.btnAutoSetRTC = new System.Windows.Forms.Button();
            this.txtRecieve = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.tboxpassword = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label15 = new System.Windows.Forms.Label();
            this.cboxmodifytweek = new System.Windows.Forms.ComboBox();
            this.label14 = new System.Windows.Forms.Label();
            this.btnManualSetRTC = new System.Windows.Forms.Button();
            this.cboxmodifyweek = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.cboxmodifysecond = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.cboxmodifyminute = new System.Windows.Forms.ComboBox();
            this.cboxmodifyhour = new System.Windows.Forms.ComboBox();
            this.cboxmodifyday = new System.Windows.Forms.ComboBox();
            this.cboxmodifymonth = new System.Windows.Forms.ComboBox();
            this.cboxmodifyyear = new System.Windows.Forms.ComboBox();
            this.labTime = new System.Windows.Forms.Label();
            this.labDate = new System.Windows.Forms.Label();
            this.timer_update = new System.Windows.Forms.Timer(this.components);
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.btnCloseListenalarm = new System.Windows.Forms.Button();
            this.btnListenalarm = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.cboxSeven = new System.Windows.Forms.CheckBox();
            this.cboxSix = new System.Windows.Forms.CheckBox();
            this.cboxFive = new System.Windows.Forms.CheckBox();
            this.cboxFour = new System.Windows.Forms.CheckBox();
            this.cboxThree = new System.Windows.Forms.CheckBox();
            this.cboxTwo = new System.Windows.Forms.CheckBox();
            this.cboxOne = new System.Windows.Forms.CheckBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.cboxClockday = new System.Windows.Forms.ComboBox();
            this.cboxClockmonth = new System.Windows.Forms.ComboBox();
            this.cboxClockyear = new System.Windows.Forms.ComboBox();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.btnReadalarmNumber = new System.Windows.Forms.Button();
            this.btndeleteallalarm = new System.Windows.Forms.Button();
            this.btndeletenowalarm = new System.Windows.Forms.Button();
            this.btnNewalarm = new System.Windows.Forms.Button();
            this.cboxClockNo = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.btnclockOK = new System.Windows.Forms.Button();
            this.cboxClockbellNo = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.cboxClockSecond = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.cboxClockMinute = new System.Windows.Forms.ComboBox();
            this.cboxClockHour = new System.Windows.Forms.ComboBox();
            this.label12 = new System.Windows.Forms.Label();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.btnSetPassword = new System.Windows.Forms.Button();
            this.tboxSetPassword = new System.Windows.Forms.TextBox();
            this.timer_usart = new System.Windows.Forms.Timer(this.components);
            this.btnsetvolume = new System.Windows.Forms.Button();
            this.cboxvolume = new System.Windows.Forms.ComboBox();
            this.label19 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // SPdataControl
            // 
            this.SPdataControl.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SPdataControl_DataReceived);
            // 
            // btnSearch
            // 
            this.btnSearch.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnSearch.Location = new System.Drawing.Point(229, 14);
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.Size = new System.Drawing.Size(164, 35);
            this.btnSearch.TabIndex = 0;
            this.btnSearch.Text = "搜索在线设备";
            this.btnSearch.UseVisualStyleBackColor = true;
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 13.8F);
            this.label1.Location = new System.Drawing.Point(9, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(130, 24);
            this.label1.TabIndex = 1;
            this.label1.Text = "当前设备：";
            // 
            // cboxNO
            // 
            this.cboxNO.FormattingEnabled = true;
            this.cboxNO.Location = new System.Drawing.Point(163, 21);
            this.cboxNO.Name = "cboxNO";
            this.cboxNO.Size = new System.Drawing.Size(60, 23);
            this.cboxNO.TabIndex = 2;
            this.cboxNO.Text = "ALL";
            // 
            // btnAutoSetRTC
            // 
            this.btnAutoSetRTC.Location = new System.Drawing.Point(871, 23);
            this.btnAutoSetRTC.Name = "btnAutoSetRTC";
            this.btnAutoSetRTC.Size = new System.Drawing.Size(132, 27);
            this.btnAutoSetRTC.TabIndex = 3;
            this.btnAutoSetRTC.Text = "同步电脑时间";
            this.btnAutoSetRTC.UseVisualStyleBackColor = true;
            this.btnAutoSetRTC.Click += new System.EventHandler(this.btnAutoSetRTC_Click);
            // 
            // txtRecieve
            // 
            this.txtRecieve.Location = new System.Drawing.Point(12, 24);
            this.txtRecieve.Multiline = true;
            this.txtRecieve.Name = "txtRecieve";
            this.txtRecieve.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtRecieve.Size = new System.Drawing.Size(908, 117);
            this.txtRecieve.TabIndex = 4;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.txtRecieve);
            this.groupBox1.Location = new System.Drawing.Point(0, 282);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(924, 147);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "信息提示区域";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.cboxNO);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.btnSearch);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(400, 57);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "设置";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.tboxpassword);
            this.groupBox3.Location = new System.Drawing.Point(418, 13);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(108, 57);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "设备密码区";
            // 
            // tboxpassword
            // 
            this.tboxpassword.Location = new System.Drawing.Point(23, 21);
            this.tboxpassword.Name = "tboxpassword";
            this.tboxpassword.Size = new System.Drawing.Size(41, 25);
            this.tboxpassword.TabIndex = 0;
            this.tboxpassword.Text = "0000";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(91, 28);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 20);
            this.label2.TabIndex = 8;
            this.label2.Text = "年";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(177, 28);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(28, 19);
            this.label3.TabIndex = 9;
            this.label3.Text = "月";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(270, 28);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(28, 19);
            this.label4.TabIndex = 10;
            this.label4.Text = "日";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.Location = new System.Drawing.Point(348, 28);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(28, 19);
            this.label8.TabIndex = 14;
            this.label8.Text = "时";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label15);
            this.groupBox4.Controls.Add(this.cboxmodifytweek);
            this.groupBox4.Controls.Add(this.label14);
            this.groupBox4.Controls.Add(this.btnManualSetRTC);
            this.groupBox4.Controls.Add(this.cboxmodifyweek);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Controls.Add(this.btnAutoSetRTC);
            this.groupBox4.Controls.Add(this.cboxmodifysecond);
            this.groupBox4.Controls.Add(this.label7);
            this.groupBox4.Controls.Add(this.cboxmodifyminute);
            this.groupBox4.Controls.Add(this.cboxmodifyhour);
            this.groupBox4.Controls.Add(this.cboxmodifyday);
            this.groupBox4.Controls.Add(this.label8);
            this.groupBox4.Controls.Add(this.cboxmodifymonth);
            this.groupBox4.Controls.Add(this.cboxmodifyyear);
            this.groupBox4.Controls.Add(this.label2);
            this.groupBox4.Controls.Add(this.label3);
            this.groupBox4.Controls.Add(this.label4);
            this.groupBox4.Location = new System.Drawing.Point(12, 76);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(1009, 63);
            this.groupBox4.TabIndex = 15;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "设置时间";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label15.Location = new System.Drawing.Point(725, 28);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(28, 19);
            this.label15.TabIndex = 24;
            this.label15.Text = "周";
            // 
            // cboxmodifytweek
            // 
            this.cboxmodifytweek.FormattingEnabled = true;
            this.cboxmodifytweek.Location = new System.Drawing.Point(674, 26);
            this.cboxmodifytweek.Name = "cboxmodifytweek";
            this.cboxmodifytweek.Size = new System.Drawing.Size(45, 23);
            this.cboxmodifytweek.TabIndex = 23;
            this.cboxmodifytweek.Text = "1";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label14.Location = new System.Drawing.Point(640, 28);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(28, 19);
            this.label14.TabIndex = 22;
            this.label14.Text = "第";
            // 
            // btnManualSetRTC
            // 
            this.btnManualSetRTC.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnManualSetRTC.Location = new System.Drawing.Point(796, 24);
            this.btnManualSetRTC.Name = "btnManualSetRTC";
            this.btnManualSetRTC.Size = new System.Drawing.Size(59, 27);
            this.btnManualSetRTC.TabIndex = 21;
            this.btnManualSetRTC.Text = "修改";
            this.btnManualSetRTC.UseVisualStyleBackColor = true;
            this.btnManualSetRTC.Click += new System.EventHandler(this.btnManualSetRTC_Click);
            // 
            // cboxmodifyweek
            // 
            this.cboxmodifyweek.FormattingEnabled = true;
            this.cboxmodifyweek.Location = new System.Drawing.Point(579, 27);
            this.cboxmodifyweek.Name = "cboxmodifyweek";
            this.cboxmodifyweek.Size = new System.Drawing.Size(45, 23);
            this.cboxmodifyweek.TabIndex = 20;
            this.cboxmodifyweek.Text = "1";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(545, 28);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(28, 19);
            this.label5.TabIndex = 19;
            this.label5.Text = "周";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.Location = new System.Drawing.Point(511, 28);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(28, 19);
            this.label6.TabIndex = 18;
            this.label6.Text = "秒";
            // 
            // cboxmodifysecond
            // 
            this.cboxmodifysecond.FormattingEnabled = true;
            this.cboxmodifysecond.Location = new System.Drawing.Point(460, 25);
            this.cboxmodifysecond.Name = "cboxmodifysecond";
            this.cboxmodifysecond.Size = new System.Drawing.Size(45, 23);
            this.cboxmodifysecond.TabIndex = 17;
            this.cboxmodifysecond.Text = "1";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label7.Location = new System.Drawing.Point(433, 28);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(28, 19);
            this.label7.TabIndex = 16;
            this.label7.Text = "分";
            // 
            // cboxmodifyminute
            // 
            this.cboxmodifyminute.FormattingEnabled = true;
            this.cboxmodifyminute.Location = new System.Drawing.Point(382, 25);
            this.cboxmodifyminute.Name = "cboxmodifyminute";
            this.cboxmodifyminute.Size = new System.Drawing.Size(45, 23);
            this.cboxmodifyminute.TabIndex = 16;
            this.cboxmodifyminute.Text = "1";
            // 
            // cboxmodifyhour
            // 
            this.cboxmodifyhour.FormattingEnabled = true;
            this.cboxmodifyhour.Location = new System.Drawing.Point(297, 25);
            this.cboxmodifyhour.Name = "cboxmodifyhour";
            this.cboxmodifyhour.Size = new System.Drawing.Size(45, 23);
            this.cboxmodifyhour.TabIndex = 15;
            this.cboxmodifyhour.Text = "1";
            // 
            // cboxmodifyday
            // 
            this.cboxmodifyday.FormattingEnabled = true;
            this.cboxmodifyday.Location = new System.Drawing.Point(211, 25);
            this.cboxmodifyday.Name = "cboxmodifyday";
            this.cboxmodifyday.Size = new System.Drawing.Size(45, 23);
            this.cboxmodifyday.TabIndex = 10;
            this.cboxmodifyday.Text = "1";
            // 
            // cboxmodifymonth
            // 
            this.cboxmodifymonth.FormattingEnabled = true;
            this.cboxmodifymonth.Location = new System.Drawing.Point(126, 25);
            this.cboxmodifymonth.Name = "cboxmodifymonth";
            this.cboxmodifymonth.Size = new System.Drawing.Size(45, 23);
            this.cboxmodifymonth.TabIndex = 9;
            this.cboxmodifymonth.Text = "1";
            // 
            // cboxmodifyyear
            // 
            this.cboxmodifyyear.FormattingEnabled = true;
            this.cboxmodifyyear.Location = new System.Drawing.Point(23, 25);
            this.cboxmodifyyear.Name = "cboxmodifyyear";
            this.cboxmodifyyear.Size = new System.Drawing.Size(62, 23);
            this.cboxmodifyyear.TabIndex = 0;
            this.cboxmodifyyear.Text = "2017";
            // 
            // labTime
            // 
            this.labTime.AutoSize = true;
            this.labTime.Font = new System.Drawing.Font("楷体", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labTime.ForeColor = System.Drawing.Color.DarkOrange;
            this.labTime.Location = new System.Drawing.Point(702, 47);
            this.labTime.Name = "labTime";
            this.labTime.Size = new System.Drawing.Size(171, 30);
            this.labTime.TabIndex = 17;
            this.labTime.Text = "19：07：25";
            // 
            // labDate
            // 
            this.labDate.AutoSize = true;
            this.labDate.Font = new System.Drawing.Font("楷体", 13.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labDate.ForeColor = System.Drawing.Color.DodgerBlue;
            this.labDate.Location = new System.Drawing.Point(703, 12);
            this.labDate.Name = "labDate";
            this.labDate.Size = new System.Drawing.Size(290, 24);
            this.labDate.TabIndex = 16;
            this.labDate.Text = "2016年11月25日  星期六";
            // 
            // timer_update
            // 
            this.timer_update.Tick += new System.EventHandler(this.timer_update_Tick);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.btnCloseListenalarm);
            this.groupBox5.Controls.Add(this.btnListenalarm);
            this.groupBox5.Controls.Add(this.tabControl1);
            this.groupBox5.Controls.Add(this.btnReadalarmNumber);
            this.groupBox5.Controls.Add(this.btndeleteallalarm);
            this.groupBox5.Controls.Add(this.btndeletenowalarm);
            this.groupBox5.Controls.Add(this.btnNewalarm);
            this.groupBox5.Controls.Add(this.cboxClockNo);
            this.groupBox5.Controls.Add(this.label13);
            this.groupBox5.Controls.Add(this.btnclockOK);
            this.groupBox5.Controls.Add(this.cboxClockbellNo);
            this.groupBox5.Controls.Add(this.label9);
            this.groupBox5.Controls.Add(this.label10);
            this.groupBox5.Controls.Add(this.cboxClockSecond);
            this.groupBox5.Controls.Add(this.label11);
            this.groupBox5.Controls.Add(this.cboxClockMinute);
            this.groupBox5.Controls.Add(this.cboxClockHour);
            this.groupBox5.Controls.Add(this.label12);
            this.groupBox5.Location = new System.Drawing.Point(12, 155);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(1003, 106);
            this.groupBox5.TabIndex = 22;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "设置时间";
            // 
            // btnCloseListenalarm
            // 
            this.btnCloseListenalarm.Location = new System.Drawing.Point(584, 22);
            this.btnCloseListenalarm.Name = "btnCloseListenalarm";
            this.btnCloseListenalarm.Size = new System.Drawing.Size(45, 58);
            this.btnCloseListenalarm.TabIndex = 37;
            this.btnCloseListenalarm.Text = "取消铃声";
            this.btnCloseListenalarm.UseVisualStyleBackColor = true;
            this.btnCloseListenalarm.Click += new System.EventHandler(this.btnCloseListenalarm_Click);
            // 
            // btnListenalarm
            // 
            this.btnListenalarm.Location = new System.Drawing.Point(481, 64);
            this.btnListenalarm.Name = "btnListenalarm";
            this.btnListenalarm.Size = new System.Drawing.Size(86, 27);
            this.btnListenalarm.TabIndex = 36;
            this.btnListenalarm.Text = "试听铃声";
            this.btnListenalarm.UseVisualStyleBackColor = true;
            this.btnListenalarm.Click += new System.EventHandler(this.btnListenalarm_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(635, 15);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(277, 85);
            this.tabControl1.TabIndex = 35;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.cboxSeven);
            this.tabPage1.Controls.Add(this.cboxSix);
            this.tabPage1.Controls.Add(this.cboxFive);
            this.tabPage1.Controls.Add(this.cboxFour);
            this.tabPage1.Controls.Add(this.cboxThree);
            this.tabPage1.Controls.Add(this.cboxTwo);
            this.tabPage1.Controls.Add(this.cboxOne);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(269, 56);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "周次";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // cboxSeven
            // 
            this.cboxSeven.AutoSize = true;
            this.cboxSeven.Location = new System.Drawing.Point(210, 3);
            this.cboxSeven.Name = "cboxSeven";
            this.cboxSeven.Size = new System.Drawing.Size(59, 19);
            this.cboxSeven.TabIndex = 39;
            this.cboxSeven.Text = "周日";
            this.cboxSeven.UseVisualStyleBackColor = true;
            // 
            // cboxSix
            // 
            this.cboxSix.AutoSize = true;
            this.cboxSix.Location = new System.Drawing.Point(145, 31);
            this.cboxSix.Name = "cboxSix";
            this.cboxSix.Size = new System.Drawing.Size(59, 19);
            this.cboxSix.TabIndex = 38;
            this.cboxSix.Text = "周六";
            this.cboxSix.UseVisualStyleBackColor = true;
            // 
            // cboxFive
            // 
            this.cboxFive.AutoSize = true;
            this.cboxFive.Location = new System.Drawing.Point(145, 3);
            this.cboxFive.Name = "cboxFive";
            this.cboxFive.Size = new System.Drawing.Size(59, 19);
            this.cboxFive.TabIndex = 37;
            this.cboxFive.Text = "周五";
            this.cboxFive.UseVisualStyleBackColor = true;
            // 
            // cboxFour
            // 
            this.cboxFour.AutoSize = true;
            this.cboxFour.Location = new System.Drawing.Point(80, 31);
            this.cboxFour.Name = "cboxFour";
            this.cboxFour.Size = new System.Drawing.Size(59, 19);
            this.cboxFour.TabIndex = 36;
            this.cboxFour.Text = "周四";
            this.cboxFour.UseVisualStyleBackColor = true;
            // 
            // cboxThree
            // 
            this.cboxThree.AutoSize = true;
            this.cboxThree.Location = new System.Drawing.Point(80, 6);
            this.cboxThree.Name = "cboxThree";
            this.cboxThree.Size = new System.Drawing.Size(59, 19);
            this.cboxThree.TabIndex = 35;
            this.cboxThree.Text = "周三";
            this.cboxThree.UseVisualStyleBackColor = true;
            // 
            // cboxTwo
            // 
            this.cboxTwo.AutoSize = true;
            this.cboxTwo.Location = new System.Drawing.Point(7, 31);
            this.cboxTwo.Name = "cboxTwo";
            this.cboxTwo.Size = new System.Drawing.Size(59, 19);
            this.cboxTwo.TabIndex = 34;
            this.cboxTwo.Text = "周二";
            this.cboxTwo.UseVisualStyleBackColor = true;
            // 
            // cboxOne
            // 
            this.cboxOne.AutoSize = true;
            this.cboxOne.Location = new System.Drawing.Point(7, 6);
            this.cboxOne.Name = "cboxOne";
            this.cboxOne.Size = new System.Drawing.Size(59, 19);
            this.cboxOne.TabIndex = 33;
            this.cboxOne.Text = "周一";
            this.cboxOne.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.cboxClockday);
            this.tabPage2.Controls.Add(this.cboxClockmonth);
            this.tabPage2.Controls.Add(this.cboxClockyear);
            this.tabPage2.Controls.Add(this.label16);
            this.tabPage2.Controls.Add(this.label17);
            this.tabPage2.Controls.Add(this.label18);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(269, 56);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "日期";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // cboxClockday
            // 
            this.cboxClockday.FormattingEnabled = true;
            this.cboxClockday.Location = new System.Drawing.Point(191, 15);
            this.cboxClockday.Name = "cboxClockday";
            this.cboxClockday.Size = new System.Drawing.Size(45, 23);
            this.cboxClockday.TabIndex = 16;
            this.cboxClockday.Text = "1";
            // 
            // cboxClockmonth
            // 
            this.cboxClockmonth.FormattingEnabled = true;
            this.cboxClockmonth.Location = new System.Drawing.Point(106, 15);
            this.cboxClockmonth.Name = "cboxClockmonth";
            this.cboxClockmonth.Size = new System.Drawing.Size(45, 23);
            this.cboxClockmonth.TabIndex = 14;
            this.cboxClockmonth.Text = "1";
            // 
            // cboxClockyear
            // 
            this.cboxClockyear.FormattingEnabled = true;
            this.cboxClockyear.Location = new System.Drawing.Point(3, 15);
            this.cboxClockyear.Name = "cboxClockyear";
            this.cboxClockyear.Size = new System.Drawing.Size(62, 23);
            this.cboxClockyear.TabIndex = 11;
            this.cboxClockyear.Text = "2017";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label16.Location = new System.Drawing.Point(71, 18);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(29, 20);
            this.label16.TabIndex = 12;
            this.label16.Text = "年";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label17.Location = new System.Drawing.Point(157, 18);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(28, 19);
            this.label17.TabIndex = 13;
            this.label17.Text = "月";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label18.Location = new System.Drawing.Point(236, 18);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(28, 19);
            this.label18.TabIndex = 15;
            this.label18.Text = "日";
            // 
            // btnReadalarmNumber
            // 
            this.btnReadalarmNumber.Location = new System.Drawing.Point(6, 64);
            this.btnReadalarmNumber.Name = "btnReadalarmNumber";
            this.btnReadalarmNumber.Size = new System.Drawing.Size(120, 27);
            this.btnReadalarmNumber.TabIndex = 34;
            this.btnReadalarmNumber.Text = "读取所有闹铃";
            this.btnReadalarmNumber.UseVisualStyleBackColor = true;
            this.btnReadalarmNumber.Click += new System.EventHandler(this.btnReadalarmNumber_Click);
            // 
            // btndeleteallalarm
            // 
            this.btndeleteallalarm.Location = new System.Drawing.Point(358, 64);
            this.btndeleteallalarm.Name = "btndeleteallalarm";
            this.btndeleteallalarm.Size = new System.Drawing.Size(117, 27);
            this.btndeleteallalarm.TabIndex = 33;
            this.btndeleteallalarm.Text = "删除所有闹铃";
            this.btndeleteallalarm.UseVisualStyleBackColor = true;
            this.btndeleteallalarm.Click += new System.EventHandler(this.btndeleteallalarm_Click);
            // 
            // btndeletenowalarm
            // 
            this.btndeletenowalarm.Location = new System.Drawing.Point(235, 64);
            this.btndeletenowalarm.Name = "btndeletenowalarm";
            this.btndeletenowalarm.Size = new System.Drawing.Size(117, 27);
            this.btndeletenowalarm.TabIndex = 25;
            this.btndeletenowalarm.Text = "删除当前闹铃";
            this.btndeletenowalarm.UseVisualStyleBackColor = true;
            this.btndeletenowalarm.Click += new System.EventHandler(this.btndeletenowalarm_Click);
            // 
            // btnNewalarm
            // 
            this.btnNewalarm.Location = new System.Drawing.Point(132, 64);
            this.btnNewalarm.Name = "btnNewalarm";
            this.btnNewalarm.Size = new System.Drawing.Size(97, 27);
            this.btnNewalarm.TabIndex = 24;
            this.btnNewalarm.Text = "新增闹铃";
            this.btnNewalarm.UseVisualStyleBackColor = true;
            this.btnNewalarm.Click += new System.EventHandler(this.btnNewalarm_Click);
            // 
            // cboxClockNo
            // 
            this.cboxClockNo.FormattingEnabled = true;
            this.cboxClockNo.Location = new System.Drawing.Point(113, 27);
            this.cboxClockNo.Name = "cboxClockNo";
            this.cboxClockNo.Size = new System.Drawing.Size(45, 23);
            this.cboxClockNo.TabIndex = 22;
            this.cboxClockNo.SelectedIndexChanged += new System.EventHandler(this.cboxClockNo_SelectedIndexChanged);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label13.Location = new System.Drawing.Point(6, 31);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(104, 19);
            this.label13.TabIndex = 23;
            this.label13.Text = "闹铃编号：";
            // 
            // btnclockOK
            // 
            this.btnclockOK.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnclockOK.Location = new System.Drawing.Point(918, 46);
            this.btnclockOK.Name = "btnclockOK";
            this.btnclockOK.Size = new System.Drawing.Size(79, 42);
            this.btnclockOK.TabIndex = 21;
            this.btnclockOK.Text = "确定";
            this.btnclockOK.UseVisualStyleBackColor = true;
            this.btnclockOK.Click += new System.EventHandler(this.btnclockOK_Click);
            // 
            // cboxClockbellNo
            // 
            this.cboxClockbellNo.FormattingEnabled = true;
            this.cboxClockbellNo.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10"});
            this.cboxClockbellNo.Location = new System.Drawing.Point(517, 27);
            this.cboxClockbellNo.Name = "cboxClockbellNo";
            this.cboxClockbellNo.Size = new System.Drawing.Size(45, 23);
            this.cboxClockbellNo.TabIndex = 20;
            this.cboxClockbellNo.Text = "1";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label9.Location = new System.Drawing.Point(448, 31);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(66, 19);
            this.label9.TabIndex = 19;
            this.label9.Text = "铃声：";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label10.Location = new System.Drawing.Point(399, 30);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(28, 19);
            this.label10.TabIndex = 18;
            this.label10.Text = "秒";
            // 
            // cboxClockSecond
            // 
            this.cboxClockSecond.FormattingEnabled = true;
            this.cboxClockSecond.Location = new System.Drawing.Point(348, 27);
            this.cboxClockSecond.Name = "cboxClockSecond";
            this.cboxClockSecond.Size = new System.Drawing.Size(45, 23);
            this.cboxClockSecond.TabIndex = 17;
            this.cboxClockSecond.Text = "0";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label11.Location = new System.Drawing.Point(321, 30);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(28, 19);
            this.label11.TabIndex = 16;
            this.label11.Text = "分";
            // 
            // cboxClockMinute
            // 
            this.cboxClockMinute.FormattingEnabled = true;
            this.cboxClockMinute.Location = new System.Drawing.Point(270, 27);
            this.cboxClockMinute.Name = "cboxClockMinute";
            this.cboxClockMinute.Size = new System.Drawing.Size(45, 23);
            this.cboxClockMinute.TabIndex = 16;
            this.cboxClockMinute.Text = "1";
            // 
            // cboxClockHour
            // 
            this.cboxClockHour.FormattingEnabled = true;
            this.cboxClockHour.Location = new System.Drawing.Point(185, 27);
            this.cboxClockHour.Name = "cboxClockHour";
            this.cboxClockHour.Size = new System.Drawing.Size(45, 23);
            this.cboxClockHour.TabIndex = 15;
            this.cboxClockHour.Text = "1";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label12.Location = new System.Drawing.Point(236, 30);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(28, 19);
            this.label12.TabIndex = 14;
            this.label12.Text = "时";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.btnSetPassword);
            this.groupBox6.Controls.Add(this.tboxSetPassword);
            this.groupBox6.Location = new System.Drawing.Point(532, 13);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(170, 57);
            this.groupBox6.TabIndex = 8;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "设备密码区";
            // 
            // btnSetPassword
            // 
            this.btnSetPassword.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnSetPassword.Location = new System.Drawing.Point(70, 20);
            this.btnSetPassword.Name = "btnSetPassword";
            this.btnSetPassword.Size = new System.Drawing.Size(88, 27);
            this.btnSetPassword.TabIndex = 22;
            this.btnSetPassword.Text = "确定修改";
            this.btnSetPassword.UseVisualStyleBackColor = true;
            this.btnSetPassword.Click += new System.EventHandler(this.btnSetPassword_Click);
            // 
            // tboxSetPassword
            // 
            this.tboxSetPassword.Location = new System.Drawing.Point(23, 21);
            this.tboxSetPassword.Name = "tboxSetPassword";
            this.tboxSetPassword.Size = new System.Drawing.Size(41, 25);
            this.tboxSetPassword.TabIndex = 0;
            this.tboxSetPassword.Text = "0001";
            // 
            // timer_usart
            // 
            this.timer_usart.Interval = 3000;
            this.timer_usart.Tick += new System.EventHandler(this.timer_usart_Tick);
            // 
            // btnsetvolume
            // 
            this.btnsetvolume.Location = new System.Drawing.Point(930, 371);
            this.btnsetvolume.Name = "btnsetvolume";
            this.btnsetvolume.Size = new System.Drawing.Size(75, 47);
            this.btnsetvolume.TabIndex = 23;
            this.btnsetvolume.Text = "设置音量";
            this.btnsetvolume.UseVisualStyleBackColor = true;
            this.btnsetvolume.Click += new System.EventHandler(this.btnsetvolume_Click);
            // 
            // cboxvolume
            // 
            this.cboxvolume.FormattingEnabled = true;
            this.cboxvolume.Location = new System.Drawing.Point(948, 326);
            this.cboxvolume.Name = "cboxvolume";
            this.cboxvolume.Size = new System.Drawing.Size(57, 23);
            this.cboxvolume.TabIndex = 24;
            this.cboxvolume.Text = "30";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label19.Location = new System.Drawing.Point(930, 295);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(66, 19);
            this.label19.TabIndex = 25;
            this.label19.Text = "音量：";
            // 
            // MainControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1033, 430);
            this.Controls.Add(this.label19);
            this.Controls.Add(this.cboxvolume);
            this.Controls.Add(this.btnsetvolume);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.labTime);
            this.Controls.Add(this.labDate);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainControl";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "RTC_Control     VersionNO：1.1     VersionTime：2017.7.2     Author：Tang娃      南京工业" +
                "职业技术学院";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainControl_FormClosing);
            this.Load += new System.EventHandler(this.MainControl_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort SPdataControl;
        private System.Windows.Forms.Button btnSearch;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cboxNO;
        private System.Windows.Forms.Button btnAutoSetRTC;
        private System.Windows.Forms.TextBox txtRecieve;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox tboxpassword;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.ComboBox cboxmodifyyear;
        private System.Windows.Forms.ComboBox cboxmodifymonth;
        private System.Windows.Forms.ComboBox cboxmodifyday;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox cboxmodifyminute;
        private System.Windows.Forms.ComboBox cboxmodifyhour;
        private System.Windows.Forms.ComboBox cboxmodifyweek;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox cboxmodifysecond;
        private System.Windows.Forms.Button btnManualSetRTC;
        private System.Windows.Forms.Label labTime;
        private System.Windows.Forms.Label labDate;
        private System.Windows.Forms.Timer timer_update;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button btnclockOK;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ComboBox cboxClockSecond;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.ComboBox cboxClockMinute;
        private System.Windows.Forms.ComboBox cboxClockHour;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ComboBox cboxClockNo;
        private System.Windows.Forms.ComboBox cboxClockbellNo;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button btnNewalarm;
        private System.Windows.Forms.Button btndeletenowalarm;
        private System.Windows.Forms.Button btndeleteallalarm;
        private System.Windows.Forms.Button btnReadalarmNumber;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button btnSetPassword;
        private System.Windows.Forms.TextBox tboxSetPassword;
        private System.Windows.Forms.ComboBox cboxmodifytweek;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.CheckBox cboxSeven;
        private System.Windows.Forms.CheckBox cboxSix;
        private System.Windows.Forms.CheckBox cboxFive;
        private System.Windows.Forms.CheckBox cboxFour;
        private System.Windows.Forms.CheckBox cboxThree;
        private System.Windows.Forms.CheckBox cboxTwo;
        private System.Windows.Forms.CheckBox cboxOne;
        private System.Windows.Forms.ComboBox cboxClockday;
        private System.Windows.Forms.ComboBox cboxClockmonth;
        private System.Windows.Forms.ComboBox cboxClockyear;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Button btnListenalarm;
        private System.Windows.Forms.Button btnCloseListenalarm;
        private System.Windows.Forms.Timer timer_usart;
        private System.Windows.Forms.Button btnsetvolume;
        private System.Windows.Forms.ComboBox cboxvolume;
        private System.Windows.Forms.Label label19;
    }
}