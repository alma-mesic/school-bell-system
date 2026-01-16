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
            //kad se pritisne dugme da treperi led matrica i oglasi se zvono prema morseovom kodu sos

            Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();
            jsonObjekat["tip"] = "emergency";
            jsonObjekat["stanje"] = "ukljuceno";

            string json = JsonConvert.SerializeObject(jsonObjekat);

            MessageBox.Show(json);
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
            //ovaj button sluzi da dodam na listu neku obavezu ili dogadjaj koji imam
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

            listBox1.Items.Add(stavka); //popravit ovo da se ne vidi kod naprijed gdje sastavi datum i vrijeme u svrhu sortiranja
        }

            
        

        private void bellstart_Click(object sender, EventArgs e)
        {
            //testira zvono i treba pocet zvonit

            // 1. Pravimo jednostavan JSON objekat
            Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();

            // 2. Tip poruke - znamo da je vezano za zvono
            jsonObjekat["tip"] = "zvono";

            // 3. Akcija - start (pocni zvoniti)
            jsonObjekat["akcija"] = "start";

            // 4. Pretvaranje u JSON tekst
            string json = JsonConvert.SerializeObject(jsonObjekat, Formatting.Indented);

            // 5. Za sada samo prikaz (kasnije ide slanje na ESP32)
            MessageBox.Show(json);
        }

        private void bellstop_Click(object sender, EventArgs e)
        {
            //testira zvono i zaustavlja ga ako zboni ako ne onda nista se ne desi
            // 1. Pravimo JSON objekat
            Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();

            // 2. Tip poruke
            jsonObjekat["tip"] = "zvono";

            // 3. Akcija - stop (prestani zvoniti)
            jsonObjekat["akcija"] = "stop";

            // 4. Pretvaranje u JSON tekst
            string json = JsonConvert.SerializeObject(jsonObjekat, Formatting.Indented);

            // 5. Prikaz (test)
            MessageBox.Show(json);
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

        private void button4_Click(object sender, EventArgs e)
        {
            listBox1.Items.Remove(listBox1.SelectedItem);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //salje raspored iz datagridview na esp eeprom i gleda da li je selektan radiobuttn 1 ili 2
            //i na osnovu toga bira po kojem rasporedu ce zvoniti zvono (datagridview1 ili datagridview2


            // 1. Odabir rasporeda (redovni ili skraceni)
            DataGridView aktivniGrid;

            if (radioButton1.Checked) // pretpostavka: radioButton1 = redovni
                aktivniGrid = dataGridView1;
            else
                aktivniGrid = dataGridView2;

            // 2. Lista casova (ovo ce u JSON ici kao niz)
            List<Dictionary<string, string>> casovi = new List<Dictionary<string, string>>();

            foreach (DataGridViewRow row in aktivniGrid.Rows)
            {
                if (row.Cells[0].Value == null) continue;

                // jedan cas
                Dictionary<string, string> cas = new Dictionary<string, string>();
                cas["cas"] = row.Cells[0].Value.ToString();
                cas["pocetak"] = row.Cells[1].Value.ToString();
                cas["kraj"] = row.Cells[2].Value.ToString();

                casovi.Add(cas);
            }

            // 3. Glavni JSON objekat
            Dictionary<string, object> jsonObjekat = new Dictionary<string, object>();
            jsonObjekat["tip"] = "raspored";
            jsonObjekat["nacin"] = radioButton1.Checked ? "redovni" : "skraceni";
            jsonObjekat["casovi"] = casovi;

            // 4. Pretvaranje u JSON tekst
            string json = JsonConvert.SerializeObject(jsonObjekat, Formatting.Indented);

            // 5. Za sad samo prikaz (kasnije ide na ESP32)
            MessageBox.Show(json);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            //salje obavijesti i dogadjaje iz listboxa na esp32

            List<Dictionary<string, string>> dogadjaji = new List<Dictionary<string, string>>();

            foreach (string item in listBox1.Items)
            {
                // format: yyyyMMddHHmm NAZIV dd.MM.yyyy. HH:mm
                string[] dijelovi = item.Split(' ');

                Dictionary<string, string> dogadjaj = new Dictionary<string, string>();
                dogadjaj["naziv"] = dijelovi[1];
                dogadjaj["datumVrijeme"] = dijelovi[2] + " " + dijelovi[3];

                dogadjaji.Add(dogadjaj);
            }

            Dictionary<string, object> jsonObjekat = new Dictionary<string, object>();
            jsonObjekat["tip"] = "obavijesti";
            jsonObjekat["lista"] = dogadjaji;

            string json = JsonConvert.SerializeObject(jsonObjekat, Formatting.Indented);

            MessageBox.Show(json);
        }

        private void editAdmin_Click(object sender, EventArgs e)
        {
            //kupi vrijednosti iz textbox2(username),textbox3(stari password) i textbox4(novi password)
            //i sacuva te info o adminu jer trebaju za login formu


            string noviUsername = textBox2.Text;
            string stariPassword = textBox3.Text;
            string noviPassword = textBox4.Text;

            // ucitaj trenutne podatke iz json-a
            string trenutniUser = "admin";
            string trenutniPass = "1234";

            if (File.Exists("admin.json"))
            {
                string jsonRead = File.ReadAllText("admin.json");

                Dictionary<string, string> adminData =
                    JsonConvert.DeserializeObject<Dictionary<string, string>>(jsonRead);

                trenutniUser = adminData["username"];
                trenutniPass = adminData["password"];
            }

            // provjera starog passworda
            if (stariPassword != trenutniPass)
            {
                MessageBox.Show("Stari password nije tačan!");
                return;
            }

            // snimanje novih podataka
            Dictionary<string, string> noviAdmin = new Dictionary<string, string>();
            noviAdmin["username"] = noviUsername;
            noviAdmin["password"] = noviPassword;

            string json = JsonConvert.SerializeObject(noviAdmin, Formatting.Indented);
            File.WriteAllText("admin.json", json);

            MessageBox.Show("Admin podaci su uspješno promijenjeni!");
        }

        private void editWifi_Click(object sender, EventArgs e)
        {
            //kupi vrijednosti iz textbox5(naziv wifi) i textbox6(sifra wifi) i sacuva te
            //info o wifi te sluzi da promjeni na arduino fajlu kako bi radio esp odnosno rtc

           

            Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();
            jsonObjekat["tip"] = "wifi";
            jsonObjekat["ssid"] = textBox5.Text;
            jsonObjekat["password"] = textBox6.Text;

            string json = JsonConvert.SerializeObject(jsonObjekat, Formatting.Indented);

            MessageBox.Show(json);
        }
    }
}
