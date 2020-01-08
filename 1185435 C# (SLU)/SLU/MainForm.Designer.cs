namespace SLU
{
    partial class MainForm
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnCalc = new System.Windows.Forms.Button();
            this.lblResult = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.udn = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.udn)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCalc
            // 
            this.btnCalc.Location = new System.Drawing.Point(305, 12);
            this.btnCalc.Name = "btnCalc";
            this.btnCalc.Size = new System.Drawing.Size(86, 20);
            this.btnCalc.TabIndex = 0;
            this.btnCalc.Text = "Расчёт";
            this.btnCalc.UseVisualStyleBackColor = true;
            this.btnCalc.Click += new System.EventHandler(this.btnCalc_Click);
            // 
            // lblResult
            // 
            this.lblResult.AutoSize = true;
            this.lblResult.Location = new System.Drawing.Point(412, 19);
            this.lblResult.Name = "lblResult";
            this.lblResult.Size = new System.Drawing.Size(0, 13);
            this.lblResult.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 12);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(194, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Количество уравнений (переменных)";
            // 
            // udn
            // 
            this.udn.Location = new System.Drawing.Point(236, 12);
            this.udn.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.udn.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.udn.Name = "udn";
            this.udn.Size = new System.Drawing.Size(52, 20);
            this.udn.TabIndex = 5;
            this.udn.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
            this.udn.ValueChanged += new System.EventHandler(this.udn_ValueChanged);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(615, 355);
            this.Controls.Add(this.udn);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.lblResult);
            this.Controls.Add(this.btnCalc);
            this.Name = "MainForm";
            this.Text = "Системы линейных уравнений";
            ((System.ComponentModel.ISupportInitialize)(this.udn)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        const int left = 20;
        const int top = 50;
        const int w = 30;
        const int h = 20;
        const int gap = 10;

        private System.Windows.Forms.TextBox[,] txtA;
        private System.Windows.Forms.TextBox[] txtB;
        private System.Windows.Forms.TextBox[] txtX;

        private System.Windows.Forms.Button btnCalc;
        private System.Windows.Forms.Label lblResult;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown udn;
    }
}

