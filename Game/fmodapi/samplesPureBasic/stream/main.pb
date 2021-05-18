;===============================================================================================
; stream.EXE
; Copyright (c), Firelight Technologies Pty, Ltd, 1999-2004.
;
; This example takes a command line parameter, a wav/mp2/mp3/ogg etc file, And uses the streamer 
; system To play it back.
;===============================================================================================
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions

XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

; 1 = Windows 2 = Linux 
#OS = 1
#USECALLBACK = 1
#STREAM_FROM_MEMORY = 0
#DEBUGGING = 0

Global channel.l
Global curpos.l

Structure FILE
  handle.l
EndStructure
Global file.FILE

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

Procedure instcallback(pMod.l, param.l)
     PrintN("")
     PrintN("Inst Callback : param " + Str(PeekB(@param)) )
EndProcedure

Procedure ordercallback(pMod.l, param.l)
     PrintN("")
     PrintN("Order Callback : param " + Str(PeekB(@param)) )
EndProcedure


;[
;    [DESCRIPTION]
;    End of stream user callback, initialized with FSOUND_Stream_SetEndCallback Or 
;    FSOUND_Stream_SetSynchCallback
;
;    [PARAMETERS]
;    'stream'      A pointer To the stream that ended.
;    'buff'        This is NULL For End of stream callbacks, Or a string For synch callbacks.
;    'len'         This is reserved And is always 0 For End And synch callbacks. ignore.
;    'param'       This is the value passed To FSOUND_Stream_SetEndCallback Or 
;                  FSOUND_Stream_SetSynchCallback as a user Data value.
; 
;    [RETURN_VALUE]
;    TRUE Or FALSE, the value is ignored.
;
;    [REMARKS]
;
;    [SEE_ALSO]
;]

Procedure.l endcallback(*stream.l, *buff.l, len.l, param.l)
;     End of stream callback doesnt have a 'buff' value, If it doesnt it could be a synch point.
    ConsoleLocate(0, 25)  
    If *buff
          PrintN("")
          PrintN("SYNCHPOINT : " + PeekS(*buff))
    Else
          PrintN("")
          PrintN("STREAM ENDED!!")
    EndIf
    
    ProcedureReturn #True
EndProcedure
     
     OpenConsole()
     ClearConsole()
     ConsoleTitle ("PureBasic - FMOD Simple Example")

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
    
     If #DEBUGGING = 1
      filename.s = "j:\Mp3\ChrisHuelbeck\Katakis_Medley_2000_G_.mp3"
     Else
      filename.s = ProgramParameter() 
     EndIf
    
     If filename = ""
        PrintN("-------------------------------------------------------------")
        PrintN("PureBasic - FMOD Streamer example.")
        PrintN("Copyright (c) Firelight Technologies Pty, Ltd, 1999-2003.")
        PrintN("-------------------------------------------------------------")
        PrintN("Syntax: stream infile.[mp2 mp3 wav ogg wma asf]")
        PrintN("")
        Delay(2000)
        CloseConsole()
        End
     EndIf
     
     ; Select OUTPUT METHOD
    
CompilerIf #OS = 1

