namespace AlmConfig
{
    partial class MainForm
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
            this.callSign_textBox = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.ssid_numericUpDown = new System.Windows.Forms.NumericUpDown();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.txRate_numericUpDown = new System.Windows.Forms.NumericUpDown();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.beacon_textBox = new System.Windows.Forms.TextBox();
            this.readConfig_button = new System.Windows.Forms.Button();
            this.writeConfig_button = new System.Windows.Forms.Button();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.baudRate_comboBox = new System.Windows.Forms.ComboBox();
            this.textBoxInfo = new System.Windows.Forms.TextBox();
            this.path_groupBox = new System.Windows.Forms.GroupBox();
            this.routing_comboBox = new System.Windows.Forms.ComboBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabSettings = new System.Windows.Forms.TabPage();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.baudRateAlm_comboBox = new System.Windows.Forms.ComboBox();
            this.tabTest = new System.Windows.Forms.TabPage();
            this.tabTerminal = new System.Windows.Forms.TabPage();
            this.comPort_comboBox = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ssid_numericUpDown)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.txRate_numericUpDown)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.path_groupBox.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabSettings.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.tabTerminal.SuspendLayout();
            this.SuspendLayout();
            // 
            // callSign_textBox
            // 
            this.callSign_textBox.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.callSign_textBox.Location = new System.Drawing.Point(6, 19);
            this.callSign_textBox.MaxLength = 6;
            this.callSign_textBox.Name = "callSign_textBox";
            this.callSign_textBox.ShortcutsEnabled = false;
            this.callSign_textBox.Size = new System.Drawing.Size(100, 20);
            this.callSign_textBox.TabIndex = 1;
            this.callSign_textBox.Text = "NOCALL";
            this.callSign_textBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.callSign_textBox_KeyDown);
            this.callSign_textBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.callSign_textBox_KeyPress);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.ssid_numericUpDown);
            this.groupBox1.Controls.Add(this.callSign_textBox);
            this.groupBox1.Location = new System.Drawing.Point(6, 9);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(160, 53);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Callsign";
            // 
            // ssid_numericUpDown
            // 
            this.ssid_numericUpDown.Location = new System.Drawing.Point(111, 19);
            this.ssid_numericUpDown.Maximum = new decimal(new int[] {
            15,
            0,
            0,
            0});
            this.ssid_numericUpDown.Name = "ssid_numericUpDown";
            this.ssid_numericUpDown.Size = new System.Drawing.Size(43, 20);
            this.ssid_numericUpDown.TabIndex = 1;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.txRate_numericUpDown);
            this.groupBox2.Location = new System.Drawing.Point(6, 73);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(160, 72);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Transmit Rate";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(80, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "seconds";
            // 
            // txRate_numericUpDown
            // 
            this.txRate_numericUpDown.Location = new System.Drawing.Point(6, 19);
            this.txRate_numericUpDown.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.txRate_numericUpDown.Minimum = new decimal(new int[] {
            60,
            0,
            0,
            0});
            this.txRate_numericUpDown.Name = "txRate_numericUpDown";
            this.txRate_numericUpDown.Size = new System.Drawing.Size(68, 20);
            this.txRate_numericUpDown.TabIndex = 2;
            this.txRate_numericUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.txRate_numericUpDown.Value = new decimal(new int[] {
            60,
            0,
            0,
            0});
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.beacon_textBox);
            this.groupBox3.Location = new System.Drawing.Point(6, 213);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(345, 53);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Beacon";
            // 
            // beacon_textBox
            // 
            this.beacon_textBox.Location = new System.Drawing.Point(6, 19);
            this.beacon_textBox.MaxLength = 40;
            this.beacon_textBox.Name = "beacon_textBox";
            this.beacon_textBox.Size = new System.Drawing.Size(329, 20);
            this.beacon_textBox.TabIndex = 2;
            this.beacon_textBox.Text = "NOBEACON";
            // 
            // readConfig_button
            // 
            this.readConfig_button.Location = new System.Drawing.Point(178, 14);
            this.readConfig_button.Name = "readConfig_button";
            this.readConfig_button.Size = new System.Drawing.Size(170, 23);
            this.readConfig_button.TabIndex = 3;
            this.readConfig_button.Text = "Read Configuration";
            this.readConfig_button.UseVisualStyleBackColor = true;
            this.readConfig_button.Click += new System.EventHandler(this.button1_Click);
            // 
            // writeConfig_button
            // 
            this.writeConfig_button.Enabled = false;
            this.writeConfig_button.Location = new System.Drawing.Point(178, 43);
            this.writeConfig_button.Name = "writeConfig_button";
            this.writeConfig_button.Size = new System.Drawing.Size(170, 23);
            this.writeConfig_button.TabIndex = 4;
            this.writeConfig_button.Text = "Write Configuration";
            this.writeConfig_button.UseVisualStyleBackColor = true;
            this.writeConfig_button.Click += new System.EventHandler(this.writeConfig_button_Click);
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 4800;
            this.serialPort.PortName = "COM4";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.comPort_comboBox);
            this.groupBox4.Controls.Add(this.baudRate_comboBox);
            this.groupBox4.Location = new System.Drawing.Point(12, 320);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(137, 72);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Communication Settings";
            // 
            // baudRate_comboBox
            // 
            this.baudRate_comboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.baudRate_comboBox.DisplayMember = "1";
            this.baudRate_comboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.baudRate_comboBox.FormattingEnabled = true;
            this.baudRate_comboBox.Items.AddRange(new object[] {
            "4800 (Standard)",
            "19200 (Optional)"});
            this.baudRate_comboBox.Location = new System.Drawing.Point(6, 45);
            this.baudRate_comboBox.Name = "baudRate_comboBox";
            this.baudRate_comboBox.Size = new System.Drawing.Size(125, 21);
            this.baudRate_comboBox.TabIndex = 8;
            this.baudRate_comboBox.ValueMember = "1";
            this.baudRate_comboBox.SelectedIndexChanged += new System.EventHandler(this.BaudRatecomboBox_SelectedIndexChanged);
            // 
            // textBoxInfo
            // 
            this.textBoxInfo.AcceptsReturn = true;
            this.textBoxInfo.BackColor = System.Drawing.SystemColors.Window;
            this.textBoxInfo.Cursor = System.Windows.Forms.Cursors.Default;
            this.textBoxInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxInfo.Enabled = false;
            this.textBoxInfo.Font = new System.Drawing.Font("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxInfo.Location = new System.Drawing.Point(0, 0);
            this.textBoxInfo.Multiline = true;
            this.textBoxInfo.Name = "textBoxInfo";
            this.textBoxInfo.ReadOnly = true;
            this.textBoxInfo.Size = new System.Drawing.Size(361, 277);
            this.textBoxInfo.TabIndex = 10;
            this.textBoxInfo.TabStop = false;
            this.textBoxInfo.Text = "EMPTY";
            // 
            // path_groupBox
            // 
            this.path_groupBox.Controls.Add(this.routing_comboBox);
            this.path_groupBox.Location = new System.Drawing.Point(6, 151);
            this.path_groupBox.Name = "path_groupBox";
            this.path_groupBox.Size = new System.Drawing.Size(345, 53);
            this.path_groupBox.TabIndex = 3;
            this.path_groupBox.TabStop = false;
            this.path_groupBox.Text = "Path";
            // 
            // routing_comboBox
            // 
            this.routing_comboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.routing_comboBox.DisplayMember = "1";
            this.routing_comboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.routing_comboBox.FormattingEnabled = true;
            this.routing_comboBox.Items.AddRange(new object[] {
            "WIDE 1-1 (Standard)",
            "WIDE 1-1,WIDE 2-2 (Optional)"});
            this.routing_comboBox.Location = new System.Drawing.Point(6, 19);
            this.routing_comboBox.Name = "routing_comboBox";
            this.routing_comboBox.Size = new System.Drawing.Size(329, 21);
            this.routing_comboBox.TabIndex = 9;
            this.routing_comboBox.ValueMember = "1";
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabSettings);
            this.tabControl1.Controls.Add(this.tabTest);
            this.tabControl1.Controls.Add(this.tabTerminal);
            this.tabControl1.Location = new System.Drawing.Point(12, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(369, 303);
            this.tabControl1.TabIndex = 11;
            // 
            // tabSettings
            // 
            this.tabSettings.Controls.Add(this.groupBox5);
            this.tabSettings.Controls.Add(this.groupBox1);
            this.tabSettings.Controls.Add(this.path_groupBox);
            this.tabSettings.Controls.Add(this.writeConfig_button);
            this.tabSettings.Controls.Add(this.groupBox2);
            this.tabSettings.Controls.Add(this.readConfig_button);
            this.tabSettings.Controls.Add(this.groupBox3);
            this.tabSettings.Location = new System.Drawing.Point(4, 22);
            this.tabSettings.Name = "tabSettings";
            this.tabSettings.Padding = new System.Windows.Forms.Padding(3);
            this.tabSettings.Size = new System.Drawing.Size(361, 277);
            this.tabSettings.TabIndex = 0;
            this.tabSettings.Text = "Settings";
            this.tabSettings.UseVisualStyleBackColor = true;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.baudRateAlm_comboBox);
            this.groupBox5.Location = new System.Drawing.Point(178, 73);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(170, 72);
            this.groupBox5.TabIndex = 8;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Communication Settings";
            // 
            // baudRateAlm_comboBox
            // 
            this.baudRateAlm_comboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.baudRateAlm_comboBox.DisplayMember = "1";
            this.baudRateAlm_comboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.baudRateAlm_comboBox.FormattingEnabled = true;
            this.baudRateAlm_comboBox.Items.AddRange(new object[] {
            "4800 (Standard)",
            "19200 (Optional)"});
            this.baudRateAlm_comboBox.Location = new System.Drawing.Point(6, 19);
            this.baudRateAlm_comboBox.Name = "baudRateAlm_comboBox";
            this.baudRateAlm_comboBox.Size = new System.Drawing.Size(117, 21);
            this.baudRateAlm_comboBox.TabIndex = 8;
            this.baudRateAlm_comboBox.ValueMember = "1";
            // 
            // tabTest
            // 
            this.tabTest.AutoScroll = true;
            this.tabTest.Location = new System.Drawing.Point(4, 22);
            this.tabTest.Name = "tabTest";
            this.tabTest.Padding = new System.Windows.Forms.Padding(3);
            this.tabTest.Size = new System.Drawing.Size(361, 277);
            this.tabTest.TabIndex = 1;
            this.tabTest.Text = "Test";
            this.tabTest.UseVisualStyleBackColor = true;
            // 
            // tabTerminal
            // 
            this.tabTerminal.Controls.Add(this.textBoxInfo);
            this.tabTerminal.Location = new System.Drawing.Point(4, 22);
            this.tabTerminal.Name = "tabTerminal";
            this.tabTerminal.Size = new System.Drawing.Size(361, 277);
            this.tabTerminal.TabIndex = 2;
            this.tabTerminal.Text = "Terminal";
            this.tabTerminal.UseVisualStyleBackColor = true;
            // 
            // comPort_comboBox
            // 
            this.comPort_comboBox.DisplayMember = "1";
            this.comPort_comboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comPort_comboBox.FormattingEnabled = true;
            this.comPort_comboBox.Location = new System.Drawing.Point(6, 18);
            this.comPort_comboBox.Name = "comPort_comboBox";
            this.comPort_comboBox.Size = new System.Drawing.Size(125, 21);
            this.comPort_comboBox.TabIndex = 9;
            this.comPort_comboBox.ValueMember = "1";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(393, 400);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.groupBox4);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "MainForm";
            this.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.Text = "  AlmConfig Ver 2.0";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ssid_numericUpDown)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.txRate_numericUpDown)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.path_groupBox.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabSettings.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.tabTerminal.ResumeLayout(false);
            this.tabTerminal.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown ssid_numericUpDown;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown txRate_numericUpDown;
        protected internal System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox beacon_textBox;
        private System.Windows.Forms.Button readConfig_button;
        private System.Windows.Forms.Button writeConfig_button;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox textBoxInfo;
        protected internal System.Windows.Forms.GroupBox path_groupBox;
        private System.Windows.Forms.ComboBox routing_comboBox;
        private System.Windows.Forms.TextBox callSign_textBox;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabSettings;
        private System.Windows.Forms.TabPage tabTest;
        private System.Windows.Forms.TabPage tabTerminal;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.ComboBox baudRateAlm_comboBox;
        private System.Windows.Forms.ComboBox baudRate_comboBox;
        private System.Windows.Forms.ComboBox comPort_comboBox;
    }
}

