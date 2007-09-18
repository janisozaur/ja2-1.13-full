using System.Collections.Generic;

namespace INIEditor.BackEnd.INIHelper
{
    public class INISection
    {
        #region Fields
        private string _name;
        private List<INIProperty> _propertys = new List<INIProperty>();
        private object _tag = null;
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