@echo off

set exec=Team_18_CS3843_Project_03.exe

set testfile=_test_files\AfterDebugging.jpg
set outenc=_m3_results\AfterDebugging.jpg.enc
set outdec=_m3_results\AfterDebugging.jpg.enc.dec.jpg
set testfileenc=_test_files\Mile#3\AfterDebugging.jpg_M03_R01_18_CDEBA.enc

echo Encrypt on "AfterDebugging.jpg"
%exec% -e %testfile% -k Key.dat -p SECRET -r 1 -o %outenc%

echo Decrypt on "AfterDebugging.jpg.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -r 1 -o %outdec%

echo Comparing Decrypted File and Original:
FC %testfile% %outdec% >NUL && Echo Passed! || Echo Failed!

echo Comparing Encrypted File with Provided Encrypted File
FC %outenc% %testfileenc% >NUL && Echo Passed! || Echo Failed!
pause
echo.
echo.
echo TEST 2
echo.
echo.


set testfile=_test_files\ATest.txt
set outenc=_m3_results\ATest.txt.enc
set outdec=_m3_results\ATest.txt.enc.dec.txt
set testfileenc=_test_files\Mile#3\ATest.txt_M03_R01_18_CDEBA.enc

echo Encrypt on "ATest.txt"
%exec% -e %testfile% -k Key.dat -p SECRET -r 1 -o %outenc%

echo Decrypt on "ATest.txt.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -r 1 -o %outdec%

echo Comparing Decrypted File and Original:
FC %testfile% %outdec% >NUL && Echo Passed! || Echo Failed!

echo Comparing Encrypted File with Provided Encrypted File
FC %outenc% %testfileenc% >NUL && Echo Passed! || Echo Failed!
pause
echo.
echo.
echo TEST 3
echo.
echo.

set testfile=_test_files\Frustration.gif
set outenc=_m3_results\Frustration.gif.enc
set outdec=_m3_results\Frustration.gif.enc.dec.gif
set testfileenc=_test_files\Mile#3\Frustration.gif_M03_R01_18_CDEBA.enc

echo Encrypt on "ATest.txt"
%exec% -e %testfile% -k Key.dat -p SECRET -r 1 -o %outenc%

echo Decrypt on "ATest.txt.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -r 1 -o %outdec%

echo Comparing Decrypted File and Original:
FC %testfile% %outdec% >NUL && Echo Passed! || Echo Failed!

echo Comparing Encrypted File with Provided Encrypted File
FC %outenc% %testfileenc% >NUL && Echo Passed! || Echo Failed!
pause
echo.
echo.
echo TEST 4
echo.
echo.

set testfile=_test_files\Truth.mp3
set outenc=_m3_results\Truth.mp3.enc
set outdec=_m3_results\Truth.mp3.enc.dec.mp3
set testfileenc=_test_files\Mile#3\Truth.mp3_M03_R02_18_CDEBA.enc

echo Encrypt on "ATest.txt"
%exec% -e %testfile% -k Key.dat -p SECRET -r 2 -o %outenc%

echo Decrypt on "ATest.txt.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -r 2 -o %outdec%

echo Comparing Decrypted File and Original:
FC %testfile% %outdec% >NUL && Echo Passed! || Echo Failed!

echo Comparing Encrypted File with Provided Encrypted File
FC %outenc% %testfileenc% >NUL && Echo Passed! || Echo Failed!
pause
echo.
echo.
echo TEST 5
echo.
echo.

set testfile=_test_files\Z_Frustration.gif
set outenc=_m3_results\Z_Frustration.gif.enc
set outdec=_m3_results\Z_Frustration.gif.enc.dec.gif
set testfileenc=_test_files\Mile#3\Z_Frustration.gif_M03_R03_18_CDEBA.enc

echo Encrypt on "ATest.txt"
%exec% -e %testfile% -k Key.dat -p SECRET -r 3 -o %outenc%

echo Decrypt on "ATest.txt.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -r 3 -o %outdec%

echo Comparing Decrypted File and Original:
FC %testfile% %outdec% >NUL && Echo Passed! || Echo Failed!

echo Comparing Encrypted File with Provided Encrypted File
FC %outenc% %testfileenc% >NUL && Echo Passed! || Echo Failed!

pause