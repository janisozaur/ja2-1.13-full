namespace INIEditor.GUI
{
    partial class SearchForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SearchForm));
            this.lblFindWhat = new System.Windows.Forms.Label();
            this.txtFindWhat = new System.Windows.Forms.TextBox();
            this.grpLookIn = new System.Windows.Forms.GroupBox();
            this.chkProperties = new System.Windows.Forms.CheckBox();
            this.chkSectionDescriptions = new System.Windows.Forms.CheckBox();
            this.chkPropertyDescriptions = new System.Windows.Forms.CheckBox();
            this.chkPropertyValues = new System.Windows.Forms.CheckBox();
            this.btnSearch = new System.Windows.Forms.Button();
            this.picLanguage_GER = new System.Windows.Forms.PictureBox();
            this.picLanguage_ENG = new System.Windows.Forms.PictureBox();
            this.picLanguage_RUS = new System.Windows.Forms.PictureBox();
            this.grpLookIn.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picLanguage_GER)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picLanguage_ENG)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picLanguage_RUS)).BeginInit();
            this.SuspendLayout();
            // 
            // lblFindWhat
            // 
            this.lblFindWhat.AutoSize = true;
            this.lblFindWhat.Location = new System.Drawing.Point(12, 9);
            this.lblFindWhat.Name = "lblFindWhat";
            this.lblFindWhat.Size = new System.Drawing.Size(56, 13);
            this.lblFindWhat.TabIndex = 0;
            this.lblFindWhat.Text = "Find what:";
            // 
            // txtFindWhat
            // 
            this.txtFindWhat.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtFindWhat.Location = new System.Drawing.Point(12, 27);
            this.txtFindWhat.Name = "txtFindWhat";
            this.txtFindWhat.Size = new System.Drawing.Size(335, 20);
            this.txtFindWhat.TabIndex = 1;
            // 
            // grpLookIn
            // 
            this.grpLookIn.Controls.Add(this.chkProperties);
            this.grpLookIn.Controls.Add(this.chkSectionDescriptions);
            this.grpLookIn.Controls.Add(this.chkPropertyDescriptions);
            this.grpLookIn.Controls.Add(this.chkPropertyValues);
            this.grpLookIn.Location = new System.Drawing.Point(12, 68);
            this.grpLookIn.Name = "grpLookIn";
            this.grpLookIn.Size = new System.Drawing.Size(358, 92);
            this.grpLookIn.TabIndex = 3;
            this.grpLookIn.TabStop = false;
            this.grpLookIn.Text = "Look in:";
            // 
            // chkProperties
            // 
            this.chkProperties.AutoSize = true;
            this.chkProperties.Checked = true;
            this.chkProperties.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkProperties.Location = new System.Drawing.Point(209, 20);
            this.chkProperties.Name = "chkProperties";
            this.chkProperties.Size = new System.Drawing.Size(73, 17);
            this.chkProperties.TabIndex = 4;
            this.chkProperties.Text = "Properties";
            this.chkProperties.UseVisualStyleBackColor = true;
            this.chkProperties.CheckedChanged += new System.EventHandler(this.chkProperties_CheckedChanged);
            // 
            // chkSectionDescriptions
            // 
            this.chkSectionDescriptions.AutoSize = true;
            this.chkSectionDescriptions.Location = new System.Drawing.Point(6, 20);
            this.chkSectionDescriptions.Name = "chkSectionDescriptions";
            this.chkSectionDescriptions.Size = new System.Drawing.Size(123, 17);
            this.chkSectionDescriptions.TabIndex = 3;
            this.chkSectionDescriptions.Text = "Section Descriptions";
            this.chkSectionDescriptions.UseVisualStyleBackColor = true;
            this.chkSectionDescriptions.CheckedChanged += new System.EventHandler(this.chkSectionDescriptions_CheckedChanged);
            // 
            // chkPropertyDescriptions
            // 
            this.chkPropertyDescriptions.AutoSize = true;
            this.chkPropertyDescriptions.Checked = true;
            this.chkPropertyDescriptions.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkPropertyDescriptions.Location = new System.Drawing.Point(209, 66);
            this.chkPropertyDescriptions.Name = "chkPropertyDescriptions";
            this.chkPropertyDescriptions.Size = new System.Drawing.Size(126, 17);
            this.chkPropertyDescriptions.TabIndex = 2;
            this.chkPropertyDescriptions.Text = "Property Descriptions";
            this.chkPropertyDescriptions.UseVisualStyleBackColor = true;
            this.chkPropertyDescriptions.CheckedChanged += new System.EventHandler(this.chkPropertyDescriptions_CheckedChanged);
            // 
            // chkPropertyValues
            // 
            this.chkPropertyValues.AutoSize = true;
            this.chkPropertyValues.Checked = true;
            this.chkPropertyValues.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkPropertyValues.Location = new System.Drawing.Point(209, 43);
            this.chkPropertyValues.Name = "chkPropertyValues";
            this.chkPropertyValues.Size = new System.Drawing.Size(100, 17);
            this.chkPropertyValues.TabIndex = 1;
            this.chkPropertyValues.Text = "Property Values";
            this.chkPropertyValues.UseVisualStyleBackColor = true;
            this.chkPropertyValues.CheckedChanged += new System.EventHandler(this.chkPropertyValues_CheckedChanged);
            // 
            // btnSearch
            // 
            this.btnSearch.Location = new System.Drawing.Point(295, 168);
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.Size = new System.Drawing.Size(75, 23);
            this.btnSearch.TabIndex = 4;
            this.btnSearch.Text = "Search";
            this.btnSearch.UseVisualStyleBackColor = true;
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // picLanguage_GER
            // 
            this.picLanguage_GER.Image = ((System.Drawing.Image)(resources.GetObject("picLanguage_GER.Image")));
            this.picLanguage_GER.Location = new System.Drawing.Point(356, 29);
            this.picLanguage_GER.Name = "picLanguage_GER";
            this.picLanguage_GER.Size = new System.Drawing.Size(16, 16);
            this.picLanguage_GER.TabIndex = 5;
            this.picLanguage_GER.TabStop = false;
            // 
            // picLanguage_ENG
            // 
            this.picLanguage_ENG.Image = ((System.Drawing.Image)(resources.GetObject("picLanguage_ENG.Image")));
            this.picLanguage_ENG.Location = new System.Drawing.Point(356, 29);
            this.picLanguage_ENG.Name = "picLanguage_ENG";
            this.picLanguage_ENG.Size = new System.Drawing.Size(16, 16);
            this.picLanguage_ENG.TabIndex = 6;
            this.picLanguage_ENG.TabStop = false;
            // 
            // picLanguage_RUS
            // 
            this.picLanguage_RUS.Image = ((System.Drawing.Image)(resources.GetObject("picLanguage_RUS.Image")));
            this.picLanguage_RUS.Location = new System.Drawing.Point(356, 29);
            this.picLanguage_RUS.Name = "picLanguage_RUS";
            this.picLanguage_RUS.Size = new System.Drawing.Size(16, 16);
            this.picLanguage_RUS.TabIndex = 7;
            this.picLanguage_RUS.TabStop = false;
            // 
            // SearchForm
            // 
            this.AcceptButton = this.btnSearch;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 203);
            this.Controls.Add(this.picLanguage_RUS);
            this.Controls.Add(this.picLanguage_ENG);
            this.Controls.Add(this.picLanguage_GER);
            this.Controls.Add(this.btnSearch);
            this.Controls.Add(this.grpLookIn);
            this.Controls.Add(this.txtFindWhat);
            this.Controls.Add(this.lblFindWhat);
            this.HelpButton = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(390, 280);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(390, 237);
            this.Name = "SearchForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Search";
            this.grpLookIn.ResumeLayout(false);
            this.grpLookIn.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picLanguage_GER)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picLanguage_ENG)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picLanguage_RUS)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblFindWhat;
        private System.Windows.Forms.TextBox txtFindWhat;
        private System.Windows.Forms.GroupBox grpLookIn;
        private System.Windows.Forms.CheckBox chkPropertyDescriptions;
        private System.Windows.Forms.CheckBox chkPropertyValues;
        private System.Windows.Forms.Button btnSearch;
        private System.Windows.Forms.CheckBox chkSectionDescriptions;
        private System.Windows.Forms.CheckBox chkProperties;
        private System.Windows.Forms.PictureBox picLanguage_GER;
        private System.Windows.Forms.PictureBox picLanguage_ENG;
        private System.Windows.Forms.PictureBox picLanguage_RUS;
    }
}