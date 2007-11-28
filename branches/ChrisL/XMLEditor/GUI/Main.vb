Module Main
    Public DB As XmlDB
    Public MainWindow As MainForm
    Public Splash As SplashForm
    Public Sub Main()
        If CheckForDuplicateProcess(My.Application.Info.AssemblyName) Then End

        Try
            AddHandler ErrorHandler.FatalError, AddressOf ExitDueToError

#If DEBUG Then
            IO.Directory.SetCurrentDirectory("C:\Documents and Settings\mbech\My Documents\Visual Studio Projects\JA2\Data\GameDir")
#End If
            Splash = New SplashForm()
            Splash.Show()

            Splash.UpdateLoadingText("Initializing...")

            IniFile.ReadFile("XMLEditorInit.xml")
            XmlDB.DataDirectory = IniFile.DataDirectory

            Splash.UpdateLoadingText("Creating database...")

#If DEBUG Then
            MakeDB()
#End If
            If Not IO.File.Exists(SchemaFileName) Then
                MakeDB()
            End If

            DB = New XmlDB()
            AddHandler DB.AfterLoadAll, AddressOf DB_AfterLoadAll
            AddHandler DB.LoadingTable, AddressOf DB_LoadingTable
            DB.LoadSchema(SchemaFileName)

            Splash.UpdateLoadingText("Loading Images...")
            'load stuff
            ItemImages.LoadAllImages(IniFile.DataDirectory)

            Splash.UpdateLoadingText("Loading XML Files...")
            DB.LoadAllData()

            Splash.UpdateLoadingText("Application Starting...")
            'start app
            MainWindow = New MainForm

            Splash.Hide()
            Application.Run(MainWindow)
        Catch ex As Exception
            ErrorHandler.ShowError("Unhandled error.  Please report this error to Madd Mugsy.", ex)
        End Try
    End Sub

    Private Sub DB_AfterLoadAll()
        Splash.UpdateLoadingText("Building Item Table...")
    End Sub

    Private Sub DB_LoadingTable(ByVal fileName As String)
        If fileName.Contains("\") Then fileName = fileName.Remove(0, fileName.LastIndexOf("\") + 1)
        Splash.UpdateLoadingText("Loading XML Files... " & fileName)
    End Sub

    Private Sub ExitDueToError()
        End
    End Sub


    'Determines if there already is a 'processName' running in the local host.
    'Returns true if it finds more than 'one processName' running
    Private Function CheckForDuplicateProcess(ByVal processName As String) As Boolean
        'function returns true if it finds more than one 'processName' running
        Dim Procs() As Process
        Dim proc As Process
        'get ALL processes running on this machine in all desktops
        'this also finds all services running as well.
        Procs = Process.GetProcesses()
        Dim count As Integer = 0
        For Each proc In Procs
            If proc.ProcessName.ToString.Equals(processName) Then
                count += 1
            End If
        Next proc
        If count > 1 Then
            Return True
        Else
            Return False
        End If
    End Function
End Module
