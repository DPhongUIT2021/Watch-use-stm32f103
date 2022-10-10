SET PATH=C:\Keil_v5\ARM\ARMCLANG\Bin;C:\Program Files\Common Files\Oracle\Java\javapath;C:\WinAVR-20100110\bin;C:\WinAVR-20100110\utils\bin;C:\Program Files (x86)\VMware\VMware Workstation\bin\;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\Microsoft SQL Server\Client SDK\ODBC\170\Tools\Binn\;C:\Program Files (x86)\Microsoft SQL Server\150\Tools\Binn\;C:\Program Files\Microsoft SQL Server\150\Tools\Binn\;C:\Program Files\Microsoft SQL Server\150\DTS\Binn\;C:\Program Files (x86)\Microsoft SQL Server\150\DTS\Binn\;C:\Program Files\Azure Data Studio\bin;C:\msys64\mingw64\bin;C:\Program Files\dotnet\;C:\Program Files\Microchip\xc8\v2.36\bin;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Program Files\Git\cmd;C:\msys64\mingw64\bin;C:\Users\ATC\AppData\Local\Microsoft\WindowsApps;C:\altera\13.0sp1\modelsim_ase\win32aloem;C:\Program Files\Azure Data Studio\bin;C:\Users\ATC\AppData\Local\Programs\Microsoft VS Code\bin;C:\Users\ATC\.dotnet\tools;C:\Users\ATC\AppData\Local\GitHubDesktop\bin
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\main.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\misc.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\startup_stm32f10x_md._ac"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\Objects\WATCH.lnp"
