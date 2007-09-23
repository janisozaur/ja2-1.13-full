using System;
using System.Collections;
using System.IO;
using System.Windows.Forms;
using INIEditor.BackEnd;
using INIEditor.BackEnd.INIHelper;
using INIEditor.BackEnd.XML;

namespace INIEditor.GUI
{
    public partial class MainForm : Form
    {
        #region Fields
        private INIFile _iniFile = null;
        private Hashtable _iniSettingsList = null;
        private Enumerations.Language _descriptionLanguage = Enumerations.Language.English;
        private Enumerations.Permission _permission = Enumerations.Permission.Admin;    // TODO: Change to "User" in Release version!
        private Control _ctlPropertyNewValue = new Control();
        #endregion
        #region CTOR
        public MainForm()
        {
            InitializeComponent();
            InitializeMenu();
            InitializeFiles();
            InitializeXMLSettingsFile();

            // Select the first INI - File in the combo box
            if (cmbFiles.Items.Count > 0)
            {
                cmbFiles.SelectedIndex = 0;
            }

        }
        #endregion
        #region Initialize
        private void InitializeMenu()
        {
            // Disable some menu entries that should not be visible for the user
            if (_permission == Enumerations.Permission.User)
            {
                mnuToolsGenerateXMLFile.Visible = false;
            }
        }

        private void InitializeXMLSettingsFile()
        {
            string path = Path.Combine(Constants.XML_SETTINGS_PATH, Constants.INI_EDITOR_INIT_FILE);
            Settings xmlIniSettings = Helper.LoadObjectFromXMLFile(path);

            // ------------
            // Store the sections in an hashtable
            // ------------
            // Root (INI File)
            _iniSettingsList = new Hashtable();
            _iniSettingsList.Add(Constants.INI_FILE, xmlIniSettings);

            // Sections
            foreach (Section xmlSection in xmlIniSettings.Sections)
            {
                _iniSettingsList.Add(xmlSection.Name, xmlSection);
            }
        }

        private void InitializeFiles()
        {
            DirectoryInfo ja2DirInfo = new DirectoryInfo(Constants.JA2_PATH);

            DirectoryInfo[] dataDirs = ja2DirInfo.GetDirectories();

            foreach (DirectoryInfo di in dataDirs)
            {
                // Only list the directory, if they include the "ja2_options.ini"
                FileInfo[] files = di.GetFiles(Constants.INI_FILE, SearchOption.TopDirectoryOnly);

                if (files.Length > 0)
                {
                    string iniFile = Path.Combine(di.Name, files[0].Name);
                    cmbFiles.Items.Add(iniFile);
                }
            }
        }

        private void InitializeSectionTree()
        {
            trvSections.Nodes.Clear();

            _iniFile = new INIFile();
            string iniFilePath = Path.Combine(Constants.JA2_PATH, cmbFiles.SelectedItem.ToString());
            _iniFile.ReadFile(iniFilePath);

            _iniFile.XMLSettings = GetXMLIniFile();
             
            TreeNode iniFileNode = new TreeNode();
            iniFileNode.Name = cmbFiles.SelectedItem.ToString();
            iniFileNode.Text = cmbFiles.SelectedItem.ToString();
            iniFileNode.Tag = _iniFile;
            trvSections.Nodes.Add(iniFileNode);

            // The ini file has a reference to the tree node
            _iniFile.Tag = iniFileNode;

            // Loop through all the sections of the ini file
            foreach (INISection section in _iniFile.Sections)
            {
                section.XMLSection = GetXMLSection(section.Name);

                TreeNode sectionNode = new TreeNode();
                sectionNode.Name = section.Name;
                sectionNode.Text = section.Name;
                sectionNode.Tag = section;
                iniFileNode.Nodes.Add(sectionNode);

                // The section has a reference to the tree node
                section.Tag = sectionNode;

                // Loop through all the properties of the current selection
                foreach (INIProperty property in section.Properties)
                {
                    property.XMLProperty = GetXMLProperty(section.Name, property.Name);

                    TreeNode propertyNode = new TreeNode();
                    propertyNode.Name = property.Name;
                    propertyNode.Text = property.Name;
                    propertyNode.Tag = property;
                    sectionNode.Nodes.Add(propertyNode);   

                    // The property has a reference to the tree node
                    property.Tag = propertyNode;
                }
            }
        }

