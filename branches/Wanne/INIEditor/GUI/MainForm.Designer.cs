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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.mnuMain = new System.Windows.Forms.MenuStrip();
            this.mnuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuFileSave = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuViewDescLanguage = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuViewDescLanguageEnglish = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuViewDescLanguageGerman = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuTools = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuToolsGenerateXML = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuView = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuViewDescriptionLanguage = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuViewDescLangEnglish = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuViewDescLangGerman = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuToolsGenerateXMLFile = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuHelp = new System.Windows.Forms.ToolStripMenuItem();
            this.tbrMain = new System.Windows.Forms.ToolStrip();
            this.tbrSave = new System.Windows.Forms.ToolStripButton();
            this.stsMain = new System.Windows.Forms.StatusStrip();
            this.pnlFiles = new System.Windows.Forms.Panel();
            this.cmbFiles = new System.Windows.Forms.ComboBox();
            this.lblFiles = new System.Windows.Forms.Label();
            this.trvSections = new System.Windows.Forms.TreeView();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.splVertical = new System.Windows.Forms.Splitter();
            this.pnlSectionHeader = new System.Windows.Forms.Panel();
            this.lblSectionHeader = new System.Windows.Forms.Label();
            this.pnlSectionDescription = new System.Windows.Forms.Panel();
            this.txtSectionDescription = new System.Windows.Forms.TextBox();
            this.splHorizontal = new System.Windows.Forms.Splitter();
            this.tabActions = new System.Windows.Forms.TabControl();
            this.tpSection = new System.Windows.Forms.TabPage();
            this.dgvProperties = new System.Windows.Forms.DataGridView();
            this.colSection = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colProperty = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colDescription = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colCurrentValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colNewValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tpProperty = new System.Windows.Forms.TabPage();
            this.pnlProperty = new System.Windows.Forms.Panel();
            this.txtPropertyCurrentValue = new System.Windows.Forms.TextBox();
            this.lblPropertyCurrentValue = new System.Windows.Forms.Label();
            this.lblPropertyNewValue = new System.Windows.Forms.Label();
            this.txtPropertyInterval = new System.Windows.Forms.TextBox();
            this.lblPropertyInterval = new System.Windows.Forms.Label();
            this.txtPropertyMaxValue = new System.Windows.Forms.TextBox();
            this.txtPropertyMinValue = new System.Windows.Forms.TextBox();
            this.txtPropertyDataType = new System.Windows.Forms.TextBox();
            this.lblPropertyMaxValue = new System.Windows.Forms.Label();
            this.lblPropertyMinValue = new System.Windows.Forms.Label();
            this.lblPropertyDataType = new System.Windows.Forms.Label();
            this.splPropertyDescription = new System.Windows.Forms.Splitter();
            this.pnlPropertyDescription = new System.Windows.Forms.Panel();
            this.txtPropertyDescription = new System.Windows.Forms.TextBox();
            this.mnuMain.SuspendLayout();
            this.tbrMain.SuspendLayout();
            this.pnlFiles.SuspendLayout();
            this.pnlSectionHeader.SuspendLayout();
            this.pnlSectionDescription.SuspendLayout();
            this.tabActions.SuspendLayout();
            this.tpSection.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvProperties)).BeginInit();
            this.tpProperty.SuspendLayout();
            this.pnlProperty.SuspendLayout();
            this.pnlPropertyDescription.SuspendLayout();
            this.SuspendLayout();
            // 
            // mnuMain
            // 
            this.mnuMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuFile,
            this.viewToolStripMenuItem,
            this.mnuTools});
            this.mnuMain.Location = new System.Drawing.Point(0, 0);
            this.mnuMain.Name = "mnuMain";
            this.mnuMain.Size = new System.Drawing.Size(883, 24);
            this.mnuMain.TabIndex = 0;
            this.mnuMain.Text = "menuStrip1";
            // 
            // mnuFile
            // 
            this.mnuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuFileSave,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.mnuFile.Name = "mnuFile";
            this.mnuFile.Size = new System.Drawing.Size(35, 20);
            this.mnuFile.Text = "File";
            // 
            // mnuFileSave
            // 
            this.mnuFileSave.Image = ((System.Drawing.Image)(resources.GetObject("mnuFileSave.Image")));
            this.mnuFileSave.Name = "mnuFileSave";
            this.mnuFileSave.Size = new System.Drawing.Size(152, 22);
            this.mnuFileSave.Text = "Save";
            this.mnuFileSave.ToolTipText = "Save File";
            this.mnuFileSave.Click += new System.EventHandler(this.mnuFileSave_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(149, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuViewDescLanguage});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // mnuViewDescLanguage
            // 
            this.mnuViewDescLanguage.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuViewDescLanguageEnglish,
            this.mnuViewDescLanguageGerman});
            this.mnuViewDescLanguage.Name = "mnuViewDescLanguage";
            this.mnuViewDescLanguage.Size = new System.Drawing.Size(188, 22);
            this.mnuViewDescLanguage.Text = "Description Language";
            // 
            // mnuViewDescLanguageEnglish
            // 
            this.mnuViewDescLanguageEnglish.Name = "mnuViewDescLanguageEnglish";
            this.mnuViewDescLanguageEnglish.Size = new System.Drawing.Size(122, 22);
            this.mnuViewDescLanguageEnglish.Text = "English";
            this.mnuViewDescLanguageEnglish.Click += new System.EventHandler(this.mnuViewDescLanguageEnglish_Click);
            // 
            // mnuViewDescLanguageGerman
            // 
            this.mnuViewDescLanguageGerman.Name = "mnuViewDescLanguageGerman";
            this.mnuViewDescLanguageGerman.Size = new System.Drawing.Size(122, 22);
            this.mnuViewDescLanguageGerman.Text = "German";
            this.mnuViewDescLanguageGerman.Click += new System.EventHandler(this.mnuViewDescLanguageGerman_Click);
            // 
            // mnuTools
            // 
            this.mnuTools.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuToolsGenerateXML});
            this.mnuTools.Name = "mnuTools";
            this.mnuTools.Size = new System.Drawing.Size(44, 20);
            this.mnuTools.Text = "Tools";
            // 
            // mnuToolsGenerateXML
            // 
            this.mnuToolsGenerateXML.Name = "mnuToolsGenerateXML";
            this.mnuToolsGenerateXML.Size = new System.Drawing.Size(215, 22);
            this.mnuToolsGenerateXML.Text = "Generate XML File [ADMIN]";
            this.mnuToolsGenerateXML.Click += new System.EventHandler(this.mnuToolsGenerateXML_Click);
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
            // mnuView
            // 
            this.mnuView.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuViewDescriptionLanguage});
            this.mnuView.Name = "mnuView";
            this.mnuView.Size = new System.Drawing.Size(41, 20);
            this.mnuView.Text = "View";
            // 
            // mnuViewDescriptionLanguage
            // 
            this.mnuViewDescriptionLanguage.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuViewDescLangEnglish,
            this.mnuViewDescLangGerman});
            this.mnuViewDescriptionLanguage.Name = "mnuViewDescriptionLanguage";
            this.mnuViewDescriptionLanguage.Size = new System.Drawing.Size(188, 22);
            this.mnuViewDescriptionLanguage.Text = "Description Language";
            // 
            // mnuViewDescLangEnglish
            // 
            this.mnuViewDescLangEnglish.Name = "mnuViewDescLangEnglish";
            this.mnuViewDescLangEnglish.Size = new System.Drawing.Size(78, 22);
            // 
            // mnuViewDescLangGerman
            // 
            this.mnuViewDescLangGerman.Name = "mnuViewDescLangGerman";
            this.mnuViewDescLangGerman.Size = new System.Drawing.Size(78, 22);
            // 
            // mnuToolsGenerateXMLFile
            // 
            this.mnuToolsGenerateXMLFile.Name = "mnuToolsGenerateXMLFile";
            this.mnuToolsGenerateXMLFile.Size = new System.Drawing.Size(32, 19);
            // 
            // mnuHelp
            // 
            this.mnuHelp.Name = "mnuHelp";
            this.mnuHelp.Size = new System.Drawing.Size(40, 20);
            this.mnuHelp.Text = "Help";
            // 
            // tbrMain
            // 
            this.tbrMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tbrSave});
            this.tbrMain.Location = new System.Drawing.Point(0, 24);
            this.tbrMain.Name = "tbrMain";
            this.tbrMain.Size = new System.Drawing.Size(883, 25);
            this.tbrMain.TabIndex = 1;
            this.tbrMain.Text = "toolStrip1";
            // 
            // tbrSave
            // 
            this.tbrSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tbrSave.Image = ((System.Drawing.Image)(resources.GetObject("tbrSave.Image")));
            this.tbrSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tbrSave.Name = "tbrSave";
            this.tbrSave.Size = new System.Drawing.Size(23, 22);
            this.tbrSave.Text = "Save";
            this.tbrSave.ToolTipText = "Save File";
            this.tbrSave.Click += new System.EventHandler(this.tbrSave_Click);
            // 
            // stsMain
            // 
            this.stsMain.Location = new System.Drawing.Point(0, 631);
            this.stsMain.Name = "stsMain";
            this.stsMain.Size = new System.Drawing.Size(883, 22);
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
            this.pnlFiles.Size = new System.Drawing.Size(883, 42);
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
            this.trvSections.ImageIndex = 0;
            this.trvSections.ImageList = this.imageList;
            this.trvSections.Location = new System.Drawing.Point(0, 91);
            this.trvSections.Name = "trvSections";
            this.trvSections.SelectedImageKey = "INIFile.ico";
            this.trvSections.Size = new System.Drawing.Size(341, 540);
            this.trvSections.TabIndex = 9;
            this.trvSections.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.trvSections_AfterSelect);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList.Images.SetKeyName(0, "INIFile.ico");
            this.imageList.Images.SetKeyName(1, "Section.ico");
            this.imageList.Images.SetKeyName(2, "SectionSelected.ico");
            this.imageList.Images.SetKeyName(3, "Property.ico");
            this.imageList.Images.SetKeyName(4, "PropertySelected.ico");
            this.imageList.Images.SetKeyName(5, "Generate.ico");
            this.imageList.Images.SetKeyName(6, "Save.ico");
            this.imageList.Images.SetKeyName(7, "Search.ico");
            // 
            // splVertical
            // 
            this.splVertical.BackColor = System.Drawing.Color.Silver;
            this.splVertical.Location = new System.Drawing.Point(341, 91);
            this.splVertical.Name = "splVertical";
            this.splVertical.Size = new System.Drawing.Size(3, 540);
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
            this.pnlSectionHeader.Size = new System.Drawing.Size(539, 39);
            this.pnlSectionHeader.TabIndex = 12;
            // 
            // lblSectionHeader
            // 
            this.lblSectionHeader.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lblSectionHeader.AutoSize = true;
            this.lblSectionHeader.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSectionHeader.Location = new System.Drawing.Point(3, 11);
            this.lblSectionHeader.Name = "lblSectionHeader";
            this.lblSectionHeader.Size = new System.Drawing.Size(0, 18);
            this.lblSectionHeader.TabIndex = 0;
            // 
            // pnlSectionDescription
            // 
            this.pnlSectionDescription.Controls.Add(this.txtSectionDescription);
            this.pnlSectionDescription.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlSectionDescription.Location = new System.Drawing.Point(344, 130);
            this.pnlSectionDescription.Name = "pnlSectionDescription";
            this.pnlSectionDescription.Size = new System.Drawing.Size(539, 112);
            this.pnlSectionDescription.TabIndex = 15;
            // 
            // txtSectionDescription
            // 
            this.txtSectionDescription.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtSectionDescription.Location = new System.Drawing.Point(4, 6);
            this.txtSectionDescription.Multiline = true;
            this.txtSectionDescription.Name = "txtSectionDescription";
            this.txtSectionDescription.Size = new System.Drawing.Size(531, 100);
            this.txtSectionDescription.TabIndex = 0;
            // 
            // splHorizontal
            // 
            this.splHorizontal.BackColor = System.Drawing.Color.Silver;
            this.splHorizontal.Dock = System.Windows.Forms.DockStyle.Top;
            this.splHorizontal.Location = new System.Drawing.Point(344, 242);
            this.splHorizontal.Name = "splHorizontal";
            this.splHorizontal.Size = new System.Drawing.Size(539, 3);
            this.splHorizontal.TabIndex = 16;
            this.splHorizontal.TabStop = false;
            // 
            // tabActions
            // 
            this.tabActions.Controls.Add(this.tpSection);
            this.tabActions.Controls.Add(this.tpProperty);
            this.tabActions.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabActions.Location = new System.Drawing.Point(344, 245);
            this.tabActions.Name = "tabActions";
            this.tabActions.SelectedIndex = 0;
            this.tabActions.Size = new System.Drawing.Size(539, 386);
            this.tabActions.TabIndex = 17;
            // 
            // tpSection
            // 
            this.tpSection.Controls.Add(this.dgvProperties);
            this.tpSection.Location = new System.Drawing.Point(4, 22);
            this.tpSection.Name = "tpSection";
            this.tpSection.Size = new System.Drawing.Size(531, 360);
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
            this.colDescription,
            this.colCurrentValue,
            this.colNewValue});
            this.dgvProperties.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvProperties.Location = new System.Drawing.Point(0, 0);
            this.dgvProperties.MultiSelect = false;
            this.dgvProperties.Name = "dgvProperties";
            this.dgvProperties.ReadOnly = true;
            this.dgvProperties.RowHeadersVisible = false;
            this.dgvProperties.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvProperties.Size = new System.Drawing.Size(531, 360);
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
            // colDescription
            // 
            this.colDescription.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colDescription.HeaderText = "Description";
            this.colDescription.Name = "colDescription";
            this.colDescription.ReadOnly = true;
            // 
            // colCurrentValue
            // 
            this.colCurrentValue.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.colCurrentValue.HeaderText = "Current Value";
            this.colCurrentValue.Name = "colCurrentValue";
            this.colCurrentValue.ReadOnly = true;
            this.colCurrentValue.Width = 94;
            // 
            // colNewValue
            // 
            this.colNewValue.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.colNewValue.HeaderText = "New Value";
            this.colNewValue.Name = "colNewValue";
            this.colNewValue.ReadOnly = true;
            this.colNewValue.Width = 82;
            // 
            // tpProperty
            // 
            this.tpProperty.Controls.Add(this.pnlProperty);
            this.tpProperty.Controls.Add(this.splPropertyDescription);
            this.tpProperty.Controls.Add(this.pnlPropertyDescription);
            this.tpProperty.Location = new System.Drawing.Point(4, 22);
            this.tpProperty.Name = "tpProperty";
            this.tpProperty.Padding = new System.Windows.Forms.Padding(3);
            this.tpProperty.Size = new System.Drawing.Size(531, 360);
            this.tpProperty.TabIndex = 1;
            this.tpProperty.UseVisualStyleBackColor = true;
            // 
            // pnlProperty
            // 
            this.pnlProperty.Controls.Add(this.txtPropertyCurrentValue);
            this.pnlProperty.Controls.Add(this.lblPropertyCurrentValue);
            this.pnlProperty.Controls.Add(this.lblPropertyNewValue);
            this.pnlProperty.Controls.Add(this.txtPropertyInterval);
            this.pnlProperty.Controls.Add(this.lblPropertyInterval);
            this.pnlProperty.Controls.Add(this.txtPropertyMaxValue);
            this.pnlProperty.Controls.Add(this.txtPropertyMinValue);
            this.pnlProperty.Controls.Add(this.txtPropertyDataType);
            this.pnlProperty.Controls.Add(this.lblPropertyMaxValue);
            this.pnlProperty.Controls.Add(this.lblPropertyMinValue);
            this.pnlProperty.Controls.Add(this.lblPropertyDataType);
            this.pnlProperty.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlProperty.Location = new System.Drawing.Point(3, 106);
            this.pnlProperty.Name = "pnlProperty";
            this.pnlProperty.Size = new System.Drawing.Size(525, 251);
            this.pnlProperty.TabIndex = 2;
            // 
            // txtPropertyCurrentValue
            // 
            this.txtPropertyCurrentValue.BackColor = System.Drawing.Color.LemonChiffon;
            this.txtPropertyCurrentValue.Enabled = false;
            this.txtPropertyCurrentValue.Location = new System.Drawing.Point(130, 145);
            this.txtPropertyCurrentValue.Name = "txtPropertyCurrentValue";
            this.txtPropertyCurrentValue.Size = new System.Drawing.Size(161, 20);
            this.txtPropertyCurrentValue.TabIndex = 10;
            // 
            // lblPropertyCurrentValue
            // 
            this.lblPropertyCurrentValue.AutoSize = true;
            this.lblPropertyCurrentValue.Location = new System.Drawing.Point(4, 148);
            this.lblPropertyCurrentValue.Name = "lblPropertyCurrentValue";
            this.lblPropertyCurrentValue.Size = new System.Drawing.Size(74, 13);
            this.lblPropertyCurrentValue.TabIndex = 9;
            this.lblPropertyCurrentValue.Text = "Current Value:";
            // 
            // lblPropertyNewValue
            // 
            this.lblPropertyNewValue.AutoSize = true;
            this.lblPropertyNewValue.Location = new System.Drawing.Point(4, 183);
            this.lblPropertyNewValue.Name = "lblPropertyNewValue";
            this.lblPropertyNewValue.Size = new System.Drawing.Size(62, 13);
            this.lblPropertyNewValue.TabIndex = 8;
            this.lblPropertyNewValue.Text = "New Value:";
            // 
            // txtPropertyInterval
            // 
            this.txtPropertyInterval.BackColor = System.Drawing.Color.LemonChiffon;
            this.txtPropertyInterval.Enabled = false;
            this.txtPropertyInterval.Location = new System.Drawing.Point(130, 112);
            this.txtPropertyInterval.Name = "txtPropertyInterval";
            this.txtPropertyInterval.Size = new System.Drawing.Size(161, 20);
            this.txtPropertyInterval.TabIndex = 7;
            // 
            // lblPropertyInterval
            // 
            this.lblPropertyInterval.AutoSize = true;
            this.lblPropertyInterval.Location = new System.Drawing.Point(4, 115);
            this.lblPropertyInterval.Name = "lblPropertyInterval";
            this.lblPropertyInterval.Size = new System.Drawing.Size(48, 13);
            this.lblPropertyInterval.TabIndex = 6;
            this.lblPropertyInterval.Text = "Inverval:";
            // 
            // txtPropertyMaxValue
            // 
            this.txtPropertyMaxValue.BackColor = System.Drawing.Color.LemonChiffon;
            this.txtPropertyMaxValue.Enabled = false;
            this.txtPropertyMaxValue.Location = new System.Drawing.Point(130, 78);
            this.txtPropertyMaxValue.Name = "txtPropertyMaxValue";
            this.txtPropertyMaxValue.Size = new System.Drawing.Size(161, 20);
            this.txtPropertyMaxValue.TabIndex = 5;
            // 
            // txtPropertyMinValue
            // 
            this.txtPropertyMinValue.BackColor = System.Drawing.Color.LemonChiffon;
            this.txtPropertyMinValue.Enabled = false;
            this.txtPropertyMinValue.Location = new System.Drawing.Point(130, 42);
            this.txtPropertyMinValue.Name = "txtPropertyMinValue";
            this.txtPropertyMinValue.Size = new System.Drawing.Size(161, 20);
            this.txtPropertyMinValue.TabIndex = 4;
            // 
            // txtPropertyDataType
            // 
            this.txtPropertyDataType.BackColor = System.Drawing.Color.LemonChiffon;
            this.txtPropertyDataType.Enabled = false;
            this.txtPropertyDataType.Location = new System.Drawing.Point(130, 7);
            this.txtPropertyDataType.Name = "txtPropertyDataType";
            this.txtPropertyDataType.Size = new System.Drawing.Size(161, 20);
            this.txtPropertyDataType.TabIndex = 3;
            // 
            // lblPropertyMaxValue
            // 
            this.lblPropertyMaxValue.AutoSize = true;
            this.lblPropertyMaxValue.Location = new System.Drawing.Point(3, 81);
            this.lblPropertyMaxValue.Name = "lblPropertyMaxValue";
            this.lblPropertyMaxValue.Size = new System.Drawing.Size(84, 13);
            this.lblPropertyMaxValue.TabIndex = 2;
            this.lblPropertyMaxValue.Text = "Maximum Value:";
            // 
            // lblPropertyMinValue
            // 
            this.lblPropertyMinValue.AutoSize = true;
            this.lblPropertyMinValue.Location = new System.Drawing.Point(4, 45);
            this.lblPropertyMinValue.Name = "lblPropertyMinValue";
            this.lblPropertyMinValue.Size = new System.Drawing.Size(81, 13);
            this.lblPropertyMinValue.TabIndex = 1;
            this.lblPropertyMinValue.Text = "Minimum Value:";
            // 
            // lblPropertyDataType
            // 
            this.lblPropertyDataType.AutoSize = true;
            this.lblPropertyDataType.Location = new System.Drawing.Point(4, 10);
            this.lblPropertyDataType.Name = "lblPropertyDataType";
            this.lblPropertyDataType.Size = new System.Drawing.Size(60, 13);
            this.lblPropertyDataType.TabIndex = 0;
            this.lblPropertyDataType.Text = "Data Type:";
            // 
            // splPropertyDescription
            // 
            this.splPropertyDescription.BackColor = System.Drawing.Color.Silver;
            this.splPropertyDescription.Dock = System.Windows.Forms.DockStyle.Top;
            this.splPropertyDescription.Location = new System.Drawing.Point(3, 103);
            this.splPropertyDescription.Name = "splPropertyDescription";
            this.splPropertyDescription.Size = new System.Drawing.Size(525, 3);
            this.splPropertyDescription.TabIndex = 1;
            this.splPropertyDescription.TabStop = false;
            // 
            // pnlPropertyDescription
            // 
            this.pnlPropertyDescription.Controls.Add(this.txtPropertyDescription);
            this.pnlPropertyDescription.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlPropertyDescription.Location = new System.Drawing.Point(3, 3);
            this.pnlPropertyDescription.Name = "pnlPropertyDescription";
            this.pnlPropertyDescription.Size = new System.Drawing.Size(525, 100);
            this.pnlPropertyDescription.TabIndex = 0;
            // 
            // txtPropertyDescription
            // 
            this.txtPropertyDescription.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtPropertyDescription.Location = new System.Drawing.Point(3, 3);
            this.txtPropertyDescription.Multiline = true;
            this.txtPropertyDescription.Name = "txtPropertyDescription";
            this.txtPropertyDescription.Size = new System.Drawing.Size(517, 91);
            this.txtPropertyDescription.TabIndex = 0;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(883, 653);
            this.Controls.Add(this.tabActions);
            this.Controls.Add(this.splHorizontal);
            this.Controls.Add(this.pnlSectionDescription);
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
            this.tbrMain.ResumeLayout(false);
            this.tbrMain.PerformLayout();
            this.pnlFiles.ResumeLayout(false);
            this.pnlFiles.PerformLayout();
            this.pnlSectionHeader.ResumeLayout(false);
            this.pnlSectionHeader.PerformLayout();
            this.pnlSectionDescription.ResumeLayout(false);
            this.pnlSectionDescription.PerformLayout();
            this.tabActions.ResumeLayout(false);
            this.tpSection.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvProperties)).EndInit();
            this.tpProperty.ResumeLayout(false);
            this.pnlProperty.ResumeLayout(false);
            this.pnlProperty.PerformLayout();
            this.pnlPropertyDescription.ResumeLayout(false);
            this.pnlPropertyDescription.PerformLayout();
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
        private System.Windows.Forms.ToolStripMenuItem mnuEdit;
        private System.Windows.Forms.ToolStripMenuItem mnuHelp;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem mnuToolsGenerateXMLFile;
        private System.Windows.Forms.Panel pnlSectionDescription;
        private System.Windows.Forms.Splitter splHorizontal;
        private System.Windows.Forms.TabControl tabActions;
        private System.Windows.Forms.TabPage tpSection;
        private System.Windows.Forms.DataGridView dgvProperties;
        private System.Windows.Forms.TabPage tpProperty;
        private System.Windows.Forms.TextBox txtSectionDescription;
        private System.Windows.Forms.ToolStripMenuItem mnuView;
        private System.Windows.Forms.ToolStripMenuItem mnuViewDescriptionLanguage;
        private System.Windows.Forms.ToolStripMenuItem mnuViewDescLangEnglish;
        private System.Windows.Forms.ToolStripMenuItem mnuViewDescLangGerman;
        private System.Windows.Forms.Splitter splPropertyDescription;
        private System.Windows.Forms.Panel pnlPropertyDescription;
        private System.Windows.Forms.TextBox txtPropertyDescription;
        private System.Windows.Forms.Panel pnlProperty;
        private System.Windows.Forms.TextBox txtPropertyMaxValue;
        private System.Windows.Forms.TextBox txtPropertyMinValue;
        private System.Windows.Forms.TextBox txtPropertyDataType;
        private System.Windows.Forms.Label lblPropertyMaxValue;
        private System.Windows.Forms.Label lblPropertyMinValue;
        private System.Windows.Forms.Label lblPropertyDataType;
        private System.Windows.Forms.Label lblPropertyNewValue;
        private System.Windows.Forms.TextBox txtPropertyInterval;
        private System.Windows.Forms.Label lblPropertyInterval;
        private System.Windows.Forms.TextBox txtPropertyCurrentValue;
        private System.Windows.Forms.Label lblPropertyCurrentValue;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.ToolStripMenuItem mnuFile;
        private System.Windows.Forms.ToolStripMenuItem mnuFileSave;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem mnuTools;
        private System.Windows.Forms.ToolStripMenuItem mnuViewDescLanguage;
        private System.Windows.Forms.ToolStripMenuItem mnuViewDescLanguageEnglish;
        private System.Windows.Forms.ToolStripMenuItem mnuViewDescLanguageGerman;
        private System.Windows.Forms.ToolStripMenuItem mnuToolsGenerateXML;
        private System.Windows.Forms.DataGridViewTextBoxColumn colSection;
        private System.Windows.Forms.DataGridViewTextBoxColumn colProperty;
        private System.Windows.Forms.DataGridViewTextBoxColumn colDescription;
        private System.Windows.Forms.DataGridViewTextBoxColumn colCurrentValue;
        private System.Windows.Forms.DataGridViewTextBoxColumn colNewValue;
        private System.Windows.Forms.ToolStripButton tbrSave;
    }
}