CompilerEndIf
     FSOUND_SetOutput(#FSOUND_OUTPUT_WINMM)
CompilerIf #OS = 2
     FSOUND_SetOutput(#FSOUND_OUTPUT_OSS)
CompilerEndIf

    ; Set custom file callbacks?  This doesnt have To be done, its just here as an example.

    FSOUND_File_SetCallbacks(@myopen(), @myclose(), @myread(), @myseek(), @mytell())

    ; ==========================================================================================
    ;  Select DRIVER               
    ; ==========================================================================================
    
    ; The following list are the drivers For the output method selected above.
    
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
     PrintN("Press a corresponding number or ESC to quit")        
    
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

          
     FSOUND_SetDriver(Val(driver) - 1)                    ; Select sound card (0 = default)
     
     ; ==========================================================================================
     ; INITIALIZE          
     ; ==========================================================================================
    
     If FSOUND_Init(44100, 32, 0) <= 0
        PrintN("FSOUND_Init() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        FSOUND_Close()
        Delay(1000)
        CloseConsole()
        End
     EndIf              
   
    FSOUND_Stream_SetBufferSize(1000)  
    
     ; ==========================================================================================
     ; OPEN STREAM (use #if 1 For streaming from memory)
     ; ==========================================================================================
     If #STREAM_FROM_MEMORY
          If ReadFile(0, filename)
               FileLength = Lof()
               *pData   = AllocateMemory(FileLength)
               ReadData(*pData, FileLength)
               CloseFile(0)
              stream = FSOUND_Stream_Open(*pData, #FSOUND_NORMAL | #FSOUND_MPEGACCURATE | #FSOUND_LOADMEMORY, 0, FileLength)
              If stream <= 0 
                PrintN("Stream_Open Error! : can't open " + filename$ + " stream = " + Str(stream))
                PrintN(FMOD_ErrorString(FSOUND_GetError()))
                FSOUND_Close()
                CloseConsole()
                End
              EndIf                 
          EndIf        
     Else
          If Mid(filename, 1, 5) = "http:"
               PrintN("Connecting to " + filename + " please wait (this may take some time)....")
          Else
               stream = FSOUND_Stream_Open(@filename, #FSOUND_NORMAL | #FSOUND_MPEGACCURATE, 0, 0)
               If stream <= 0 
                  PrintN("Stream_Open Error! : can't open " + filename + " stream = " + Str(stream))
                  PrintN(FMOD_ErrorString(FSOUND_GetError()))
                  FSOUND_Close()
                  CloseConsole()
                  End
               EndIf    
          EndIf               
        EndIf
        
     ; ==========================================================================================
     ; SET AN End OF STREAM CALLBACK And RIFF SYNCH POINTS CALLBACK
     ; ==========================================================================================     
     
     FSOUND_Stream_SetEndCallback(stream, @endcallback(), 0)
     FSOUND_Stream_SetSyncCallback(stream,@endcallback(), 0)               

     PrintN("=========================================================================")
     PrintN("Press SPACE to pause/unpause")
     PrintN("Press 'f'   to fast forward 2 seconds")
     PrintN("Press ESC   to quit")
     PrintN("=========================================================================")
     PrintN("Playing stream...")
     PrintN("")
    
     sptr = FSOUND_Stream_GetSample(stream);
     If sptr > 0
        freq.l
        
        FSOUND_Sample_GetDefaults(sptr, @freq, #Null, #Null, #Null)
        PrintN("Name      : " +  PeekS(FSOUND_Sample_GetName(sptr)) )
        PrintN("Frequency : " + Str(freq))
        PrintN("")
        
     EndIf   

     channel = -1
     curpos.l = 0
     Repeat     
          ; a simple hack to update the console window
          keys.s = Left(Inkey(),1)             
          ConsoleLocate(0, 18)

          If channel < 0
            ; ==========================================================================================
            ; PLAY STREAM
            ; ==========================================================================================
            channel = FSOUND_Stream_PlayEx(#FSOUND_FREE, stream, #Null, #True)
            FSOUND_SetPaused(channel, #False)
          EndIf
          
          cpuusage.s = " cpu  " + StrF(FSOUND_GetCPUUsage(), 2) + " %" 
          Print("pos " + Str(FSOUND_Stream_GetPosition(stream)) + "/" + Str(FSOUND_Stream_GetLength(stream)) + " time " + RSet$(Str(FSOUND_Stream_GetTime(stream) / 1000 / 60), 2, "0") + ":" + RSet$(Str((FSOUND_Stream_GetTime(stream) / 1000)%60), 2, "0") + "/" + RSet$(Str(FSOUND_Stream_GetLengthMs(stream) / 1000 / 60), 2, "0") + ":" + RSet$(Str((FSOUND_Stream_GetLengthMs(stream) / 1000)%60), 2, "0") + cpuusage  )

          If Asc(keys)  = 27
               FSOUND_Stream_Close(stream)
               FSOUND_Close()
               Delay(2000)
               CloseConsole()
               End
          EndIf
          
          If keys = "f"
            curpos = FSOUND_Stream_GetTime(stream)
            Debug curpos
            curpos + 2000
            Debug curpos            
            FSOUND_Stream_SetTime(stream, curpos)
          ElseIf keys = " "
            FSOUND_SetPaused(channel, FSOUND_GetPaused(channel) ! 1)
          EndIf
          
          Delay(10)
     Until Asc(Left(String$,1)) = 27
     PrintN("")
     PrintN("Exiting...") 

     FSOUND_Stream_Close(stream)
     FSOUND_Close()
     CloseFMOD()
     CloseConsole()
     End      
                 
         
     
             
                                    
; jaPBe Version=1.4.4.25
; Build=72
; Language=0x0000 Language Neutral
; FirstLine=301
; CursorPosition=219
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\stream\stream.exe
; DontSaveDeclare
; EOF