        private void InitializeTabs<T>(T item, string sectionHeader)
        {
            dgvProperties.Rows.Clear();

            // INI file item
            if (item is INIFile)
            {
                if (tabActions.TabPages.Contains(tpProperty))
                {
                    tabActions.TabPages.Remove(tpProperty);
                }
                tpSection.Text = Constants.TP_SECTIONS_INI_FILE;
                if (!tabActions.TabPages.Contains(tpSection))
                {
                    tabActions.TabPages.Add(tpSection);
                }

                lblSectionHeader.Text = sectionHeader;
                
                colSection.Visible = true;

                INIFile file = item as INIFile;
                //txtSectionDescription.Text = file.XMLSettings.Description_ENG;
                txtSectionDescription.Text = GetDescription(file.XMLSettings);

                foreach (INISection section in file.Sections)
                {
                    foreach (INIProperty property in section.Properties)
                    {
                        int rowIndex = dgvProperties.Rows.Add();

                        dgvProperties[colSection.Index, rowIndex].Value = section.Name;
                        dgvProperties[colProperty.Index, rowIndex].Value = property.Name;
                        dgvProperties[colValue.Index, rowIndex].Value = property.Value;

                        dgvProperties[colDescription.Index, rowIndex].Value = GetDescription(property.XMLProperty);

                        dgvProperties[colSection.Index, rowIndex].Tag = section;
                        dgvProperties[colProperty.Index, rowIndex].Tag = property;
                    }
                }

            }
            // Section item
            else if (item is INISection)
            {
                if (tabActions.TabPages.Contains(tpProperty))
                {
                    tabActions.TabPages.Remove(tpProperty);
                }
                tpSection.Text = Constants.TP_SECTIONS_SECTION;
                if (!tabActions.TabPages.Contains(tpSection))
                {
                    tabActions.TabPages.Add(tpSection);
                }

                lblSectionHeader.Text = sectionHeader;

                colSection.Visible = false;

                INISection section = item as INISection;
                txtSectionDescription.Text = GetDescription(section.XMLSection);

                foreach (INIProperty property in section.Properties)
                {
                    int rowIndex = dgvProperties.Rows.Add();

                    dgvProperties[colProperty.Index, rowIndex].Value = property.Name;
                    dgvProperties[colValue.Index, rowIndex].Value = property.Value;
                    dgvProperties[colDescription.Index, rowIndex].Value = GetDescription(property.XMLProperty);

                    dgvProperties[colSection.Index, rowIndex].Tag = section;
                    dgvProperties[colProperty.Index, rowIndex].Tag = property;
                }
            }
            else if (item is INIProperty)
            {
                lblSectionHeader.Text = sectionHeader;

                INIProperty property = item as INIProperty;
                txtSectionDescription.Text = GetDescription(property.Section.XMLSection);
                txtPropertyDescription.Text = GetDescription(property.XMLProperty);
                
                SetPropertyValues(property);
                
                if (tabActions.TabPages.Contains(tpSection))
                {
                    tabActions.TabPages.Remove(tpSection);
                }

                tpProperty.Text = property.Name;
                if (!tabActions.TabPages.Contains(tpProperty))
                {
                    tabActions.TabPages.Add(tpProperty);
                }
            }

            SelectTreeNode(item);
        }

        private void InitializeNumericProperty(INIProperty property)
        {
            // Show used controls
            lblPropertyMinValue.Visible = true;
            txtPropertyMinValue.Visible = true;
            lblPropertyMaxValue.Visible = true;
            txtPropertyMaxValue.Visible = true;
            lblPropertyInterval.Visible = true;
            txtPropertyInterval.Visible = true;

            int diffHeight = 35;

            // Set the Top location of the controls. Left locations is already set by the designer!
            lblPropertyMinValue.Top = lblPropertyDataType.Top + diffHeight;
            txtPropertyMinValue.Top = txtPropertyDataType.Top + diffHeight;
            lblPropertyMaxValue.Top = lblPropertyMinValue.Top + diffHeight;
            txtPropertyMaxValue.Top = txtPropertyMinValue.Top + diffHeight;
            lblPropertyInterval.Top = lblPropertyMaxValue.Top + diffHeight;
            txtPropertyInterval.Top = txtPropertyMaxValue.Top + diffHeight;
            lblPropertyCurrentValue.Top = lblPropertyInterval.Top + diffHeight;
            txtPropertyCurrentValue.Top = txtPropertyInterval.Top + diffHeight;
            lblPropertyNewValue.Top = lblPropertyCurrentValue.Top + diffHeight;

            NumericUpDown nudPropertyNewValue = new NumericUpDown();
            nudPropertyNewValue.Name = "ctlPropertyNewValue";
            nudPropertyNewValue.Increment = property.XMLProperty.Interval;
            nudPropertyNewValue.Minimum = property.XMLProperty.MinValue;
            nudPropertyNewValue.Maximum = property.XMLProperty.MaxValue;
            nudPropertyNewValue.Value = Convert.ToDecimal(property.XMLProperty.Value);

            nudPropertyNewValue.Left = txtPropertyInterval.Left;
            nudPropertyNewValue.Top = txtPropertyCurrentValue.Top + diffHeight;
            nudPropertyNewValue.Width = txtPropertyDataType.Width;

            // Store the control in the class variable
            _ctlPropertyNewValue = nudPropertyNewValue;

            pnlProperty.Controls.Add(_ctlPropertyNewValue);
        }

