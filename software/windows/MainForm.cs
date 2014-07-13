/******************************************************************************

 MainForm.c for AlmConfig

 Allmogetracker receives data from a GPS receiver and transmits it
 over amature radio using the APRS protocol version 1.0.

 Copyright (C)2011, Andreas Kingbäck  (andki234@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AlmConfig
{     
    public partial class MainForm : Form
    {
        // Boolean flag used to determine when a space character than a number is entered.
        private bool spaceCharEntered = false;
            
        public MainForm()
        {
            string[] portNames;
            int nof_comports = 0;

            InitializeComponent();

            portNames = System.IO.Ports.SerialPort.GetPortNames();

            foreach (string port in portNames)
            {
                comPort_comboBox.Items.Add(port);
                nof_comports++;
            }

            if (nof_comports == 0)
            {
                comPort_comboBox.Items.Add("");
            }

            comPort_comboBox.SelectedIndex = 0;
            baudRate_comboBox.SelectedIndex = 0;
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            baudRateAlm_comboBox.SelectedIndex = 0;
            comPort_comboBox.SelectedIndex = 0;
            baudRate_comboBox.SelectedItem = 0;
            baudRate_comboBox.SelectedIndex = 0;
            routing_comboBox.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {     
            char[] rx_buffer;
            char[] rx;
            int numBytes = 0;
            int a = 0;
            int b = 0;
            string data = "";

            rx_buffer = new char[550];
            rx = new char[1];
            
            if (serialPort.IsOpen == false)
            {
                try
                {
                    serialPort.Open();
                    serialPort.DiscardInBuffer();
                    serialPort.DiscardOutBuffer();
                    serialPort.ReadTimeout = 1500;
                    serialPort.NewLine = "\r";

                    serialPort.WriteLine("$EEPRE,*6B");   // Send data request command

                    do
	                {
                        a = serialPort.Read(rx, 0, 1);
                        
                        if (a == 1)
                        {
                            rx_buffer[numBytes] = rx[0];
                            data = data + rx[0].ToString();
                            
                            numBytes++;
                        }

                        if ((numBytes - 3) < 0)
                        {
                            b = 0;
                        }
                        else
                        {
                            b = numBytes - 3;
                        }
                    } while (data[b] != '*');

                    serialPort.Close();

                    textBoxInfo.Clear();
                    callSign_textBox.Clear();
                    beacon_textBox.Clear();

                    textBoxInfo.AppendText(data);

                    a = 8;

                    for (int i = 0; i < 6; i++)   // Extract callsign and put in callsign text box
                    {
                        rx[0] = (char)(int.Parse(data.Substring(a, 2), System.Globalization.NumberStyles.AllowHexSpecifier) >> 1);
                        
                        if (rx[0].ToString() != " ")
                        {
                            callSign_textBox.AppendText(rx[0].ToString());
                        }
                        
                        a += 2;
                    }

                    // Extract SSID and put in ssid_numericUpDown numeric box
                    ssid_numericUpDown.Value = (int.Parse(data.Substring(a, 2), System.Globalization.NumberStyles.AllowHexSpecifier) - 0x60) >> 1;
                    a += 2;

                    // Extract transmit rate and put in txRate_numericUpDown numeric box
                    txRate_numericUpDown.Value = int.Parse(data.Substring(a, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                    a += 2;

                    for (int i = 0; i < 50; i++)   // Extract beacon and put in beacon_textBox
                    {
                        rx[0] = (char)(int.Parse(data.Substring(a, 2), System.Globalization.NumberStyles.AllowHexSpecifier));

                        if ((int)rx[0] != 0)
                        {
                            beacon_textBox.AppendText(rx[0].ToString());
                            a += 2;
                        }
                        else
                        {
                            i = 50;
                        }
                    }

                    // Extract path variant
                    a = 0x7C;

                    routing_comboBox.SelectedIndex = int.Parse(data.Substring(a, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                    
                    writeConfig_button.Enabled = true;  // Enable Write config
                }
                catch (System.Exception ex)
                {
                    serialPort.Close();
                    MessageBox.Show(ex.Message);
                }
            }     
        }

        private void writeConfig_button_Click(object sender, EventArgs e)
        {
            int b;
            char[] rx;
            string text = ""; 

            // Add Command
            string output = "$EEPWR,";

            // Add data reboot counter data
            output = output + textBoxInfo.Lines[0].Substring(0, 8);

            // Add callsign data 
            text = callSign_textBox.Lines[0];

            for (int i = 0; i < text.Length; i++)   
            {
                b = (int)text[i];
                b = b << 1;
                output = output + b.ToString("X2");
            }

            if (text.Length < 6)
            {
                b = (int)(' ' << 1);

                for (int i = text.Length; i < 6; i++)
                {
                    output = output + b.ToString("X2");
                }
            }

            // Add SSID data
            b = 0x60 +(int)ssid_numericUpDown.Value * 2; 
            output = output + b.ToString("X2");

            // Add transmit rate
            b = (int)txRate_numericUpDown.Value; 
            output = output + b.ToString("X2");

            // Add Beacon
            text = beacon_textBox.Lines[0];

            for (int i = 0; i < text.Length; i++)
            {
                b = (int)text[i];
                output = output + b.ToString("X2");
            }

            output = output + "00";

            for (int i = text.Length; i < 49; i++)
            {
                b = 0xFF;
                output = output + b.ToString("X2");
            }

            // Add path 
            b = routing_comboBox.SelectedIndex;
            output = output + b.ToString("X2");

            //Add eeOK
            text = "eeOK";

            for (int i = 0; i < text.Length; i++)
            {
                b = (int)text[i];
                output = output + b.ToString("X2");
            }

            output = output + "00";

            //Add Checksum
            b = 0;

            for (int i = 1; i < output.Length; i++)
            {
                b = b ^ (int)output[i];
            }

            output = output + "*";
            output = output + b.ToString("X2");

            // Update textbox
            textBoxInfo.Clear();

            textBoxInfo.AppendText(output);

            writeConfig_button.Enabled = false;  // Disable Write config

            if (serialPort.IsOpen == false)
            {
                try
                {
                    serialPort.Open();
                    serialPort.DiscardInBuffer();
                    serialPort.DiscardOutBuffer();
                    serialPort.ReadTimeout = 1500;
                    serialPort.NewLine = "\r\n";

                    serialPort.WriteLine(output);   // Send data request command

                    // Get Response
                    rx = new char[2];

                    for (int i = 0; i < 2 ; i++)
                    {
                        do
                        {
                            b = serialPort.Read(rx, i, 1);
                        } while (b != 1);
                    }
                    
                    if ((rx[0] != 'O') || (rx[1] != 'K'))
                    {
                        MessageBox.Show("Write Configuration FAILED!");
                    }

                    serialPort.Close();
                }
                catch (System.Exception ex)
                {
                    serialPort.Close();
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void BaudRatecomboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (baudRate_comboBox.SelectedIndex)
            {
                case 0:
                {
                    serialPort.BaudRate = 4800;
                        
                    break;
                }

                case 1:
                {
                    serialPort.BaudRate = 19200;
                        
                    break;
                }
                
                default:
                {
                    serialPort.BaudRate = 4800;
                     
                    break;
                }
            }
        }

        private void ComPortcomboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            serialPort.PortName = comPort_comboBox.SelectedItem.ToString();
        }

        private void callSign_textBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Space)
            {
                spaceCharEntered = true;
            }
            else
            {
                spaceCharEntered = false;
            }       
        }

        private void callSign_textBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (true == spaceCharEntered)
            {
                e.Handled = true;   // Stop the space character from being entered into the callsign textbox
            }
        }
    }
}
