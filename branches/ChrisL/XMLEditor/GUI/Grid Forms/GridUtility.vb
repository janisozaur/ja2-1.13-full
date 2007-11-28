Module GridUtility
    Friend Const LookupColumnSuffix As String = "_Lkp"
    Friend Sub InitializeGrid(ByVal grid As DataGridView, ByVal view As DataView, Optional ByVal subTable As String = Nothing)
        grid.DataSource = view
        If grid.Columns.Count = 0 Then Exit Sub
        AddHandler grid.DataError, AddressOf Grid_DataError
        For Each c As DataColumn In view.Table.Columns
            Application.DoEvents()
            With grid.Columns(c.ColumnName)
                .HeaderText = c.Caption
                .AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells
                .Resizable = DataGridViewTriState.True
                .Visible = Not GetBooleanProperty(c, ColumnProperty.Grid_Hidden)

                If .Visible Then
                    If GetBooleanProperty(c, ColumnProperty.SubTable) Then
                        If subTable Is Nothing OrElse subTable.Length = 0 OrElse Not c.ColumnName.Contains(subTable) Then
                            .Visible = False
                            Continue For
                        End If
                    End If

                    .ToolTipText = c.ColumnName 'GetStringProperty(c, ColumnProperty.ToolTip)

                    Dim lookupTable As String = GetStringProperty(c, ColumnProperty.Lookup_Table)
                    If Not lookupTable Is Nothing Then
                        Dim lookupValueColumn As String = GetStringProperty(c, ColumnProperty.Lookup_ValueColumn)
                        Dim lookupTextColumn As String = GetStringProperty(c, ColumnProperty.Lookup_TextColumn)
                        Dim lookupFilter As String = GetStringProperty(c, ColumnProperty.Lookup_Filter)

                        'create a new column with the lookup values in it
                        Dim cbc As New DataGridViewComboBoxColumn()
                        cbc.AutoSizeMode = .AutoSizeMode
                        cbc.HeaderText = .HeaderText
                        cbc.Name = .Name & LookupColumnSuffix
                        cbc.DataPropertyName = .DataPropertyName
                        cbc.ToolTipText = .ToolTipText
                        cbc.SortMode = .SortMode
                        cbc.ValueMember = lookupValueColumn
                        cbc.DisplayMember = lookupTextColumn
                        cbc.DataSource = New DataView(view.Table.DataSet.Tables(lookupTable), lookupFilter, lookupTextColumn, DataViewRowState.CurrentRows)
                        If grid.ReadOnly Then cbc.DisplayStyle = DataGridViewComboBoxDisplayStyle.Nothing Else cbc.DisplayStyle = DataGridViewComboBoxDisplayStyle.ComboBox
                        cbc.AutoComplete = True
                        grid.Columns.Insert(.Index, cbc)
                        cbc.Visible = .Visible
                        .Visible = False
                    End If
                End If
            End With
        Next
    End Sub

    Friend Sub DeleteGridRow(ByVal grid As DataGridView, ByVal e As System.Windows.Forms.DataGridViewRowCancelEventArgs)
        If grid.SelectionMode = DataGridViewSelectionMode.FullRowSelect AndAlso Not e.Row.IsNewRow Then
            Dim resp As DialogResult = MessageBox.Show("Are you sure you want to delete row #" & e.Row.Index + 1 & "?", "Delete Row", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2)
            If resp = Windows.Forms.DialogResult.No Then
                e.Cancel = True
            End If
        End If
    End Sub

    Friend Sub Grid_DataError(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewDataErrorEventArgs)
        If e.Exception.GetType.Equals(GetType(ArgumentException)) Then
            e.Cancel = True 'lame I know, but it makes the comboboxes work
        ElseIf e.Exception.GetType.Equals(GetType(ConstraintException)) Then
            ErrorHandler.ShowError("This value has already been used. Please enter a different value.", MessageBoxIcon.Exclamation)
        Else
            ErrorHandler.ShowError(e.Exception.Message, MessageBoxIcon.Error)
        End If
    End Sub

    Friend Function GetActualGridHeight(ByVal grid As DataGridView) As Integer
        Dim height As Integer = grid.RowCount * grid.RowTemplate.Height
        height += grid.ColumnHeadersHeight + 30
        Return height
    End Function

    Friend Function GetActualGridWidth(ByVal grid As DataGridView) As Integer
        Dim width As Integer = grid.RowHeadersWidth + 10
        For Each c As DataGridViewColumn In grid.Columns
            If c.Visible Then width += c.Width
        Next
        Return width
    End Function
End Module
