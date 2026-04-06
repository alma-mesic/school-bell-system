using System;
using System.IO;
using System.Windows.Forms;
using Tesseract;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;

namespace converting
{ 
    public partial class Form1 : Form
    {
        public class DanRaspored
        {
            public string date { get; set; }
            public Dictionary<string, string> schedule { get; set; } = new Dictionary<string, string>();
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void select_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                textBox1.Text = openFileDialog1.FileName;
            }
        }

        private void convert_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(textBox1.Text))
            {
                MessageBox.Show("Molim vas prvo odaberite PDF fajl.");
                return;
            }

            try
            {
                Spire.Pdf.PdfDocument doc = new Spire.Pdf.PdfDocument();
                doc.LoadFromFile(textBox1.Text);
                Bitmap bmp = (Bitmap)doc.SaveAsImage(0);
                doc.Close();

                var finalniRaspored = new Dictionary<string, DanRaspored>();

                string[] naziviDana = { "Ponedjeljak", "Utorak", "Srijeda", "Četvrtak", "Petak" };
                string[] satnice = {
                    "07:00 do 08:00", "08:00 do 09:00", "09:00 do 10:00", "10:00 do 11:00",
                    "11:00 do 12:00", "12:00 do 13:00", "13:00 do 14:00", "14:00 do 15:00",
                    "15:00 do 16:00", "16:00 do 17:00", "17:00 do 18:00", "18:00 do 19:00"
                };

                int startX = 150;
                int startY = 250;
                int sirinaCelije = 120;
                int visinaCelije = 35;

                using (var engine = new TesseractEngine(@"./tessdata", "bos", EngineMode.Default))
                {
                    for (int kolona = 0; kolona < 5; kolona++)
                    {
                        string trenutniDan = naziviDana[kolona];

                        if (!finalniRaspored.ContainsKey(trenutniDan))
                        {
                            finalniRaspored[trenutniDan] = new DanRaspored
                            {
                                date = DateTime.Now.Date.AddDays(kolona).ToString("yyyy-MM-dd"),
                                schedule = new Dictionary<string, string>()
                            };
                        }

                        for (int red = 0; red < 12; red++)
                        {
                            Rectangle rect = new Rectangle(
                                startX + (kolona * sirinaCelije),
                                startY + (red * visinaCelije),
                                sirinaCelije,
                                visinaCelije
                            );

                            using (Bitmap celijaBmp = bmp.Clone(rect, bmp.PixelFormat))
                            using (var img = PixConverter.ToPix(celijaBmp))
                            using (var page = engine.Process(img, PageSegMode.SingleLine))
                            {
                                string procitanoIme = page.GetText().Trim();

                                if (!string.IsNullOrWhiteSpace(procitanoIme))
                                {
                                    procitanoIme = System.Text.RegularExpressions.Regex.Replace(procitanoIme, @"[|._\-]", "");
                                    finalniRaspored[trenutniDan].schedule[satnice[red]] = procitanoIme;
                                }
                                else
                                {
                                    finalniRaspored[trenutniDan].schedule[satnice[red]] = "";
                                }
                            }
                        }
                    }
                }

                File.WriteAllText("raspored.json", JsonConvert.SerializeObject(finalniRaspored, Formatting.Indented));
                MessageBox.Show("Konvertovanje gotovo!");
            }
            catch (Exception ex) { MessageBox.Show("Greška: " + ex.Message); }
        }
    }
}



