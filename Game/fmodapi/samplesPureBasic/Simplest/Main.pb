;/*===============================================================================================
; SIMPLEST.EXE
; Copyright (c), Firelight Technologies Pty, Ltd, 1999,2000.
;
; This is the SIMPLEST way To play a song through FMOD.  It is basically Init, Load, play!
;===============================================================================================*/
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions

XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

Global mod.l

  OpenConsole()
  ConsoleTitle("PureBasic - FMOD Simpletest Exemple")
   
; INSTANCIATE FMOD DLL
  LoadFMOD()

  result.f = FSOUND_GetVersion()
  ; call fpu register and store (pop) the value
  !FSTP dword [esp]   
  !POP [v_result]
  If (StrF(result, 2) <> StrF(FMOD_VERSION, 2)) 
    PrintN("Error : You are using the wrong DLL version!  You should be using FMOD " + StrF(FMOD_VERSION, 2))
    Delay(2000)
    CloseConsole()
    End
  EndIf
  
;   INITIALIZE    
    If FSOUND_Init(32000, 64, 0) <= 0
        PrintN("FSOUND_Init() Error!")
        PrintN(Str(FMOD_ErrorString(FSOUND_GetError())))
        Delay(1000)
        Gosub Exit
    EndIf  
    

;   LOAD SONG    
    ; Trim the current path to the sounds directory path
    currentDir$ = Space(255)
    GetCurrentDirectory_(255, @currentDir$)
    For i = Len(currentDir$) To 1 Step -1
      If Mid(currentDir$, i, 1) = "\"
        found = found + 1
        If found = 2 
          position = i
        EndIf
      EndIf
    Next i
    soundDir$ = Mid(currentDir$, 1, position) + "media/invtro94.s3m"
    mod = FMUSIC_LoadSong(soundDir$)
    
    If (mod <= 0)
        PrintN("FMUSIC_LoadSong() Error!")
        PrintN(Str(FMOD_ErrorString(FSOUND_GetError())))
        Delay(1000)
        Gosub Exit
    EndIf
    
;   PLAY SONG              
    FMUSIC_PlaySong(mod)
    
;   UPDATE INTERFACE    
    PrintN("Press any key to quit")
    PrintN("=========================================================================")
    PrintN("Playing ..." + PeekS(FMUSIC_GetName(mod)) )

    Repeat
      String$ = Inkey()
      ConsoleLocate(0, 4)
      Print("order = " + Str(FMUSIC_GetOrder(mod)) + "/" )
      ConsoleLocate(10, 4)
      Print(Str(FMUSIC_GetNumOrders(mod)) )
      ConsoleLocate(12, 4)
      Print(" row = " + Str(FMUSIC_GetRow(mod)) + "/")      
      ConsoleLocate(22, 4)
      Print(Str(FMUSIC_GetPatternLength(mod, FMUSIC_GetOrder(mod))) )
      ConsoleLocate(24, 4)
      Print(" channels playing = " + Str(FSOUND_GetChannelsPlaying()) )
      ConsoleLocate(45, 4)
      usage.f = FSOUND_GetCPUUsage()
      ; crashes after 5 sec ... but you can use the PureBasic Library to avoid this bug :)
      ;!FSTP dword [esp]
      ;!POP [v_usage] 
      Print(" cpu usage = " + StrF(usage) )      
      Delay(100)
   Until String$ <> ""
   Gosub Exit
    
    
Exit:
    ; CLEANUP And SHUTDOWN
    FMUSIC_StopSong(mod) 
    FMUSIC_FreeSong(mod)
    FSOUND_Close()
    CloseFMOD()
    CloseConsole()
    End
    Return     
; jaPBe Version=1.4.4.25
; Build=7
; Language=0x0000 Language Neutral
; FirstLine=44
; CursorPosition=84
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\Simplest\simplest.exe
; DontSaveDeclare
; EOF