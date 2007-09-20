using System.Collections.Generic;
using INIEditor.BackEnd.XML;

namespace INIEditor.BackEnd.INIHelper
{
    public class INISection
    {
        #region Fields
        private string _name;
        private List<INIProperty> _propertys = new List<INIProperty>();
        private object _tag = null;
        private Section _xmlSection = null;
        //private string _description_ger = Constants.MISSING_SECTION_DESCRIPTION_GER;
        //private string _description_eng = Constants.MISSING_SECTION_DESCRIPTION_ENG;
        #endregion
        #region Properties
        public List<INIProperty> Properties
        {
            get { return this._propertys; }
        }

        public string Name
        {
            get
            {
                return this._name;
            }
            set
            {
                this._name = value;
            }
        }

        public object Tag
        {
            get
            {
                return _tag;
            }
            set
            {
                _tag = value;
            }
        }

        //public string Description_GER
        //{
        //    get { return _description_ger; }
        //    set { _description_ger = value; }
        //}

        //public string Description_ENG
        //{
        //    get { return _description_eng; }
        //    set { _description_eng = value; }
        //}

        public Section XMLSection
        {
            get { return _xmlSection;  }
            set { _xmlSection = value; }
        }

        #endregion
        #region Public Methods
        public INIProperty GetPropertyByName(string propertyName)
        {
            INIProperty matchingProperty = null;
            foreach (INIProperty property in this.Properties)
            {
                if (property.Name.ToLower() == propertyName.ToLower())
                {
                    matchingProperty = property;
                    break;
                }
            }
            return matchingProperty;
        }
        #endregion
        #region Private Methods
        public void AddProperty(INIProperty prop)
        {
            this._propertys.Add(prop);
        }
        #endregion
    }
}