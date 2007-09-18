using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml.Serialization;

namespace INIEditor.BackEnd.XML
{
    public class Helper
    {
        public static INISettings LoadObjectFromXMLFile(string path)
        {
            INISettings iniSettings = null;
            try
            {
                // Use Unicode (=UTF-16) encoding for German "Umlaute"
                using (StreamReader s = new StreamReader(@path, Encoding.Unicode))
                {
                    // typeof (root of the xml file)
                    XmlSerializer xmlSerial = new XmlSerializer(typeof(INISettings));
                    iniSettings = (INISettings) xmlSerial.Deserialize(s);

                    return iniSettings;
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }

            return iniSettings;
        }

        public static void SaveObjectToXMLFile(INISettings iniSettings, string path)
        {
            try
            {
                using (StreamWriter sw = new StreamWriter(@path, false, Encoding.Unicode))
                {
                    XmlSerializer xmlSerial = new XmlSerializer(typeof (INISettings));
                    xmlSerial.Serialize(sw, iniSettings);
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
        }
    }
}
