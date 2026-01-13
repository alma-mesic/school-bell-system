using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

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
            string adminPass = "1234"; // kasnije ovo mozemo spremiti u config ili enkriptovati

            if (textBox1.Text == adminUser && textBox2.Text == adminPass)
            {
                ETSbell mainForm = new ETSbell();
                mainForm.Show();
                this.Hide();
            }
            else
            {
                MessageBox.Show("Pogrešan username ili lozinka!", "Greška", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void LoginForm_Load(object sender, EventArgs e)
        {

        }
    }
}
