using INIEditor.BackEnd.XML;

namespace INIEditor.BackEnd.INIHelper
{
    public class INIProperty
    {
        #region Fields
        private string _name;
        private string _value;
        private object _tag = null;
        private Property _xmlProperty = null;

        //private string _description_ger = Constants.MISSING_PROPERTY_DESCRIPTION_GER;
        //private string _description_eng = Constants.MISSING_PROPERTY_DESCRIPTION_ENG;
        //private string _dataType = Constants.MISSING_DATA_TYPE;
        //private int _minValue = Constants.MISSING_MIN_VALUE;
        //private int _maxValue = Constants.MISSING_MAX_VALUE;
        #endregion
        #region Properties
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

        public string Value
        {
            get
            {
                return this._value;
            }
            set
            {
                this._value = value;
            }
        }

        public Property XMLProperty
        {
            get { return _xmlProperty; }
            set { _xmlProperty = value; }
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

        //public string DataType
        //{
        //    get { return _dataType;  }
        //    set { _dataType = value; }
        //}

        //public int MinValue
        //{
        //    get { return _minValue;  }
        //    set { _minValue = value; }
        //}

        //public int MaxValue
        //{
        //    get { return _maxValue;  }
        //    set { _maxValue = value; }
        //}
        #endregion
    }
}