for /f "delims=" %%i in ('python altirra_bridge.py "atasm-build.json" %*') do set ALTIRRA_EXEC=%ALTIRRA_EXEC% %%i
%ALTIRRA_EXEC%
