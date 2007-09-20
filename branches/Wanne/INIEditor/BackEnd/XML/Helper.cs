using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml.Serialization;

namespace INIEditor.BackEnd.XML
{
    public class Helper
    {
        public static Settings LoadObjectFromXMLFile(string path)
        {
            Settings iniSettings = null;
            try
            {
                // Use Unicode (=UTF-16) encoding for German "Umlaute"
                using (StreamReader s = new StreamReader(@path, Encoding.Unicode))
                {
                    // typeof (root of the xml file)
                    XmlSerializer xmlSerial = new XmlSerializer(typeof(Settings));
                    iniSettings = (Settings) xmlSerial.Deserialize(s);

                    return iniSettings;
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }

            return iniSettings;
        }

        public static void SaveObjectToXMLFile(Settings iniSettings, string path)
        {
            try
            {
                using (StreamWriter sw = new StreamWriter(@path, false, Encoding.Unicode))
                {
                    XmlSerializer xmlSerial = new XmlSerializer(typeof (Settings));
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
