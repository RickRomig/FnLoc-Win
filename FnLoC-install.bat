@echo off
:: FnLoC installation
:: R. B. Romig

echo Installing FnLoC...
if not exist "C:\Program Files (x86)\FnLoC\" mkdir "C:\Program Files (x86)\FnLoC\"
xcopy *.* "C:\Program Files (x86)\FnLoC\" /i /y
echo FnLoC installed...

