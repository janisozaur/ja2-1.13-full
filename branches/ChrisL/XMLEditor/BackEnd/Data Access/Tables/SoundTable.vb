Public Class SoundTable
    Inherits AutoIncrementTable

    Public Sub New(ByVal table As DataTable, ByVal database As XmlDB)
        MyBase.New(table, database)
    End Sub


    Public Overrides Sub LoadData()
        tbl.Clear()
        Dim xr As New Xml.XmlTextReader(XmlDB.BaseDirectory & GetStringProperty(tbl, TableProperty.FileName))
        While xr.Read
            If xr.NodeType = Xml.XmlNodeType.Text Then
                Dim r As DataRow = tbl.NewRow
                r(Tables.LookupTableFields.Name) = xr.Value
                tbl.Rows.Add(r)
            End If
        End While
        xr.Close()
    End Sub

    Public Overrides Sub SaveData()
        Dim xw As New Xml.XmlTextWriter(XmlDB.BaseDirectory & GetStringProperty(tbl, TableProperty.FileName), Text.Encoding.UTF8)
        xw.WriteStartElement(GetStringProperty(tbl, TableProperty.DataSetName))
        xw.WriteString(vbLf)
        For Each r As DataRow In tbl.Rows
            xw.WriteString(vbTab)
            xw.WriteElementString(Tables.Sounds, r(Tables.LookupTableFields.Name))
            xw.WriteString(vbLf)
        Next
        xw.WriteEndElement()
        xw.WriteString(vbLf)
        xw.Close()
        tbl.AcceptChanges()
    End Sub
End Class
