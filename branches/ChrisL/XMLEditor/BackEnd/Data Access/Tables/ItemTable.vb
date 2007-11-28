Public Class ItemTable
    Inherits DefaultTable

    Public Sub New(ByVal table As DataTable, ByVal database As XmlDB)
        MyBase.New(table, database)
    End Sub

    Protected Sub AfterLoadAll() Handles db.AfterLoadAll
        ExtendPrimaryItemTable()
        AddImageColumn()
        PopulateImageColumn()
    End Sub

    Protected Sub BeforeSaveAll() Handles db.BeforeSaveAll
        RebuildExtendedItemTables()
    End Sub

    Protected Sub AfterSaveAll() Handles db.AfterSaveAll
        ClearExtendedItemTables()
    End Sub

    Protected Sub AddImageColumn()
        If Not tbl.Columns.Contains(Tables.Items.Fields.ItemImage) Then
            Dim c As New DataColumn
            c.ColumnName = Tables.Items.Fields.ItemImage
            c.DataType = GetType(System.Drawing.Image)
            c.Caption = "Image"
            tbl.Columns.Add(c)
            c.SetOrdinal(tbl.Columns(Tables.Items.Fields.Name).Ordinal + 1)
        End If

    End Sub

    Public Sub PopulateImageColumn()
        For Each r As DataRow In tbl.Rows
            Application.DoEvents()
            r(Tables.Items.Fields.ItemImage) = ItemImages.BigItemImage(r(Tables.Items.Fields.GraphicType), r(Tables.Items.Fields.GraphicIndex))
        Next
    End Sub

    Protected Sub RemoveImageColumn(ByVal table As DataTable)
        If table.Columns.Contains(Tables.Items.Fields.ItemImage) Then
            table.Columns.Remove(Tables.Items.Fields.ItemImage)
        End If
    End Sub

    Public Sub ExtendPrimaryItemTable()
        'store the values from the 1-to-1 related tables in the items table
        'this includes: magazines, weapons, armour, explosives
        Dim mags As DataTable = db.DataSet.Tables(Tables.Magazines.Name)
        Dim weapons As DataTable = db.DataSet.Tables(Tables.Weapons.Name)
        Dim armours As DataTable = db.DataSet.Tables(Tables.Armours)
        Dim explosives As DataTable = db.DataSet.Tables(Tables.Explosives)
        Dim germanItems As DataTable = db.DataSet.Tables(Tables.GermanItems)
        Dim russianItems As DataTable = db.DataSet.Tables(Tables.RussianItems)

        CopyFromExtendedTable(weapons, Tables.Items.Fields.ItemClass & ">" & ItemClass.None & " AND " & Tables.Items.Fields.ItemClass & "<=" & ItemClass.Punch)
        CopyFromExtendedTable(mags, Tables.Items.Fields.ItemClass & "=" & ItemClass.Ammo)
        CopyFromExtendedTable(armours, Tables.Items.Fields.ItemClass & "=" & ItemClass.Armour)
        CopyFromExtendedTable(explosives, Tables.Items.Fields.ItemClass & "=" & ItemClass.Grenade & " OR " & Tables.Items.Fields.ItemClass & "=" & ItemClass.Bomb)
        CopyFromExtendedTable(germanItems, "", True)
        CopyFromExtendedTable(russianItems, "", True)

        'add expressions to table
        tbl.Columns(Tables.Weapons.Name & Tables.Weapons.Fields.Name).Expression = Tables.Items.Fields.Name

    End Sub

    Public Sub RebuildExtendedItemTables()
        'restore the values to the 1-to-1 related tables from the items table

        Dim mags As DataTable = db.DataSet.Tables(Tables.Magazines.Name)
        Dim weapons As DataTable = db.DataSet.Tables(Tables.Weapons.Name)
        Dim armours As DataTable = db.DataSet.Tables(Tables.Armours)
        Dim explosives As DataTable = db.DataSet.Tables(Tables.Explosives)
        Dim germanItems As DataTable = db.DataSet.Tables(Tables.GermanItems)
        Dim russianItems As DataTable = db.DataSet.Tables(Tables.RussianItems)

        'todo: test!
        CopyToExtendedTable(germanItems, "", Tables.Items.Fields.ID, True, False)
        CopyToExtendedTable(russianItems, "", Tables.Items.Fields.ID, True, False)
        CopyToExtendedTable(explosives, Tables.Items.Fields.ItemClass & "=" & ItemClass.Grenade & " OR " & Tables.Items.Fields.ItemClass & "=" & ItemClass.Bomb, Tables.Items.Fields.ID)
        CopyToExtendedTable(armours, Tables.Items.Fields.ItemClass & "=" & ItemClass.Armour, Tables.Items.Fields.ID)
        CopyToExtendedTable(mags, Tables.Items.Fields.ItemClass & "=" & ItemClass.Ammo, mags.TableName & Tables.Magazines.Fields.Caliber & "," & mags.TableName & Tables.Magazines.Fields.MagSize & "," & mags.TableName & Tables.Magazines.Fields.AmmoType)
        CopyToExtendedTable(weapons, Tables.Items.Fields.ItemClass & ">" & ItemClass.None & " AND " & Tables.Items.Fields.ItemClass & "<=" & ItemClass.Punch, Tables.Items.Fields.ID, True)

    End Sub

    Protected Sub ClearExtendedItemTables()
        With db.DataSet
            .Tables(Tables.Magazines.Name).Clear()
            .Tables(Tables.Weapons.Name).Clear()
            .Tables(Tables.Armours).Clear()
            .Tables(Tables.Explosives).Clear()
            .Tables(Tables.GermanItems).Clear()
            .Tables(Tables.RussianItems).Clear()
        End With
    End Sub

    Protected Sub CopyFromExtendedTable(ByVal extTable As DataTable, ByVal filter As String, Optional ByVal matchIDs As Boolean = False)
        For Each c As DataColumn In extTable.Columns
            Application.DoEvents()
            If c.ColumnName <> Tables.Items.Fields.ID AndAlso Not tbl.Columns.Contains(extTable.TableName & c.ColumnName) Then
                Dim newCol As DataColumn = CopyColumn(c)
                newCol.ExtendedProperties.Add(ColumnProperty.SubTable, True)
                newCol.ColumnName = extTable.TableName & newCol.ColumnName
                tbl.Columns.Add(newCol)
            End If
        Next

        Dim itemView As New DataView(tbl, filter, Tables.Items.Fields.FKey, DataViewRowState.CurrentRows)

        For i As Integer = 0 To itemView.Count - 1
            Application.DoEvents()
            Dim key As Integer
            If Not matchIDs Then
                key = itemView(i)(Tables.Items.Fields.FKey)
            Else
                key = itemView(i)(Tables.Items.Fields.ID)
            End If
            Dim r As DataRow = extTable.Rows.Find(key)
            If Not r Is Nothing Then
                For Each c As DataColumn In extTable.Columns
                    If c.ColumnName <> Tables.Items.Fields.ID AndAlso Not tbl.Columns(extTable.TableName & c.ColumnName).ReadOnly Then
                        itemView(i)(extTable.TableName & c.ColumnName) = r(c.ColumnName)
                    End If
                Next
                itemView(i)(Tables.Items.Fields.FKey) = 0
            Else
                'Stop
            End If
        Next

        extTable.Rows.Clear()
        itemView.Dispose()
    End Sub

    Protected Sub CopyToExtendedTable(ByVal extTable As DataTable, ByVal filter As String, ByVal sort As String, Optional ByVal matchIDs As Boolean = False, Optional ByVal useForeignKey As Boolean = True)
        Dim itemView As New DataView(tbl, filter, sort, DataViewRowState.CurrentRows)

        For i As Integer = 0 To itemView.Count - 1
            Application.DoEvents()
            Dim r As DataRow = extTable.NewRow()
            If Not matchIDs Then
                r(extTable.PrimaryKey(0)) = i
                itemView(i)(Tables.Items.Fields.FKey) = i
            Else
                r(extTable.PrimaryKey(0)) = itemView(i)(Tables.Items.Fields.ID)
                If useForeignKey Then itemView(i)(Tables.Items.Fields.FKey) = itemView(i)(Tables.Items.Fields.ID)
            End If

            For Each c As DataColumn In extTable.Columns
                If c.ColumnName <> Tables.Items.Fields.ID Then
                    r(c) = itemView(i)(extTable.TableName & c.ColumnName)
                End If
            Next

            extTable.Rows.Add(r)
        Next

        itemView.Dispose()
    End Sub

    Protected Sub RemoveExtendedColumns(ByVal itemTable As DataTable)
        For i As Integer = itemTable.Columns.Count - 1 To 0 Step -1
            Application.DoEvents()
            If GetBooleanProperty(itemTable.Columns(i), ColumnProperty.SubTable) Then
                itemTable.Columns.RemoveAt(i)
            End If
        Next
    End Sub

    Public Overrides Sub SaveData()
        Dim table As DataTable = tbl.Copy
        RemoveImageColumn(table)
        RemoveExtendedColumns(table)
        SaveData(table)
        tbl.AcceptChanges()
        table.Clear()
        table.Dispose()
    End Sub


    Public Overrides Sub DeleteRow(ByVal key As Integer)
        Dim row As DataRow = tbl.Rows.Find(key)
        Dim nothingRow As DataRow = tbl.Rows.Find(0)
        If row IsNot Nothing Then
            Dim maxID = tbl.Compute("MAX(" & tbl.PrimaryKey(0).ColumnName & ")", Nothing)
            If row(Tables.Items.Fields.ID) = maxID Then 'if this is the last record, then we can delete it
                row.Delete()
            Else 'otherwise, we need to turn this into a nothing item, 
                'because JA2 will stop looking for items once it hits this one's zero value itemclass
                For Each c As DataColumn In tbl.Columns
                    If Not c.ReadOnly AndAlso c IsNot tbl.PrimaryKey(0) Then row(c) = nothingRow(c)
                Next
                row(Tables.Items.Fields.ItemClass) = ItemClass.None
                row(Tables.Items.Fields.Description) = "Index " & row(Tables.Items.Fields.ID)
            End If
        End If
    End Sub

End Class
