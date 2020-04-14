cd Samples\VC
for /d %%i in (*) do ( 
cd %%i
rd x86 x64 ipch /s /q
del *.sdf *.suo *.log *.user *.filters *.ipch *.aps /s /a
cd..)
cd..

cd C#\KSJShow3D_CSharp
rd obj /s /q
del *.suo /s
