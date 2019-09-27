:: \HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Command Processor\AutoRun=%THISBAT%
@echo off
doskey ls=dir
doskey which=for %%x in ($1) do @echo %%~$PATH:x