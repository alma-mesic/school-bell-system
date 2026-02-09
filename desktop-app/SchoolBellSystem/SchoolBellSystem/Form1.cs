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
using System.IO.Ports;
using System.Drawing.Drawing2D;

namespace SchoolBellSystem
{
    public partial class ETSbell : Form
    {
        SerialPort serialPort;
        Timer obavijestiTimer;

        // -------------------- FUNCTIONS -----------------------
        public static void SetRoundCorners(Control c, int radius)
        {
            GraphicsPath path = new GraphicsPath();

            path.AddArc(0, 0, radius, radius, 180, 90);
            path.AddArc(c.Width - radius, 0, radius, radius, 270, 90);
            path.AddArc(c.Width - radius, c.Height - radius, radius, radius, 0, 90);
            path.AddArc(0, c.Height - radius, radius, radius, 90, 90);

            c.Region = new Region(path);
        }
        void SendToESP(string json) // slanje podataka na ESP32
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.Write(json + "\n"); // OBAVEZNO \n
                serialPort.BaseStream.Flush(); // forsira slanje
            }
            else
            {
                MessageBox.Show("Serial port nije otvoren!");
            }
        }

        private void ObavijestiTimer_Tick(object sender, EventArgs e) // uskladjivanje notifikacija s vremenom
        {
            DateTime sada = DateTime.Now;

            List<string> zaBrisanje = new List<string>();

            foreach (string item in listBox1.Items)
            {
                // format: yyyyMMddHHmm NAZIV dd.MM.yyyy. HH:mm
                string[] dijelovi = item.Split(' ');

                if (dijelovi.Length < 4) continue;

                // uzmi vrijeme događaja
                DateTime vrijemeDogadjaja = DateTime.ParseExact(
                    dijelovi[2] + " " + dijelovi[3],
                    "dd.MM.yyyy. HH:mm",
                    System.Globalization.CultureInfo.InvariantCulture
                );

                // dozvoli da se prikazuje još 1 min nakon događaja
                if (sada > vrijemeDogadjaja.AddMinutes(1))
                {
                    zaBrisanje.Add(item);
                }
            }

            // brisanje iz listboxa
            foreach (string item in zaBrisanje)
            {
                listBox1.Items.Remove(item);
            }
        }

        private void LoadSavedNotifications() // ucitavanje sacuvanih notifikacija
        {
            try
            {
                string path = "obavijesti.json";
                if (!File.Exists(path)) return;

                string json = File.ReadAllText(path);

                dynamic doc = JsonConvert.DeserializeObject(json);

                listBox1.Items.Clear();

                foreach (var o in doc.lista)
                {
                    string naziv = o.naziv;
                    DateTime dt = DateTime.ParseExact((string)o.datumVrijeme, "yyyy-MM-dd HH:mm",
                        System.Globalization.CultureInfo.InvariantCulture);

                    string stavka = dt.ToString("yyyyMMddHHmm") + " " + naziv + " " + dt.ToString("dd.MM.yyyy. HH:mm");

                    listBox1.Items.Add(stavka);
                }

                listBox1.Sorted = true;
            }
            catch
            {
                // ignorisi greške pri čitanju
            }
        }

        // ------------------------ GLAVNI KOD --------------------------
        public ETSbell()
        {
            InitializeComponent();
        }
        private void ETSbell_Load(object sender, EventArgs e) // LOAD
        {
            panel1.BackColor = Color.FromArgb(170, 255, 255, 255);
            panel2.BackColor = Color.FromArgb(170, 255, 255, 255);
            panel3.BackColor = Color.FromArgb(170, 255, 255, 255);
            panel4.BackColor = Color.FromArgb(170, 255, 255, 255);
            SetRoundCorners(panel1, 20);
            SetRoundCorners(panel2, 20);
            SetRoundCorners(panel3, 20);
            SetRoundCorners(panel4, 20);

            label5.BackColor = Color.Transparent;
            label6.BackColor = Color.Transparent;
            label7.BackColor = Color.Transparent;
            label8.BackColor = Color.Transparent;
            label9.BackColor = Color.Transparent;
            label10.BackColor = Color.Transparent;
            label11.BackColor = Color.Transparent;
            label12.BackColor = Color.Transparent;
            label13.BackColor = Color.Transparent;
            label14.BackColor = Color.Transparent;
            label15.BackColor = Color.Transparent;
            checkBox1.BackColor = Color.Transparent;
            checkBox2.BackColor = Color.Transparent;

            pictureBox1.BackColor = Color.Transparent;
            textBox3.UseSystemPasswordChar = true;
            textBox4.UseSystemPasswordChar = true;
            checkBox1.Checked = false;

            textBox6.UseSystemPasswordChar = true;
            checkBox2.Checked = false;

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

            // popuni SVE ostale kolone razmakom (da se vide)
            foreach (DataGridViewRow row in dataGridView1.Rows)
            {
                for (int c = 3; c < dataGridView1.Columns.Count; c++)
                {
                    row.Cells[c].Value = " ";
                }
            }


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

            foreach (DataGridViewRow row in dataGridView2.Rows)
            {
                for (int c = 3; c < dataGridView2.Columns.Count; c++)
                {
                    row.Cells[c].Value = " ";
                }
            }


            try
            {
                serialPort = new SerialPort("COM3", 115200); // PROMIJENI COM
                serialPort.NewLine = "\n";

                serialPort.Open();
                System.Threading.Thread.Sleep(1000);

                MessageBox.Show("ESP32 povezan!", "Povezivanje", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Greska pri spajanju na ESP32:\n" + ex.Message, "Povezivanje", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            serialPort.ReadTimeout = 1000;
            serialPort.WriteTimeout = 1000;

            obavijestiTimer = new Timer();
            obavijestiTimer.Interval = 60 * 1000; // 1 minuta
            obavijestiTimer.Tick += ObavijestiTimer_Tick;
            obavijestiTimer.Start();

            // učitaj prethodno sačuvane obavijesti u listBox
            LoadSavedNotifications();



        }

        // ----------------------------- HOME ------------------------------------
        private void button1_Click(object sender, EventArgs e) // emergency dugme
        {
            Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();
            jsonObjekat["tip"] = "emergency";
            jsonObjekat["stanje"] = "ukljuceno";

            string json = JsonConvert.SerializeObject(jsonObjekat);

            SendToESP(json);

            MessageBox.Show("Emergency dugme pritisnuto!", "Emergency", MessageBoxButtons.OK, MessageBoxIcon.Information); // dodati ovdje yes no za potvrdu
        }


        // ---------------------------- OBAVJESTENJA -------------------------------
        private void button2_Click(object sender, EventArgs e) // Add na listbox za obavjestenja
        {
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
            textBox1.Clear();
        }
        private void button4_Click(object sender, EventArgs e) // Remove sa listboxa1
        {
            listBox1.Items.Remove(listBox1.SelectedItem);
        }

        private void button5_Click(object sender, EventArgs e) //salje obavijesti i dogadjaje iz listboxa na esp32
        {
            List<Dictionary<string, string>> dogadjaji = new List<Dictionary<string, string>>();

            foreach (string item in listBox1.Items)
            {
                string[] dijelovi = item.Split(' ');

                Dictionary<string, string> dogadjaj = new Dictionary<string, string>();
                dogadjaj["naziv"] = dijelovi[1];
                DateTime puniDatum = DateTime.ParseExact(
                    dijelovi[2] + " " + dijelovi[3],
                    "dd.MM.yyyy. HH:mm",
                    System.Globalization.CultureInfo.InvariantCulture
                );

                dogadjaj["datumVrijeme"] = puniDatum.ToString("yyyy-MM-dd HH:mm");

                dogadjaji.Add(dogadjaj);
            }

            Dictionary<string, object> jsonObjekat = new Dictionary<string, object>();
            jsonObjekat["tip"] = "obavijesti";
            jsonObjekat["lista"] = dogadjaji;

            string json = JsonConvert.SerializeObject(jsonObjekat);

            SendToESP(json);

            // Spremi i lokalno u obavijesti.json
            File.WriteAllText("obavijesti.json", json);

            MessageBox.Show("Obavjesti poslane na ESP32", "Poslano", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }


        // ------------------------------- RASPORED ---------------------------------
        private void button3_Click(object sender, EventArgs e) //salje raspored iz datagridview na esp eeprom i gleda da li je selektan radiobuttn 1 ili 2
        {                                                       //i na osnovu toga bira po kojem rasporedu ce zvoniti zvono (datagridview1 ili datagridview2

            DataGridView aktivniGrid;

            if (radioButton1.Checked) // pretpostavka: radioButton1 = redovni
                aktivniGrid = dataGridView1;
            else
                aktivniGrid = dataGridView2;

            List<Dictionary<string, string>> casovi = new List<Dictionary<string, string>>();

            string[] dani = { "Ponedjeljak", "Utorak", "Srijeda", "Četvrtak", "Petak" };

            foreach (DataGridViewRow row in aktivniGrid.Rows)
            {
                if (row.Cells[0].Value == null) continue;

                for (int d = 0; d < dani.Length; d++)
                {
                    string dezurni = row.Cells[3 + d].Value?.ToString().Trim();

                    if (string.IsNullOrWhiteSpace(dezurni))
                        continue;

                    Dictionary<string, string> zapis = new Dictionary<string, string>();
                    zapis["dan"] = dani[d];
                    zapis["cas"] = row.Cells[0].Value.ToString();
                    zapis["pocetak"] = row.Cells[1].Value.ToString();
                    zapis["kraj"] = row.Cells[2].Value.ToString();
                    zapis["dezurni"] = dezurni;

                    casovi.Add(zapis);
                }
            }


            Dictionary<string, object> jsonObjekat = new Dictionary<string, object>();
            jsonObjekat["tip"] = "raspored";
            jsonObjekat["nacin"] = radioButton1.Checked ? "redovni" : "skraceni";
            jsonObjekat["casovi"] = casovi;

            string json = JsonConvert.SerializeObject(jsonObjekat);

            SendToESP(json);
            MessageBox.Show("Raspored je poslan!", "Poslano", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }


        // --------------------------- TESTIRANJE -------------------------------
        private void bellstart_Click(object sender, EventArgs e) // test zvona (pocinje zvoniti)
        {

            Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();

            jsonObjekat["tip"] = "zvono";
            jsonObjekat["akcija"] = "start";
            string json = JsonConvert.SerializeObject(jsonObjekat);

            SendToESP(json);

            MessageBox.Show("Testiranje zvona započeto!", "Test", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        private void bellstop_Click(object sender, EventArgs e) // test zvona (prestaje zvoniti)
        {

            Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();

            jsonObjekat["tip"] = "zvono";
            jsonObjekat["akcija"] = "stop";
            string json = JsonConvert.SerializeObject(jsonObjekat);

            SendToESP(json);

            MessageBox.Show("Testiranje zvona završilo!", "Test", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }


        // ------------------------------- POSTAVKE --------------------------------

        // ************* KORISNIK ***************
        private void editAdmin_Click(object sender, EventArgs e) // informacije o adminu (postavljanje novog passworda)
        {
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

            MessageBox.Show("Admin podaci su uspješno promijenjeni!", "Admin", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        private void checkBox1_CheckedChanged(object sender, EventArgs e) // vidljivost passworda
        {
            if (checkBox1.Checked)
            {
                textBox3.UseSystemPasswordChar = false;
                textBox4.UseSystemPasswordChar = false;
            }
            else
            {
                textBox3.UseSystemPasswordChar = true;
                textBox4.UseSystemPasswordChar = true;
            }
        }

        // **************** WIFI *******************
        private void editWifi_Click(object sender, EventArgs e) // informacije o wifi mrezi (promjena naziva i passworda)
        {
            //kupi vrijednosti iz textbox5(naziv wifi) i textbox6(sifra wifi) i sacuva te
            //info o wifi te sluzi da promjeni na arduino fajlu kako bi radio esp odnosno rtc

            Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();
            jsonObjekat["tip"] = "wifi";
            jsonObjekat["ssid"] = textBox5.Text;
            jsonObjekat["password"] = textBox6.Text;

            string json = JsonConvert.SerializeObject(jsonObjekat);


            SendToESP(json);

            MessageBox.Show("Wifi podaci su uspješno promijenjeni!", "Wifi", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        private void checkBox2_CheckedChanged(object sender, EventArgs e) // vidljivost passworda
        {
            if (checkBox2.Checked)
            {
                textBox6.UseSystemPasswordChar = false;

            }
            else
            {
                textBox6.UseSystemPasswordChar = true;

            }
        }

        // ************* CISCENJE MEMORIJE *****************
        private void button6_Click(object sender, EventArgs e) // ciscenje kompletnog eeproma na ESP32
        {
            string adminPass = textBox3.Text.Trim(); // ADMIN PASSWORD

            if (string.IsNullOrWhiteSpace(adminPass))
            {
                MessageBox.Show("Unesite admin password!");
                return;
            }

            // Učitavanje podataka o adminu iz admin.json
            string trenutniPass = "1234"; // default
            if (File.Exists("admin.json"))
            {
                string jsonRead = File.ReadAllText("admin.json");
                Dictionary<string, string> adminData =
                    JsonConvert.DeserializeObject<Dictionary<string, string>>(jsonRead);
                trenutniPass = adminData.ContainsKey("password") ? adminData["password"] : "1234";
            }

            // provjera unesenog passworda
            if (adminPass != trenutniPass)
            {
                MessageBox.Show("Neispravan admin password!");
                return;
            }

            DialogResult r = MessageBox.Show(
                "DA LI STE SIGURNI?\nOvo briše SVE sa ESP32!",
                "UPOZORENJE",
                MessageBoxButtons.YesNo,
                MessageBoxIcon.Warning
            );

            if (r != DialogResult.Yes) return;

            Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();
            jsonObjekat["tip"] = "clear_eeprom";
            jsonObjekat["password"] = adminPass;

            string json = JsonConvert.SerializeObject(jsonObjekat);

            SendToESP(json);

            // Očisti i listBox lokalno
            listBox1.Items.Clear();

            MessageBox.Show("EEPROM komanda poslana ESP32!");
        }


        // ------------------------ ZATVARANJE FORME -------------------------------
        private void ETSbell_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.Close();
            }
        }

        

        private void button7_Click(object sender, EventArgs e)
        {
            
            // trenutni dan u sedmici (0 = pon, 4 = pet)
            int danIndex = (int)DateTime.Now.DayOfWeek - 1; // Monday = 0
            if (danIndex < 0 || danIndex > 4) // subota/nedjelja
            {
                // šalji ESP-u da nema dezurnog
                Dictionary<string, string> jsonObjekat = new Dictionary<string, string>();
                jsonObjekat["tip"] = "dezurni";
                jsonObjekat["ime"] = "Nema dezurnog";

                string json = JsonConvert.SerializeObject(jsonObjekat);
                SendToESP(json);
                return;
            }

            int sadaMin = DateTime.Now.Hour * 60 + DateTime.Now.Minute;
            string dezurniTrenutno = "";

            foreach (DataGridViewRow row in dataGridView3.Rows)
            {
                if (row.Cells[0].Value == null) continue;

                string dez = row.Cells[3 + danIndex].Value?.ToString().Trim();
                if (string.IsNullOrEmpty(dez) || dez == " ") continue;

                // parsiraj početak i kraj časa sigurno
                if (!TimeSpan.TryParse(row.Cells[1].Value?.ToString(), out TimeSpan start)) continue;
                if (!TimeSpan.TryParse(row.Cells[2].Value?.ToString(), out TimeSpan end)) continue;

                int startMin = start.Hours * 60 + start.Minutes;
                int endMin = end.Hours * 60 + end.Minutes;

                if (sadaMin >= startMin && sadaMin < endMin)
                {
                    dezurniTrenutno = dez;
                    break;
                }
            }

            if (string.IsNullOrEmpty(dezurniTrenutno))
                dezurniTrenutno = "Nema dezurnog";

            // šalji ESP-u
            Dictionary<string, string> jsonObjekatFinal = new Dictionary<string, string>();
            jsonObjekatFinal["tip"] = "dezurni";
            jsonObjekatFinal["ime"] = dezurniTrenutno;

            string jsonFinal = JsonConvert.SerializeObject(jsonObjekatFinal);
            SendToESP(jsonFinal);

        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void pictureBox6_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void pictureBox7_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void button8_Click(object sender, EventArgs e)
        {
            LoginForm login = new LoginForm();
            login.ShowDialog();
            this.Close();
        }
        
        // ------------------------ SLUCAJNO PRITISNUTO -----------------------------
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
        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        } 
        private void Obavijesti_Click(object sender, EventArgs e)
        {

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void Home_Click(object sender, EventArgs e)
        {

        }
    }
}
