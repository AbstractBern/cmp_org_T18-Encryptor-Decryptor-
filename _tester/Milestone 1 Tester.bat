@echo off

echo Encrypt on "AfterDebugging.jpg"
Team_18_CS3843_Project_01.exe -e _test_files\AfterDebugging.jpg -k Key.dat -o _m1_results\AfterDebugging.jpg.enc

echo Decrypt on "AfterDebugging.jpg.enc"
Team_18_CS3843_Project_01.exe -d _m1_results\AfterDebugging.jpg.enc -k Key.dat -o _m1_results\AfterDebugging.jpg.enc.dec

echo Comparing Decrypted File and Original:
FC _test_files\AfterDebugging.jpg _m1_results\AfterDebugging.jpg.enc.dec >NUL && Echo Passed! || Echo Failed!

echo Comparing Encrypted File with Provided Encrypted File
FC _m1_results\AfterDebugging.jpg.enc _test_files\Mile#1\AfterDebugging.jpg_M01_R01_18_CDEBA.enc >NUL && Echo Passed! || Echo Failed!
echo.
pause