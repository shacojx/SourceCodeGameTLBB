; ===============================================================================================
; MULTIPLE.EXE
; Copyright (c), Firelight Technologies Pty, Ltd 1999-2004.

; This example demonstrates how To use dynamic loading of fmod.dll To achieve MULTIPLE soundcard
; output at the same time.  If you do not have 2 soundcards you will have To Select the same
;    device twice.
;    Besides This, it is a good helper To display how fmod.dll can be loaded dynamically without 
;    having To link an import library.
    
;    IMPORTANT!!! you must copy fmod.dll To fmod2.dll Or libfmod-3.63.so To libfmod-3.63_2.so To
;    avoid operating systems caching the dll!
; ===============================================================================================*/
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions
    
XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"
XIncludeFile "../../api/PureBasic/fmoddyn.pbi"

#FMOD_LIB  = "fmod.dll"
#FMOD_LIB2 = "fmod2.dll"

#FMOD_INSTANCE_1 = 1
#FMOD_INSTANCE_2 = 2

Global fmod1.FMOD_INSTANCE, fmod2.FMOD_INSTANCE
Global samp1.l, samp2.l

    OpenConsole()
    ConsoleTitle("FMOD Multiple Example")
    
    FMOD_CreateInstance(@fmod1, #FMOD_INSTANCE_1, #FMOD_LIB)
    FMOD_CreateInstance(@fmod2, #FMOD_INSTANCE_2, #FMOD_LIB2)
    
    CallFunctionFast(fmod1\FSOUND_GetVersion)
    ; call fpu register and store (pop) the value
    !FSTP dword [esp]   
    !POP [v_result]     
    If (result < FMOD_VERSION)
      PrintN("Error : You are using the wrong DLL version!  You should be using FMOD " + StrF(FMOD_VERSION))
      Delay(2000)
      CloseConsole()
      End
    EndIf
    
    CallFunctionFast(fmod2\FSOUND_GetVersion)
    ; call fpu register and store (pop) the value
    !FSTP dword [esp]   
    !POP [v_result]     
    If (result < FMOD_VERSION)
      PrintN("Error : You are using the wrong DLL version!  You should be using FMOD " + StrF(FMOD_VERSION))
      Delay(2000)
      FMOD_FreeInstance(@fmod1)
      FMOD_FreeInstance(@fmod2)
      CloseConsole()
      End
    EndIf
    
    CallFunctionFast(fmod1\FSOUND_SetOutput, #FSOUND_OUTPUT_DSOUND)
    CallFunctionFast(fmod2\FSOUND_SetOutput, #FSOUND_OUTPUT_DSOUND)
    
    ; ==========================================================================================
    ; Select DRIVER 1
    ; ==========================================================================================
    
    ;  The following list are the drivers For the output method selected above.    
    
    PrintN("---------------------------------------------------------")
    PrintN("Select soundcard #1")
    PrintN("---------------------------------------------------------")
    
    For i = 0 To (CallFunctionFast(fmod1\FSOUND_GetNumDrivers) - 1)
      PrintN(Str(i + 1) + " - " + PeekS(CallFunctionFast(fmod1\FSOUND_GetDriverName, i)) )
    Next i
    PrintN("---------------------------------------------------------")
    PrintN("Press a corresponding number or ESC to quit")
    
    Repeat     
      driver.s = Left(Inkey(),1)          
      If Asc(driver) = 27
        PrintN("FSOUND_Init() Error!")
        PrintN(FMOD_ErrorString(CallFunctionFast(fmod1\FSOUND_GetError)))
        CallFunctionFast(fmod1\FSOUND_Close)
        FMOD_FreeInstance(@fmod1)
        FMOD_FreeInstance(@fmod2)
        Delay(1000)
        CloseConsole()
        End
      EndIf 
      Delay(1)
    Until ( Val(driver) > 0 ) And ( Val(driver) <= CallFunctionFast(fmod1\FSOUND_GetNumDrivers) )
    
    CallFunctionFast(fmod1\FSOUND_SetDriver, (Val(driver) - 1) )                     ; Select sound card (0 = default)
    
    
    ; ==========================================================================================
    ; Select DRIVER 2
    ; ==========================================================================================
    
    ;  The following list are the drivers For the output method selected above.    
    
    PrintN("---------------------------------------------------------")
    PrintN("Select soundcard #2")
    PrintN("---------------------------------------------------------")
    
    For i = 0 To (CallFunctionFast(fmod2\FSOUND_GetNumDrivers) - 1)
      PrintN(Str(i + 1) + " - " + PeekS(CallFunctionFast(fmod2\FSOUND_GetDriverName, i)) )
    Next i
    PrintN("---------------------------------------------------------")
    PrintN("Press a corresponding number or ESC to quit")
    
    Repeat     
      driver.s = Left(Inkey(),1)          
      If Asc(driver) = 27
        PrintN("FSOUND_Init() Error!")
        PrintN(FMOD_ErrorString(CallFunctionFast(fmod2\FSOUND_GetError)))
        CallFunctionFast(fmod2\FSOUND_Close)
        FMOD_FreeInstance(@fmod1)
        FMOD_FreeInstance(@fmod2)
        Delay(1000)
        CloseConsole()
        End
      EndIf 
      Delay(1)
    Until ( Val(driver) > 0 ) And ( Val(driver) <= CallFunctionFast(fmod2\FSOUND_GetNumDrivers) )
    
    CallFunctionFast(fmod2\FSOUND_SetDriver, (Val(driver) - 1) )                     ; Select sound card (0 = default)
    
    ; ==========================================================================================
    ; INITIALIZE
    ; ==========================================================================================
    
    If CallFunctionFast(fmod1\FSOUND_Init, 44100, 32, #FSOUND_INIT_USEDEFAULTMIDISYNTH) <= 0
      PrintN("FSOUND_Init() Error!")
      PrintN(FMOD_ErrorString(CallFunctionFast(fmod1\FSOUND_GetError)) )
      CallFunctionFast(fmod1\FSOUND_Close)
      FMOD_FreeInstance(@fmod1)
      FMOD_FreeInstance(@fmod2)       
      Delay(2000)
      CloseConsole()
      End
    EndIf 
    
    If CallFunctionFast(fmod2\FSOUND_Init, 44100, 32, #FSOUND_INIT_USEDEFAULTMIDISYNTH) <= 0
      PrintN("FSOUND_Init() Error!")
      PrintN(FMOD_ErrorString(CallFunctionFast(fmod2\FSOUND_GetError)) )
      CallFunctionFast(fmod2\FSOUND_Close)
      FMOD_FreeInstance(@fmod1)
      FMOD_FreeInstance(@fmod2)        
      Delay(2000)
      CloseConsole()
      End
    EndIf  
    
    ; ==========================================================================================
    ; LOAD SAMPLE (twice)
    ; ==========================================================================================
    
    ; Trim the current path to the sounds directory path
    currentDir.s = Space(255)
    GetCurrentDirectory_(255, @currentDir)
    For i = Len(currentDir) To 1 Step -1
      If Mid(currentDir, i, 1) = "\"
        found = found + 1
        If found = 2 
          position = i
        EndIf
      EndIf
    Next i
    
    soundDir.s = Mid(currentDir, 1, position) + "media/drumloop.wav"
    samp1 = CallFunctionFast(fmod1\FSOUND_Sample_Load, #FSOUND_UNMANAGED, @soundDir, #FSOUND_NORMAL | #FSOUND_2D, 0, 0)
    If samp1 <= 0
      PrintN("FSOUND_Sample_Load(samp1) Error!")
      PrintN(FMOD_ErrorString(CallFunctionFast(fmod1\FSOUND_GetError)) )
      FMOD_FreeInstance(@fmod1)
      FMOD_FreeInstance(@fmod2)
      CloseConsole()
      End
    EndIf
    
    CallFunctionFast(fmod1\FSOUND_Sample_SetMode, samp1, #FSOUND_LOOP_OFF)  ; this wav has loop points in it which turns looping on.. turn it off!
    
    samp2 = CallFunctionFast(fmod2\FSOUND_Sample_Load, #FSOUND_UNMANAGED, @soundDir, #FSOUND_NORMAL | #FSOUND_2D, 0, 0)
    If samp2 <= 0
      PrintN("FSOUND_Sample_Load(samp1) Error!")
      PrintN(FMOD_ErrorString(CallFunctionFast(fmod2\FSOUND_GetError)) )
      FMOD_FreeInstance(@fmod1)
      FMOD_FreeInstance(@fmod2)
      CloseConsole()
      End
    EndIf
    
    CallFunctionFast(fmod2\FSOUND_Sample_SetMode, samp2, #FSOUND_LOOP_OFF)  ; this wav has loop points in it which turns looping on.. turn it off!   
    
    ; ==========================================================================================
    ; display HELP
    ; ==========================================================================================
    
    PrintN("=========================================================================")
    PrintN("Press 1       Play 16bit sound on soundcard #1")
    PrintN("      2       Play 16bit sound on soundcard #2")
    PrintN("      ESC     Quit")
    PrintN("=========================================================================")
    
    Repeat
      keys.s = Left(Inkey(),1)
      
      If keys     = "1"
        CallFunctionFast(fmod1\FSOUND_PlaySound, #FSOUND_FREE, samp1)
      ElseIf keys = "2"
        CallFunctionFast(fmod2\FSOUND_PlaySound, #FSOUND_FREE, samp2)
      EndIf
      
      Delay(10)
      
    Until Asc(keys) = 27
    
    ; ==========================================================================================
    ; CLEANUP And SHUTDOWN
    ; ==========================================================================================
    
    CallFunctionFast(fmod1\FSOUND_Sample_Free, samp1)
    CallFunctionFast(fmod2\FSOUND_Sample_Free, samp2)
    
    CallFunctionFast(fmod1\FSOUND_Close)
    CallFunctionFast(fmod2\FSOUND_Close)
    
    FMOD_FreeInstance(@fmod1)
    FMOD_FreeInstance(@fmod2)
    
    CloseConsole()
    End
     
; jaPBe Version=1.4.4.25
; Build=3
; FirstLine=189
; CursorPosition=217
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\multiple\multiple.exe
; DontSaveDeclare
; EOF