        private void InitializeBooleanProperty(INIProperty property)
        {
            int diffHeight = 35;

            // Hide unused controls
            lblPropertyMinValue.Visible = false;
            txtPropertyMinValue.Visible = false;
            lblPropertyMaxValue.Visible = false;
            txtPropertyMaxValue.Visible = false;
            lblPropertyInterval.Visible = false;
            txtPropertyInterval.Visible = false;

            // Set the top location of the controls. Left location is already set by the designer
            lblPropertyCurrentValue.Top = lblPropertyDataType.Top + diffHeight;
            txtPropertyCurrentValue.Top = txtPropertyDataType.Top + diffHeight;
            
            lblPropertyNewValue.Top = txtPropertyCurrentValue.Top + diffHeight;

            ComboBox cmbPropertyNewValue = new ComboBox();
            cmbPropertyNewValue.Name = "ctlPropertyNewValue";
            cmbPropertyNewValue.Items.Add("TRUE");
            cmbPropertyNewValue.Items.Add("FALSE");
            cmbPropertyNewValue.SelectedText = property.XMLProperty.Value;

            cmbPropertyNewValue.Left = txtPropertyInterval.Left;
            cmbPropertyNewValue.Top = txtPropertyCurrentValue.Top + diffHeight;
            cmbPropertyNewValue.Width = txtPropertyDataType.Width;

            // Store the control in the class variable
            _ctlPropertyNewValue = cmbPropertyNewValue;

            pnlProperty.Controls.Add(_ctlPropertyNewValue);
        }


        #endregion
        #region Private Methods
        private void SetPropertyValues(INIProperty property)
        {
            txtPropertyDataType.Text = property.XMLProperty.DataType;
            txtPropertyMinValue.Text = Convert.ToString(property.XMLProperty.MinValue);
            txtPropertyMaxValue.Text = Convert.ToString(property.XMLProperty.MaxValue);
            txtPropertyInterval.Text = Convert.ToString(property.XMLProperty.Interval);
            txtPropertyCurrentValue.Text = Convert.ToString(property.XMLProperty.Value);

            _ctlPropertyNewValue.Text = "_ctlPropertyNewValue";

            if (pnlProperty.Controls.Contains(_ctlPropertyNewValue))
            {
                pnlProperty.Controls.Remove(_ctlPropertyNewValue);
            }

            // Boolean -> Drop down list control
            if (property.XMLProperty.DataType.ToLower() == Enumerations.DataType.Boolean.ToString().ToLower())
            {
                InitializeBooleanProperty(property);                
            }
            // Numeric -> Numeric up down control
            else if (property.XMLProperty.DataType.ToLower() == Enumerations.DataType.Numeric.ToString().ToLower())
            {
                InitializeNumericProperty(property);
            }
            // String -> Text box control
            else if (property.XMLProperty.DataType.ToLower() == Enumerations.DataType.String.ToString().ToLower())
            {
                
            }   
        }

        private Settings GetXMLIniFile()
        {
            Settings xmlIniSettings = null;
            if (_iniSettingsList.ContainsKey(Constants.INI_FILE))
            {
                xmlIniSettings = _iniSettingsList[Constants.INI_FILE] as Settings;
            }

            if (xmlIniSettings == null)
            {
                xmlIniSettings = new Settings();
                xmlIniSettings.Description_ENG = Constants.MISSING_INI_DESCRIPTION_ENG;
                xmlIniSettings.Description_GER = Constants.MISSING_INI_DESCRIPTION_GER;
            }
            return xmlIniSettings;
        }

        private Section GetXMLSection(string section)
        {
            Section matchingXMLSection = null;

            if (_iniSettingsList.ContainsKey(section))
            {
                matchingXMLSection = _iniSettingsList[section] as Section;
            }

            if (matchingXMLSection == null)
            {
                matchingXMLSection = new Section();
                matchingXMLSection.Description_ENG = Constants.MISSING_SECTION_DESCRIPTION_ENG;
                matchingXMLSection.Description_GER = Constants.MISSING_SECTION_DESCRIPTION_GER;
            }

            return matchingXMLSection;
        }

