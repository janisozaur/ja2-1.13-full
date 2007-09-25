using System;
using System.Collections.Generic;
using System.Text;

namespace INIEditor.BackEnd
{
    public class SearchParams
    {
        private string _findWhat;
        private bool _lookInSectionDescriptions;
        private bool _lookInPropertyDescriptions;
        private bool _lookInPropertyValues;
        private Enumerations.Language _language;

        #region CTOR
        public SearchParams(string findWhat, bool lookInSectionDescriptions,
            bool lookInPropertyDescriptions, bool lookInPropertyValues, Enumerations.Language language)
        {
            _findWhat = findWhat;
            _lookInSectionDescriptions = lookInSectionDescriptions;
            _lookInPropertyDescriptions = lookInPropertyDescriptions;
            _lookInPropertyValues = lookInPropertyValues;
            _language = language;
        }
        #endregion

        public string FindWhat
        {
            get { return _findWhat; }
        }

        public bool LookInSectionDescriptions
        {
            get { return _lookInSectionDescriptions; }
        }

        public bool LookInPropertyDescriptions
        {
            get { return _lookInPropertyDescriptions; }
        }

        public bool LookInPropertyValues
        {
            get { return _lookInPropertyValues; }
        }

        public Enumerations.Language Language
        {
            get { return _language; }
        }
    }
}
