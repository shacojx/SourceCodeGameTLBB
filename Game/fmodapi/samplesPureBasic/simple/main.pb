;===============================================================================================
; SIMPLE.EXE
; Copyright (c), Firelight Technologies Pty, Ltd 1999-2003.
;
; This example demonstrates some fundamental FMOD usage, including device enumeration, output
; mode selection, user file i/O callbacks, loading And playing samples And a music file, And
; calling some runtime manipulation And information functions.
;===============================================================================================
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions

XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

Global mod.l
Global samp1.l, samp2.l, samp3.l

; 1 = Windows 2 = Linux 
#OS = 1
#USECALLBACK = 1
#USEMEMORY = 1
#DEBUGGING = 0

; File callbacks

Structure FILE
  handle.l
EndStructure
Global file.FILE
Global *mem_ptr.l

; File callbacks

Procedure.l myopen(name.l)
  file\handle = CreateFile_(PeekS(name), #GENERIC_READ, #FILE_SHARE_READ, 0, #OPEN_EXISTING, #FILE_ATTRIBUTE_NORMAL, 0)  
  ProcedureReturn @file
EndProcedure

Procedure.l myclose(handle.l)
  CloseHandle_(@file\handle)
EndProcedure

Procedure.l myread(*Buffer.l, size.l, handle.l)
Protected m_dwSizeToRead.l
  m_dwSizeToRead = size
  ReadFile_(file\handle, *Buffer, m_dwSizeToRead, @ret, 0)
  If #DEBUGGING = 1
    PrintN("MyRead():: m_dwSizeToRead = " + Str(m_dwSizeToRead) + " Size = " + Str(size) + " ret = " + Str(ret))      
  EndIf 
  ProcedureReturn ret
EndProcedure
    
Procedure.l myseek(handle.l, pos.l, mode.l)
Protected m_dwFilePosition.l
     m_dwFilePosition = pos
     ret = SetFilePointer_(file\handle, m_dwFilePosition, 0, mode)
     If #DEBUGGING = 1    
       PrintN("MySeek():: m_dwFilePosition = " + Str(m_dwFilePosition) + " pos = " + Str(pos) + " mode = " + Str(mode))
     EndIf 
     ProcedureReturn ret
EndProcedure
    
Procedure.l mytell(handle.l)
      ret = SetFilePointer_(file\handle, 0, #Null, #FILE_CURRENT)
      If #DEBUGGING = 1
        PrintN("MyTell():: ret = " + Str(ret))
      EndIf
      ProcedureReturn ret
EndProcedure
    
; Memory allocation callbacks     

Procedure.l myalloc(size.l)
     *mem_ptr = GlobalAlloc_(#GMEM_FIXED, size)
     If #DEBUGGING = 1    
      PrintN("FMOD Malloc'ed " + Str(size) + " bytes" + " at " + Str(*mem_ptr) + " address")  
     EndIf
     ProcedureReturn *mem_ptr
EndProcedure

Procedure.l myrealloc(*pData.l, size.l)
    *mem_ptr = GlobalReAlloc_(*pData, size, #GMEM_Moveable)
    If #DEBUGGING = 1
      PrintN("FMOD Realloced'ed " + Str(size) + "bytes" + " at " + Str(*mem_ptr) + " address")
    EndIf
    ProcedureReturn *mem_ptr
EndProcedure
     
Procedure myfree(ptr)
    GlobalFree_(ptr)
    If #DEBUGGING = 1  
      PrintN("FMOD freed some memory")            
    EndIf
EndProcedure     

Procedure instcallback(pMod.l, param.l)
     PrintN("")
     PrintN("Inst Callback : param " + Str(PeekB(@param)) )
EndProcedure

Procedure ordercallback(pMod.l, param.l)
     PrintN("")
     PrintN("Order Callback : param " + Str(PeekB(@param)) )
EndProcedure


Procedure.l Modulo(a,b)

  ProcedureReturn a-(a/b)*b

EndProcedure
     
    OpenConsole()
    ClearConsole()
    ConsoleTitle ("PureBasic - FMOD Simple Example")
    
    LoadFMOD()
    result.f = FSOUND_GetVersion()
    Debug result
    ; call fpu register and store (pop) the value
    !FSTP dword [esp]   
    !POP [v_result]
    If (StrF(result, 2) <> StrF(FMOD_VERSION, 2)) 
      PrintN("Error : You are using the wrong DLL version!  You should be using FMOD " + StrF(FMOD_VERSION, 2))
      Delay(2000)
      CloseConsole()
      End
    EndIf
     
     String$ = Inkey() 
     If String$ = ""
           Delay(250)
           Goto Main
      EndIf
     CloseConsole()
     

Exit:
     ; CLEANUP And SHUTDOWN

     FSOUND_Sample_Free(samp1)
     FSOUND_Sample_Free(samp2)
     FSOUND_Sample_Free(samp3)
     FMUSIC_FreeSong(mod)

     FSOUND_Close()
     CloseFMOD()
     CloseConsole()
     End
     Return 


Main:
     
   ; Select OUTPUT METHOD
    
    PrintN("---------------------------------------------------------")
    PrintN("Output Type");    
    PrintN("---------------------------------------------------------")
CompilerIf #OS = 1
    PrintN("1 - Direct Sound")
    PrintN("2 - Windows Multimedia Waveout")
    PrintN("3 - ASIO")
CompilerEndIf
    
CompilerIf #OS = 2
    PrintN("1 - OSS - Open Sound System")
    PrintN("2 - ESD - Elightment Sound Daemon")
    PrintN("3 - ALSA 0.9 - Advanced Linux Sound Architecture")   
CompilerEndIf

    PrintN("4 - NoSound")   
    PrintN("---------------------------------------------------------")    ; print driver names 
    PrintN("Press a corresponding number or ESC to quit")        

     Repeat
          output.s = Left(Inkey(),1)          
          If Asc(output) = 27
               Gosub Exit
          EndIf          
     Until (Val(output) >= 1 And Val(output) <= 4)      

     Select Val(output)
          Case 1
               FSOUND_SetOutput(#FSOUND_OUTPUT_DSOUND)
          
          Case 2
     
               FSOUND_SetOutput(#FSOUND_OUTPUT_WINMM)
               
          Case 3

               FSOUND_SetOutput(#FSOUND_OUTPUT_ASIO)
               
          Case 4
               FSOUND_SetOutput(#FSOUND_OUTPUT_NOSOUND)
               
     EndSelect

    ;    Set custom file callbacks?  This doesnt have To be done, its just here as an example.
    ;    Not MIDI files do not use file callbacks so midi loading will fail.  FMUSIC_LoadSongMemory could be used To load the midi.
         
     If #USECALLBACK
        FSOUND_File_SetCallbacks(@myopen(), @myclose(), @myread(), @myseek(), @mytell())
     EndIf
         
    ;    Set custom memory callbacks?  This is optional as well of course.
    
    If #USECALLBACK
          If FSOUND_SetMemorySystem(#Null, 0, @myalloc(), @myrealloc(), @myfree()) <= 0
               PrintN("Error!")
               PrintN(Str(FMOD_ErrorString(FSOUND_GetError())))
               Delay(1000)              
               Gosub Exit
            EndIf
    Else
          memPtr = AllocateMemory(4*1024*1024)
          If FSOUND_SetMemorySystem(memPtr, 4*1024*1024, #Null, #Null, #Null) <= 0
               PrintN("FSOUND_SetMemorySystem() Error!")
               PrintN(Str(FMOD_ErrorString(FSOUND_GetError())))
               Delay(1000)              
               Gosub Exit
          EndIf                    
    EndIf

    ;  Select DRIVER               
    
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
          PrintN(Str(i) + " - " + PeekS(FSOUND_GetDriverName(i)) )
     Next i
     PrintN("---------------------------------------------------------")
     PrintN("Press a corresponding number or ESC to quit")        

     Repeat     
          driver.s = Left(Inkey(),1)          
          If Asc(driver) = 27
               Gosub Exit
          EndIf
          Delay(1)  
     Until ( Val(driver) > 0 ) And ( Val(driver) <= FSOUND_GetNumDrivers() )
          
     FSOUND_SetDriver(Val(driver) - 1)                     ; Select sound card (0 = default)
          
     ; INITIALIZE          
    
     If FSOUND_Init(44100, 32, #FSOUND_INIT_USEDEFAULTMIDISYNTH) <= 0
        PrintN("FSOUND_Init() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        Gosub Exit
     EndIf              
    
    ;  LOAD SONG    
    
    ;  The following list are the drivers For the output method selected above.    
    
     PrintN("---------------------------------------------------------")    
     PrintN(" Select Music Type\n")
     PrintN("---------------------------------------------------------")
     PrintN("1 - MOD")
     PrintN("2 - MIDI (Using Default Software Synth)")
     PrintN("---------------------------------------------------------")
     PrintN("Press a corresponding number or ESC to quit")

     Repeat     
          keys.s = Left(Inkey(),1)          
          If Asc(keys) = 27
               Gosub Exit
          EndIf
          Delay(1)
     Until ( keys = "1" ) Or ( keys = "2" )
        
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
        
     Select Val(keys)
     
        Case 1
               soundDir.s = Mid(currentDir, 1, position) + "media/invtro94.s3m"
               mod = FMUSIC_LoadSong(soundDir)
              
        Case 2     
               soundDir.s = Mid(currentDir$, 1, position) + "media/canyon.mid"
               mod = FMUSIC_LoadSong(soundDir)
               
     EndSelect
     
     If mod <= 0
        PrintN("FMUSIC_LoadSong(mod) Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        Delay(1000)
        Gosub Exit
     EndIf
     
    ;  LOAD SAMPLES     
     samp1 = 0
     samp2 = 0
     samp3 = 0
    
    ;  PCM,44,100 Hz, 8 Bit, Mono
     soundDir = Mid(currentDir, 1, position) + "media/drumloop.wav"   
     samp1 = FSOUND_Sample_Load(#FSOUND_UNMANAGED, @soundDir, #FSOUND_NORMAL | #FSOUND_HW2D, 0, 0)    ; hardware? why not, just To show it can be done
     
     If samp1 <= 0
        PrintN("FSOUND_Sample_Load(samp1) Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        Gosub Exit
     EndIf
     
     FSOUND_Sample_SetMode(samp1, #FSOUND_LOOP_OFF);  ; this wav has loop points in it which turns looping on.. turn it off!
     
    ;  PCM,22,050 Hz, 16 Bit, Mono
     soundDir = Mid(currentDir, 1, position) + "media/jaguar.wav" 
     samp2 = FSOUND_Sample_Load(#FSOUND_UNMANAGED, @soundDir, #FSOUND_NORMAL, 0, 0)
    
     If samp2 <= 0
        PrintN("Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        Delay(1000)
        Gosub Exit
     EndIf     
          
     ; PCM,22,050 Hz, 8 Bit, Stereo
     soundDir = Mid(currentDir, 1, position) + "media/chimes.wav"   
     samp3 = FSOUND_Sample_Load(#FSOUND_UNMANAGED, @soundDir, #FSOUND_NORMAL, 0, 0)
     
     If samp3 <= 0
        PrintN("Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        Delay(1000)
        Gosub Exit
     EndIf          
    
    ; DISPLAY HELP         
    
     PrintN("FSOUND Output Method : ")
     Select FSOUND_GetOutput()
     
          Case #FSOUND_OUTPUT_NOSOUND
               PrintN("FSOUND_OUTPUT_NOSOUND")
               
          Case #FSOUND_OUTPUT_WINMM
               PrintN("FSOUND_OUTPUT_WINMM")
               
          Case #FSOUND_OUTPUT_DSOUND
               PrintN("FSOUND_OUTPUT_DSOUND")
               
          Case #FSOUND_OUTPUT_ASIO
               PrintN("FSOUND_OUTPUT_ASIO")
               
          Case #FSOUND_OUTPUT_OSS          
               PrintN("FSOUND_OUTPUT_OSS")
               
          Case #FSOUND_OUTPUT_ESD
               PrintN("FSOUND_OUTPUT_ESD")
               
          Case #FSOUND_OUTPUT_ALSA
               PrintN("FSOUND_OUTPUT_ALSA")
               
     EndSelect
     
    Print("FSOUND Mixer         : ")
    Select FSOUND_GetMixer()
          
          Case #FSOUND_MIXER_BLENDMODE
               PrintN("FSOUND_MIXER_BLENDMODE")
          
          Case #FSOUND_MIXER_MMXP5
               PrintN("FSOUND_MIXER_MMXP5")
               
          Case #FSOUND_MIXER_MMXP6
               PrintN("FSOUND_MIXER_MMXP6")
               
          Case #FSOUND_MIXER_QUALITY_FPU
               PrintN("FSOUND_MIXER_QUALITY_FPU")
               
          Case #FSOUND_MIXER_QUALITY_MMXP5
               PrintN("FSOUND_MIXER_QUALITY_MMXP5")
               
          Case #FSOUND_MIXER_QUALITY_MMXP6
               PrintN("FSOUND_MIXER_QUALITY_MMXP6")
               
     EndSelect
     PrintN("FSOUND Driver        : " + PeekS(FSOUND_GetDriverName(FSOUND_GetDriver())))
     
     PrintN("=========================================================================")
     PrintN("Press 1       Play 16bit sound at any time")
     PrintN("      2       Play 8bit sound at any time (limited to 3 at a time)")
     PrintN("      3       Play 16bit STEREO sound at any time")
     PrintN("      <       Rewind mod back 1 order")
     PrintN("      >       FastForward mod forward 1 order")
     PrintN("      SPACE   Pause/unpause music at any time")
     PrintN("      ESC     Quit")
     PrintN("=========================================================================")
     PrintN("Playing " + PeekS(FMUSIC_GetName(mod)) + "...")
     
     ;For count=0 To ((FMUSIC_GetNumSamples(mod) - 2) And count < 20)  Step 2
      For count=0 To FMUSIC_GetNumSamples(mod) Step 2    
        If count = 20 
          Break
        EndIf
               first = count
               second = count + 1
               a$ = PeekS(FSOUND_Sample_GetName(FMUSIC_GetSample(mod, count)))
               b$ = PeekS(FSOUND_Sample_GetName(FMUSIC_GetSample(mod, count+1)))

               length = 33 - Len(a$)
               If count < 10
                If (count % 2 = 0)
                  Print(" ")
                EndIf
                   length + 1
               EndIf
               Print(Str(count) +  " - " + a$)                
               trimChars$ = LSet(" ", length)
               Print(trimChars$)
               PrintN(Str(count+1) + " - " + b$)
     Next count
     
     
     FSOUND_Sample_SetMaxPlaybacks(samp2, 3)
    
     ;  START PLAYING MUSIC!
    
     FMUSIC_SetOrderCallback(mod, @ordercallback(), 1)
     FMUSIC_SetInstCallback(mod, @instcallback(), 5)
     FMUSIC_SetMasterVolume(mod, 192)
     FMUSIC_SetLooping(mod, #False)
     FMUSIC_PlaySong(mod)   
   
     Repeat     
      ConsoleLocate(0, 25)                                                                                                                                                                                                                                                                                     ; x & $FF ==> unsigned to signed conversion
          Print("order = " + Str(FMUSIC_GetOrder(mod)) + "/" + Str(FMUSIC_GetNumOrders(mod)) + " row = " + Str(FMUSIC_GetRow(mod)) + "/" + Str(FMUSIC_GetPatternLength(mod, FMUSIC_GetOrder(mod))) + " time = " + Str(FMUSIC_GetTime(mod)/1000) + "." + Str(Modulo(FMUSIC_GetTime(mod), 1000)/10) + " finished " + Str(FMUSIC_IsFinished(mod) & $FF) + " channels = "+  Str(FSOUND_GetChannelsPlaying()) + " cpu = " + StrF(FSOUND_GetCPUUsage(),2) )          
          keys.s = Left(Inkey(),1)
         
          If Asc(keys)  = 27
               Gosub Exit
          EndIf
          
          ; 1
          If keys     = "1"
               FSOUND_PlaySound(#FSOUND_FREE, samp1)
          ElseIf keys = "2"
               channel = FSOUND_PlaySoundEx(#FSOUND_FREE, samp2, #Null, #True)
               FSOUND_SetCurrentPosition(channel, FSOUND_Sample_GetLength(samp2) - 1)
               FSOUND_SetFrequency(channel, -22050)    ; Play it backwards!
               FSOUND_SetVolume(channel, 255)
               FSOUND_SetPan(channel, 255)             ; pan it all the way To the right 
               FSOUND_SetPaused(channel, #False)
          ElseIf keys = "3"
               channel = FSOUND_PlaySoundEx(#FSOUND_FREE, samp3, #Null, #True)
               FSOUND_SetPaused(channel, #False)
          ; '<'
          ElseIf keys = "<"
            FMUSIC_SetOrder(mod, FMUSIC_GetOrder(mod)-1)
          ; '>'            
          ElseIf keys = ">"
               FMUSIC_SetOrder(mod, FMUSIC_GetOrder(mod)+1)
          EndIf
          
          Delay(10)
     Until Asc(Left(String$,1)) = 27
     PrintN("")
     PrintN("Exiting...") 
     Gosub Exit
     End      
                 
         
     
             
                                    
; jaPBe Version=1.4.4.25
; Build=41
; FirstLine=366
; CursorPosition=343
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\simple\simple.exe
; DontSaveDeclare
; EOF