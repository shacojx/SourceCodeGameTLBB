;===============================================================================================
; STREAM2.EXE
; Copyright (c), Firelight Technologies Pty, Ltd, 1999-2004.
;
; This sample specifically demonstrates the user callback streaming facility, And generates a
; very strange noise! :)
;===============================================================================================
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions

XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

; 1 = windows 2 = linux
#OS = 1

Global t1.f, t2.f
Global v1.f, v2.f

;
;[
;    [DESCRIPTION]
;    user dsp callback For a stream!
;      
;    [PARAMETERS]
;      
;    [RETURN_VALUE]
;      
;    [REMARKS]
;      
;    [SEE_ALSO]
;]
;      
Procedure.l dspcallback(originalbuffer.l, newbuffer.l, length.l, param.l)
  *stereo16bitbuffers.w =  newbuffer
  
  For count = 0 To length - 1
     PokeW(*stereo16bitbuffers, PeekW(*stereo16bitbuffers) >> 1 )
      *stereo16bitbuffers + 2
     PokeW(*stereo16bitbuffers, PeekW(*stereo16bitbuffers) >> 1 )
      *stereo16bitbuffers + 2
  Next count
  
  ProcedureReturn newbuffer
  
EndProcedure

;[
;    [DESCRIPTION]
;    user streamer callback 
;    
;    [PARAMETERS]
;    'stream'         pointer To the stream supplying the callback
;    'buff'           pointer To the streamer buffer To fill.
;    'len'            length of the Data block in BYTES
;    
;    [RETURN_VALUE]
;    
;    [REMARKS]
;    What a strange noise!!!
;    (heh heh)
;    
;    [SEE_ALSO]
;]
;
Procedure.l streamcallback(*stream.l, *buff.l, len.l, param.l)
  stereo16bitbuffers.w
  
  *stereo16bitbuffers = *buff

  For count = 0 To (len >> 2) - 1
    PokeW(*stereo16bitbuffers, (Sin(t1) * 32767.0) )
     *stereo16bitbuffers + 2
    PokeW(*stereo16bitbuffers, (Sin(t2) * 32767.0) )
     *stereo16bitbuffers + 2
   
     t1 + 0.1   + v1
     t2 + 0.142 + v2
     v1 + (Sin(t1) * 0.02)
     v2 + (Sin(t2) * 0.02)
     
  Next count        

  ProcedureReturn 1
  
EndProcedure

;
;[
;    [DESCRIPTION]
;    End of stream user callback, initialized with FSOUND_Stream_SetEndCallback Or 
;    FSOUND_Stream_SetSynchCallback
;    
;    [PARAMETERS]
;    'stream'     a pointer To the stream that ended.
;    'buff'       This is NULL For End of stream callbacks, Or a string For synch callbacks.
;    'len'        This is reserved And is always 0 For End And synch callbacks. ignore.
;    'param'      This is the value passed To FSOUND_Stream_SetEndCallback Or 
;     FSOUND_Stream_SetSynchCallback as a user Data value.
;          
;    [RETURN_VALUE]
;    TRUE Or FALSE, the value is ignored.
;          
;    [REMARKS]
;          
;    [SEE_ALSO]
;]
;
Procedure.l endcallback(*stream.l, *buff.l, len.l, param.l)
  
  If *buff
    PrintN("")
    PrintN("SYNCHPOINT : " + PeekS(*buff))
  Else
    PrintN("")
    PrintN("STREAM ENDED!!")
  EndIf

  ProcedureReturn #True
  
EndProcedure

;
;[
;    [DESCRIPTION]
;    
;    [PARAMETERS]
;    
;    [RETURN_VALUE]
;    
;    [REMARKS]
;    
;    [SEE_ALSO]
;    ]
;
    
     OpenConsole()
     ClearConsole()
     ConsoleTitle ("PureBasic - FMOD Stream2 Example")
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
     
     PrintN("-------------------------------------------------------------")
     PrintN("PureBasic - FSOUND Streamer example.")
     PrintN("Copyright (c) Firelight Technologies Pty, Ltd, 2001-2003.")
     PrintN("-------------------------------------------------------------")
     
     PrintN("---------------------------------------------------------") 
     PrintN("Output Type")
     PrintN("---------------------------------------------------------")
     
