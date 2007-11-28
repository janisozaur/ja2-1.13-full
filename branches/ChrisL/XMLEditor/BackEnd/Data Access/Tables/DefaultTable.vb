Public Class DefaultTable
    Protected tbl As DataTable
    Protected WithEvents db As XmlDB

    Public Sub New(ByVal table As DataTable, ByVal database As XmlDB)
        tbl = table
        db = database
    End Sub

    Public ReadOnly Property Table() As DataTable
        Get
            Return tbl
        End Get
    End Property

    Public Overridable Sub LoadData()
        Const Temp As String = "temp"
        Dim t As DataTable = Nothing
        tbl.BeginLoadData()
        tbl.Clear()
        Dim fileName As String = GetStringProperty(tbl, TableProperty.FileName)
        Dim sourceName As String = GetStringProperty(tbl, TableProperty.SourceTableName)
        If sourceName Is Nothing Then
            tbl.ReadXml(XmlDB.BaseDirectory & fileName)
        Else
            Dim tableName As String = tbl.TableName
            For Each t In tbl.DataSet.Tables
                If t.TableName = sourceName Then
                    t.TableName = Temp
                    Exit For
                End If
            Next

            tbl.TableName = sourceName
            tbl.ReadXml(XmlDB.BaseDirectory & fileName)
            tbl.TableName = tableName
            If t IsNot Nothing AndAlso t.TableName = Temp Then t.TableName = sourceName
        End If
        tbl.EndLoadData()
    End Sub

    Public Overridable Sub ClearData()
        tbl.Clear()
    End Sub

    Public Overridable Sub SaveData()
        SaveData(tbl)
    End Sub

    Protected Overridable Sub SaveData(ByVal table As DataTable)
        If Not table.ExtendedProperties.Contains(TableProperty.SourceTableName) Then
            WriteXml(table, XmlDB.BaseDirectory & table.ExtendedProperties(TableProperty.FileName))
        Else
            Dim t As DataTable = table.Copy
            t.TableName = table.ExtendedProperties(TableProperty.SourceTableName)
            WriteXml(t, XmlDB.BaseDirectory & GetStringProperty(t, TableProperty.FileName))
            t.Dispose()
        End If
    End Sub

    'trim property = true results in all 0/blank values not being written to xml, except for the first entry, which is preserved for reference
    Protected Sub WriteXml(ByVal table As DataTable, ByVal fileName As String)
        'the stupid table.WriteXml method doesn't let you sort the data first
        Dim view As New DataView(table, "", table.Columns(0).ColumnName, DataViewRowState.CurrentRows)
        Dim trim As Boolean = GetBooleanProperty(table, TableProperty.Trim)
        Dim sourceDSName = GetStringProperty(table, TableProperty.DataSetName)
        If sourceDSName Is Nothing Then
            If table.DataSet IsNot Nothing Then
                sourceDSName = table.DataSet.DataSetName
            Else
                sourceDSName = SchemaName
            End If
        End If

        Dim xw As New Xml.XmlTextWriter(fileName, Text.Encoding.UTF8)
        xw.WriteStartDocument()
        xw.WriteWhitespace(vbLf)
        xw.WriteStartElement(sourceDSName)
        xw.WriteWhitespace(vbLf)

        For i As Long = 0 To view.Count - 1
            xw.WriteString(vbTab)
            xw.WriteStartElement(table.TableName)
            xw.WriteString(vbLf)

            For Each c As DataColumn In table.Columns
                If Not trim OrElse (i = 0 OrElse c Is table.PrimaryKey(0) OrElse ((c.DataType.Equals(GetType(Boolean)) OrElse c.DataType.Equals(GetType(Integer))) AndAlso view(i)(c.ColumnName) <> 0) _
                    OrElse (c.DataType.Equals(GetType(String)) AndAlso view(i)(c.ColumnName) <> "")) Then

                    xw.WriteString(vbTab)
                    xw.WriteString(vbTab)
                    If Not c.DataType.Equals(GetType(Boolean)) Then
                        xw.WriteElementString(c.ColumnName, view(i)(c.ColumnName))
                    Else
                        If view(i)(c.ColumnName) Then
                            xw.WriteElementString(c.ColumnName, 1)
                        Else
                            xw.WriteElementString(c.ColumnName, 0)
                        End If
                    End If
                        xw.WriteString(vbLf)
                End If
            Next
            xw.WriteString(vbTab)
            xw.WriteEndElement()
            xw.WriteString(vbLf)
        Next
        xw.WriteEndElement()
        xw.Close()
        view.Dispose()

        table.AcceptChanges()
    End Sub

    'this only works when the pk is an integer, which it should be 99% of the time in our xml files anyway
    'it also just works on single pk tables for now
    Public Overridable Function GetNextPrimaryKeyValue() As Integer
        Dim pk As String = tbl.PrimaryKey(0).ColumnName
        If tbl.Rows.Count > 0 Then
            Return tbl.Compute("MAX(" & pk & ")", Nothing) + 1
        Else
            Return 0
        End If
    End Function

    'only works w/single key values
    Public Overridable Function NewRow() As DataRow
        Dim row As DataRow = tbl.NewRow
        row(tbl.PrimaryKey(0)) = GetNextPrimaryKeyValue()
        tbl.Rows.Add(row)
        Return row
    End Function

    Public Overridable Sub DeleteRow(ByVal key As Integer)
        Dim row As DataRow = tbl.Rows.Find(key)
        If row IsNot Nothing Then row.Delete()
    End Sub

    'only works when there's a single key table at the top of the relation
    Public Overridable Function DuplicateRow(ByVal key As Integer) As DataRow
        Dim row As DataRow = tbl.Rows.Find(key)
        If row IsNot Nothing Then
            Return DuplicateRows(New DataRow() {row}, Nothing, key)
        Else
            Return Nothing
        End If
    End Function

    Protected Function DuplicateRows(ByVal rows() As DataRow, ByVal parentRow As DataRow, ByVal baseKey As Integer) As DataRow
        Dim dupeRow As DataRow = Nothing
        Dim canAddRow As Boolean
        For Each row As DataRow In rows
            canAddRow = False
            dupeRow = row.Table.NewRow
            For Each c As DataColumn In row.Table.Columns
                If Not c.ReadOnly Then dupeRow(c) = row(c)
            Next

            If dupeRow.Table.PrimaryKey.Length = 1 Then
                dupeRow(dupeRow.Table.PrimaryKey(0)) = GetTableHandler(row.Table).GetNextPrimaryKeyValue()
                If parentRow IsNot Nothing Then dupeRow.SetParentRow(parentRow)
                canAddRow = True
            ElseIf parentRow IsNot Nothing Then
                For i As Integer = 0 To dupeRow.Table.PrimaryKey.Length - 1
                    If dupeRow(dupeRow.Table.PrimaryKey(i)) = baseKey Then
                        dupeRow(dupeRow.Table.PrimaryKey(i)) = parentRow(parentRow.Table.PrimaryKey(0))
                        canAddRow = True
                    End If
                Next
            End If
            If canAddRow Then row.Table.Rows.Add(dupeRow)

            For Each dr As DataRelation In row.Table.ChildRelations
                DuplicateRows(row.GetChildRows(dr), dupeRow, baseKey)
            Next
        Next
        Return dupeRow 'return last row copied
    End Function
End Class
