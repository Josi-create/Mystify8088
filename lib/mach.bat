@echo off
echo --------------------------------
tasm %1
echo --------------------------------
tcc -c %2
echo --------------------------------
tlink c0s %1 %2,%2,,cs emu maths graphics > error.log
type error.log
echo ---------------------------- ----
