;===============================================================================================
; DSP.EXE
; Copyright (c), Firelight Technologies Pty, Ltd, 1999-2004.
;
; This example demonstrates advanced DSP usage.
; You can now attach sounds To DSP units.  The DSP units To be attached To must have a NULL
; callback.  It is simply a holder For sounds To attach To, And have a specific position in 
; The DSP chain.. see The diagram below For a visual representation of The DSP chain.
; It also demonstrates The use of hardware DirectX 8 FX.
;===============================================================================================
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions

    
XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

; GLOBALS And DEFINIITIONS
#REVERB_NUMTAPS = 7

Structure REVERBTAP
     Unit.l
     historybuff.l
     workarea.l
     delayms.l
     volume.l
     pan.l
     historyoffset.l
     historylen.l
EndStructure

Structure Delays
  x.l
EndStructure

Structure Volumes
  x.l
EndStructure

Structure Pans
  x.l
EndStructure


; Reverb stuff
Dim DSP_ReverbTap.REVERBTAP(#REVERB_NUMTAPS)    
Dim Delays.Delays(#REVERB_NUMTAPS)    
Dim Volumes.Volumes(#REVERB_NUMTAPS)    
Dim Pans.Pans(#REVERB_NUMTAPS)    

; Dry sfx unit
DrySFXUnit.l = 0

Global samp1.l, samp2.l
Global fxChannel.l
Global echoid.l, echoid2.l, flangeid.l, firsttime.l

;[
;    [DESCRIPTION]
;    Callback To mix in one reverb tap.  It copies the buffer into its own history buffer also.
;
;    [PARAMETERS]
;    'originalbuffer'    Pointer To the original mixbuffer, not any buffers passed down 
;                        through the dsp chain.  They are in newbuffer.
;    'newbuffer'         Pointer To buffer passed from previous DSP unit.
;    'length'            Length in SAMPLES of buffer being passed.
;    'param'             User parameter.  In this Case it is a pointer To DSP_LowPassBuffer.
; 
;    [RETURN_VALUE]
;    a pointer To the buffer that was passed in, with a tap mixed into it.
;
;    [REMARKS]
;]

Procedure DSP_ReverbCallback(originalbuffer.l, newbuffer.l, length.l, param.l)

     mixertype = FSOUND_GetMixer()
     *tap.REVERBTAP = param
     
;     must be 16bit stereo integer buffer.. sorry fpu (32bit float) dont support this.     
     If (mixertype = #FSOUND_MIXER_BLENDMODE Or mixertype = #FSOUND_MIXER_QUALITY_FPU)
        ProcedureReturn newbuffer
     EndIf
     
     #FEEDBACK = 1    ; this determines whether To use already pre-reverbed Data For the tap, Or the original clean buffer
     
;    Reverb history buffer is a ringbuffer.  If the length makes the copy wrap, then split the copy 
;    into End part, And start part..      

    If (*tap\historyoffset + length > *tap\historylen)
          taillen = *tap\historylen - *tap\historyoffset
          startlen = length - taillen
        
;         Mix a scaled version of history buffer into output        
          FSOUND_DSP_MixBuffers(newbuffer, *tap\historybuff + (*tap\historyoffset << 2), taillen, 44100, *tap\volume, *tap\pan, #FSOUND_STEREO | #FSOUND_16BITS);
          FSOUND_DSP_MixBuffers(newbuffer+((*tap\historylen - *tap\historyoffset) << 2), *tap\historybuff, startlen, 44100, *tap\volume, *tap\pan, #FSOUND_STEREO | #FSOUND_16BITS)
          
;         Now copy input into reverb/history buffer 

         If #FEEDBACK          
               CopyMemory(newbuffer, *tap\historybuff + (*tap\historyoffset << 2), taillen<<2)
               CopyMemory(newbuffer + ((*tap\historylen - *tap\historyoffset) << 2), *tap\historybuff, startlen << 2)
         Else
               CopyMemory(originalbuffer, *tap\historybuff + (*tap\historyoffset << 2), taillen<<2)
               CopyMemory(originalbuffer + ((*tap\historylen - *tap\historyoffset) << 2), *tap\historybuff, startlen << 2)
         EndIf
    
;   No wrapping reverb buffer, just write dest
     Else
        ; Mix a scaled version of history buffer into output                     
        FSOUND_DSP_MixBuffers(newbuffer, *tap\historybuff + (*tap\historyoffset << 2), length, 44100, *tap\volume, *tap\pan, #FSOUND_STEREO | #FSOUND_16BITS)

;       Now copy input into reverb/history buffer 
         If #FEEDBACK  
               CopyMemory(newbuffer, *tap\historybuff + (*tap\historyoffset << 2), length<<2)
         Else
               CopyMemory(originalbuffer, *tap\historybuff + (*tap\historyoffset << 2), length<<2)
         EndIf
         
    EndIf
    
    *tap\historyoffset = *tap\historyoffset + length    
    If (*tap\historyoffset >= *tap\historylen) 
        *tap\historyoffset = *tap\historyoffset - *tap\historylen
    EndIf    
    
;   Reverb history has been mixed into new buffer, so Return it.

    ProcedureReturn newbuffer
    
  EndProcedure
  

;[
;  [DESCRIPTION]
;  
;  [PARAMETERS]
;  
;  [RETURN_VALUE]
;  
;  [REMARKS]
;  
;  [SEE_ALSO]
;]
Procedure SetupReverb()

; ====================================================================================================================
; REVERB SETUP
; ====================================================================================================================
     
     Restore delay
     For i = 0 To #REVERB_NUMTAPS - 1
          Read Delays(i)\x
     Next i

     Restore volume     
     For i = 0 To #REVERB_NUMTAPS - 1
          Read Volumes(i)\x
     Next i
     
     Restore pan
     For i = 0 To #REVERB_NUMTAPS - 1
          Read Pans(i)\x
     Next i     
     
     For count = 0 To #REVERB_NUMTAPS -1
        DSP_ReverbTap(count)\delayms        = Delays(count)\x
        DSP_ReverbTap(count)\volume         = Volumes(count)\x
        DSP_ReverbTap(count)\pan            = Pans(count)\x
        DSP_ReverbTap(count)\historyoffset  = 0
        DSP_ReverbTap(count)\historylen     = (DSP_ReverbTap(count)\delayms * 44100 / 1000)
        If (DSP_ReverbTap(count)\historylen < FSOUND_DSP_GetBufferLength())
            DSP_ReverbTap(count)\historylen = FSOUND_DSP_GetBufferLength()    ; just in Case our calc is not the same. 
        EndIf
        
        DSP_ReverbTap(count)\historybuff    = AllocateMemory(DSP_ReverbTap(count)\historylen * 4)    ; * 4 is For 16bit stereo (mmx only) 
        DSP_ReverbTap(count)\workarea       = #Null
        DSP_ReverbTap(count)\Unit           = FSOUND_DSP_Create(@DSP_ReverbCallback(), #FSOUND_DSP_DEFAULTPRIORITY_USER+20+(count*2),    @DSP_ReverbTap(count))

        FSOUND_DSP_SetActive(DSP_ReverbTap(count)\Unit, #True)
     Next count     
     
     
  EndProcedure
  
;[
;  [DESCRIPTION]
;  
;  [PARAMETERS]
;  
;  [RETURN_VALUE]
;  
;  [REMARKS]
;  
;  [SEE_ALSO]
;]     
Procedure CloseReverb()

    For count = 0 To #REVERB_NUMTAPS - 1
        FSOUND_DSP_Free(DSP_ReverbTap(count)\Unit)
        DSP_ReverbTap(count)\Unit = #Null
        DSP_ReverbTap(count)\historybuff = #Null
        DSP_ReverbTap(count)\workarea = NULL
    Next count

    ; Free All Memory Allocated By AllocateMemory
    FreeMemory(-1)
EndProcedure


    OpenConsole()
    ClearConsole()
    ConsoleTitle ("PureBasic - FMOD DSP Example")
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
    
    ; INITIALIZE          
    FSOUND_SetBufferSize(100)

    If FSOUND_Init(44100, 32, #FSOUND_INIT_ENABLESYSTEMCHANNELFX) <= 0
       PrintN("FSOUND_Init() Error!")
       PrintN(FMOD_ErrorString(FSOUND_GetError()))
       FSOUND_Close()
       CloseFMOD()
       Delay(2000)
       CloseConsole()
       End
    EndIf        
     
     ; LOAD SAMPLES     
     
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
           
     ; PCM,44,100 Hz, 8 Bit, Mono 
     soundsDir.s = Mid(currentDir$, 1, position) + "media/drumloop.wav"
     samp1 = FSOUND_Sample_Load(#FSOUND_FREE, @soundsDir, #FSOUND_2D, 0, 0)
     If samp1 <= 0 
          PrintN("Error!")
          PrintN(FMOD_ErrorString(FSOUND_GetError()))
          FSOUND_Close()
          CloseFMOD()          
          CloseConsole()
          End
     EndIf        
     FSOUND_Sample_SetMode(samp1, #FSOUND_LOOP_OFF)     
     
     ; PCM,44,100 Hz, 16 Bit, Stereo
     soundsDir = Mid(currentDir$, 1, position) + "media/jules.mp3"
     samp2 = FSOUND_Sample_Load(#FSOUND_FREE, @soundsDir, #FSOUND_HW2D | #FSOUND_ENABLEFX, 0, 0)
     If samp2 <= 0 
          PrintN("Error!")
          PrintN(FMOD_ErrorString(FSOUND_GetError()))
          FSOUND_Close()
          CloseFMOD()
          CloseConsole()
          End
     EndIf             
   
     ; DISPLAY HELP     
    Print("FSOUND Output Method : ")     
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
          
          Case #FSOUND_MIXER_QUALITY_MMXP5
               PrintN("FSOUND_MIXER_QUALITY_MMXP5")
               
          Case #FSOUND_MIXER_QUALITY_MMXP6
               PrintN("FSOUND_MIXER_QUALITY_MMXP6")
               
          Case #FSOUND_MIXER_QUALITY_FPU
               PrintN("FSOUND_MIXER_QUALITY_FPU")
               
     EndSelect     
     
     PrintN("FSOUND Driver        : " + PeekS(FSOUND_GetDriverName(FSOUND_GetDriver())))
     
     PrintN("=========================================================================")
     PrintN("Press 1       Play SOFTWARE sound affected by following reverb dsp unit (wet)")
     PrintN("      2       Play SOFTWARE sound unaffected by following reverb dsp unit (dry)")
     If (FSOUND_GetOutput() = #FSOUND_OUTPUT_DSOUND)
       PrintN("      3       Play HARDWARE FX enabled sound using Direct X 8 (echo+flange)")
       PrintN("      4       Set EQ on global software output to be affect by DX8 FX")
       PrintN("              Press 1 or 2 to hear the effect (3 is unaffected)")
       PrintN("      5       Turn off EQ on global software output")
     EndIf
     PrintN("      ESC     Quit")
     PrintN("=========================================================================")
     
     ; SET UP DSPS!     
     SetupReverb()
     
     ; Note If we are using a dsp unit For playing sounds, callback And parameter are ignored!
     DrySFXUnit = FSOUND_DSP_Create(#Null, #FSOUND_DSP_DEFAULTPRIORITY_USER+100, 0)
     FSOUND_DSP_SetActive(DrySFXUnit, #True)
     
     ; You must pause the software output before getting the FX handle on it.     
     If (FSOUND_GetOutput() = #FSOUND_OUTPUT_DSOUND)    
       FSOUND_SetPaused(#FSOUND_SYSTEMCHANNEL, #True)
     
       eqid1 = FSOUND_FX_Enable(#FSOUND_SYSTEMCHANNEL, #FSOUND_FX_PARAMEQ)
       eqid2 = FSOUND_FX_Enable(#FSOUND_SYSTEMCHANNEL, #FSOUND_FX_PARAMEQ)     

       FSOUND_SetPaused(#FSOUND_SYSTEMCHANNEL, #False)           
     EndIf 
     
     ; START PLAYING!
     fxChannel = #FSOUND_FREE
     echoid.l = -1: echoid2.l = -1: flangeid.l = -1
    
    Repeat
          keys.s = Left(Inkey(),1)          
          
          ConsoleLocate(0,12)
          ; Use The Fmod PureBasic Library If You Want To Get FSOUND_GetCPUUsage Working
          ;CpuUsage.l = FSOUND_GetCPUUsage()
          ;Print("channels playing = " + Str(FSOUND_GetChannelsPlaying()) + " cpu usage = " +  StrF(FSOUND_GetCPUUsage(), 2))          
          Print("channels playing = " + Str(FSOUND_GetChannelsPlaying()) )          
          
          If keys = "1"
               channel = FSOUND_PlaySound(#FSOUND_FREE, samp1)
          
          ElseIf keys = "2"
               FSOUND_PlaySoundEx(#FSOUND_FREE, samp1, DrySFXUnit, #False)
          
          ElseIf keys = "3"
               If (fxChannel= #FSOUND_FREE)
                    firsttime = #True
               Else
                    firsttime = #False
               EndIf
               
               fxChannel= FSOUND_PlaySoundEx(fxChannel, samp2, DrySFXUnit, #True)
                                             
               ; NOTE! Even though it is For hardware FX, set it To a DrySFXUnit just 
               ; in Case a non hardware output mode has been selected (such as 
               ; WINMM/Linux etc) And it actually drops back To 100% software 
               
               FSOUND_SetVolume(fxChannel, 120) ; turn it down a bit! 

               If (firsttime = #True)
                    echoid   = FSOUND_FX_Enable(fxChannel, #FSOUND_FX_ECHO)
                    echoid2  = FSOUND_FX_Enable(fxChannel, #FSOUND_FX_ECHO)
                    flangeid = FSOUND_FX_Enable(fxChannel, #FSOUND_FX_FLANGER)
               EndIf
               
               FSOUND_SetPaused(fxChannel, #False)

               FSOUND_FX_SetEcho(echoid,  80.0, 70.0, 100.0, 100.0, #True)
               FSOUND_FX_SetEcho(echoid2,  100.0, 70.0, 10, 10, #False)
               
          
          ElseIf keys = "4"
               FSOUND_FX_SetParamEQ(eqid1, 8000, 36, -15)
               FSOUND_FX_SetParamEQ(eqid2, 16000, 36, -15)
               
          ElseIf keys = "5"
               FSOUND_FX_SetParamEQ(eqid1, 8000, 15, 0)
               FSOUND_FX_SetParamEQ(eqid2, 8000, 15, 0)          
          EndIf
            
          Delay(100)
     
     Until Asc(keys) = 27

     ; CLEANUP And SHUTDOWN     
     
     FSOUND_DSP_Free(DrySFXUnit)
    
     CloseReverb()

     FSOUND_Sample_Free(samp1)
     FSOUND_Sample_Free(samp2)

     FSOUND_Close()
     CloseFMOD()
     CloseConsole()
     End
     

DataSection
     delay:
     Data.l    131, 149, 173, 211, 281, 401, 457
     
     volume:
     Data.l    120, 100, 95, 90, 80, 60, 50
     
     pan:
     Data.l    100, 128, 128, 152, 128, 100, 152
EndDataSection     
      
; jaPBe Version=1.4.4.25
; Build=30
; Language=0x0000 Language Neutral
; FirstLine=178
; CursorPosition=203
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\dsp\dsp.exe
; DontSaveDeclare
; EOF