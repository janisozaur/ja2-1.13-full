Public Class ItemDataForm
    Protected view As DataView
    Protected id As Integer

    Public Sub New(ByVal itemID As Integer, ByVal formText As String)
        LoadingForm.Show()
        Application.DoEvents()
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.AcceptButton = OKButton
        Me.Text = formText
        id = itemID

        Initialize()
    End Sub

    Protected Sub Initialize()

        Bind(id)

        ClassNameLabel.DataBindings.Add("Text", usItemClassComboBox, "Text")

        GraphicTypeCombo.SelectedIndex = view(0)(Tables.Items.Fields.GraphicType)
        GraphicIndexUpDown.Maximum = ItemImages.SmallItems(GraphicTypeCombo.SelectedIndex).Length - 1
        GraphicIndexUpDown.Value = view(0)(Tables.Items.Fields.GraphicIndex)

        DisplayTabs()

        SetupGrid(AttachmentGrid, Tables.Attachments.Name, Tables.Attachments.Fields.ItemID)
        SetupGrid(AttachmentInfoGrid, Tables.AttachmentInfo.Name, Tables.AttachmentInfo.Fields.ItemID)
        SetupGrid(AttachToGrid, Tables.Attachments.Name, Tables.Attachments.Fields.AttachmentID)
        SetupGrid(IncompatibleAttachmentGrid, Tables.IncompatibleAttachments.Name, Tables.IncompatibleAttachments.Fields.ItemID)
        SetupGrid(LaunchableGrid, Tables.Launchables.Name, Tables.Launchables.Fields.ItemID)
        SetupGrid(LauncherGrid, Tables.Launchables.Name, Tables.Launchables.Fields.LaunchableID)
        SetupGrid(CompatibleFaceItemGrid, Tables.CompatibleFaceItems.Name, Tables.CompatibleFaceItems.Fields.ItemID)

        LoadInventoryData()
    End Sub

#Region " Graphics Tab "
    Protected Sub LoadImages(ByVal type As Integer, ByVal index As Integer)
        SmallItemImage.Image = ItemImages.SmallItemImage(type, index)
        MediumItemImage.Image = ItemImages.MediumItemImage(type, index)
        BigItemImage.Image = ItemImages.BigItemImage(type, index)
    End Sub

    Private Sub GraphicIndexUpDown_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles GraphicIndexUpDown.ValueChanged
        LoadImages(GraphicTypeCombo.SelectedIndex, GraphicIndexUpDown.Value)
    End Sub

    Private Sub GraphicTypeCombo_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles GraphicTypeCombo.SelectedIndexChanged
        GraphicIndexUpDown.Maximum = ItemImages.SmallItems(GraphicTypeCombo.SelectedIndex).Length - 1
        LoadImages(GraphicTypeCombo.SelectedIndex, GraphicIndexUpDown.Value)
        ImageListBox.DataSource = ItemImages.BigItems(GraphicTypeCombo.SelectedIndex)
    End Sub

    Private Sub ImageListBox_MeasureItem(ByVal sender As Object, ByVal e As System.Windows.Forms.MeasureItemEventArgs) Handles ImageListBox.MeasureItem
        If e.Index < 0 Then Return

        Dim img As Image = CType(ImageListBox.Items(e.Index), Image)
        Dim hgt As Single = Math.Max(img.Height, e.Graphics.MeasureString("1", ImageListBox.Font).Height) * 1.1
        e.ItemHeight = hgt
        e.ItemWidth = ImageListBox.Width
    End Sub

    Private Sub ImageListBox_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ImageListBox.SelectedIndexChanged
        GraphicIndexUpDown.Value = ImageListBox.SelectedIndex
    End Sub

    Private Sub ImageListBox_DrawItem(ByVal sender As Object, ByVal e As System.Windows.Forms.DrawItemEventArgs) Handles ImageListBox.DrawItem
        Dim g As Graphics = e.Graphics
        If e.State = DrawItemState.Selected Then
            'fill the background of the item 
            g.FillRectangle(Brushes.Blue, e.Bounds)
            'draw the image from the image list control, offset it by 5 pixels and makes sure it's centered vertically 
            Dim myImage As Bitmap = CType(ImageListBox.Items(e.Index), Bitmap)
            g.DrawImage(myImage, 5, e.Bounds.Top + (e.Bounds.Height - myImage.Height) \ 2)
        Else
            'this block does the same thing as above but uses different colors to represent the different state. 
            Dim myImage As Bitmap = CType(ImageListBox.Items(e.Index), Bitmap)
            g.FillRectangle(Brushes.White, e.Bounds)
            g.DrawImage(myImage, 5, e.Bounds.Top + (e.Bounds.Height - myImage.Height) \ 2)
        End If
    End Sub
