@echo off

rem Trying to find DOXYGEN TOOL, attempt #1:
if exist %doxypath%\doxygen.exe goto add_doxy

rem Trying to find DOXYGEN TOOL, attempt #2, look inside PATH variable:
for %%I in (doxygen.exe) do set doxy_invoke=%%~$PATH:I
if NOT "%doxy_invoke%"=="" goto set_dox

rem Trying to find DOXYGEN TOOL, attempt #3: Doxy is NOT in PATH, try PrFiles -> doxypath
if exist %ProgramFiles%\doxygen\bin\doxygen.exe set doxypath=%ProgramFiles%\doxygen\bin
if exist %doxypath%\doxygen.exe goto add_doxy

rem Trying to find DOXYGEN TOOL, attempt #34: no further idea :-(
rem lets try to scan different user specific paths
if exist d:\C\Versionsverwaltung\doxygen\bin\doxygen.exe set doxypath=d:\C\Versionsverwaltung\doxygen\bin
if exist C:\doxygen\bin\doxygen.exe set doxypath=C:\doxygen\bin

:add_doxy
set doxy_invoke=%doxypath%\doxygen.exe
echo --- Using '%doxy_invoke%'


:set_dox
for /R %%i in (*.doxyfile) do (
	pushd %%~pi
	echo calling %doxy_invoke% %%~nxi ...
	%doxy_invoke% %%~nxi
	popd
	)

echo err=%errorlevel%
if %errorlevel% GTR 1 pause
