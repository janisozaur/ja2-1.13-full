using System;
using System.Windows.Forms;
using INIEditor.BackEnd;

namespace INIEditor.GUI
{
    public partial class SearchForm : Form
    {
        #region Fields
        private readonly MainForm _mainForm;
        #endregion

        #region CTOR
        public SearchForm(MainForm mainForm, SearchParams searchParams, Enumerations.Language descLanguage)
        {
            _mainForm = mainForm;
            InitializeComponent();
            InitializeSearchForm(searchParams, descLanguage);
        }
        #endregion

        #region Initialize
        private void InitializeSearchForm(SearchParams searchParams, Enumerations.Language descLanguage)
        {
            if (descLanguage == Enumerations.Language.English)
            {
                picLanguage_ENG.Visible = true;
                picLanguage_GER.Visible = false;
            }
            else if (descLanguage == Enumerations.Language.German)
            {
                picLanguage_GER.Visible = true;
                picLanguage_ENG.Visible = false;
            }

            txtFindWhat.Text = searchParams.FindWhat;
            chkSectionDescriptions.Checked = searchParams.LookInSectionDescriptions;
            chkProperties.Checked = searchParams.LookInProperties;
            chkPropertyDescriptions.Checked = searchParams.LookInPropertyDescriptions;
            chkPropertyValues.Checked = searchParams.LookInPropertyValues;
        }
        #endregion

        #region Private Methods
        private void SetSearchButtonState()
        {
            if (chkSectionDescriptions.Checked == false &&
                chkPropertyDescriptions.Checked == false &&
                chkPropertyValues.Checked == false &&
                chkProperties.Checked == false)
            {
                btnSearch.Enabled = false;
            }
            else
            {
                btnSearch.Enabled = true;
            }
        }
        #endregion

        #region Events
        private void btnSearch_Click(object sender, EventArgs e)
        {
            SearchParams searchParams = new SearchParams(txtFindWhat.Text.Trim(), chkSectionDescriptions.Checked, chkPropertyDescriptions.Checked,
                chkPropertyValues.Checked, chkProperties.Checked);

            _mainForm.Search(searchParams);
        }

        private void chkSectionDescriptions_CheckedChanged(object sender, EventArgs e)
        {
            SetSearchButtonState();
        }

        private void chkPropertyDescriptions_CheckedChanged(object sender, EventArgs e)
        {
            SetSearchButtonState();
        }

        private void chkPropertyValues_CheckedChanged(object sender, EventArgs e)
        {
            SetSearchButtonState();
        }

        private void chkProperties_CheckedChanged(object sender, EventArgs e)
        {
            SetSearchButtonState();
        }
        #endregion
    }
}