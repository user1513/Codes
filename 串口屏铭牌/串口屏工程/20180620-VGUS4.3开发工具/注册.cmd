if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86" (copy mscomm*.* %windir%\system32\ /y) ELSE (copy mscomm*.* %windir%\SysWOW64\ /y)
if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86" (Regsvr32 %windir%\system32\mscomm32.ocx /s) ELSE (Regsvr32 %windir%\SysWOW64\mscomm32.ocx /s)
reg add "HKCR\Licenses\4250E830-6AC2-11cf-8ADB-00AA00C00905" /v "" /d "kjljvjjjoquqmjjjvpqqkqmqykypoqjquoun"
pause