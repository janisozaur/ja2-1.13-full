using System.Collections.Generic;

namespace INIEditor.BackEnd.INIHelper
{
    public class INISection
    {
        private string name;
        private List<INIProperty> propertys = new List<INIProperty>();

        

        public void AddProperty(INIProperty prop)
        {
            this.propertys.Add(prop);
        }

        public List<INIProperty> GetPropertys()
        {
            return this.propertys;
        }

        public INIProperty this[string propertyName]
        {
            get
            {
                INIProperty matchingProp = null;
                foreach (INIProperty prop in this.propertys)
                {
                    if (prop.Name == propertyName)
                    {
                        matchingProp = prop;
                        break;
                    }
                }
                return matchingProp;
            }
            set
            {
                for (int i = 0; i < this.propertys.Count; i++ )
                {
                    INIProperty prop = this.propertys[i];
                    if (prop.Name == propertyName)
                    {
                        prop = value;
                    }
                }
            }
        }

        public string Name
        {
            get
            {
                return this.name;
            }
            set
            {
                this.name = value;
            }   
        }
    }
}