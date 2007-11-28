<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class GridForm
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim DataGridViewCellStyle1 As System.Windows.Forms.DataGridViewCellStyle = New System.Windows.Forms.DataGridViewCellStyle
        Me.Grid = New System.Windows.Forms.DataGridView
        Me.GridContextMenu = New System.Windows.Forms.ContextMenuStrip(Me.components)
        Me.SelectColumnsToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
        Me.FilterToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
        CType(Me.Grid, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GridContextMenu.SuspendLayout()
        Me.SuspendLayout()
        '
        'Grid
        '
        Me.Grid.AllowUserToAddRows = False
        Me.Grid.AllowUserToDeleteRows = False
        Me.Grid.AllowUserToOrderColumns = True
        DataGridViewCellStyle1.BackColor = System.Drawing.Color.FromArgb(CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer))
        Me.Grid.AlternatingRowsDefaultCellStyle = DataGridViewCellStyle1
        Me.Grid.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText
        Me.Grid.ContextMenuStrip = Me.GridContextMenu
        Me.Grid.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Grid.Location = New System.Drawing.Point(0, 0)
        Me.Grid.MultiSelect = False
        Me.Grid.Name = "Grid"
        Me.Grid.ReadOnly = True
        Me.Grid.RowHeadersVisible = False
        Me.Grid.RowHeadersWidth = 24
        Me.Grid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect
        Me.Grid.Size = New System.Drawing.Size(552, 324)
        Me.Grid.TabIndex = 0
        '
        'GridContextMenu
        '
        Me.GridContextMenu.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.SelectColumnsToolStripMenuItem, Me.FilterToolStripMenuItem})
        Me.GridContextMenu.Name = "ContextMenu"
        Me.GridContextMenu.Size = New System.Drawing.Size(147, 48)
        '
        'SelectColumnsToolStripMenuItem
        '
        Me.SelectColumnsToolStripMenuItem.Name = "SelectColumnsToolStripMenuItem"
        Me.SelectColumnsToolStripMenuItem.Size = New System.Drawing.Size(146, 22)
        Me.SelectColumnsToolStripMenuItem.Text = "Select &Columns"
        '
        'FilterToolStripMenuItem
        '
        Me.FilterToolStripMenuItem.Name = "FilterToolStripMenuItem"
        Me.FilterToolStripMenuItem.Size = New System.Drawing.Size(146, 22)
        Me.FilterToolStripMenuItem.Text = "&Filter"
        '
        'GridForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(552, 324)
        Me.Controls.Add(Me.Grid)
        Me.Name = "GridForm"
        Me.ShowIcon = False
        Me.Text = "Items"
        CType(Me.Grid, System.ComponentModel.ISupportInitialize).EndInit()
        Me.GridContextMenu.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents Grid As System.Windows.Forms.DataGridView
    Friend WithEvents GridContextMenu As System.Windows.Forms.ContextMenuStrip
    Friend WithEvents SelectColumnsToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents FilterToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
End Class
