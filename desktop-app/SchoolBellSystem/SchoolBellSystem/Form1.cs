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
            dataGridView1.Rows.Clear();

            dataGridView1.Rows.Add(1, "07:30", "08:15");
            dataGridView1.Rows.Add(2, "08:20", "09:05");
            dataGridView1.Rows.Add(3, "09:10", "09:55");
            dataGridView1.Rows.Add(4, "10:15", "11:00");
            dataGridView1.Rows.Add(5, "11:05", "11:50");
            dataGridView1.Rows.Add(6, "11:55", "12:40");
            dataGridView1.Rows.Add(7, "12:45", "13:30");
            dataGridView1.Rows.Add(8, "13:35", "14:20");
            dataGridView1.Rows.Add(9, "14:25", "15:10");
            dataGridView1.Rows.Add(10, "15:30", "16:15");
            dataGridView1.Rows.Add(11, "16:20", "17:05");
            dataGridView1.Rows.Add(12, "17:10", "17:55");
            dataGridView1.Rows.Add(13, "18:00", "18:45");

            dataGridView2.Rows.Clear();

            dataGridView2.Rows.Add(1, "07:30", "08:00");
            dataGridView2.Rows.Add(2, "08:05", "08:35");
            dataGridView2.Rows.Add(3, "08:40", "09:10");
            dataGridView2.Rows.Add(4, "09:30", "10:00");
            dataGridView2.Rows.Add(5, "10:05", "10:35");
            dataGridView2.Rows.Add(6, "10:40", "11:10");
            dataGridView2.Rows.Add(7, "11:15", "11:45");
            dataGridView2.Rows.Add(8, "11:50", "12:20");
            dataGridView2.Rows.Add(9, "12:25", "12:55");
            dataGridView2.Rows.Add(10, "13:15", "13:45");
            dataGridView2.Rows.Add(11, "13:50", "14:20");
            dataGridView2.Rows.Add(12, "14:25", "14:55");
            dataGridView2.Rows.Add(13, "15:00", "15:30");
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

        private void label13_Click(object sender, EventArgs e)
        {

        }

        private void Postavke_Click(object sender, EventArgs e)
        {

        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void Raspored_Click(object sender, EventArgs e)
        {
           

        }
    }
}
