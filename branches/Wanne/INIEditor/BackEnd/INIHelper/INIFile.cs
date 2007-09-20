using System.Collections.Generic;
using System.IO;
using INIEditor.BackEnd.XML;

namespace INIEditor.BackEnd.INIHelper
{
    public class INIFile
    {
        #region Fields
        private List<INISection> _sections = new List<INISection>();
        private object _tag = null;
        private Settings _xmlSettings = null;
        #endregion
        #region Properties
        public object Tag
        {
            get { return _tag; }
            set { _tag = value; }
        }

        public List<INISection> Sections
        {
            get { return this._sections; }
        }

        public Settings XMLSettings
        {
            get { return _xmlSettings; }
            set { _xmlSettings = value; }
        }
        #endregion
        #region Public Methods
        public INISection GetSectionByName(string sectionName)
        {
            INISection matchingSection = null;
            foreach (INISection section in this.Sections)
            {
                if (section.Name.ToLower() == sectionName.ToLower())
                {
                    matchingSection = section;
                    break;
                }
            }
            return matchingSection;
        }

        public INIProperty GetPropertyByName(string sectionName, string propertyName)
        {
            INIProperty matchingProperty = null;
            foreach (INISection section in this.Sections)
            {
                if (section.Name.ToLower() == sectionName.ToLower())
                {
                    foreach (INIProperty property in section.Properties)
                    {
                        if (property.Name.ToLower() == propertyName.ToLower())
                        {
                            matchingProperty = property;
                            break;
                        }
                    }
                }
            }
            return matchingProperty;
        }

        public bool IsProperty(string line)
        {
            return line.Contains("=");
        }

        public bool IsSection(string line)
        {
            if (line.StartsWith("["))
            {
                return line.EndsWith("]");
            }
            return false;
        }

        public void ReadFile(string name)
        {
            StreamReader reader = File.OpenText(name);
            INISection section = null;
            int num = -1;
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                line.Trim();

                if (line.StartsWith(";") == false)
                {
                    // Skip comments
                    if (this.IsSection(line))
                    {
                        section = new INISection();
                        section.Name = this.ExtractSectionName(line);
                        this._sections.Add(section);
                        num++;
                    }
                    else if (this.IsProperty(line) && (section != null))
                    {
                        INIProperty prop = new INIProperty();
                        prop.Name = this.ExtractPropertyName(line);
                        prop.Value = this.ExtractPropertyValue(line);
                        this._sections[num].AddProperty(prop);
                    }
                }
            }
            reader.Close();
        }

        public void WriteFile(string name)
        {
            StreamWriter writer = File.CreateText(name);
            List<INISection>.Enumerator enumerator = this._sections.GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    INISection section = enumerator.Current;
                    writer.WriteLine("[" + section.Name + "]");
                    List<INIProperty>.Enumerator enumerator2 = section.Properties.GetEnumerator();
                    try
                    {
                        while (enumerator2.MoveNext())
                        {
                            INIProperty property = enumerator2.Current;
                            writer.WriteLine(property.Name + "=" + property.Value);
                        }
                        continue;
                    }
                    finally
                    {
                        enumerator2.Dispose();
                    }
                }
            }
            finally
            {
                enumerator.Dispose();
            }
            writer.Close();
        }
        #endregion
        #region Private Methods
        private string ExtractPropertyName(string line)
        {
            string property = line.Split(new char[] { '=' })[0].Trim();
            return property;
        }

        private string ExtractPropertyValue(string line)
        {
            string value = line.Split(new char[] { '=' })[1].Trim();
            return value;
        }

        private string ExtractSectionName(string line)
        {
            string section = line.Substring(1, line.Length - 2).Trim();
            return section;
        }
        #endregion
    }
}

