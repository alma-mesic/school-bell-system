using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;
using System.IO;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Button;

namespace SchoolBellSystem
{
    public partial class LoginForm : Form
    {
        public LoginForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string adminUser = "admin";
            string adminPass = "1234";

            // ako postoji admin.json, ucitaj podatke iz njega
            if (File.Exists("admin.json"))
            {
                string json = File.ReadAllText("admin.json");

                Dictionary<string, string> adminData =
                    JsonConvert.DeserializeObject<Dictionary<string, string>>(json);

                adminUser = adminData["username"];
                adminPass = adminData["password"];
            }

            // provjera unosa
            if (textBox1.Text == adminUser && textBox2.Text == adminPass)
            {
                ETSbell mainForm = new ETSbell();
                mainForm.Show();
                this.Hide();
            }
            else
            {
                MessageBox.Show(
                    "Pogrešan username ili lozinka!",
                    "Greška",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error

                );
                textBox1.Clear();
                textBox2.Clear();
            }
        }

        private void LoginForm_Load(object sender, EventArgs e)
        {
            panel1.BackColor = Color.FromArgb(170, 255, 255, 255);
            ETSbell.SetRoundCorners(panel1, 20);
            pictureBox1.BackColor = Color.Transparent;
            textBox1.BackColor = Color.White;
            textBox2.BackColor = Color.White;
            label1.BackColor = Color.Transparent;
            label2.BackColor = Color.Transparent;
            checkBox1.BackColor = Color.Transparent;
            textBox2.UseSystemPasswordChar = true;
            checkBox1.Checked = false;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                textBox2.UseSystemPasswordChar = false; // password VIDLJIV

            }
            else
            {
                textBox2.UseSystemPasswordChar = true;  //password SKRIVEN

            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
