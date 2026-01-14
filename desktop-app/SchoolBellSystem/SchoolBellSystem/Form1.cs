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
    public partial class ETSbell : Form
    {
        public ETSbell()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //kad se pritisne dugme da treperi led matrica
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void ETSbell_Load(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            /*dateTimePicker1.Format = DateTimePickerFormat.Short;
            dateTimePicker1.CustomFormat = "dd.MM.yyyy.";

            dateTimePicker2.Format = DateTimePickerFormat.Time;
            dateTimePicker2.CustomFormat = "hh:mm:ss";

            string naziv = textBox1.Text;
            string datum = dateTimePicker1.Value.ToString("dd.MM.yyyy.");
            string vrijeme = dateTimePicker2.Value.ToString("HH:mm");
            listBox1.Items.Add(naziv +" "+ datum+" "+vrijeme);-nema sorta*/


            listBox1.Sorted = true;
            string naziv = textBox1.Text;

            DateTime datum = dateTimePicker1.Value;
            DateTime vrijeme = dateTimePicker2.Value;

            // Spojimo datum i vrijeme
            DateTime puniDatum = new DateTime(
                datum.Year,
                datum.Month,
                datum.Day,
                vrijeme.Hour,
                vrijeme.Minute,
                0
            );

            // Sort ključ + prikaz (korisnik vidi cijeli tekst)
            string stavka = puniDatum.ToString("yyyyMMddHHmm") + " " + naziv + " " + puniDatum.ToString("dd.MM.yyyy. HH:mm");

            listBox1.Items.Add(stavka);
        }

            
        

        private void bellstart_Click(object sender, EventArgs e)
        {
            //testira zvono i treba pocet zvonit
        }

        private void bellstop_Click(object sender, EventArgs e)
        {
            //testira zvono i zaustavlja ga ako zboni ako ne onda nista se ne desi
        }
    }
}
