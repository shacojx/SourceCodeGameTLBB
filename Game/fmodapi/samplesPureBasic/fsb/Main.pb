XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

; 1 = windows 2 = linux
#OS = 1

Global mod.l, Sample.l
Global sampleindex.l, variation.l, lastopenstate.l

    OpenConsole()
    ClearConsole()
    ConsoleTitle ("PureBasic - FMOD FSB Example")
    LoadFMOD()
    
    result.f = FSOUND_GetVersion()
     ; call fpu register and store (pop) the value
    !FSTP dword [esp]   
    !POP [v_result]     
    If (result < FMOD_VERSION)
      PrintN("Error : You are using the wrong DLL version!  You should be using FMOD " + StrF(FMOD_VERSION))
      Delay(2000)
      CloseConsole()
      End
    EndIf

CompilerSelect #OS
    CompilerCase 1
      FSOUND_SetOutput(#FSOUND_OUTPUT_WINMM)

    CompilerCase 2
      FSOUND_SetOutput(#FSOUND_OUTPUT_OSS)

CompilerEndSelect
      
      ; ==========================================================================================
      ; Select DRIVER
      ; ==========================================================================================
      
      ;  The following list are the drivers For the output method selected above.
      
      PrintN("---------------------------------------------------------")    
      
      Select FSOUND_GetOutput()
        CompilerIf #OS = 1
        
      Case #FSOUND_OUTPUT_NOSOUND
        Print("NoSound ")
        
      Case #FSOUND_OUTPUT_WINMM
        Print("Windows Multimedia Waveout ")
        
      Case #FSOUND_OUTPUT_DSOUND
        Print("Direct Sound ")
        
      Case #FSOUND_OUTPUT_ASIO  
        Print("ASIO ")  
        
        CompilerEndIf
        
        CompilerIf #OS = 2
        
      Case #FSOUND_OUTPUT_OSS
        Print("Open Sound System")
        
      Case #FSOUND_OUTPUT_ESD
        Print("Enlightment Sound Daemon")
        
      Case #FSOUND_OUTPUT_ALSA
        Print("ALSA")
        
        CompilerEndIf
        
    EndSelect
    
    PrintN("Driver list")
    PrintN("---------------------------------------------------------")
    
    For i = 0 To FSOUND_GetNumDrivers() - 1
      PrintN(Str(i + 1) + " - " + PeekS(FSOUND_GetDriverName(i)) )
    Next i
    PrintN("---------------------------------------------------------")
    
    Repeat     
      driver.s = Left(Inkey(),1)          
      If Asc(driver) = 27
        PrintN("FSOUND_Init() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        FSOUND_Close()
        Delay(1000)
        CloseConsole()
        End
      EndIf 
      Delay(1)
    Until ( Val(driver) > 0 ) And ( Val(driver) <= FSOUND_GetNumDrivers() )
    
    FSOUND_SetDriver(Val(driver) - 1)                     ; Select sound card (0 = default)
    
    ; ==========================================================================================
    ; INITIALIZE
    ; ==========================================================================================
    
    If FSOUND_Init(44100, 32, 0) <= 0
      PrintN("FSOUND_Init() Error!")
      PrintN(FMOD_ErrorString(FSOUND_GetError()))
      FSOUND_Close()
      CloseFMOD()        
      Delay(2000)
      CloseConsole()
      End
    EndIf 
    
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
    
    ; ==========================================================================================
    ; OPEN FSB
    ; ==========================================================================================
    mod = 0
    Sample = 0
    soundDir.s = Mid(currentDir, 1, position) + "media/footsteps.fsb"
    mod = FMUSIC_LoadSongEx(@soundDir, 0, 0, #FSOUND_NONBLOCKING, 0, 0)
    If (mod <= 0)
      PrintN("Error!\n");
      PrintN(FMOD_ErrorString(FSOUND_GetError()))
      Delay(1000)      
      FSOUND_Close()
      CloseFMOD()
      CloseConsole()
      End
    EndIf
    
    PrintN("=========================================================================")
    PrintN("Press SPACE to toggle pitch/volume variation")
    PrintN("Press ESC   to quit")
    PrintN("=========================================================================")
    PrintN("")
    
    sampleindex = 0
    variation = 1
    lastopenstate = -1
    
    Repeat 
      keys.s = Left(Inkey(),1)
      
      ConsoleLocate(0,10)
      If (variation)
        PrintN("Pitch/volume variation:    on")
      Else
        PrintN("Pitch/volume variation:   off")
      EndIf
      
      ; Set initial defaults For both samples. do This only once as soon as the FSB has finished loading.
      If ( (lastopenstate <> 0) And (FMUSIC_GetOpenState(mod) = 0))
        Sample = FMUSIC_GetSample(mod, 0)
        FSOUND_Sample_SetDefaultsEx(Sample, -1, -1, -1, -1, 2000, 128, -1)
        Sample = FMUSIC_GetSample(mod, 1)
        FSOUND_Sample_SetDefaultsEx(Sample, -1, -1, -1, -1, 2000, 128, -1)
        lastopenstate = 0
      EndIf
      
      ; Play a Sample from the FSB. do This once every frame when the FSB has finished loading.
      If (FMUSIC_GetOpenState(mod) = 0)
        Sample  = FMUSIC_GetSample(mod, (sampleindex + 1) & 1)
        FSOUND_PlaySound(#FSOUND_FREE, Sample)
      EndIf
      
      If (keys <> "")
       
        If (keys = " ")
          variation ! 1
        EndIf
      
        If (variation)
          Sample = FMUSIC_GetSample(mod, 0)
          FSOUND_Sample_SetDefaultsEx(Sample, -1, -1, -1, -1, 2000, 128, -1)
          Sample = FMUSIC_GetSample(mod, 1)
          FSOUND_Sample_SetDefaultsEx(Sample, -1, -1, -1, -1, 2000, 128, -1)
        Else
          Sample = FMUSIC_GetSample(mod, 0)
          FSOUND_Sample_SetDefaultsEx(Sample, -1, -1, -1, -1, 0, 0, 0)
          Sample = FMUSIC_GetSample(mod, 1)
          FSOUND_Sample_SetDefaultsEx(Sample, -1, -1, -1, -1, 0, 0, 0)

        EndIf
      EndIf
      
      If (variation)
        Delay(600 + Random(100) )
      Else
        Delay(650)
      EndIf
      
    Until Asc(Left(keys,1)) = 27
    PrintN("")
    FMUSIC_FreeSong(mod)
    FSOUND_Close()
    CloseFMOD()
    CloseConsole()
    End 
; jaPBe Version=1.4.4.25
; Build=7
; FirstLine=163
; CursorPosition=9
; ExecutableFormat=Windows
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\fsb\fsb.exe
; DontSaveDeclare
; EOF