CompilerSelect #OS
     CompilerCase 1
      PrintN("1 - Direct Sound ")
      PrintN("2 - Windows Multimedia Waveout")
      PrintN("3 - ASIO")
     
     CompilerCase 2
      PrintN("1 - OSS - Open Sound System)
      PrintN("2 - ESD - Enlightment Sound Daemon)
      PrintN("3 - ALSA 0.9 - Advance Linux Sound Architecture)
      
CompilerEndSelect

      PrintN("4 - NoSound")
      PrintN("---------------------------------------------------------") ; print driver names
      PrintN("Press a corresponding number or ESC to quit")
        
     Repeat
      output.s = Left(Inkey(),1)          
      If Asc(output) = 27
          FSOUND_Close()
          CloseFMOD()
          Delay(2000)
          CloseConsole()
          End
      EndIf  
      Delay(1)
     Until (Val(output) >= 1 And Val(output) <= 4) 
        
     Select Val(output)
      CompilerSelect #OS
        CompilerCase  1
            Case 1
                FSOUND_SetOutput(#FSOUND_OUTPUT_DSOUND)
            Case 2
                FSOUND_SetOutput(#FSOUND_OUTPUT_WINMM)
            Case 3
                FSOUND_SetOutput(#FSOUND_OUTPUT_ASIO)                
        
        CompilerCase 2
            Case 1
                FSOUND_SetOutput(#FSOUND_OUTPUT_OSS)
            Case 2
                FSOUND_SetOutput(#FSOUND_OUTPUT_ESD)
            Case 3
                FSOUND_SetOutput(#FSOUND_OUTPUT_ALSA)
        CompilerEndSelect     
            Case 4
                FSOUND_SetOutput(#FSOUND_OUTPUT_NOSOUND)
            EndSelect 
            
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
    
     If FSOUND_Init(44100, 16, 0) <= 0
        PrintN("FSOUND_Init() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        FSOUND_Close()
        CloseFMOD()        
        Delay(2000)
        CloseConsole()
        End
      EndIf     
      
    ; ==========================================================================================
    ; CREATE user stream
    ; ==========================================================================================     
    stream = FSOUND_Stream_Create(@streamcallback(), 6*2048, #FSOUND_NORMAL | #FSOUND_16BITS | #FSOUND_STEREO, 44100, 12345)
    If stream <= 0
        PrintN("FSOUND_Stream_Create() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        FSOUND_Close()
        CloseFMOD()        
        Delay(2000)
        CloseConsole()
        End
    EndIf   
    
   FSOUND_Stream_SetEndCallback(stream, @endcallback(), 0)         
  
   dsp1 = FSOUND_Stream_CreateDSP(stream, @dspcallback(), 0, 0) ; priority 0 = it comes first in dsp chain.
   dsp2 = FSOUND_Stream_CreateDSP(stream, @dspcallback(), 1, 0) ; priority 1 = it comes last

   PrintN("Press any key to quit")
   PrintN("=========================================================================")
   PrintN("Playing stream...")
  
    ; ==========================================================================================
    ; PLAY stream
    ; ==========================================================================================
   If FSOUND_Stream_Play(#FSOUND_FREE, stream) = -1
        PrintN("FSOUND_Stream_Play() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        FSOUND_Close()
        CloseFMOD()        
        Delay(2000)
        CloseConsole()
        End
   EndIf    
   
   PrintN("******* Hit a key to active stream DSP unit #1 to halve the stream volume.")
   Repeat 
    String$ = Inkey()
   Until String$ <> ""
   FSOUND_DSP_SetActive(dsp1, 1)
   
   PrintN("******* Now hit a key to active stream DSP unit #2 to quarter the stream volume.")
   Repeat 
    String$ = Inkey()
   Until String$ <> ""
   FSOUND_DSP_SetActive(dsp2, 1);
   
   PrintN("******* Now hit a key to finish.")
   Repeat 
    String$ = Inkey()
   Until String$ <> ""  
   PrintN("")
   
   FSOUND_DSP_Free(dsp1)
   FSOUND_DSP_Free(dsp2)
   
   FSOUND_Stream_Close(stream)
   
   FSOUND_Close()
   CloseFMOD()
   CloseConsole()
   End
   
   
   
      
   
   
   
   


   
    
; jaPBe Version=1.4.4.25
; Build=8
; Language=0x0000 Language Neutral
; FirstLine=316
; CursorPosition=270
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\stream2\stream2.exe
; DontSaveDeclare
; EOF