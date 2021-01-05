@set MK=m328p
@set PRG=arduino
@set PORT=COM11
@set BAUD=57600
@set FLASHFILE=Release\Demo_MT12232D.hex
@set AVRDUDEPATH=Avrdude

CLS

REM FLASH
%AVRDUDEPATH%avrdude -p %MK% -c %PRG% -P %PORT% -b %BAUD% -U flash:w:%FLASHFILE%:i -v
