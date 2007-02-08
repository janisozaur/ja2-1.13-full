@echo off
if     %doxypath%*==* set doxy_invoke=doxygen
if not %doxypath%*==* set doxy_invoke=%doxypath%\doxygen


for /R %%i in (*.doxyfile) do (
	pushd %%~pi
	echo calling %doxy_invoke% %%~nxi ...
	%doxy_invoke% %%~nxi
	popd
	)

echo err=%errorlevel%
if %errorlevel% GTR 1 pause
