Imports System.Windows.Forms

Public Class MainForm
    Private WithEvents client As MdiClient

    Private Sub MainForm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        With My.Settings

            Me.BackgroundImageToolStripMenuItem.Checked = .View_Background
            Me.ToolBarToolStripMenuItem.Checked = .View_Toolbar
            Me.StatusBarToolStripMenuItem.Checked = .View_StatusBar

            If .MainForm_Size <> New Size(0, 0) Then Me.Size = .MainForm_Size
            If .MainForm_Location <> New Point(0, 0) Then Me.Location = .MainForm_Location
            Me.WindowState = .MainForm_State
        End With

        For Each ctl As Control In Me.Controls
            If TypeOf ctl Is MdiClient Then
                client = ctl
                Exit For
            End If
        Next
        If Me.BackgroundImageToolStripMenuItem.Checked Then
            client.BackgroundImage = My.Resources.DESKTOP
            client.BackgroundImageLayout = ImageLayout.Stretch
        End If
        StatusLabel.Text = "Welcome to the JA2 1.13 XML Editor!"
    End Sub

    Private Sub MainForm_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        With My.Settings
            .View_Background = Me.BackgroundImageToolStripMenuItem.Checked
            .View_Toolbar = Me.ToolBarToolStripMenuItem.Checked
            .View_StatusBar = Me.StatusBarToolStripMenuItem.Checked
            If Me.WindowState = FormWindowState.Normal Then
                .MainForm_Location = Me.Location
                .MainForm_Size = Me.Size
            End If
            .MainForm_State = Me.WindowState
            .Save()
        End With
    End Sub

    Private Sub client_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles client.Paint
        If client.BackgroundImage IsNot Nothing Then
            Static flag As Boolean
            If Not flag Then
                Dim g As Graphics = e.Graphics
                g.DrawImage(client.BackgroundImage, New RectangleF(0, 0, client.Size.Width, client.Size.Height))
            Else
                flag = True
                client.Invalidate()
            End If
        End If
    End Sub

    Private Sub client_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles client.Resize
        client.Invalidate()
    End Sub

    Private Sub ChildForm_Resized(ByVal sender As Object, ByVal e As EventArgs)
        client.Invalidate()
    End Sub

    Private Sub OpenFile(ByVal sender As Object, ByVal e As EventArgs) Handles OpenToolStripMenuItem.Click, OpenToolStripButton.Click
        Windows.Forms.Cursor.Current = Cursors.WaitCursor
        Splash.Show()
        Splash.UpdateLoadingText("Reloading Data...")
        Me.Hide()
        Application.DoEvents()
        DB.LoadAllData()
        Windows.Forms.Cursor.Current = Cursors.Arrow
        Splash.Hide()
        Me.Show()
    End Sub

    Private Sub SaveAsToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles SaveToolStripMenuItem.Click, SaveToolStripButton.Click
        Windows.Forms.Cursor.Current = Cursors.WaitCursor
        LoadingForm.Show(True)
        Application.DoEvents()
        DB.SaveAllData()
        Windows.Forms.Cursor.Current = Cursors.Arrow
        LoadingForm.Close()
    End Sub

    Friend Function FormOpen(ByVal formText As String) As Boolean
        For Each f As Form In MdiChildren
            If f.Text = formText Then
                f.Activate()
                Return True
            End If
        Next
        Return False
    End Function

    Friend Sub ShowForm(ByVal frm As Form)
        frm.MdiParent = Me
        AddHandler frm.Resize, AddressOf ChildForm_Resized
        frm.Show()
    End Sub

    Private Sub ExitToolsStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles ExitToolStripMenuItem.Click
        Me.Close()
    End Sub

    Private Sub CutToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles CutToolStripMenuItem.Click
        ' Use My.Computer.Clipboard to insert the selected text or images into the clipboard
    End Sub

    Private Sub CopyToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles CopyToolStripMenuItem.Click
        ' Use My.Computer.Clipboard to insert the selected text or images into the clipboard
    End Sub

    Private Sub PasteToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles PasteToolStripMenuItem.Click
        'Use My.Computer.Clipboard.GetText() or My.Computer.Clipboard.GetData to retrieve information from the clipboard.
    End Sub

    Private Sub ToolBarToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles ToolBarToolStripMenuItem.Click
        Me.ToolStrip.Visible = Me.ToolBarToolStripMenuItem.Checked
    End Sub

    Private Sub StatusBarToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles StatusBarToolStripMenuItem.Click
        Me.StatusStrip.Visible = Me.StatusBarToolStripMenuItem.Checked
    End Sub

    Private Sub CascadeToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles CascadeToolStripMenuItem.Click
        Me.LayoutMdi(MdiLayout.Cascade)
    End Sub

    Private Sub TileVerticleToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles TileVerticalToolStripMenuItem.Click
        Me.LayoutMdi(MdiLayout.TileVertical)
    End Sub

    Private Sub TileHorizontalToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles TileHorizontalToolStripMenuItem.Click
        Me.LayoutMdi(MdiLayout.TileHorizontal)
    End Sub

    Private Sub ArrangeIconsToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles ArrangeIconsToolStripMenuItem.Click
        Me.LayoutMdi(MdiLayout.ArrangeIcons)
    End Sub

    Private Sub CloseAllToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles CloseAllToolStripMenuItem.Click
        ' Close all child forms of the parent.
        For Each ChildForm As Form In Me.MdiChildren
            ChildForm.Close()
        Next
    End Sub

    Private Sub ShowItemGridForm(ByVal text As String, ByVal filter As String, ByVal subTable As String, Optional ByVal customFilter As String = Nothing)
        If Not FormOpen(text) Then
            Dim frm As New ItemGridForm(text, New DataView(DB.Table(Tables.Items.Name), filter, Tables.Items.Fields.ID, DataViewRowState.CurrentRows), subTable)
            If customFilter IsNot Nothing AndAlso customFilter.Length > 0 Then frm.Filter = customFilter
            ShowForm(frm)
        End If
    End Sub

    Private Sub ShowLookupGridForm(ByVal text As String, ByVal tableName As String)
        If Not FormOpen(text) Then
            Dim frm As New LookupGridForm(text, New DataView(DB.Table(tableName), "", DB.Table(tableName).PrimaryKey(0).ColumnName, DataViewRowState.CurrentRows))
            ShowForm(frm)
        End If
    End Sub

    Private Sub ShowDataGridForm(ByVal text As String, ByVal tableName As String, Optional ByVal sortByKey As Boolean = True)
        If Not FormOpen(text) Then
            Dim sort As String = ""
            If sortByKey Then sort = DB.Table(tableName).PrimaryKey(0).ColumnName
            Dim frm As New DataGridForm(text, New DataView(DB.Table(tableName), "", sort, DataViewRowState.CurrentRows))
            ShowForm(frm)
        End If
    End Sub

    Private Sub ShowChildDataGridForm(ByVal text As String, ByVal tableName As String, Optional ByVal sortByKey As Boolean = True)
        If Not FormOpen(text) Then
            Dim sort As String = ""
            If sortByKey Then sort = DB.Table(tableName).PrimaryKey(0).ColumnName
            Dim frm As New ChildDataGridForm(text, New DataView(DB.Table(tableName), "", sort, DataViewRowState.CurrentRows))
            ShowForm(frm)
        End If
    End Sub

    Private Sub AllItemsToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles AllItemsToolStripMenuItem.Click
        ShowItemGridForm("Items", "", Nothing)
    End Sub

    Private Sub GunsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles GunsToolStripMenuItem.Click
        ShowItemGridForm("Items - Guns", Tables.Items.Fields.ItemClass & "=" & ItemClass.Gun & " AND " & Tables.Items.Fields.RocketLauncher & "= 0", Tables.Weapons.Name)
    End Sub

    Private Sub AmmoToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AmmoToolStripMenuItem.Click
        ShowItemGridForm("Items - Ammo", Tables.Items.Fields.ItemClass & "=" & ItemClass.Ammo, Tables.Magazines.Name)
    End Sub

    Private Sub LaunchersToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles LaunchersToolStripMenuItem.Click
        ShowItemGridForm("Items - Launchers", Tables.Items.Fields.ItemClass & "=" & ItemClass.Launcher & " OR " & Tables.Items.Fields.RocketLauncher & "= 1", Tables.Weapons.Name)
    End Sub

    Private Sub GrenadesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles GrenadesToolStripMenuItem.Click
        ShowItemGridForm("Items - Grenades", Tables.Items.Fields.ItemClass & "=" & ItemClass.Grenade, Tables.Explosives)
    End Sub

    Private Sub ExplosivesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ExplosivesToolStripMenuItem.Click
        ShowItemGridForm("Items - Explosives", Tables.Items.Fields.ItemClass & "=" & ItemClass.Bomb, Tables.Explosives)
    End Sub

    Private Sub KnivesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles KnivesToolStripMenuItem.Click
        ShowItemGridForm("Items", Tables.Items.Fields.ItemClass & "=" & ItemClass.Knife & " OR " & Tables.Items.Fields.ItemClass & "=" & ItemClass.ThrowingKnife, Tables.Weapons.Name)
    End Sub

    Private Sub OtherWeaponsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OtherWeaponsToolStripMenuItem.Click
        ShowItemGridForm("Items - Other Weapons", Tables.Items.Fields.ItemClass & "=" & ItemClass.Thrown & " OR " & Tables.Items.Fields.ItemClass & "=" & ItemClass.Punch & " OR " & Tables.Items.Fields.ItemClass & "=" & ItemClass.Tentacle, Tables.Weapons.Name)
    End Sub

    Private Sub ArmourToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ArmourToolStripMenuItem.Click
        ShowItemGridForm("Items - Armours", Tables.Items.Fields.ItemClass & "=" & ItemClass.Armour, Tables.Armours)
    End Sub

    Private Sub FaceToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FaceToolStripMenuItem.Click
        ShowItemGridForm("Items - Facial Gear", Tables.Items.Fields.ItemClass & "=" & ItemClass.Face, Nothing)
    End Sub

    Private Sub KitsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles KitsToolStripMenuItem.Click
        ShowItemGridForm("Items - Kits", Tables.Items.Fields.ItemClass & "=" & ItemClass.MedKit & " OR " & Tables.Items.Fields.ItemClass & "=" & ItemClass.Kit, Nothing)
    End Sub

    Private Sub KeysToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles KeysToolStripMenuItem.Click
        ShowItemGridForm("Items - Keys", Tables.Items.Fields.ItemClass & "=" & ItemClass.Key, Nothing)
    End Sub

    Private Sub MiscToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MiscToolStripMenuItem.Click
        ShowItemGridForm("Items - Miscellaneous", Tables.Items.Fields.ItemClass & "=" & ItemClass.Misc & " OR " & Tables.Items.Fields.ItemClass & "=" & ItemClass.Money, Nothing)
    End Sub

    Private Sub NoneItemToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NoneItemToolStripMenuItem.Click
        ShowItemGridForm("Items - None", Tables.Items.Fields.ItemClass & "=" & ItemClass.None, Nothing)
    End Sub

    Private Sub AttachmentsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AttachmentsToolStripMenuItem.Click
        ShowItemGridForm("Items - Attachments", Tables.Items.Fields.Attachment & "= 1", Nothing)
    End Sub

    Private Sub TonsOfGunsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TonsOfGunsToolStripMenuItem.Click
        ShowItemGridForm("Items - Tons of Guns Mode", Tables.Items.Fields.TonsOfGuns & "= 1", Nothing)
    End Sub

    Private Sub NormalGunsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NormalGunsToolStripMenuItem.Click
        ShowItemGridForm("Items - Normal Guns Mode", Tables.Items.Fields.TonsOfGuns & "= 0", Nothing)
    End Sub

    Private Sub SciFiToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SciFiToolStripMenuItem.Click
        ShowItemGridForm("Items - Sci-Fi Mode", Tables.Items.Fields.SciFi & "= 1", Nothing)
    End Sub

    Private Sub NonSciFiToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NonSciFiToolStripMenuItem.Click
        ShowItemGridForm("Items - Normal Mode", Tables.Items.Fields.SciFi & "= 0", Nothing)
    End Sub

    Private Sub ArmourClassesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ArmourClassesToolStripMenuItem.Click
        ShowLookupGridForm("Data - Armour Classes", Tables.ArmourClasses)
    End Sub

    Private Sub CursorsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CursorsToolStripMenuItem.Click
        ShowLookupGridForm("Data - Cursors", Tables.Cursors)
    End Sub

    Private Sub ExplosionTypesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ExplosionTypesToolStripMenuItem.Click
        ShowLookupGridForm("Data - Explosion Types", Tables.ExplosionTypes)
    End Sub

    Private Sub ItemClassesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ItemClassesToolStripMenuItem.Click
        ShowLookupGridForm("Data - Item Classes", Tables.ItemClasses)
    End Sub

    Private Sub MergeTypesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MergeTypesToolStripMenuItem.Click
        ShowLookupGridForm("Data - Merge Types", Tables.MergeTypes)
    End Sub

    Private Sub SkillChecksToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SkillChecksToolStripMenuItem.Click
        ShowLookupGridForm("Data - Skill Checks", Tables.SkillCheckTypes)
    End Sub

    Private Sub WeaponClassesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles WeaponClassesToolStripMenuItem.Click
        ShowLookupGridForm("Data - Weapon Classes", Tables.WeaponClasses)
    End Sub

    Private Sub WeaponTypesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles WeaponTypesToolStripMenuItem.Click
        ShowLookupGridForm("Data - Weapon Types", Tables.WeaponTypes)
    End Sub

    Private Sub ExplosionsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ExplosionsToolStripMenuItem.Click
        ShowDataGridForm("Data - Explosions", Tables.ExplosionData)
    End Sub

    Private Sub TypesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TypesToolStripMenuItem.Click
        ShowDataGridForm("Data - Ammo Types", Tables.AmmoTypes)
    End Sub

    Private Sub StandardToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StandardToolStripMenuItem.Click
        ShowDataGridForm("Data - Sounds", Tables.Sounds)
    End Sub

    Private Sub BurstToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BurstToolStripMenuItem.Click
        ShowDataGridForm("Data - Burst Sounds", Tables.BurstSounds)
    End Sub

    Private Sub ToolStripMenuItem2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripMenuItem2.Click
        Dim frm As New EnterItemIDForm()
        If Not FormOpen(frm.Text) Then ShowForm(frm)
    End Sub

    Private Sub CreateNewToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CreateNewToolStripMenuItem.Click
        Dim frm As New NewItemForm()
        If Not FormOpen(frm.Text) Then ShowForm(frm)
    End Sub

    Private Sub StandardToolStripMenuItem1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StandardToolStripMenuItem1.Click
        ShowDataGridForm("Standard Merges", Tables.Merges, False)
    End Sub

    Private Sub AttachmentToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AttachmentToolStripMenuItem.Click
        ShowDataGridForm("Attachment Merges", Tables.AttachmentComboMerges)
    End Sub

    Private Sub ListingToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ListingToolStripMenuItem.Click
        ShowChildDataGridForm("Attachment Listing", Tables.Attachments.Name, False)
    End Sub

    Private Sub InfoToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InfoToolStripMenuItem.Click
        ShowDataGridForm("Attachment Info", Tables.AttachmentInfo.Name)
    End Sub

    Private Sub IncompatibleToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles IncompatibleToolStripMenuItem.Click
        ShowChildDataGridForm("Incompatible Attachments", Tables.IncompatibleAttachments.Name, False)
    End Sub

    Private Sub LaunchablesToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles LaunchablesToolStripMenuItem.Click
        ShowChildDataGridForm("Launchables", Tables.Launchables.Name, False)
    End Sub

    Private Sub CompatibleFaceItemsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CompatibleFaceItemsToolStripMenuItem.Click
        ShowChildDataGridForm("Compatible Face Items", Tables.CompatibleFaceItems.Name, False)
    End Sub

    Private Sub EnglishToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnglishToolStripMenuItem.Click
        ShowDataGridForm("Data - Ammo Calibers (English)", Tables.AmmoStrings)
    End Sub

    Private Sub GermanToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles GermanToolStripMenuItem.Click
        ShowDataGridForm("Data - Ammo Calibers (German)", Tables.GermanAmmoStrings)
    End Sub

    Private Sub RussianToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RussianToolStripMenuItem.Click
        ShowDataGridForm("Data - Ammo Calibers (Russian)", Tables.RussianAmmoStrings)
    End Sub

    Private Sub AlbertoToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AlbertoToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Alberto", Tables.AlbertoInventory)
    End Sub

    Private Sub ArnieToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ArnieToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Arnie", Tables.ArnieInventory)
    End Sub

    Private Sub CarloToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CarloToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Carlo", Tables.CarloInventory)
    End Sub

    Private Sub DevinToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DevinToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Devin", Tables.DevinInventory)
    End Sub

    Private Sub ElginToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ElginToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Elgin", Tables.ElginInventory)
    End Sub

    Private Sub FrankToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FrankToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Frank", Tables.FrankInventory)
    End Sub

    Private Sub FranzToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FranzToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Franz", Tables.FranzInventory)
    End Sub

    Private Sub FredoToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FredoToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Fredo", Tables.FredoInventory)
    End Sub

    Private Sub GabbyToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles GabbyToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Gabby", Tables.GabbyInventory)
    End Sub

    Private Sub HerveToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles HerveToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Herve", Tables.HerveInventory)
    End Sub

    Private Sub HowardToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles HowardToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Howard", Tables.HowardInventory)
    End Sub

    Private Sub JakeToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles JakeToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Jake", Tables.JakeInventory)
    End Sub

    Private Sub KeithToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles KeithToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Keith", Tables.KeithInventory)
    End Sub

    Private Sub ToolStripMenuItem5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolStripMenuItem5.Click
        ShowDataGridForm("Inventory - Manny", Tables.MannyInventory)
    End Sub

    Private Sub MickeyToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MickeyToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Mickey", Tables.MickeyInventory)
    End Sub

    Private Sub PerkoToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PerkoToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Perko", Tables.PerkoInventory)
    End Sub

    Private Sub PeterToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PeterToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Peter", Tables.PeterInventory)
    End Sub

    Private Sub SamToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SamToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Sam", Tables.SamInventory)
    End Sub

    Private Sub TonyToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TonyToolStripMenuItem.Click
        ShowDataGridForm("Inventory - Tony", Tables.TonyInventory)
    End Sub

    Private Sub IMPItemsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles IMPItemsToolStripMenuItem.Click
        ShowLookupGridForm("IMP Item Choices", Tables.IMPItems)
    End Sub

    Private Sub CustomFilterToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CustomFilterToolStripMenuItem.Click
        Dim frm As New CustomFilterForm
        frm.ShowDialog(Me)
        ShowItemGridForm("Items - Custom Filter", "", "", frm.Filter)
    End Sub

    Private Sub EnemyGunsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnemyGunsToolStripMenuItem.Click
        ShowLookupGridForm("Enemy Gun Choices", Tables.EnemyGuns)
    End Sub

    Private Sub EnemyItemsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnemyItemsToolStripMenuItem.Click
        ShowLookupGridForm("Enemy Item Choices", Tables.EnemyItems)
    End Sub

    Private Sub EnemyAmmoToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnemyAmmoToolStripMenuItem.Click
        ShowLookupGridForm("Enemy Ammo Choices", Tables.EnemyAmmo)
    End Sub

    Private Sub EnemyWeaponDropRateToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnemyWeaponDropRateToolStripMenuItem.Click
        ShowLookupGridForm("Weapon Drop Rates", Tables.EnemyWeaponDrops)
    End Sub

    Private Sub EnemyAmmoDropRateToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnemyAmmoDropRateToolStripMenuItem.Click
        ShowDataGridForm("Ammo Drop Rates", Tables.EnemyAmmoDrops)
    End Sub

    Private Sub EnemyArmourDropRateToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnemyArmourDropRateToolStripMenuItem.Click
        ShowLookupGridForm("Armour Drop Rates", Tables.EnemyArmourDrops)
    End Sub

    Private Sub EnemyExplosiveDropRateToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnemyExplosiveDropRateToolStripMenuItem.Click
        ShowLookupGridForm("Explosive Drop Rates", Tables.EnemyExplosiveDrops)
    End Sub

    Private Sub EnemyMiscDropRateToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnemyMiscDropRateToolStripMenuItem.Click
        ShowLookupGridForm("Misc. Item Drop Rates", Tables.EnemyMiscDrops)
    End Sub

    Private Sub BackgroundImageToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BackgroundImageToolStripMenuItem.Click
        If BackgroundImageToolStripMenuItem.Checked Then
            client.BackgroundImage = My.Resources.DESKTOP
            client.BackgroundImageLayout = ImageLayout.Stretch
        Else
            client.BackgroundImage = Nothing
        End If
    End Sub
End Class
