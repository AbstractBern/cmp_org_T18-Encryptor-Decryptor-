@echo off

echo Encrypt on "AfterDebugging.jpg"
Team_18_CS3843_Project_02.exe -e _test_files\AfterDebugging.jpg -k Key.dat -p SECRET -o _m2_results\AfterDebugging.jpg.enc

echo Decrypt on "AfterDebugging.jpg.enc"
Team_18_CS3843_Project_02.exe -d _m2_results\AfterDebugging.jpg.enc -k Key.dat -p SECRET -o _m2_results\AfterDebugging.jpg.enc.dec

echo Comparing Decrypted File and Original:
FC _test_files\AfterDebugging.jpg _m2_results\AfterDebugging.jpg.enc.dec >NUL && Echo Passed! || Echo Failed!

echo Comparing Encrypted File with Provided Encrypted File
FC _m2_results\AfterDebugging.jpg.enc _test_files\Mile#2\AfterDebugging.jpg_M02_R01_18_CDEBA.enc >NUL && Echo Passed! || Echo Failed!
echo.
pause