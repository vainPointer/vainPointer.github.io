function psrc {notepad C:\Users\John\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1}

function su-fun {Start-Process powershell -Verb runAs -argumentlist '-NoExit', "cd '$pwd'"}
set-alias su su-fun
set-alias sudo su-fun

function py3lib {cd C:\Users\John\AppData\Local\Programs\Python\Python37-32\lib\site-packages}
set-alias which Get-Command
