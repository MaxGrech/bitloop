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

rem ─── 2) Ensure MSVC (cl.exe) ────────────────────────────
where cl >nul 2>&1
if ERRORLEVEL 1 (
  echo Installing MSVC Build Tools via winget
  winget install --id Microsoft.VisualStudio.2022.BuildTools --silent --accept-package-agreements --accept-source-agreements
) else (
  echo MSVC Build Tools already installed.
)

rem ─── 3) Ensure Ninja ───────────────────────────────────
where ninja >nul 2>&1
if ERRORLEVEL 1 (
  echo Installing Ninja via winget
  winget install --id Kitware.Ninja --silent --accept-package-agreements --accept-source-agreements
) else (
  echo Ninja already installed.
)

rem ─── 4) Bootstrap vcpkg ────────────────────────────────
echo Bootstrapping vcpkg…
"%REPO_ROOT%\vcpkg\bootstrap-vcpkg.bat" || (
  echo ERROR: vcpkg bootstrap failed.
  exit /b 1
)

rem ─── 5) Persist BITLOOP_ROOT ──────────────────────────
echo Setting BITLOOP_ROOT to "%REPO_ROOT%"
setx BITLOOP_ROOT "%REPO_ROOT%"

echo.
echo ─── Bootstrap complete. Please restart this CMD session to pick up BITLOOP_ROOT.
endlocal