rem start "VSSC Help System" .\doxydoc\html\index.html 

for /R %%i in (index.htm*) do (
	start "Help System" %%i
	)

