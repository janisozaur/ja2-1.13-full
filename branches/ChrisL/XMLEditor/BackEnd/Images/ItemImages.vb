Imports System.Windows.Forms
Imports System.Drawing

'this is a shared/utility class
Public Class ItemImages

    Protected Const imageFileExt As String = ".sti"

    Protected Const bigItemDir As String = "BigItems\"
    Protected Const mediumItemDir As String = "Interface\"
    Protected Const smallItemDir As String = "tilesets\0\"

    Protected Const bigGunPrefix As String = "gun"
    Protected Const bigItemPrefix As String = "p?item"

    Protected Const mediumGunGraphicType As String = "mdguns.sti"
    Protected Const mediumItemGraphicType As String = "mdp?items.sti"

    Protected Const smallGunGraphicType As String = "smguns.sti"
    Protected Const smallItemGraphicType As String = "smp?items.sti"

    Protected Const itemTypes As Integer = 3

    Protected Shared bigItemList(itemTypes)() As Bitmap
    Protected Shared mediumItemList(itemTypes)() As Bitmap
    Protected Shared smallItemList(itemTypes)() As Bitmap

    Protected Shared tallestBigImageSize As Integer = 0

    Protected Shared dataDir As String

    Public Shared Sub LoadAllImages(ByVal dataDirectory As String)
        Try
            dataDir = dataDirectory
            For i As Integer = 0 To itemTypes
                LoadSmallImages(i)
                LoadMediumImages(i)
                LoadBigImages(i)
            Next
        Catch ex As Exception
            ErrorHandler.ShowError("Unable to load images.", ex)
            ErrorHandler.TriggerFatalError()
        End Try
    End Sub

    Protected Shared Sub LoadSmallImages(ByVal type As Integer)
        Dim sti As STI
        Dim fileName As String = smallGunGraphicType

        If type > 0 Then fileName = Replace(smallItemGraphicType, "?", type)
        sti = New STI(dataDir & smallItemDir & fileName)
        ReDim smallItemList(type)(sti.NumberOfSubImages - 1)

        For i As Integer = 0 To sti.NumberOfSubImages - 1
            Application.DoEvents()
            Dim bmp As Bitmap = DirectCast(sti(i), Bitmap)
            SetTransparentColours(bmp)
            'also do bright green for outlines
            bmp.MakeTransparent(Color.FromArgb(0, 255, 0))
            smallItemList(type)(i) = bmp
        Next
    End Sub

    Protected Shared Sub LoadMediumImages(ByVal type As Integer)
        Dim sti As STI
        Dim fileName As String = mediumGunGraphicType

        If type > 0 Then fileName = Replace(mediumItemGraphicType, "?", type)
        sti = New STI(dataDir & mediumItemDir & fileName)
        ReDim mediumItemList(type)(sti.NumberOfSubImages - 1)

        For i As Integer = 0 To sti.NumberOfSubImages - 1
            Application.DoEvents()
            Dim bmp As Bitmap = DirectCast(sti(i), Bitmap)
            SetTransparentColours(bmp)
            'also do bright green for outlines
            bmp.MakeTransparent(Color.FromArgb(0, 255, 0))
            mediumItemList(type)(i) = bmp
        Next
    End Sub

    Protected Shared Sub LoadBigImages(ByVal type As Integer)
        Dim sti As STI
        Dim prefix As String = bigGunPrefix
        If type > 0 Then prefix = Replace(bigItemPrefix, "?", type)

        'note: mediumitems must be done first!
        ReDim bigItemList(type)(mediumItemList(type).GetUpperBound(0))

        For i As Integer = 0 To bigItemList(type).GetUpperBound(0)
            Application.DoEvents()
            Dim indexText As String = CStr(i)
            If i < 10 Then indexText = "0" & indexText

            Dim fileName As String = dataDir & bigItemDir & prefix & indexText & imageFileExt
            sti = New STI(fileName)
            Dim bmp As Bitmap = DirectCast(sti(0), Bitmap)
            SetTransparentColours(bmp)
            bigItemList(type)(i) = bmp
            If sti(0).Height > tallestBigImageSize Then tallestBigImageSize = sti(0).Height
        Next
    End Sub

    Protected Shared Sub SetTransparentColours(ByVal bmp As Bitmap)
        'figure out the transparent colour
        Dim topLeft, topRight, bottomLeft, bottomRight As Color
        topLeft = bmp.GetPixel(0, 0)
        topRight = bmp.GetPixel(bmp.Width - 1, 0)
        bottomLeft = bmp.GetPixel(0, bmp.Height - 1)
        bottomRight = bmp.GetPixel(bmp.Width - 1, bmp.Height - 1)

        Dim topEqual, bottomEqual, rightEqual, leftEqual, diagRightEqual, diagLeftEqual As Boolean
        topEqual = (topLeft = topRight)
        bottomEqual = (bottomLeft = bottomRight)
        leftEqual = (bottomLeft = topLeft)
        rightEqual = (bottomRight = topRight)
        diagRightEqual = (topLeft = bottomRight)
        diagLeftEqual = (topRight = bottomLeft)

        If topEqual OrElse leftEqual OrElse diagRightEqual Then
            bmp.MakeTransparent(topLeft)
        ElseIf bottomEqual Then
            bmp.MakeTransparent(bottomLeft)
        ElseIf rightEqual OrElse diagLeftEqual Then
            bmp.MakeTransparent(topRight)
        End If
    End Sub

    Public Shared ReadOnly Property GreatestBigImageHeight() As Integer
        Get
            Return tallestBigImageSize
        End Get
    End Property

    Public Shared ReadOnly Property SmallItems(ByVal type As Integer) As Bitmap()
        Get
            Return smallItemList(type)
        End Get
    End Property

    Public Shared ReadOnly Property SmallItemImage(ByVal type As Integer, ByVal index As Integer) As Bitmap
        Get
            Return smallItemList(type)(index)
        End Get
    End Property

    Public Shared ReadOnly Property MediumItems(ByVal type As Integer) As Bitmap()
        Get
            Return mediumItemList(type)
        End Get
    End Property

    Public Shared ReadOnly Property MediumItemImage(ByVal type As Integer, ByVal index As Integer) As Bitmap
        Get
            Return mediumItemList(type)(index)
        End Get
    End Property

    Public Shared ReadOnly Property BigItems(ByVal type As Integer) As Bitmap()
        Get
            Return bigItemList(type)
        End Get
    End Property

    Public Shared ReadOnly Property BigItemImage(ByVal type As Integer, ByVal index As Integer) As Bitmap
        Get
            Return bigItemList(type)(index)
        End Get
    End Property
End Class
