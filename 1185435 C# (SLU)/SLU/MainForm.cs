using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SLU
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            int N = Convert.ToInt32(this.udn.Maximum);
            int maxCount = N * N;
            int n = Convert.ToInt32(this.udn.Value);
            int count = n * n;

            this.txtA = new System.Windows.Forms.TextBox[N, N];
            for (int row = 0; row < N; row++)
            {
                for (int col = 0; col < N; col++)
                {
                    this.txtA[row, col] = new System.Windows.Forms.TextBox();
                    int x = left + col * (w + gap);
                    int y = top + row * (h + gap);
                    this.txtA[row, col].Location = new System.Drawing.Point(x, y);
                    this.txtA[row, col].Name = "txtA_" + row.ToString() + "_" + col.ToString();
                    this.txtA[row, col].Size = new System.Drawing.Size(w, h);
                    //this.txtA[row, col].TabIndex = 4 + i;
                    this.txtA[row, col].Text = "0";
                    if (row < n && col < n)
                    {
                        this.txtA[row, col].Visible = true;
                    }
                    else
                    {
                        this.txtA[row, col].Visible = false;
                    }
                    this.Controls.Add(this.txtA[row, col]);
                }
            }

            this.txtB = new System.Windows.Forms.TextBox[N];
            int xB = left + gap + n * (w + gap);
            for (int i = 0; i < N; i++)
            {
                this.txtB[i] = new System.Windows.Forms.TextBox();
                int y = top  + i * (h + gap);
                this.txtB[i].Location = new System.Drawing.Point(xB, y);
                this.txtB[i].Name = "txtB" + i.ToString();
                this.txtB[i].Size = new System.Drawing.Size(w, h);
                //this.txtB[i].TabIndex = 4 + count + i;
                this.txtB[i].Text = "0";
                if (i < n)
                { 
                    this.txtB[i].Visible = true;
                }
                else
                {
                    this.txtB[i].Visible = false;
                }
                this.Controls.Add(this.txtB[i]);
            }
            this.txtX = new System.Windows.Forms.TextBox[N];
            int xX = left + gap + gap + w + n * (w + gap);
            for (int i = 0; i < N; i++)
            {
                this.txtX[i] = new System.Windows.Forms.TextBox();
                int y = top + i * (h + gap);
                this.txtX[i].Location = new System.Drawing.Point(xX, y);
                this.txtX[i].Name = "txtX" + i.ToString();
                this.txtX[i].Size = new System.Drawing.Size(w, h);
                this.txtX[i].Text = "";
                this.txtX[i].BackColor = Color.Aqua;
                this.txtX[i].ReadOnly = true;
                if (i < n)
                {
                    this.txtX[i].Visible = true;
                }
                else
                {
                    this.txtX[i].Visible = false;
                }
                this.Controls.Add(this.txtX[i]);
            }

            SetDefaultValues();
        }

        private void SetDefaultValues()
        {
            decimal[,] A = new decimal[,]
            {
                { 1, -1, 1 },
                { 2, 1, 1 },
                { 1, 1, 2 }
            };

            decimal[] B = new decimal[3] { 3, 11, 8 };

            int n = Convert.ToInt32(this.udn.Value);
            for (int row = 0; row < n; row++)
            {
                for (int col = 0; col < n; col++)
                {
                    this.txtA[row, col].Text = A[row, col].ToString();
                }
                this.txtB[row].Text = B[row].ToString();
            }
        }

        private void btnCalc_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < this.txtX.Length; i++)
            {
                this.txtX[i].Text = "";
            }

            int n = Convert.ToInt32(this.udn.Value);
            int count = n * n;

            decimal[] A = new decimal[count];
            for (int i = 0; i < count; i++)
            {
                int row = i / n;
                int col = i % n;
                A[i] = Convert.ToInt32(this.txtA[row, col].Text);
            }

            decimal[] B = new decimal[n];
            for (int i = 0; i < n; i++)
            {
                B[i] = Convert.ToInt32(this.txtB[i].Text);
            }

            EqSys es = new EqSys();
            decimal[] X = es.Kramer(n, A, B);
            if(X != null)
            {
                for (int i = 0; i < X.Length; i++)
                {
                    this.txtX[i].Text = X[i].ToString();
                }
                lblResult.Text = "Корни системы найдены";
            }
            else
            {
                lblResult.Text = "Система не имеет корней";
            }
        }

        private void udn_ValueChanged(object sender, EventArgs e)
        {
            int N = Convert.ToInt32(this.udn.Maximum);
            int n = Convert.ToInt32(this.udn.Value);

            for (int row = 0; row < N; row++)
            {
                for (int col = 0; col < N; col++)
                {
                    int x = left + col * (w + gap);
                    int y = top + row * (h + gap);
                    this.txtA[row, col].Location = new System.Drawing.Point(x, y);
                    //this.txtA[row, col].TabIndex = 4 + i;
                    if (row < n && col < n)
                    {
                        this.txtA[row, col].Visible = true;
                    }
                    else
                    {
                        this.txtA[row, col].Visible = false;
                    }
                }
            }

            int xB = left + gap + n * (w + gap);
            for (int i = 0; i < N; i++)
            {
                int y = top + i * (h + gap);
                this.txtB[i].Location = new System.Drawing.Point(xB, y);
                //this.txtB[i].TabIndex = 4 + count + i;
                if (i < n)
                {
                    this.txtB[i].Visible = true;
                }
                else
                {
                    this.txtB[i].Visible = false;
                }
            }

            int xX = left + gap + gap + w + n * (w + gap);
            for (int i = 0; i < N; i++)
            {
                int y = top + i * (h + gap);
                this.txtX[i].Location = new System.Drawing.Point(xX, y);
                if (i < n)
                {
                    this.txtX[i].Visible = true;
                }
                else
                {
                    this.txtX[i].Visible = false;
                }
            }
        }
    }
}
