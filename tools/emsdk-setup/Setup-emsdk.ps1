$PSScriptRoot = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

cd $PSScriptRoot/../emsdk

& .\emsdk.bat update

& .\emsdk.bat install latest

& .\emsdk.bat activate latest

& .\emsdk_env.bat