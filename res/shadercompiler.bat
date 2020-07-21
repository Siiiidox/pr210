for %%f in (vs/*.hlsl) do fxc /T vs_5_0 /E VS_Main /Fo "../bin/data/shd/%%~nf.shader" "vs/%%~f"
for %%f in (ps/*.hlsl) do fxc /T ps_5_0 /E PS_Main /Fo "../bin/data/shd/%%~nf.shader" "ps/%%~f"
