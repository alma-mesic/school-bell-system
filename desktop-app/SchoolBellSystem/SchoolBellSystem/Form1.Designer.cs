namespace SchoolBellSystem
{
    partial class ETSbell
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
            this.Postavke = new System.Windows.Forms.TabPage();
            this.Testiranje = new System.Windows.Forms.TabPage();
            this.label8 = new System.Windows.Forms.Label();
            this.bellstart = new System.Windows.Forms.Button();
            this.bellstop = new System.Windows.Forms.Button();
            this.Obavijesti = new System.Windows.Forms.TabPage();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.dateTimePicker1 = new System.Windows.Forms.DateTimePicker();
            this.label7 = new System.Windows.Forms.Label();
            this.dateTimePicker2 = new System.Windows.Forms.DateTimePicker();
            this.Raspored = new System.Windows.Forms.TabPage();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Vrsta = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.Kraj = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Pocetak = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Cas = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Dan = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Home = new System.Windows.Forms.TabPage();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.Testiranje.SuspendLayout();
            this.Obavijesti.SuspendLayout();
            this.Raspored.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.Home.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.SuspendLayout();
            // 
            // Postavke
            // 
            this.Postavke.Location = new System.Drawing.Point(4, 22);
            this.Postavke.Margin = new System.Windows.Forms.Padding(2);
            this.Postavke.Name = "Postavke";
            this.Postavke.Size = new System.Drawing.Size(830, 443);
            this.Postavke.TabIndex = 3;
            this.Postavke.Text = "Postavke";
            this.Postavke.UseVisualStyleBackColor = true;
            // 
            // Testiranje
            // 
            this.Testiranje.Controls.Add(this.bellstop);
            this.Testiranje.Controls.Add(this.bellstart);
            this.Testiranje.Controls.Add(this.label8);
            this.Testiranje.Location = new System.Drawing.Point(4, 22);
            this.Testiranje.Margin = new System.Windows.Forms.Padding(2);
            this.Testiranje.Name = "Testiranje";
            this.Testiranje.Size = new System.Drawing.Size(830, 443);
            this.Testiranje.TabIndex = 2;
            this.Testiranje.Text = "Testiranje";
            this.Testiranje.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(368, 148);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(88, 13);
            this.label8.TabIndex = 0;
            this.label8.Text = "Testiranje zvona:";
            // 
            // bellstart
            // 
            this.bellstart.Location = new System.Drawing.Point(361, 178);
            this.bellstart.Name = "bellstart";
            this.bellstart.Size = new System.Drawing.Size(106, 30);
            this.bellstart.TabIndex = 1;
            this.bellstart.Text = "pokreni zvono";
            this.bellstart.UseVisualStyleBackColor = true;
            this.bellstart.Click += new System.EventHandler(this.bellstart_Click);
            // 
            // bellstop
            // 
            this.bellstop.Location = new System.Drawing.Point(361, 224);
            this.bellstop.Name = "bellstop";
            this.bellstop.Size = new System.Drawing.Size(106, 30);
            this.bellstop.TabIndex = 2;
            this.bellstop.Text = "ugasi zvono";
            this.bellstop.UseVisualStyleBackColor = true;
            this.bellstop.Click += new System.EventHandler(this.bellstop_Click);
            // 
            // Obavijesti
            // 
            this.Obavijesti.Controls.Add(this.dateTimePicker2);
            this.Obavijesti.Controls.Add(this.label7);
            this.Obavijesti.Controls.Add(this.dateTimePicker1);
            this.Obavijesti.Controls.Add(this.label6);
            this.Obavijesti.Controls.Add(this.label5);
            this.Obavijesti.Controls.Add(this.listBox1);
            this.Obavijesti.Controls.Add(this.button2);
            this.Obavijesti.Controls.Add(this.textBox1);
            this.Obavijesti.Location = new System.Drawing.Point(4, 22);
            this.Obavijesti.Margin = new System.Windows.Forms.Padding(2);
            this.Obavijesti.Name = "Obavijesti";
            this.Obavijesti.Padding = new System.Windows.Forms.Padding(2);
            this.Obavijesti.Size = new System.Drawing.Size(830, 443);
            this.Obavijesti.TabIndex = 1;
            this.Obavijesti.Text = "Obavjesti";
            this.Obavijesti.UseVisualStyleBackColor = true;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(297, 140);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(170, 20);
            this.textBox1.TabIndex = 1;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(297, 176);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(85, 29);
            this.button2.TabIndex = 2;
            this.button2.Text = "Dodaj";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(297, 234);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(215, 186);
            this.listBox1.TabIndex = 3;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(294, 25);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(73, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "Izaberi datum:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(294, 124);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(109, 13);
            this.label6.TabIndex = 5;
            this.label6.Text = "Upiši naziv događaja:";
            // 
            // dateTimePicker1
            // 
            this.dateTimePicker1.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.dateTimePicker1.Location = new System.Drawing.Point(297, 41);
            this.dateTimePicker1.Name = "dateTimePicker1";
            this.dateTimePicker1.Size = new System.Drawing.Size(200, 20);
            this.dateTimePicker1.TabIndex = 6;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(294, 73);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(77, 13);
            this.label7.TabIndex = 7;
            this.label7.Text = "Izaberi vrijeme:";
            // 
            // dateTimePicker2
            // 
            this.dateTimePicker2.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.dateTimePicker2.Location = new System.Drawing.Point(297, 89);
            this.dateTimePicker2.Name = "dateTimePicker2";
            this.dateTimePicker2.ShowUpDown = true;
            this.dateTimePicker2.Size = new System.Drawing.Size(200, 20);
            this.dateTimePicker2.TabIndex = 8;
            // 
            // Raspored
            // 
            this.Raspored.Controls.Add(this.dataGridView1);
            this.Raspored.Location = new System.Drawing.Point(4, 22);
            this.Raspored.Margin = new System.Windows.Forms.Padding(2);
            this.Raspored.Name = "Raspored";
            this.Raspored.Padding = new System.Windows.Forms.Padding(2);
            this.Raspored.Size = new System.Drawing.Size(830, 443);
            this.Raspored.TabIndex = 0;
            this.Raspored.Text = "Raspored";
            this.Raspored.UseVisualStyleBackColor = true;
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Dan,
            this.Cas,
            this.Pocetak,
            this.Kraj,
            this.Vrsta});
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(2, 2);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(826, 439);
            this.dataGridView1.TabIndex = 0;
            this.dataGridView1.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellContentClick);
            // 
            // Vrsta
            // 
            this.Vrsta.HeaderText = "Vrsta";
            this.Vrsta.Name = "Vrsta";
            // 
            // Kraj
            // 
            this.Kraj.HeaderText = "Kraj";
            this.Kraj.Name = "Kraj";
            // 
            // Pocetak
            // 
            this.Pocetak.HeaderText = "Pocetak";
            this.Pocetak.Name = "Pocetak";
            // 
            // Cas
            // 
            this.Cas.HeaderText = "Cas";
            this.Cas.Name = "Cas";
            // 
            // Dan
            // 
            this.Dan.HeaderText = "Dan";
            this.Dan.Name = "Dan";
            // 
            // Home
            // 
            this.Home.Controls.Add(this.label4);
            this.Home.Controls.Add(this.button1);
            this.Home.Controls.Add(this.label3);
            this.Home.Controls.Add(this.label2);
            this.Home.Controls.Add(this.label1);
            this.Home.Controls.Add(this.pictureBox1);
            this.Home.Location = new System.Drawing.Point(4, 22);
            this.Home.Name = "Home";
            this.Home.Size = new System.Drawing.Size(830, 443);
            this.Home.TabIndex = 4;
            this.Home.Text = "Home";
            this.Home.UseVisualStyleBackColor = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(281, 32);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(276, 183);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(332, 238);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(173, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "JU MS Elektrotehnička škola Tuzla";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(332, 271);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(162, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "\"Škola koja postavlja standarde\"";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(385, 306);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(60, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Dobrodošli!";
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.Red;
            this.button1.ForeColor = System.Drawing.Color.Snow;
            this.button1.Location = new System.Drawing.Point(441, 339);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(107, 35);
            this.button1.TabIndex = 4;
            this.button1.Text = "Emergency";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(306, 350);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(118, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "U slučaju nužde pritisni:";
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.Home);
            this.tabControl1.Controls.Add(this.Raspored);
            this.tabControl1.Controls.Add(this.Obavijesti);
            this.tabControl1.Controls.Add(this.Testiranje);
            this.tabControl1.Controls.Add(this.Postavke);
            this.tabControl1.Location = new System.Drawing.Point(10, 9);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(2);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(838, 469);
            this.tabControl1.TabIndex = 0;
            // 
            // ETSbell
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(859, 496);
            this.Controls.Add(this.tabControl1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "ETSbell";
            this.Text = "ETSbell";
            this.Load += new System.EventHandler(this.ETSbell_Load);
            this.Testiranje.ResumeLayout(false);
            this.Testiranje.PerformLayout();
            this.Obavijesti.ResumeLayout(false);
            this.Obavijesti.PerformLayout();
            this.Raspored.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.Home.ResumeLayout(false);
            this.Home.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabPage Postavke;
        private System.Windows.Forms.TabPage Testiranje;
        private System.Windows.Forms.Button bellstop;
        private System.Windows.Forms.Button bellstart;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TabPage Obavijesti;
        private System.Windows.Forms.DateTimePicker dateTimePicker2;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.DateTimePicker dateTimePicker1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TabPage Raspored;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Dan;
        private System.Windows.Forms.DataGridViewTextBoxColumn Cas;
        private System.Windows.Forms.DataGridViewTextBoxColumn Pocetak;
        private System.Windows.Forms.DataGridViewTextBoxColumn Kraj;
        private System.Windows.Forms.DataGridViewComboBoxColumn Vrsta;
        private System.Windows.Forms.TabPage Home;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.TabControl tabControl1;
    }
}

