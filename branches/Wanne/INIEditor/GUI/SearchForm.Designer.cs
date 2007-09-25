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
            this.lblFindWhat = new System.Windows.Forms.Label();
            this.txtFindWhat = new System.Windows.Forms.TextBox();
            this.grpLookIn = new System.Windows.Forms.GroupBox();
            this.chkSectionDescriptions = new System.Windows.Forms.CheckBox();
            this.chkPropertyDescriptions = new System.Windows.Forms.CheckBox();
            this.chkPropertyValues = new System.Windows.Forms.CheckBox();
            this.btnSearch = new System.Windows.Forms.Button();
            this.cmbDescLanguage = new System.Windows.Forms.ComboBox();
            this.lblDescLanguage = new System.Windows.Forms.Label();
            this.grpLookIn.SuspendLayout();
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
            this.txtFindWhat.Size = new System.Drawing.Size(360, 20);
            this.txtFindWhat.TabIndex = 1;
            // 
            // grpLookIn
            // 
            this.grpLookIn.Controls.Add(this.chkSectionDescriptions);
            this.grpLookIn.Controls.Add(this.chkPropertyDescriptions);
            this.grpLookIn.Controls.Add(this.chkPropertyValues);
            this.grpLookIn.Location = new System.Drawing.Point(12, 96);
            this.grpLookIn.Name = "grpLookIn";
            this.grpLookIn.Size = new System.Drawing.Size(357, 88);
            this.grpLookIn.TabIndex = 3;
            this.grpLookIn.TabStop = false;
            this.grpLookIn.Text = "Look in:";
            // 
            // chkSectionDescriptions
            // 
            this.chkSectionDescriptions.AutoSize = true;
            this.chkSectionDescriptions.Checked = true;
            this.chkSectionDescriptions.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkSectionDescriptions.Location = new System.Drawing.Point(6, 20);
            this.chkSectionDescriptions.Name = "chkSectionDescriptions";
            this.chkSectionDescriptions.Size = new System.Drawing.Size(123, 17);
            this.chkSectionDescriptions.TabIndex = 3;
            this.chkSectionDescriptions.Text = "Section Descriptions";
            this.chkSectionDescriptions.UseVisualStyleBackColor = true;
            // 
            // chkPropertyDescriptions
            // 
            this.chkPropertyDescriptions.AutoSize = true;
            this.chkPropertyDescriptions.Checked = true;
            this.chkPropertyDescriptions.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkPropertyDescriptions.Location = new System.Drawing.Point(6, 43);
            this.chkPropertyDescriptions.Name = "chkPropertyDescriptions";
            this.chkPropertyDescriptions.Size = new System.Drawing.Size(126, 17);
            this.chkPropertyDescriptions.TabIndex = 2;
            this.chkPropertyDescriptions.Text = "Property Descriptions";
            this.chkPropertyDescriptions.UseVisualStyleBackColor = true;
            // 
            // chkPropertyValues
            // 
            this.chkPropertyValues.AutoSize = true;
            this.chkPropertyValues.Checked = true;
            this.chkPropertyValues.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkPropertyValues.Location = new System.Drawing.Point(6, 66);
            this.chkPropertyValues.Name = "chkPropertyValues";
            this.chkPropertyValues.Size = new System.Drawing.Size(100, 17);
            this.chkPropertyValues.TabIndex = 1;
            this.chkPropertyValues.Text = "Property Values";
            this.chkPropertyValues.UseVisualStyleBackColor = true;
            // 
            // btnSearch
            // 
            this.btnSearch.Location = new System.Drawing.Point(297, 211);
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.Size = new System.Drawing.Size(75, 23);
            this.btnSearch.TabIndex = 4;
            this.btnSearch.Text = "Search";
            this.btnSearch.UseVisualStyleBackColor = true;
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // cmbDescLanguage
            // 
            this.cmbDescLanguage.FormattingEnabled = true;
            this.cmbDescLanguage.Items.AddRange(new object[] {
            "English",
            "German"});
            this.cmbDescLanguage.Location = new System.Drawing.Point(126, 63);
            this.cmbDescLanguage.Name = "cmbDescLanguage";
            this.cmbDescLanguage.Size = new System.Drawing.Size(121, 21);
            this.cmbDescLanguage.TabIndex = 5;
            // 
            // lblDescLanguage
            // 
            this.lblDescLanguage.AutoSize = true;
            this.lblDescLanguage.Location = new System.Drawing.Point(9, 66);
            this.lblDescLanguage.Name = "lblDescLanguage";
            this.lblDescLanguage.Size = new System.Drawing.Size(111, 13);
            this.lblDescLanguage.TabIndex = 6;
            this.lblDescLanguage.Text = "Description Language";
            // 
            // SearchForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 246);
            this.Controls.Add(this.lblDescLanguage);
            this.Controls.Add(this.cmbDescLanguage);
            this.Controls.Add(this.btnSearch);
            this.Controls.Add(this.grpLookIn);
            this.Controls.Add(this.txtFindWhat);
            this.Controls.Add(this.lblFindWhat);
            this.HelpButton = true;
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(390, 280);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(390, 280);
            this.Name = "SearchForm";
            this.Text = "SearchForm";
            this.grpLookIn.ResumeLayout(false);
            this.grpLookIn.PerformLayout();
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
        private System.Windows.Forms.ComboBox cmbDescLanguage;
        private System.Windows.Forms.Label lblDescLanguage;
    }
}