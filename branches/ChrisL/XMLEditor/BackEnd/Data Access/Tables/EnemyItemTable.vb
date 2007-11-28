Public Class EnemyItemTable
    Inherits DefaultTable
    Const AmmoIndex As Integer = 19 'the key of the ammo row
    Protected ammoTable As DataTable 'need a 2nd table to store the ammo data

    Public Sub New(ByVal table As DataTable, ByVal database As XmlDB)
        MyBase.New(table, database)
    End Sub

    Protected Sub AfterLoadAll() Handles db.AfterLoadAll, db.AfterSaveAll
        ammoTable = tbl.DataSet.Tables(Tables.EnemyAmmo)
        Dim ammoRow As DataRow = tbl.Rows.Find(AmmoIndex)
        If ammoRow IsNot Nothing Then
            Dim row As DataRow = ammoTable.NewRow
            row.ItemArray = ammoRow.ItemArray
            ammoTable.Rows.Add(row)
            tbl.Rows.Remove(ammoRow)
        End If
    End Sub

    Protected Sub BeforeSaveAll() Handles db.BeforeSaveAll
        If ammoTable.Rows.Count > 0 Then
            Dim row As DataRow = tbl.NewRow
            row.ItemArray = ammoTable.Rows(0).ItemArray
            tbl.Rows.Add(row)
            ammoTable.Rows.RemoveAt(0)
        End If
    End Sub
End Class
