@echo off
setlocal

rem ─── Locate this script’s directory ──────────────────────────
rem %~dp0 is something like "C:\dev\bitloop\scripts\"
set "REPO_ROOT=%~dp0"

rem ─── Compute the parent (repo root) ─────────────────────────
rem Appending .. and normalizing via a FOR
for %%I in ("%REPO_ROOT%..") do set "REPO_ROOT=%%~fI"

rem ─── Determine repo root ───────────────────────────────
echo Repo Root:
echo %REPO_ROOT%

rem ─── 1) Ensure Git ─────────────────────────────────────
where git >nul 2>&1
if ERRORLEVEL 1 (
  echo Installing Git via winget
  winget install --id Git.Git --silent --accept-package-agreements --accept-source-agreements
) else (
  echo Git already installed.
)

rem ─── 2) Ensure MSVC + C++ workload ───────────────────────────
where cl >nul 2>&1
if ERRORLEVEL 1 (
  echo Installing MSVC Build Tools + C++ workload via winget…
  rem This will install the core BuildTools plus the VCTools workload
  winget install --id Microsoft.VisualStudio.2022.BuildTools ^
    --silent ^
    --accept-package-agreements ^
    --accept-source-agreements ^
    --override "--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --passive --norestart --wait"
) else (
  echo MSVC C++ compiler already installed.
)

rem ─── 3) Ensure Ninja ───────────────────────────────────
where ninja >nul 2>&1
if ERRORLEVEL 1 (
  echo Ninja not found, installing via winget…
  winget install --id Ninja-build.Ninja --exact --silent --accept-package-agreements --accept-source-agreements
) else (
  echo Ninja already installed.
)

rem ─── Persist BITLOOP_ROOT ───────────────────────────────────
echo Setting BITLOOP_ROOT to "%REPO_ROOT%"...
setx BITLOOP_ROOT "%REPO_ROOT%" >nul

rem ─── Append to user PATH if missing ─────────────────────────
rem Query the current user-level PATH from the registry
for /f "tokens=2,*" %%A in ('
  reg query "HKCU\Environment" /v PATH 2^>nul
') do set "USER_PATH=%%B"

rem ─── Append to user PATH if missing ─────────────────────────
echo %PATH% | findstr /i /c:"%REPO_ROOT%" >nul
if errorlevel 1 (
  echo Adding "%REPO_ROOT%" to your user PATH...
  rem setx PATH appends to the user PATH (merging with system PATH automatically)
  setx PATH "%PATH%;%REPO_ROOT%" >nul
) else (
  echo "%REPO_ROOT%" is already in your PATH.
)

:: Also update the **current** session
set BITLOOP_ROOT=%REPO_ROOT%
set PATH=%PATH%;%REPO_ROOT%

rem ─── 4) Bootstrap vcpkg ────────────────────────────────
echo Bootstrapping vcpkg...
"%REPO_ROOT%\vcpkg\bootstrap-vcpkg.bat" || (
  echo ERROR: vcpkg bootstrap failed.
  exit /b 1
)

echo.
echo ─── Done. Close and reopen your Command Prompt to pick up the changes.