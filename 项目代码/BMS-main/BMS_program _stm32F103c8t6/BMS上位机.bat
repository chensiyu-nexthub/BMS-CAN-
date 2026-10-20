@echo off
chcp 65001 >nul
title BMS 上位机 - BQ76940 9串
set "PY=%LOCALAPPDATA%\Programs\Python\Python312\python.exe"
if not exist "%PY%" set "PY=python"
"%PY%" "%~dp0bms_upper_computer.py"
pause
