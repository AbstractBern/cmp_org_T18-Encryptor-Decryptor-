@echo off

set exec=Team_18_CS3843_Project_02.exe

set testfile=_test_files\AfterDebugging.jpg
set outenc=_m2_results\AfterDebugging.jpg.enc
set outdec=_m2_results\AfterDebugging.jpg.enc.dec
set testfileenc=_test_files\Mile#2\AfterDebugging.jpg_M02_R01_18_CDEBA.enc

echo Encrypt on "AfterDebugging.jpg"
%exec% -e %testfile% -k Key.dat -p SECRET -o %outenc%

echo Decrypt on "AfterDebugging.jpg.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -o %outdec%

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
set outenc=_m2_results\ATest.txt.enc
set outdec=_m2_results\ATest.txt.enc.dec
set testfileenc=_test_files\Mile#2\ATest.txt_M02_R01_18_CDEBA.enc

echo Encrypt on "ATest.txt"
%exec% -e %testfile% -k Key.dat -p SECRET -o %outenc%

echo Decrypt on "ATest.txt.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -o %outdec%

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
set outenc=_m2_results\Frustration.gif.enc
set outdec=_m2_results\Frustration.gif.enc.dec
set testfileenc=_test_files\Mile#2\Frustration.gif_M02_R01_18_CDEBA.enc

echo Encrypt on "ATest.txt"
%exec% -e %testfile% -k Key.dat -p SECRET -o %outenc%

echo Decrypt on "ATest.txt.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -o %outdec%

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
set outenc=_m2_results\Truth.mp3.enc
set outdec=_m2_results\Truth.mp3.enc.dec
set testfileenc=_test_files\Mile#2\Truth.mp3_M02_R01_18_CDEBA.enc

echo Encrypt on "ATest.txt"
%exec% -e %testfile% -k Key.dat -p SECRET -o %outenc%

echo Decrypt on "ATest.txt.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -o %outdec%

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
set outenc=_m2_results\Z_Frustration.gif.enc
set outdec=_m2_results\Z_Frustration.gif.enc.dec
set testfileenc=_test_files\Mile#2\Z_Frustration.gif_M02_R01_18_CDEBA.enc

echo Encrypt on "ATest.txt"
%exec% -e %testfile% -k Key.dat -p SECRET -o %outenc%

echo Decrypt on "ATest.txt.enc"
%exec% -d %outenc% -k Key.dat -p SECRET -o %outdec%

echo Comparing Decrypted File and Original:
FC %testfile% %outdec% >NUL && Echo Passed! || Echo Failed!

echo Comparing Encrypted File with Provided Encrypted File
FC %outenc% %testfileenc% >NUL && Echo Passed! || Echo Failed!

pause