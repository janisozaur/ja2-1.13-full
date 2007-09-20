namespace INIEditor.GUI
{
    partial class MainForm
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
            this.mnuMain = new System.Windows.Forms.MenuStrip();
            this.mnuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuTools = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuToolsGenerateXMLFile = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuHelp = new System.Windows.Forms.ToolStripMenuItem();
            this.tbrMain = new System.Windows.Forms.ToolStrip();
            this.stsMain = new System.Windows.Forms.StatusStrip();
            this.pnlFiles = new System.Windows.Forms.Panel();
            this.cmbFiles = new System.Windows.Forms.ComboBox();
            this.lblFiles = new System.Windows.Forms.Label();
            this.trvSections = new System.Windows.Forms.TreeView();
            this.splVertical = new System.Windows.Forms.Splitter();
            this.pnlSectionHeader = new System.Windows.Forms.Panel();
            this.lblSectionHeader = new System.Windows.Forms.Label();
            this.tabActions = new System.Windows.Forms.TabControl();
            this.tpSection = new System.Windows.Forms.TabPage();
            this.dgvProperties = new System.Windows.Forms.DataGridView();
            this.colSection = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colProperty = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colDescription = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tpProperty = new System.Windows.Forms.TabPage();
            this.mnuMain.SuspendLayout();
            this.pnlFiles.SuspendLayout();
            this.pnlSectionHeader.SuspendLayout();
            this.tabActions.SuspendLayout();
            this.tpSection.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvProperties)).BeginInit();
            this.SuspendLayout();
            // 
            // mnuMain
            // 
            this.mnuMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuFile,
            this.mnuEdit,
            this.mnuTools,
            this.mnuHelp});
            this.mnuMain.Location = new System.Drawing.Point(0, 0);
            this.mnuMain.Name = "mnuMain";
            this.mnuMain.Size = new System.Drawing.Size(798, 24);
            this.mnuMain.TabIndex = 0;
            this.mnuMain.Text = "menuStrip1";
            // 
            // mnuFile
            // 
            this.mnuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToolStripMenuItem});
            this.mnuFile.Name = "mnuFile";
            this.mnuFile.Size = new System.Drawing.Size(35, 20);
            this.mnuFile.Text = "File";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(109, 22);
            this.saveToolStripMenuItem.Text = "Save";
            // 
            // mnuEdit
            // 
            this.mnuEdit.Name = "mnuEdit";
            this.mnuEdit.Size = new System.Drawing.Size(37, 20);
            this.mnuEdit.Text = "Edit";
            // 
            // mnuTools
            // 
            this.mnuTools.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuToolsGenerateXMLFile});
            this.mnuTools.Name = "mnuTools";
            this.mnuTools.Size = new System.Drawing.Size(44, 20);
            this.mnuTools.Text = "Tools";
            // 
            // mnuToolsGenerateXMLFile
            // 
            this.mnuToolsGenerateXMLFile.Name = "mnuToolsGenerateXMLFile";
            this.mnuToolsGenerateXMLFile.Size = new System.Drawing.Size(257, 22);
            this.mnuToolsGenerateXMLFile.Text = "Generate XML Init File (ADMIN only)";
            this.mnuToolsGenerateXMLFile.Click += new System.EventHandler(this.mnuToolsGenerateXMLFile_Click);
            // 
            // mnuHelp
            // 
            this.mnuHelp.Name = "mnuHelp";
            this.mnuHelp.Size = new System.Drawing.Size(40, 20);
            this.mnuHelp.Text = "Help";
            // 
            // tbrMain
            // 
            this.tbrMain.Location = new System.Drawing.Point(0, 24);
            this.tbrMain.Name = "tbrMain";
            this.tbrMain.Size = new System.Drawing.Size(798, 25);
            this.tbrMain.TabIndex = 1;
            this.tbrMain.Text = "toolStrip1";
            // 
            // stsMain
            // 
            this.stsMain.Location = new System.Drawing.Point(0, 507);
            this.stsMain.Name = "stsMain";
            this.stsMain.Size = new System.Drawing.Size(798, 22);
            this.stsMain.TabIndex = 5;
            // 
            // pnlFiles
            // 
            this.pnlFiles.Controls.Add(this.cmbFiles);
            this.pnlFiles.Controls.Add(this.lblFiles);
            this.pnlFiles.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlFiles.Location = new System.Drawing.Point(0, 49);
            this.pnlFiles.Name = "pnlFiles";
            this.pnlFiles.Padding = new System.Windows.Forms.Padding(0, 0, 2, 0);
            this.pnlFiles.Size = new System.Drawing.Size(798, 42);
            this.pnlFiles.TabIndex = 8;
            // 
            // cmbFiles
            // 
            this.cmbFiles.FormattingEnabled = true;
            this.cmbFiles.Location = new System.Drawing.Point(111, 12);
            this.cmbFiles.Name = "cmbFiles";
            this.cmbFiles.Size = new System.Drawing.Size(288, 21);
            this.cmbFiles.TabIndex = 1;
            this.cmbFiles.SelectedIndexChanged += new System.EventHandler(this.cmbFiles_SelectedIndexChanged);
            // 
            // lblFiles
            // 
            this.lblFiles.AutoSize = true;
            this.lblFiles.Location = new System.Drawing.Point(12, 15);
            this.lblFiles.Name = "lblFiles";
            this.lblFiles.Size = new System.Drawing.Size(93, 13);
            this.lblFiles.TabIndex = 0;
            this.lblFiles.Text = "JA2 1.13 INI Files:";
            // 
            // trvSections
            // 
            this.trvSections.Dock = System.Windows.Forms.DockStyle.Left;
            this.trvSections.Location = new System.Drawing.Point(0, 91);
            this.trvSections.Name = "trvSections";
            this.trvSections.Size = new System.Drawing.Size(341, 416);
            this.trvSections.TabIndex = 9;
            this.trvSections.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.trvSections_AfterSelect);
            // 
            // splVertical
            // 
            this.splVertical.Location = new System.Drawing.Point(341, 91);
            this.splVertical.Name = "splVertical";
            this.splVertical.Size = new System.Drawing.Size(3, 416);
            this.splVertical.TabIndex = 10;
            this.splVertical.TabStop = false;
            // 
            // pnlSectionHeader
            // 
            this.pnlSectionHeader.BackColor = System.Drawing.Color.Beige;
            this.pnlSectionHeader.Controls.Add(this.lblSectionHeader);
            this.pnlSectionHeader.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlSectionHeader.Location = new System.Drawing.Point(344, 91);
            this.pnlSectionHeader.Name = "pnlSectionHeader";
            this.pnlSectionHeader.Padding = new System.Windows.Forms.Padding(0, 0, 3, 0);
            this.pnlSectionHeader.Size = new System.Drawing.Size(454, 51);
            this.pnlSectionHeader.TabIndex = 12;
            // 
            // lblSectionHeader
            // 
            this.lblSectionHeader.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lblSectionHeader.AutoSize = true;
            this.lblSectionHeader.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSectionHeader.Location = new System.Drawing.Point(3, 13);
            this.lblSectionHeader.Name = "lblSectionHeader";
            this.lblSectionHeader.Size = new System.Drawing.Size(0, 18);
            this.lblSectionHeader.TabIndex = 0;
            // 
            // tabActions
            // 
            this.tabActions.Controls.Add(this.tpSection);
            this.tabActions.Controls.Add(this.tpProperty);
            this.tabActions.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabActions.Location = new System.Drawing.Point(344, 142);
            this.tabActions.Name = "tabActions";
            this.tabActions.SelectedIndex = 0;
            this.tabActions.Size = new System.Drawing.Size(454, 365);
            this.tabActions.TabIndex = 14;
            // 
            // tpSection
            // 
            this.tpSection.Controls.Add(this.dgvProperties);
            this.tpSection.Location = new System.Drawing.Point(4, 22);
            this.tpSection.Name = "tpSection";
            this.tpSection.Size = new System.Drawing.Size(446, 339);
            this.tpSection.TabIndex = 0;
            this.tpSection.UseVisualStyleBackColor = true;
            // 
            // dgvProperties
            // 
            this.dgvProperties.AllowDrop = true;
            this.dgvProperties.AllowUserToAddRows = false;
            this.dgvProperties.AllowUserToDeleteRows = false;
            this.dgvProperties.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvProperties.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colSection,
            this.colProperty,
            this.colValue,
            this.colDescription});
            this.dgvProperties.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvProperties.Location = new System.Drawing.Point(0, 0);
            this.dgvProperties.MultiSelect = false;
            this.dgvProperties.Name = "dgvProperties";
            this.dgvProperties.ReadOnly = true;
            this.dgvProperties.RowHeadersVisible = false;
            this.dgvProperties.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvProperties.Size = new System.Drawing.Size(446, 339);
            this.dgvProperties.TabIndex = 0;
            this.dgvProperties.CellDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgvProperties_CellDoubleClick);
            // 
            // colSection
            // 
            this.colSection.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.colSection.HeaderText = "Section";
            this.colSection.Name = "colSection";
            this.colSection.ReadOnly = true;
            this.colSection.Width = 66;
            // 
            // colProperty
            // 
            this.colProperty.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.colProperty.HeaderText = "Property";
            this.colProperty.Name = "colProperty";
            this.colProperty.ReadOnly = true;
            this.colProperty.Width = 69;
            // 
            // colValue
            // 
            this.colValue.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.colValue.HeaderText = "Value";
            this.colValue.Name = "colValue";
            this.colValue.ReadOnly = true;
            this.colValue.Width = 57;
            // 
            // colDescription
            // 
            this.colDescription.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colDescription.HeaderText = "Description";
            this.colDescription.Name = "colDescription";
            this.colDescription.ReadOnly = true;
            // 
            // tpProperty
            // 
            this.tpProperty.Location = new System.Drawing.Point(4, 22);
            this.tpProperty.Name = "tpProperty";
            this.tpProperty.Padding = new System.Windows.Forms.Padding(3);
            this.tpProperty.Size = new System.Drawing.Size(446, 339);
            this.tpProperty.TabIndex = 1;
            this.tpProperty.UseVisualStyleBackColor = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(798, 529);
            this.Controls.Add(this.tabActions);
            this.Controls.Add(this.pnlSectionHeader);
            this.Controls.Add(this.splVertical);
            this.Controls.Add(this.trvSections);
            this.Controls.Add(this.pnlFiles);
            this.Controls.Add(this.stsMain);
            this.Controls.Add(this.tbrMain);
            this.Controls.Add(this.mnuMain);
            this.MainMenuStrip = this.mnuMain;
            this.Name = "MainForm";
            this.Text = "INI Editor";
            this.mnuMain.ResumeLayout(false);
            this.mnuMain.PerformLayout();
            this.pnlFiles.ResumeLayout(false);
            this.pnlFiles.PerformLayout();
            this.pnlSectionHeader.ResumeLayout(false);
            this.pnlSectionHeader.PerformLayout();
            this.tabActions.ResumeLayout(false);
            this.tpSection.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvProperties)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip mnuMain;
        private System.Windows.Forms.ToolStrip tbrMain;
        private System.Windows.Forms.StatusStrip stsMain;
        private System.Windows.Forms.Panel pnlFiles;
        private System.Windows.Forms.TreeView trvSections;
        private System.Windows.Forms.Splitter splVertical;
        private System.Windows.Forms.ComboBox cmbFiles;
        private System.Windows.Forms.Label lblFiles;
        private System.Windows.Forms.Panel pnlSectionHeader;
        private System.Windows.Forms.Label lblSectionHeader;
        private System.Windows.Forms.ToolStripMenuItem mnuFile;
        private System.Windows.Forms.ToolStripMenuItem mnuEdit;
        private System.Windows.Forms.ToolStripMenuItem mnuHelp;
        private System.Windows.Forms.TabControl tabActions;
        private System.Windows.Forms.TabPage tpSection;
        private System.Windows.Forms.DataGridView dgvProperties;
        private System.Windows.Forms.TabPage tpProperty;
        private System.Windows.Forms.DataGridViewTextBoxColumn colSection;
        private System.Windows.Forms.DataGridViewTextBoxColumn colProperty;
        private System.Windows.Forms.DataGridViewTextBoxColumn colValue;
        private System.Windows.Forms.DataGridViewTextBoxColumn colDescription;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem mnuTools;
        private System.Windows.Forms.ToolStripMenuItem mnuToolsGenerateXMLFile;
    }
}

