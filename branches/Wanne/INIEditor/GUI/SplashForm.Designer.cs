namespace INIEditor.GUI
{
    partial class SplashForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SplashForm));
            this.lblApplicationTitle = new System.Windows.Forms.Label();
            this.Version = new System.Windows.Forms.Label();
            this.lblProgressText = new System.Windows.Forms.Label();
            this.pgbSplash = new System.Windows.Forms.ProgressBar();
            this.SuspendLayout();
            // 
            // lblApplicationTitle
            // 
            this.lblApplicationTitle.BackColor = System.Drawing.Color.Transparent;
            this.lblApplicationTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblApplicationTitle.ForeColor = System.Drawing.Color.Black;
            this.lblApplicationTitle.Location = new System.Drawing.Point(12, 9);
            this.lblApplicationTitle.Name = "lblApplicationTitle";
            this.lblApplicationTitle.Size = new System.Drawing.Size(376, 32);
            this.lblApplicationTitle.TabIndex = 4;
            this.lblApplicationTitle.Text = "JA2 1.13 - INI Editor";
            this.lblApplicationTitle.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
            // 
            // Version
            // 
            this.Version.BackColor = System.Drawing.Color.Transparent;
            this.Version.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Version.ForeColor = System.Drawing.Color.Black;
            this.Version.Location = new System.Drawing.Point(14, 41);
            this.Version.Name = "Version";
            this.Version.Size = new System.Drawing.Size(374, 20);
            this.Version.TabIndex = 5;
            // 
            // lblProgressText
            // 
            this.lblProgressText.BackColor = System.Drawing.Color.Transparent;
            this.lblProgressText.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblProgressText.ForeColor = System.Drawing.Color.Black;
            this.lblProgressText.Location = new System.Drawing.Point(14, 121);
            this.lblProgressText.Name = "lblProgressText";
            this.lblProgressText.Size = new System.Drawing.Size(374, 24);
            this.lblProgressText.TabIndex = 6;
            this.lblProgressText.Text = "Loading...";
            // 
            // pgbSplash
            // 
            this.pgbSplash.Location = new System.Drawing.Point(17, 95);
            this.pgbSplash.Name = "pgbSplash";
            this.pgbSplash.Size = new System.Drawing.Size(371, 23);
            this.pgbSplash.TabIndex = 7;
            // 
            // SplashForm
            // 
            this.BackColor = System.Drawing.Color.Azure;
            this.ClientSize = new System.Drawing.Size(400, 154);
            this.ControlBox = false;
            this.Controls.Add(this.pgbSplash);
            this.Controls.Add(this.lblProgressText);
            this.Controls.Add(this.Version);
            this.Controls.Add(this.lblApplicationTitle);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SplashForm";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Load += new System.EventHandler(this.SplashForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        internal System.Windows.Forms.Label lblApplicationTitle;
        internal System.Windows.Forms.Label Version;
        internal System.Windows.Forms.Label lblProgressText;
        private System.Windows.Forms.ProgressBar pgbSplash;
    }
}