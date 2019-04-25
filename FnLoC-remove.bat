@echo off
:: FnLoC un-install
:: R. B. Romig

echo Un-installing FnLoC...
cd "C:\Program Files (x86)\FnLoC\"
del *.*
cd ..
rmdir FnLoC
:end
