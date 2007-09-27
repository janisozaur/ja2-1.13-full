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
        private readonly Enumerations.Permission _permission = Enumerations.Permission.Admin;    // TODO: Change to "User" in Release version!
        private Control _ctlPropertyNewValue = new Control();
        private readonly System.ComponentModel.ComponentResourceManager _resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
        private SearchParams _searchParams = new SearchParams();
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

        private void InitializeSectionTree(string selectedTreeNode)
        {
            trvSections.Nodes.Clear();

            _iniFile = new INIFile();
            string iniFilePath = Path.Combine(Constants.JA2_PATH, cmbFiles.SelectedItem.ToString());
            _iniFile.ReadFile(iniFilePath);

            _iniFile.XMLSettings = GetXMLIniFile();
             
            TreeNode iniFileNode = new TreeNode();
            iniFileNode.Name = cmbFiles.SelectedItem.ToString();
            iniFileNode.Text = cmbFiles.SelectedItem.ToString();
            iniFileNode.ImageKey = "INIFile.ico";
            iniFileNode.SelectedImageKey = "INIFile.ico";
            iniFileNode.Tag = _iniFile;
            //iniFileNode.Collapse();
            trvSections.Nodes.Add(iniFileNode);

            // The ini file has a reference to the tree node
            _iniFile.Tag = iniFileNode;

            // Loop through all the sections of the ini file
            foreach (INISection section in _iniFile.Sections)
            {
                section.XMLSection = GetXMLSection(section);

                TreeNode sectionNode = new TreeNode();
                sectionNode.Name = section.Name;
                sectionNode.Text = section.Name;
                sectionNode.ImageKey = "Section.ico";
                sectionNode.SelectedImageKey = "SectionSelected.ico";
                sectionNode.Tag = section;
                iniFileNode.Nodes.Add(sectionNode);

                // The section has a reference to the tree node
                section.Tag = sectionNode;

                // Loop through all the properties of the current selection
                foreach (INIProperty property in section.Properties)
                {
                    property.XMLProperty = GetXMLProperty(property);

                    TreeNode propertyNode = new TreeNode();
                    propertyNode.Name = property.Name;
                    propertyNode.Text = property.Name;
                    propertyNode.ImageKey = "Property.ico";
                    propertyNode.SelectedImageKey = "PropertySelected.ico";
                    propertyNode.Tag = property;
                    sectionNode.Nodes.Add(propertyNode);   

                    // The property has a reference to the tree node
                    property.Tag = propertyNode;
                }
            }

            ReselectTreeNodeInNewSectionTree(selectedTreeNode);
        }

        private void ReselectTreeNodeInNewSectionTree(string selectedTreeNode)
        {
            if (trvSections.Nodes.Count > 0)
            {
                if (trvSections.Nodes[0].Text == selectedTreeNode)
                {
                    trvSections.SelectedNode = trvSections.Nodes[0];
                    return;
                }

                foreach (TreeNode sectionNode in trvSections.Nodes[0].Nodes)
                {
                    if (sectionNode.Text == selectedTreeNode)
                    {
                        trvSections.SelectedNode = sectionNode;
                        return;
                    }

                    foreach (TreeNode propertyNode in sectionNode.Nodes)
                    {
                        if (propertyNode.Text == selectedTreeNode)
                        {
                            trvSections.SelectedNode = propertyNode;
                            return;
                        }
                    }
                }
            }
        }

        private void InitializeTabs<T>(T item, string sectionHeader)
        {
            dgvProperties.Rows.Clear();

            // INI file item
            if (item is INIFile)
            {
                tabActions.TabPages.Remove(tpSearchResults);

                if (tabActions.TabPages.Contains(tpProperty))
                {
                    tabActions.TabPages.Remove(tpProperty);
                }

                tpSection.Text = Constants.TP_SECTIONS_INI_FILE;
                if (!tabActions.TabPages.Contains(tpSection))
                {
                    tabActions.TabPages.Add(tpSection);
                }

                tabActions.TabPages.Add(tpSearchResults);

                lblSectionHeader.Text = sectionHeader;
                
                colSection.Visible = true;

                INIFile file = item as INIFile;
                txtSectionDescription.Text = GetDescription(file.XMLSettings);

                foreach (INISection section in file.Sections)
                {
                    foreach (INIProperty property in section.Properties)
                    {
                        int rowIndex = dgvProperties.Rows.Add();

                        dgvProperties[colSection.Index, rowIndex].Value = section.Name;
                        dgvProperties[colProperty.Index, rowIndex].Value = property.Name;
                        dgvProperties[colDescription.Index, rowIndex].Value = GetDescription(property.XMLProperty);
                        dgvProperties[colCurrentValue.Index, rowIndex].Value = property.CurrentValue;
                        dgvProperties[colNewValue.Index, rowIndex].Value = property.NewValue;

                        dgvProperties[colSection.Index, rowIndex].Tag = section;
                        dgvProperties[colProperty.Index, rowIndex].Tag = property;

                        // The value has been changed
                        if (property.CurrentValue != property.NewValue)
                        {
                            dgvProperties[colSection.Index, rowIndex].Style.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                            dgvProperties[colProperty.Index, rowIndex].Style.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                            dgvProperties[colDescription.Index, rowIndex].Style.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                            dgvProperties[colCurrentValue.Index, rowIndex].Style.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                            dgvProperties[colNewValue.Index, rowIndex].Style.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                        }
                    }
                }

            }
            // Section item
            else if (item is INISection)
            {
                tabActions.TabPages.Remove(tpSearchResults);
                if (tabActions.TabPages.Contains(tpProperty))
                {
                    tabActions.TabPages.Remove(tpProperty);
                }

                tpSection.Text = Constants.TP_SECTIONS_SECTION;
                if (!tabActions.TabPages.Contains(tpSection))
                {
                    tabActions.TabPages.Add(tpSection);
                }

                tabActions.TabPages.Add(tpSearchResults);

                lblSectionHeader.Text = sectionHeader;

                colSection.Visible = false;

                INISection section = item as INISection;
                txtSectionDescription.Text = GetDescription(section.XMLSection);

                foreach (INIProperty property in section.Properties)
                {
                    int rowIndex = dgvProperties.Rows.Add();

                    dgvProperties[colProperty.Index, rowIndex].Value = property.Name;
                    dgvProperties[colCurrentValue.Index, rowIndex].Value = property.CurrentValue;
                    dgvProperties[colNewValue.Index, rowIndex].Value = property.NewValue;
                    dgvProperties[colDescription.Index, rowIndex].Value = GetDescription(property.XMLProperty);

                    dgvProperties[colSection.Index, rowIndex].Tag = section;
                    dgvProperties[colProperty.Index, rowIndex].Tag = property;

                    // The value has been changed
                    if (property.CurrentValue != property.NewValue)
                    {
                        dgvProperties[colProperty.Index, rowIndex].Style.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                        dgvProperties[colCurrentValue.Index, rowIndex].Style.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                        dgvProperties[colNewValue.Index, rowIndex].Style.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                        dgvProperties[colDescription.Index, rowIndex].Style.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                    }
                }
            }
            else if (item is INIProperty)
            {
                tabActions.TabPages.Remove(tpSearchResults);

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

                tabActions.TabPages.Add(tpSearchResults);
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
            nudPropertyNewValue.Tag = property;
            nudPropertyNewValue.Increment = property.XMLProperty.Interval;
            nudPropertyNewValue.Minimum = property.XMLProperty.MinValue;
            nudPropertyNewValue.Maximum = property.XMLProperty.MaxValue;
            nudPropertyNewValue.Value = Convert.ToDecimal(property.NewValue);

            nudPropertyNewValue.Left = txtPropertyInterval.Left;
            nudPropertyNewValue.Top = txtPropertyCurrentValue.Top + diffHeight;
            nudPropertyNewValue.Width = txtPropertyDataType.Width;

            nudPropertyNewValue.Leave += new System.EventHandler(this.txtPropertyNewValue_Leave);

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
            cmbPropertyNewValue.Tag = property;
            cmbPropertyNewValue.Items.Add("TRUE");
            cmbPropertyNewValue.Items.Add("FALSE");
            cmbPropertyNewValue.SelectedText = property.NewValue;

            cmbPropertyNewValue.Left = txtPropertyInterval.Left;
            cmbPropertyNewValue.Top = txtPropertyCurrentValue.Top + diffHeight;
            cmbPropertyNewValue.Width = txtPropertyDataType.Width;

            cmbPropertyNewValue.Leave += new System.EventHandler(this.txtPropertyNewValue_Leave);

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
            txtPropertyCurrentValue.Text = Convert.ToString(property.CurrentValue);

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

        private Section GetXMLSection(INISection section)
        {
            Section matchingXMLSection = null;

            if (_iniSettingsList.ContainsKey(section.Name))
            {
                matchingXMLSection = _iniSettingsList[section.Name] as Section;
            }

            if (matchingXMLSection == null)
            {
                matchingXMLSection = new Section();
                matchingXMLSection.Name = section.Name;
                matchingXMLSection.Description_ENG = Constants.MISSING_SECTION_DESCRIPTION_ENG;
                matchingXMLSection.Description_GER = Constants.MISSING_SECTION_DESCRIPTION_GER;
            }

            return matchingXMLSection;
        }

        private Property GetXMLProperty(INIProperty property)
        {
            Property matchingXMLProperty = null;

            if (_iniSettingsList.ContainsKey(property.Section.Name))
            {
                Section xmlSection = _iniSettingsList[property.Section.Name] as Section;
                foreach (Property xmlProperty in xmlSection.Properties)
                {
                    if (xmlProperty.Name == property.Name)
                    {
                        matchingXMLProperty = xmlProperty;
                        break;
                    }
                }
            }

            if (matchingXMLProperty == null)
            {
                matchingXMLProperty = new Property();
                matchingXMLProperty.Name = property.Name;
                matchingXMLProperty.Description_ENG = Constants.MISSING_PROPERTY_DESCRIPTION_ENG;
                matchingXMLProperty.Description_GER = Constants.MISSING_PROPERTY_DESCRIPTION_GER;
                matchingXMLProperty.DataType = Constants.MISSING_DATA_TYPE;
                matchingXMLProperty.MinValue = Constants.MISSING_MIN_VALUE;
                matchingXMLProperty.MaxValue = Constants.MISSING_MAX_VALUE;
                matchingXMLProperty.Interval = Constants.PROPERTY_INVERVAL;
                
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
                InitializeSectionTree(cmbFiles.SelectedItem.ToString());
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
        #endregion

        private void mnuViewDescLanguageEnglish_Click(object sender, EventArgs e)
        {
            SetDescriptionLanguage(Enumerations.Language.English);
        }

        private void SetDescriptionLanguage(Enumerations.Language language)
        {
            if (_descriptionLanguage != language)
            {
                _descriptionLanguage = language;
                TreeNode selectedNode = trvSections.SelectedNode;
                InitializeTabs(selectedNode.Tag, lblSectionHeader.Text);

                if (language == Enumerations.Language.English)
                {
                    tsbLanguage.Image = ((System.Drawing.Image)(_resources.GetObject("tsbLanguageEnglish.Image")));
                }
                else if (language == Enumerations.Language.German)
                {
                    tsbLanguage.Image = ((System.Drawing.Image)(_resources.GetObject("tsbLanguageGerman.Image")));
                }
            }
        }

        private void mnuViewDescLanguageGerman_Click(object sender, EventArgs e)
        {
            SetDescriptionLanguage(Enumerations.Language.German);
        }

        /// <summary>
        /// This method creates a new "INIEditorInit_Output.xml" file,
        /// which contains all the INI-Settings. Missing settings in the
        /// file are set to constant values (see Constants.cs) for easy replacement.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void mnuToolsGenerateXML_Click(object sender, EventArgs e)
        {
            if (_iniFile != null)
            {
                Settings generatedIniSettings = new Settings();
                generatedIniSettings.Description_ENG = _iniFile.XMLSettings.Description_ENG;
                generatedIniSettings.Description_GER = _iniFile.XMLSettings.Description_GER;

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

        private void txtPropertyNewValue_Leave(object sender, EventArgs e)
        {
            Control control = sender as Control;
            INIProperty property = control.Tag as INIProperty;

            property.NewValue = control.Text;

            TreeNode propertyNode = property.Tag as TreeNode;
            INISection section = property.Section;
            TreeNode sectionNode = section.Tag as TreeNode;

            // Value has been changed to a new value
            if (property.NewValue != property.CurrentValue)
            {
                propertyNode.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
                sectionNode.ForeColor = Constants.CHANGED_TREE_NODE_TEXT_COLOR;
            }
            else
            {
                propertyNode.ForeColor = Constants.DEFAULT_TREE_NODE_TEXT_COLOR;
                sectionNode.ForeColor = Constants.DEFAULT_TREE_NODE_TEXT_COLOR;
            }
        }

        private void SaveFile()
        {
            string dataFolder = Path.GetDirectoryName(cmbFiles.SelectedItem.ToString());
            string path = Path.Combine(Constants.JA2_PATH, dataFolder);

            path = Path.Combine(path, Constants.INI_FILE_OUT);
            _iniFile.WriteFile(path);

            TreeNode selectedTreeNode = trvSections.SelectedNode;

            InitializeSectionTree(selectedTreeNode.Text);
        }

        private void mnuFileSave_Click(object sender, EventArgs e)
        {
            SaveFile();
        }

        private void tbrSave_Click(object sender, EventArgs e)
        {
            SaveFile();
        }

        private void mnuViewSearch_Click(object sender, EventArgs e)
        {
            DisplaySearchForm();
        }

        private void DisplaySearchForm()
        {
            SearchForm searchForm = new SearchForm(this, _searchParams);
            searchForm.Show(this);
            searchForm.TopLevel = true;
        }

        private void tbrSearch_Click(object sender, EventArgs e)
        {
            DisplaySearchForm();
        }

        private void tsbLanguageGerman_Click(object sender, EventArgs e)
        {
            SetDescriptionLanguage(Enumerations.Language.German);
        }

        private void tsbLanguageEnglish_Click(object sender, EventArgs e)
        {
            SetDescriptionLanguage(Enumerations.Language.English);
        }

        
        private void AddSearchResultToSearchResults(INIProperty property, bool foundSectionDesc,
            bool foundPropertyDesc, bool foundPropertyCurrentValue, bool foundPropertyNewValue)
        {
            int rowIndex = -1;

            if (_descriptionLanguage == Enumerations.Language.English)
            {
               rowIndex = dgvSearchResults.Rows.Add();
               dgvSearchResults[colSearchResultsSection.Index, rowIndex].Value = property.Section.Name;
               dgvSearchResults[colSearchResultsSectionDesc.Index, rowIndex].Value = property.Section.XMLSection.Description_ENG;
               dgvSearchResults[colSearchResultsPropertyDesc.Index, rowIndex].Value = property.XMLProperty.Description_ENG;
               dgvSearchResults[colSearchResultsPropertyCurrentValue.Index, rowIndex].Value = property.CurrentValue;
               dgvSearchResults[colSearchResultsPropertyNewValue.Index, rowIndex].Value = property.NewValue;

               // Tag
               dgvSearchResults[colSearchResultsSectionDesc.Index, rowIndex].Tag = property.Section;
               dgvSearchResults[colSearchResultsPropertyDesc.Index, rowIndex].Tag = property;

            }

            // Color the results
            if (foundSectionDesc)
               dgvSearchResults[colSearchResultsSectionDesc.Index, rowIndex].Style.ForeColor = Constants.DEFAULT_SEARCH_RESULT_TEXT_COLOR;
            if (foundPropertyDesc)
               dgvSearchResults[colSearchResultsPropertyDesc.Index, rowIndex].Style.ForeColor = Constants.DEFAULT_SEARCH_RESULT_TEXT_COLOR;
            if (foundPropertyCurrentValue)
               dgvSearchResults[colSearchResultsPropertyCurrentValue.Index, rowIndex].Style.ForeColor = Constants.DEFAULT_SEARCH_RESULT_TEXT_COLOR;
            if (foundPropertyNewValue)
               dgvSearchResults[colSearchResultsPropertyNewValue.Index, rowIndex].Style.ForeColor = Constants.DEFAULT_SEARCH_RESULT_TEXT_COLOR;

        }

        private void AddSearchResultToSearchResults(INISection section)
        {
            int rowIndex = -1;

            if (_descriptionLanguage == Enumerations.Language.English)
            {
                rowIndex = dgvSearchResults.Rows.Add();
                dgvSearchResults[colSearchResultsSection.Index, rowIndex].Value = section.Name;
                dgvSearchResults[colSearchResultsSectionDesc.Index, rowIndex].Value = section.XMLSection.Description_ENG;

                // Tag
                dgvSearchResults[colSearchResultsSectionDesc.Index, rowIndex].Tag = section;
            }

            // Color the results
            dgvSearchResults[colSearchResultsSectionDesc.Index, rowIndex].Style.ForeColor = Constants.DEFAULT_SEARCH_RESULT_TEXT_COLOR;

        }

        public void Search(SearchParams searchParams)
        {
            _searchParams = searchParams;
            // Only "Section Descriptions" was selected
            if (_searchParams.LookInSectionDescriptions && (!_searchParams.LookInPropertyDescriptions
                    || !_searchParams.LookInPropertyValues))
            {
                colSearchResultsSection.Visible = true;
                colSearchResultsSectionDesc.Visible = true;
                colSearchResultsPropertyDesc.Visible = false;
                colSearchResultsPropertyCurrentValue.Visible = false;
                colSearchResultsPropertyNewValue.Visible = false;
            }
            else
            {
                colSearchResultsSection.Visible = true;
                colSearchResultsSectionDesc.Visible = true;
                colSearchResultsPropertyDesc.Visible = true;
                colSearchResultsPropertyCurrentValue.Visible = true;
                colSearchResultsPropertyNewValue.Visible = true;
            }

            dgvSearchResults.Rows.Clear();

            tabActions.SelectTab(tpSearchResults);

            bool foundSectionDesc_ENG = false;
            bool foundPropertyNewValue = false;
            bool foundPropertyCurrentValue = false;
            bool foundPropertyDesc_ENG = false;

            foreach (INISection section in _iniFile.Sections)
            {
                foundSectionDesc_ENG = false;

                if (_searchParams.LookInSectionDescriptions && (!_searchParams.LookInPropertyDescriptions
                    || !_searchParams.LookInPropertyValues))
                {
                    string sectionDesc_ENG = section.XMLSection.Description_ENG.ToLower();
                    string sectionDesc_GER = section.XMLSection.Description_GER.ToLower();

                    if (_descriptionLanguage == Enumerations.Language.English)
                    {
                        if (searchParams.LookInSectionDescriptions
                            && sectionDesc_ENG.Contains(searchParams.FindWhat.ToLower()))
                        {
                            AddSearchResultToSearchResults(section);
                        }
                    }
                }
                else
                {
                    foreach (INIProperty property in section.Properties)
                    {
                        foundSectionDesc_ENG = false;
                        foundPropertyNewValue = false;
                        foundPropertyCurrentValue = false;
                        foundPropertyDesc_ENG = false;

                        string sectionDesc_ENG = property.Section.XMLSection.Description_ENG.ToLower();
                        string sectionDesc_GER = property.Section.XMLSection.Description_GER.ToLower();
                        string propertyDesc_ENG = property.XMLProperty.Description_ENG.ToLower();
                        string propertyDesc_GER = property.XMLProperty.Description_GER.ToLower();
                        string propertyCurrentValue = property.CurrentValue.ToLower();
                        string propertyNewValue = property.NewValue.ToLower();

                        if (_descriptionLanguage == Enumerations.Language.English)
                        {
                            if (searchParams.LookInSectionDescriptions
                                && sectionDesc_ENG.Contains(searchParams.FindWhat.ToLower()))
                            {
                                foundSectionDesc_ENG = true;
                            }
                            if (searchParams.LookInPropertyDescriptions
                                && propertyDesc_ENG.Contains(searchParams.FindWhat.ToLower()))
                            {
                                foundPropertyDesc_ENG = true;
                            }
                        }

                        if (searchParams.LookInPropertyValues
                            && propertyCurrentValue.Contains(searchParams.FindWhat.ToLower()))
                        {
                            foundPropertyCurrentValue = true;
                        }
                        if (searchParams.LookInPropertyValues
                            && propertyNewValue.Contains(searchParams.FindWhat.ToLower()))
                        {
                            foundPropertyNewValue = true;
                        }

                        if (_descriptionLanguage == Enumerations.Language.English)
                        {
                            if (foundSectionDesc_ENG || foundPropertyDesc_ENG ||
                                foundPropertyCurrentValue || foundPropertyNewValue)
                            {
                                AddSearchResultToSearchResults(property, foundSectionDesc_ENG, foundPropertyDesc_ENG,
                                                               foundPropertyCurrentValue, foundPropertyNewValue);
                            }
                        }
                    }
                }
            }
        }

        private void dgvSearchResults_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            // Get the clicked row
            DataGridViewRow row = dgvSearchResults.Rows[e.RowIndex];
            INISection section = (INISection)row.Cells[colSearchResultsSectionDesc.Index].Tag;

            if (colSearchResultsPropertyDesc.Visible)
            {
                INIProperty property = (INIProperty)row.Cells[colSearchResultsPropertyDesc.Index].Tag;
                InitializeTabs(property, section.Name);
            }
            else
            {
                InitializeTabs(_iniFile, section.Name);
            }
        }
    }
}