#End Region

#Region " Descriptions Tab "
    Private Sub DescriptionTextBox_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DescriptionTextBox.TextChanged
        DescriptionCharsLeftLabel.Text = DescriptionTextBox.MaxLength - DescriptionTextBox.TextLength
    End Sub

    Private Sub BRDescriptionTextBox_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BRDescriptionTextBox.TextChanged
        BRDescriptionCharsLeftLabel.Text = BRDescriptionTextBox.MaxLength - BRDescriptionTextBox.TextLength
    End Sub

    Private Sub GermanDescriptionTextBox_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles GermanDescriptionTextBox.TextChanged
        GermanDescriptionCharsLeftLabel.Text = GermanDescriptionTextBox.MaxLength - GermanDescriptionTextBox.TextLength
    End Sub

    Private Sub GermanBRDescriptionTextBox_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles GermanBRDescriptionTextBox.TextChanged
        GermanBRDescriptionCharsLeftLabel.Text = GermanBRDescriptionTextBox.MaxLength - GermanBRDescriptionTextBox.TextLength
    End Sub

    Private Sub RussianDescriptionTextBox_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RussianDescriptionTextBox.TextChanged
        RussianDescriptionCharsLeftLabel.Text = RussianDescriptionTextBox.MaxLength - RussianDescriptionTextBox.TextLength
    End Sub

    Private Sub RussianBRDescriptionTextBox_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RussianBRDescriptionTextBox.TextChanged
        RussianBRDescriptionCharsLeftLabel.Text = RussianBRDescriptionTextBox.MaxLength - RussianBRDescriptionTextBox.TextLength
    End Sub
#End Region

#Region " Weapons Tab "
    Private Sub ubShotsPer4TurnsUpDown_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ubShotsPer4TurnsUpDown.ValueChanged
        If ubShotsPer4TurnsUpDown.Value > 0 Then
            Dim val As Integer = Math.Round(89 / ubShotsPer4TurnsUpDown.Value, 0)
            Me.APsLabel.Text = "= " & val & " APs"
        End If
    End Sub
#End Region

