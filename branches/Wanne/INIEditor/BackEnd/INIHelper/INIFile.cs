using System.Collections.Generic;
using System.IO;

namespace INIEditor.BackEnd.INIHelper
{
    public class INIFile
    {
        private List<INISection> sections = new List<INISection>();

        public string ExtractPropertyName(string line)
        {
            string property = line.Split(new char[] { '=' })[0].Trim();
            return property;
        }

        public string ExtractPropertyValue(string line)
        {
            string value = line.Split(new char[] { '=' })[1].Trim();
            return value;
        }

        public string ExtractSectionName(string line)
        {
            string section = line.Substring(1, line.Length - 2).Trim();
            return section;
        }

        public List<INISection> GetSections()
        {
            return this.sections;
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
                        this.sections.Add(section);
                        num++;
                    }
                    else if (this.IsProperty(line) && (section != null))
                    {
                        INIProperty prop = new INIProperty();
                        prop.Name = this.ExtractPropertyName(line);
                        prop.Value = this.ExtractPropertyValue(line);
                        this.sections[num].AddProperty(prop);
                    }
                }
            }
            reader.Close();
        }

        public void WriteFile(string name)
        {
            StreamWriter writer = File.CreateText(name);
            List<INISection>.Enumerator enumerator = this.sections.GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    INISection section = enumerator.Current;
                    writer.WriteLine("[" + section.Name + "]");
                    List<INIProperty>.Enumerator enumerator2 = section.GetPropertys().GetEnumerator();
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
    }
}

