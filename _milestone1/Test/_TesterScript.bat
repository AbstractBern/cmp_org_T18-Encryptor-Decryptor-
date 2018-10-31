@echo off

echo Testing Encrypt on "1_testASCII.txt"
Team_18_CS3843_Project_01.exe -e 1_testASCII.txt -k Key.dat -p SECRET -o 1_testASCII.txt.enc.txt
echo.
echo Testing Decrypt on "1_testASCII.txt.enc"
Team_18_CS3843_Project_01.exe -d 1_testASCII.txt.enc.txt -k Key.dat -p SECRET -o 1_testASCII.txt.dec.txt
echo.
echo Testing Encrypt on "3_SI.jpg"
Team_18_CS3843_Project_01.exe -e 3_SI.jpg -k Key.dat -p SECRET -o 3_SI.jpg.enc.jpg
echo.
echo Testing Decrypt on "3_SI.jpg.enc"
Team_18_CS3843_Project_01.exe -d 3_SI.jpg.enc.jpg -k Key.dat -p SECRET -o 3_SI.jpg.dec.jpg
echo.
echo Testing Encrypt on The Profs "Truth.mp3"
Team_18_CS3843_Project_01.exe -e Truth.mp3 -k Key.dat -p SECRET -o Truth.mp3.enc.mp3
echo.
echo Testing Decrypt on The Profs "Truth.mp3.enc"
Team_18_CS3843_Project_01.exe -d Truth.mp3.enc.mp3 -k Key.dat -p SECRET -o Truth.mp3.dec.mp3
echo.
echo.
echo Finished Test