        private Property GetXMLProperty(string section, string property)
        {
            Property matchingXMLProperty = null;

            if (_iniSettingsList.ContainsKey(section))
            {
                Section xmlSection = _iniSettingsList[section] as Section;
                foreach (Property xmlProperty in xmlSection.Properties)
                {
                    if (xmlProperty.Name == property)
                    {
                        matchingXMLProperty = xmlProperty;
                        break;
                    }
                }
            }

            if (matchingXMLProperty == null)
            {
                matchingXMLProperty = new Property();
                matchingXMLProperty.Description_ENG = Constants.MISSING_PROPERTY_DESCRIPTION_ENG;
                matchingXMLProperty.Description_GER = Constants.MISSING_PROPERTY_DESCRIPTION_GER;
                matchingXMLProperty.DataType = Constants.MISSING_DATA_TYPE;
                matchingXMLProperty.MinValue = Constants.MISSING_MIN_VALUE;
                matchingXMLProperty.MaxValue = Constants.MISSING_MAX_VALUE;
                matchingXMLProperty.Interval = Constants.PROPERTY_INVERVAL;
                matchingXMLProperty.Value = Constants.MISSING_PROPERTY_VALUE;
            }

            return matchingXMLProperty;
        }

        private string GetDescription(BaseElements baseElements)
        {
            string description = "";

            switch (_descriptionLanguage)
            {
                case Enumerations.Language.English:
                    description = baseElements.Description_ENG;
                    break;
                case Enumerations.Language.German:
                    description = baseElements.Description_GER;
                    break;
            }

            return description;
        }

        private void SelectTreeNode<T>(T item)
        {
            TreeNode treeNode = null;
            if (item is INIFile)
            {
                INIFile iniFile = item as INIFile;
                treeNode = iniFile.Tag as TreeNode;
            }
            else if (item is INISection)
            {
                INISection section = item as INISection;
                treeNode = section.Tag as TreeNode;
            }
            else if (item is INIProperty)
            {
                INIProperty property = item as INIProperty;
                treeNode = property.Tag as TreeNode;
            }

            trvSections.SelectedNode = treeNode;
            trvSections.SelectedNode.Expand();
        }
        #endregion
        #region Events
        private void cmbFiles_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cmbFiles.SelectedItem != null)
            {
                InitializeSectionTree();
                if (trvSections.Nodes.Count > 0)
                {
                    trvSections.SelectedNode = trvSections.Nodes[0];
                    tpProperty.Visible = false;
                }
            }
        }

        private void trvSections_AfterSelect(object sender, TreeViewEventArgs e)
        {
            string sectionHeader = e.Node.Text;
            if (e.Node.Tag is INIProperty)
            {
                TreeNode sectionNode = e.Node.Parent;
                sectionHeader = sectionNode.Text;

            }

            InitializeTabs(e.Node.Tag, sectionHeader);
        }

        private void dgvProperties_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            // Get the clicked row
            DataGridViewRow row = dgvProperties.Rows[e.RowIndex];
            INISection section = (INISection)row.Cells[colSection.Index].Tag;
            INIProperty property = (INIProperty)row.Cells[colProperty.Index].Tag;

            InitializeTabs(property, section.Name);
        }

        private void mnuViewDescLangEnglish_Click(object sender, EventArgs e)
        {
            if (_descriptionLanguage != Enumerations.Language.English)
            {
                _descriptionLanguage = Enumerations.Language.English;
                TreeNode selectedNode = trvSections.SelectedNode;
                InitializeTabs(selectedNode.Tag, lblSectionHeader.Text);
            }
        }

        private void mnuViewDescLangGerman_Click(object sender, EventArgs e)
        {
            if (_descriptionLanguage != Enumerations.Language.German)
            {
                _descriptionLanguage = Enumerations.Language.German;
                TreeNode selectedNode = trvSections.SelectedNode;
                InitializeTabs(selectedNode.Tag, lblSectionHeader.Text);
            }
        }

        /// <summary>
        /// This method creates a new "INIEditorInit_Output.xml" file,
        /// which contains all the INI-Settings. Missing settings in the
        /// file are set to constant values (see Constants.cs) for easy replacement.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void mnuToolsGenerateXMLFile_Click(object sender, EventArgs e)
        {
            if (_iniFile != null)
            {
                Settings generatedIniSettings = new Settings();
                generatedIniSettings = _iniFile.XMLSettings;

                foreach (INISection section in _iniFile.Sections)
                {
                    Section xmlSection = new Section();
                    xmlSection = section.XMLSection;

                    foreach (INIProperty property in section.Properties)
                    {
                        Property xmlProperty = new Property();
                        xmlProperty = property.XMLProperty;

                        xmlSection.Properties.Add(xmlProperty);
                    }

                    generatedIniSettings.Sections.Add(xmlSection);
                }

                string path = Path.Combine(Constants.XML_SETTINGS_PATH, Constants.INI_EDITOR_INIT_FILE_OUTPUT);
                Helper.SaveObjectToXMLFile(generatedIniSettings, path);
            }
        }
        #endregion
    }
}