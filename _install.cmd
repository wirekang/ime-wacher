@echo off
echo ############ 1. Kill and delete existing ime-watcher
tskill /v ime-watcher.exe
sc.exe delete ime-watcher

cd %1%

echo.
echo.
echo.
echo ############ 2. Copy build output to C:/ime-watcher.exe
copy build\bin\Release\ime-watcher.exe c:\ime-watcher.exe || goto :error

echo.
echo.
echo.
echo ############ 3. Install C:/ime-watcher.exe as ime-watcher
sc.exe create ime-watcher start= auto binpath= c:\ime-watcher.exe || goto :error

echo.
echo.
echo.
echo ############ 4. Start ime-watcher service
sc.exe start ime-watcher || goto :error

:error
pause