#Region " Data Binding "
    Protected Sub Bind(ByVal itemID As Integer)
        view = New DataView(DB.Table(Tables.Items.Name), Tables.Items.Fields.ID & "=" & itemID, "", DataViewRowState.CurrentRows)
        view(0).BeginEdit()
        BindControls(CType(Me, Control))
    End Sub

    Protected Sub BindControls(ByVal parent As Control)
        For Each ctl As Control In parent.Controls
            If ctl.Tag IsNot Nothing Then
                If TypeOf ctl Is TextBox OrElse TypeOf ctl Is Label OrElse TypeOf ctl Is MaskedTextBox Then
                    ctl.DataBindings.Add("Text", view, ctl.Tag)
                ElseIf TypeOf ctl Is CheckBox OrElse TypeOf ctl Is RadioButton Then
                    ctl.DataBindings.Add("Checked", view, ctl.Tag)
                ElseIf TypeOf ctl Is NumericUpDown Then
                    ctl.DataBindings.Add("Value", view, ctl.Tag)
                ElseIf TypeOf ctl Is PictureBox Then
                    ctl.DataBindings.Add("Image", view, ctl.Tag)
                ElseIf TypeOf ctl Is ComboBox Then
                    ctl.DataBindings.Add("SelectedValue", view, ctl.Tag)

                    Dim lookupTable As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_Table)
                    If lookupTable IsNot Nothing Then
                        Dim lookupTextField As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_TextColumn)
                        Dim lookupValueField As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_ValueColumn)
                        Dim lookupFilter As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_Filter)
                        Dim lookupSort As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.Lookup_Sort)
                        If lookupSort Is Nothing Then lookupSort = lookupTextField

                        Dim cbo As ComboBox = DirectCast(ctl, ComboBox)
                        cbo.ValueMember = lookupValueField
                        cbo.DisplayMember = lookupTextField
                        cbo.DataSource = New DataView(DB.Table(lookupTable), lookupFilter, lookupSort, DataViewRowState.CurrentRows)
                    End If
                End If

                Dim tooltip As String = GetStringProperty(view.Table.Columns(ctl.Tag), ColumnProperty.ToolTip)
                If tooltip IsNot Nothing Then ItemToolTip.SetToolTip(ctl, tooltip)
            End If

            If ctl.Controls.Count > 0 Then
                BindControls(ctl)
            End If
        Next
    End Sub

    Protected Function CommitData() As Boolean
        Try
            Dim newID As Integer = view(0)(Tables.Items.Fields.ID)
            Dim oldID As Integer = view(0).Row(Tables.Items.Fields.ID, DataRowVersion.Current)
            Dim otherItem As DataRow = view.Table.Rows.Find(newID)

            view(0)(Tables.Items.Fields.ItemImage) = BigItemImage.Image
            view(0)(Tables.Items.Fields.GraphicIndex) = GraphicIndexUpDown.Value
            view(0)(Tables.Items.Fields.GraphicType) = GraphicTypeCombo.SelectedIndex

            If otherItem IsNot Nothing AndAlso otherItem IsNot view(0).Row Then
                If MessageBox.Show("The Item ID you have entered is already being used by """ & otherItem(Tables.Items.Fields.Name) & """.  Do you want to swap IDs?", "Swap IDs", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1) = Windows.Forms.DialogResult.Yes Then
                    'swap ids
                    otherItem(Tables.Items.Fields.ID) = -1
                    view(0).EndEdit()
                    otherItem(Tables.Items.Fields.ID) = oldID
                    id = newID
                    view.RowFilter = Tables.Items.Fields.ID & "=" & id
                    Me.Text = "Item - " & id & " - " & view(0)(Tables.Items.Fields.Name)
                    SaveInventoryData()
                Else
                    ErrorHandler.ShowWarning("Please enter a different ID value.", MessageBoxIcon.Exclamation)
                    Return False
                End If
            Else
                view(0).EndEdit()
                SaveInventoryData()
            End If

            view(0).Row.AcceptChanges()
            AcceptGridChanges(AttachmentGrid)
            AcceptGridChanges(AttachToGrid)
            AcceptGridChanges(IncompatibleAttachmentGrid)
            AcceptGridChanges(LaunchableGrid)
            AcceptGridChanges(LauncherGrid)
            AcceptGridChanges(AttachmentInfoGrid)
            AcceptGridChanges(CompatibleFaceItemGrid)
            Return True
        Catch ex As ConstraintException
            ErrorHandler.ShowError("One or more values must be unique. Please enter a different value(s).", MessageBoxIcon.Exclamation, ex)
        Catch ex As Exception
            ErrorHandler.ShowError(ex)
        End Try

    End Function

    Protected Sub CancelData()
        If view.Count > 0 Then view(0).CancelEdit()
        CancelGridChanges(AttachmentGrid)
        CancelGridChanges(AttachToGrid)
        CancelGridChanges(IncompatibleAttachmentGrid)
        CancelGridChanges(LaunchableGrid)
        CancelGridChanges(LauncherGrid)
        CancelGridChanges(AttachmentInfoGrid)
        CancelGridChanges(CompatibleFaceItemGrid)
        view.Dispose()
    End Sub
#End Region

#Region " Buttons "

    Private Sub ApplyButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ApplyButton.Click
        If CommitData() Then
            GraphicIndexUpDown.Value = view(0)(Tables.Items.Fields.GraphicIndex)
            view(0).BeginEdit()
        End If
    End Sub

    Private Sub OKButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OKButton.Click
        If CommitData() Then
            Close()
            view.Dispose()
        End If
    End Sub

    Private Sub CancelButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CancelButton1.Click
        CancelData()
        Close()
    End Sub

    Private Sub ChangeClassButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ChangeClassButton.Click
        Dim frm As New ChangeClassForm(view, Me)
        frm.ShowDialog()
    End Sub

    Private Sub ItemDataForm_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If e.CloseReason = CloseReason.UserClosing AndAlso Me.DialogResult <> Windows.Forms.DialogResult.OK Then
            CancelData()
        End If
    End Sub

#End Region

#Region " Grids "
    Protected Sub SetupGrid(ByVal grid As DataGridView, ByVal tableName As String, Optional ByVal itemIndexField As String = Nothing)
        Dim t As DataTable = DB.Table(tableName)
        Dim rowFilter As String = Nothing
        If itemIndexField IsNot Nothing Then
            rowFilter = itemIndexField & "=" & id
        End If

        InitializeGrid(grid, New DataView(t, rowFilter, "", DataViewRowState.CurrentRows))

        If grid.Columns.Contains(grid.Columns(itemIndexField & LookupColumnSuffix)) Then
            grid.Columns(itemIndexField & LookupColumnSuffix).Visible = False
        End If

        grid.Tag = itemIndexField

        AddHandler grid.UserDeletingRow, AddressOf Grid_UserDeletingRow
        AddHandler grid.DefaultValuesNeeded, AddressOf Grid_DefaultValuesNeeded
    End Sub

    Protected Sub Grid_UserDeletingRow(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowCancelEventArgs)
        DeleteGridRow(DirectCast(sender, DataGridView), e)
    End Sub

    Protected Sub Grid_DefaultValuesNeeded(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowEventArgs)
        Dim grid As DataGridView = DirectCast(sender, DataGridView)
        Dim itemIndexField As String = grid.Tag
        e.Row.Cells(itemIndexField).Value = id

        'set primary key for single key grids, like the attachmentinfo one
        Dim v As DataView = DirectCast(grid.DataSource, DataView)
        If v.Table.PrimaryKey.Length = 1 Then
            Dim key As String = v.Table.PrimaryKey(0).ColumnName
            Dim val As Integer = DB.GetNextPrimaryKeyValue(v.Table)
            e.Row.Cells(key).Value = val
        End If
    End Sub

    Protected Sub CancelGridChanges(ByVal grid As DataGridView)
        Dim v As DataView = DirectCast(grid.DataSource, DataView)
        v.RowStateFilter = DataViewRowState.Added Or DataViewRowState.ModifiedCurrent Or DataViewRowState.Deleted
        For i As Integer = v.Count - 1 To 0 Step -1
            If v(i).Row.RowState <> DataRowState.Detached Then v(i).Row.RejectChanges()
        Next
        v.RowStateFilter = DataViewRowState.CurrentRows
    End Sub

    Protected Sub AcceptGridChanges(ByVal grid As DataGridView)
        Dim v As DataView = DirectCast(grid.DataSource, DataView)
        v.RowStateFilter = DataViewRowState.Added Or DataViewRowState.ModifiedCurrent Or DataViewRowState.Deleted
        For i As Integer = v.Count - 1 To 0 Step -1
            v(i).Row.AcceptChanges()
        Next
        v.RowStateFilter = DataViewRowState.CurrentRows
    End Sub
#End Region

#Region " Tabs "
    Protected Sub DisplayTabs()
        With ItemTab.TabPages
            Select Case view(0)(Tables.Items.Fields.ItemClass)
                Case ItemClass.Ammo
                    .Remove(WeaponPage)
                    .Remove(ArmourPage)
                    .Remove(ExplosivePage)
                    .Remove(AttachmentPage)
                    .Remove(FacePage)
                Case ItemClass.Armour
                    .Remove(WeaponPage)
                    .Remove(AmmoPage)
                    .Remove(ExplosivePage)
                    .Remove(FacePage)
                Case ItemClass.Bomb, ItemClass.Grenade
                    .Remove(WeaponPage)
                    .Remove(ArmourPage)
                    .Remove(AmmoPage)
                    .Remove(FacePage)
                Case ItemClass.Gun
                    WeaponTab.TabPages.Remove(LauncherPage)
                    .Remove(AmmoPage)
                    .Remove(ArmourPage)
                    .Remove(ExplosivePage)
                    .Remove(AttachmentDataPage)
                    .Remove(FacePage)
                Case ItemClass.Launcher
                    .Remove(AmmoPage)
                    .Remove(ArmourPage)
                    .Remove(ExplosivePage)
                    .Remove(FacePage)
                Case ItemClass.Knife, ItemClass.ThrowingKnife, ItemClass.Thrown, ItemClass.Tentacle, ItemClass.Punch
                    WeaponTab.TabPages.Remove(LauncherPage)
                    WeaponTab.TabPages.Remove(GunPage)
                    .Remove(AmmoPage)
                    .Remove(ArmourPage)
                    .Remove(ExplosivePage)
                    .Remove(FacePage)
                Case ItemClass.Face
                    .Remove(AmmoPage)
                    .Remove(ArmourPage)
                    .Remove(ExplosivePage)
                    .Remove(WeaponPage)
                Case ItemClass.Kit, ItemClass.MedKit, ItemClass.Misc
                    .Remove(AmmoPage)
                    .Remove(ArmourPage)
                    .Remove(ExplosivePage)
                    .Remove(WeaponPage)
                    .Remove(FacePage)
                Case ItemClass.Key
                    .Remove(AttachmentPage)
                    .Remove(AttachmentDataPage)
                    .Remove(BonusesPage)
                    .Remove(AbilitiesPage)
                    .Remove(AttributesPage)
                    .Remove(WeaponPage)
                    .Remove(AmmoPage)
                    .Remove(ArmourPage)
                    .Remove(ExplosivePage)
                    .Remove(FacePage)
                Case Else 'ItemClass.None, ItemClass.Money
                    .Remove(AttachmentPage)
                    .Remove(AttachmentDataPage)
                    .Remove(BonusesPage)
                    .Remove(AbilitiesPage)
                    .Remove(AttributesPage)
                    .Remove(InventoryPage)
                    .Remove(WeaponPage)
                    .Remove(AmmoPage)
                    .Remove(ArmourPage)
                    .Remove(ExplosivePage)
                    .Remove(FacePage)
            End Select
        End With
    End Sub
#End Region

#Region " Shared methods "
    Public Shared Sub OpenItemDataForm(ByVal id As Integer, ByVal name As String)
        Dim formText As String = "Item - " & id & " - " & name
        If Not MainWindow.FormOpen(formText) Then
            Dim frm As New ItemDataForm(id, formText)
            MainWindow.ShowForm(frm)
        End If
    End Sub
#End Region

#Region " Shopkeeper Inventories "

    Private Sub InventoryCheckBox_CheckedChanged(ByVal sender As Object, ByVal e As System.EventArgs)
        Dim cb As CheckBox = DirectCast(sender, CheckBox)
        ShopkeepersPanel.Controls(cb.Text & "UpDown").Enabled = cb.Checked
    End Sub

    Protected Sub LoadInventoryData()
        LoadShopkeeperData(ShopKeepers.Alberto)
        LoadShopkeeperData(ShopKeepers.Arnie)
        LoadShopkeeperData(ShopKeepers.Carlo)
        LoadShopkeeperData(ShopKeepers.Devin)
        LoadShopkeeperData(ShopKeepers.Elgin)
        LoadShopkeeperData(ShopKeepers.Frank)
        LoadShopkeeperData(ShopKeepers.Franz)
        LoadShopkeeperData(ShopKeepers.Fredo)
        LoadShopkeeperData(ShopKeepers.Gabby)
        LoadShopkeeperData(ShopKeepers.Herve)
        LoadShopkeeperData(ShopKeepers.Howard)
        LoadShopkeeperData(ShopKeepers.Jake)
        LoadShopkeeperData(ShopKeepers.Keith)
        LoadShopkeeperData(ShopKeepers.Manny)
        LoadShopkeeperData(ShopKeepers.Mickey)
        LoadShopkeeperData(ShopKeepers.Perko)
        LoadShopkeeperData(ShopKeepers.Peter)
        LoadShopkeeperData(ShopKeepers.Sam)
        LoadShopkeeperData(ShopKeepers.Tony)
    End Sub

    Protected Sub SaveInventoryData()
        SaveShopkeeperData(ShopKeepers.Alberto)
        SaveShopkeeperData(ShopKeepers.Arnie)
        SaveShopkeeperData(ShopKeepers.Carlo)
        SaveShopkeeperData(ShopKeepers.Devin)
        SaveShopkeeperData(ShopKeepers.Elgin)
        SaveShopkeeperData(ShopKeepers.Frank)
        SaveShopkeeperData(ShopKeepers.Franz)
        SaveShopkeeperData(ShopKeepers.Fredo)
        SaveShopkeeperData(ShopKeepers.Gabby)
        SaveShopkeeperData(ShopKeepers.Herve)
        SaveShopkeeperData(ShopKeepers.Howard)
        SaveShopkeeperData(ShopKeepers.Jake)
        SaveShopkeeperData(ShopKeepers.Keith)
        SaveShopkeeperData(ShopKeepers.Manny)
        SaveShopkeeperData(ShopKeepers.Mickey)
        SaveShopkeeperData(ShopKeepers.Perko)
        SaveShopkeeperData(ShopKeepers.Peter)
        SaveShopkeeperData(ShopKeepers.Sam)
        SaveShopkeeperData(ShopKeepers.Tony)
    End Sub

    Protected Sub LoadShopkeeperData(ByVal shopKeeperName As String)
        Dim rows() As DataRow = DB.Table(shopKeeperName & Tables.Inventory).Select(Tables.InventoryTableFields.ItemID & "=" & id)
        Dim cb As CheckBox = DirectCast(ShopkeepersPanel.Controls(shopKeeperName & "CheckBox"), CheckBox)
        Dim ud As NumericUpDown = DirectCast(ShopkeepersPanel.Controls(shopKeeperName & "UpDown"), NumericUpDown)

        If rows.Length > 0 Then
            cb.Checked = True
            ud.Value = rows(0)(Tables.InventoryTableFields.Quantity)
            ud.Enabled = True
        Else
            cb.Checked = False
            ud.Value = DB.Table(shopKeeperName & Tables.Inventory).Columns(Tables.InventoryTableFields.Quantity).DefaultValue
            ud.Enabled = False
        End If

        AddHandler cb.CheckedChanged, AddressOf InventoryCheckBox_CheckedChanged
    End Sub

    Protected Sub SaveShopkeeperData(ByVal shopKeeperName As String)
        Dim rows() As DataRow = DB.Table(shopKeeperName & Tables.Inventory).Select(Tables.InventoryTableFields.ItemID & "=" & id)
        Dim cb As CheckBox = DirectCast(ShopkeepersPanel.Controls(shopKeeperName & "CheckBox"), CheckBox)
        Dim ud As NumericUpDown = DirectCast(ShopkeepersPanel.Controls(shopKeeperName & "UpDown"), NumericUpDown)

        If cb.Checked Then
            If rows.Length > 0 Then 'modify
                rows(0)(Tables.InventoryTableFields.Quantity) = ud.Value
            Else 'new
                Dim row As DataRow = DB.NewRow(shopKeeperName & Tables.Inventory)
                'key value is set automatically
                row(Tables.InventoryTableFields.ItemID) = id
                row(Tables.InventoryTableFields.Quantity) = ud.Value
            End If
        Else 'delete
            If rows.Length > 0 Then rows(0).Delete()
        End If
    End Sub
#End Region

#Region " Status Bar "
    Private Sub ItemDataForm_Activated(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Activated
        UpdateStatusBar()
    End Sub

    Private Sub ItemDataForm_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        Me.WindowState = FormWindowState.Normal
        LoadingForm.Close()
    End Sub

    Protected Sub UpdateStatusBar()
        MainWindow.StatusLabel.Text = "Editing " & Me.Text
    End Sub

    Private Sub Form_VisibleChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.VisibleChanged
        If Not Me.Visible Then MainWindow.StatusLabel.Text = ""
    End Sub

    Private Sub uiIndexUpDown_ValueChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles uiIndexUpDown.ValueChanged
        UpdateStatusBar()
    End Sub
#End Region

End Class