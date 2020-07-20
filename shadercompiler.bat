IF NOT EXIST "bin\data" md bin\data
IF NOT EXIST "bin\data\shd" md bin\data\shd

for %%f in (vs_*.hlsl) do fxc /T vs_5_0 /E VS_Main /Fo "bin/data/shd/%%~nf.shader" "%%~f"
for %%f in (ps_*.hlsl) do fxc /T ps_5_0 /E PS_Main /Fo "bin/data/shd/%%~nf.shader" "%%~f"
