;===============================================================================================
; RECORD.EXE
; Copyright (c), Firelight Technologies Pty, Ltd, 2000-2004.
;
; This example shows how To RECORD Data To a static sample, Or RECORD dynamically, And have
; a dsp Unit processing the result.
; the reverb below is taken from /samples/fmod/fmod.c 
;===============================================================================================
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions

    
XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

; GLOBALS And DEFINITIONS

; 1 = windows 2 = linux
#OS = 1

Structure RIFF_HEADER
  RIFF.l
  riffBlockSize.l
  riffBlockType.l
EndStructure

Structure WAVE_HEADER
  dataBlockType.l
  dataBlockSize.l
EndStructure

Structure WAVE_FORMAT
  wfBlockType.l
  wfBlockSize.l
  wFormatTag.w
  nChannels.w
  nSamplesPerSec.l
  nAvgBytesPerSec.l
  nBlockAlign.w
  wBitsPerSample.w
EndStructure  
#OUTPUT_FILE = 0

#RECORDRATE = 44100
#RECORDLEN  = (#RECORDRATE*5)       ;  5 seconds at 44khz 
#RECORD_DELAY_MS =  25
#RECORD_DELAY_SAMPLES = (#RECORDRATE * #RECORD_DELAY_MS / 1000)
#ENABLEREVERB = #True
#VUSPEED = 0.2

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
dspChannel.l = #FSOUND_FREE
DefType.f leftValue, rightValue


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

Procedure SetupReverb()
     
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
     
     
Procedure CloseReverb()
     

    For count = 0 To #REVERB_NUMTAPS - 1
        FSOUND_DSP_Free(DSP_ReverbTap(count)\Unit)
        DSP_ReverbTap(count)\Unit = #Null
        DSP_ReverbTap(count)\historybuff = #Null
        DSP_ReverbTap(count)\workarea = NULL
    Next count

    FreeMemory(-1)
    
  EndProcedure
  
Procedure SaveToWav(m_sample.l)
    Protected m_samplesNeeded.l   : m_sampleProp.l
    Protected m_hOrgFile.l        : m_hNewFile.l
    Protected m_frequency.l       : lenbytes.l
    Protected m_ptr1.l            : m_ptr2.l : m_len1.l    : m_len2.l
    
    Protected m_structRIFF.RIFF_HEADER
    Protected m_structWAVEHdr.WAVE_HEADER
    Protected m_structWAVEFmt.WAVE_FORMAT
    
    m_outputFilename.s = "record.wav"
    ; Prepare the Wave Header
    m_sampleProp = FSOUND_Sample_GetMode(m_sample)
    m_structWAVEFmt\wFormatTag          = 1
    If (m_sampleProp & #FSOUND_MONO)
      m_structWAVEFmt\nChannels      = 1
    Else
      m_structWAVEFmt\nChannels      = 2
    EndIf                       
    FSOUND_Sample_GetDefaults(m_sample, @m_frequency, 0, 0, 0)
    m_structWAVEFmt\nSamplesPerSec      = m_frequency
    If (m_sampleProp & #FSOUND_8BITS)
      m_structWAVEFmt\wBitsPerSample = 8
    Else
      m_structWAVEFmt\wBitsPerSample = 16
    EndIf    
    lenbytes = FSOUND_Sample_GetLength(m_sample) * m_structWAVEFmt\nChannels * m_structWAVEFmt\wBitsPerSample / 8
    m_structWAVEFmt\nBlockAlign         = m_structWAVEFmt\nChannels * m_structWAVEFmt\wBitsPerSample / 8
    m_structWAVEFmt\nAvgBytesPerSec     = m_structWAVEFmt\nSamplesPerSec * m_structWAVEFmt\nBlockAlign
    m_structWAVEFmt\wfBlockType         = $20746D66
    m_structWAVEFmt\wfBlockSize         = 16
    
    m_structRIFF\RIFF                   = $46464952
    m_structRIFF\riffBlockSize          = 0
    m_structRIFF\riffBlockType          = $45564157
    
    m_structWAVEHdr\dataBlockType       = $61746164
    m_structWAVEHdr\dataBlockSize       = 0
    
    m_hNewFile = CreateFile(#OUTPUT_FILE, m_outputFilename)
    If m_hNewFile = 0
      MessageRequester("Error", "An error occured while opening " + m_outputFilename, 0)
      FSOUND_Sample_Free(m_sample)          
      ProcedureReturn
    EndIf
    
    ; Write the WAV Header
    WriteData(m_structRIFF,    SizeOf(RIFF_HEADER))
    WriteData(m_structWAVEFmt, SizeOf(WAVE_FORMAT))
    WriteData(m_structWAVEHdr, SizeOf(WAVE_HEADER))

    FSOUND_Sample_Lock(m_sample, 0, lenbytes, @m_ptr1, @m_ptr2, @m_len1, @m_len2)
    ; write the buf
    WriteData(m_ptr1, m_len1)
      
    ; without this, the cpu is used at 100 %
    FSOUND_Sample_Unlock(m_sample, @m_ptr1, @m_ptr2, m_len1, m_len2)
   
    ; Write the End of the WAVE header
    ; We Write sizeof(pcm_data) + sizeof(riff_header) (=44) + the end of the header (2*sizeof(long)) (=8)
    m_structRIFF\riffBlockSize = lenbytes + 44 - 8
    m_structWAVEHdr\dataBlockSize = lenbytes
    
    FileSeek(5)
    WriteData(m_structRIFF\riffBlockSize,    4)        
    FileSeek(41)
    WriteData(m_structWAVEHdr\dataBlockSize, 4)        
    
    ; And close the output file
    CloseFile(#OUTPUT_FILE)        
  
EndProcedure
  
     OpenConsole()
     ConsoleTitle ("PureBasic - FMOD Record Example")
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
 
     ; Select OUTPUT METHOD
    
    PrintN("---------------------------------------------------------")
    PrintN("Output Type");    
    PrintN("---------------------------------------------------------")
    
CompilerSelect #OS
    CompilerCase 1
      PrintN("1 - Direct Sound")
      PrintN("2 - Windows Multimedia Waveout")
      PrintN("3 - NoSound")   
    
    CompilerCase 2    
      PrintN("1 - OSS - Open Sound System)
      PrintN("2 - ESD - Enlightment Sound Daemon)
      PrintN("3 - ALSA 0.9 - Advance Linux Sound Architecture)
    
CompilerEndSelect
    PrintN("---------------------------------------------------------")    ; print driver names 
    PrintN("Press a corresponding number or ESC to quit")        
    
    Repeat
      output.s = Left(Inkey(),1)          
      If Asc(output) = 27
        FSOUND_Close()
        CloseFMOD()
        Delay(1000)
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
     
     
;  Select DRIVER               
    
    ;  The following list are the drivers For the output method selected above.
    
    PrintN("---------------------------------------------------------")    
    
    Select FSOUND_GetOutput()
    
          Case #FSOUND_OUTPUT_NOSOUND
               Print("NoSound ")
          
          Case #FSOUND_OUTPUT_WINMM
               Print("Windows Multimedia Waveout ")
          
          Case #FSOUND_OUTPUT_DSOUND
            Print("Direct Sound ")
            
          Case #FSOUND_OUTPUT_OSS
            Print("Open Sound System")
            
          Case #FSOUND_OUTPUT_ESD
            Print("Enlightment Sound Daemon")
            
          Case #FSOUND_OUTPUT_ALSA
            Print("ALSA")            
    
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
          PrintN(FMOD_ErrorString(FSOUND_GetError()))
          FSOUND_Close()
          CloseFMOD()          
          Delay(1000)
          CloseConsole()
          End
        EndIf
        Delay(1)
      Until ( Val(driver) > 0 ) And ( Val(driver) <= FSOUND_GetNumDrivers() )
          
     FSOUND_SetDriver(Val(driver)-1)                    ; Select sound card (0 = default)

     ; Select MIXER
      FSOUND_SetMixer(#FSOUND_MIXER_QUALITY_AUTODETECT)
      
     ; INITIALIZE          
     If FSOUND_Init(44100, 64, #FSOUND_INIT_ACCURATEVULEVELS) <= 0
        PrintN("FSOUND_Init() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        FSOUND_Close()
        CloseFMOD()
        Delay(2000)
        CloseConsole()
        End
     EndIf        

     ; Select INPUT DRIVER (can be done before Or after init)
     

    PrintN("---------------------------------------------------------")    
    
    Select FSOUND_GetOutput()
    
          Case #FSOUND_OUTPUT_NOSOUND
               Print("NoSound ")
          
          Case #FSOUND_OUTPUT_WINMM
               Print("Windows Multimedia Waveout ")
          
          Case #FSOUND_OUTPUT_DSOUND
            Print("Direct Sound ")
            
          Case #FSOUND_OUTPUT_OSS
            Print("Open Sound System")
            
          Case #FSOUND_OUTPUT_ESD
            Print("Enlightment Sound Daemon")
            
          Case #FSOUND_OUTPUT_ALSA
            Print("ALSA")
            
     EndSelect

     PrintN("Recording device driver list")
     PrintN("---------------------------------------------------------")

     For i = 0 To FSOUND_GetNumDrivers() - 1
          PrintN(Str(i) + " - " + PeekS(FSOUND_GetDriverName(i)) )
     Next i
     PrintN("---------------------------------------------------------")
     PrintN("Press a corresponding number or ESC to quit")        
          
     Repeat     
          input.s = Left(Inkey(), 1)
          If Asc(input) = 27
               FSOUND_Close()
               CloseConsole()
               CloseFMOD()
               Delay(1000)
               End
          EndIf
          Delay(1)  
     Until ( Val(input) > 0 ) And ( Val(input) <= FSOUND_Record_GetNumDrivers() )
          
     If FSOUND_Record_SetDriver(Val(driver)-1) <= 0     ; Select input sound card (0 = default)
        PrintN("FSOUND_Record_SetDriver() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        FSOUND_Close()
        CloseFMOD()        
        Delay(1000)
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
     PrintN("FSOUND Record Driver : " + PeekS(FSOUND_Record_GetDriverName(FSOUND_GetDriver())))

     ; RECORD INTO A STATIC SAMPLE

     ; create a sample To record into     
     
     If (FSOUND_GetOutput() = #FSOUND_OUTPUT_OSS)
      samp1 = FSOUND_Sample_Alloc(#FSOUND_UNMANAGED, #RECORDLEN, #FSOUND_MONO | #FSOUND_8BITS | #FSOUND_UNSIGNED, #RECORDRATE, 255, 128, 255)
     Else
      samp1 = FSOUND_Sample_Alloc(#FSOUND_UNMANAGED, #RECORDLEN, #FSOUND_STEREO | #FSOUND_16BITS , #RECORDRATE, 255, 128, 255)
    EndIf
    
    If samp1 <= 0        ; It will RECORD into This sample For 5 seconds then stop
      PrintN("FSOUND_Sample_Alloc() Error!")
      PrintN(FMOD_ErrorString(FSOUND_GetError()))
      FSOUND_Close()
      CloseFMOD()        
      Delay(1000)        
      CloseConsole()
      End
    EndIf
    
     PrintN("")
     PrintN("=========================================================================")
     PrintN("Press a key to start recording 5 seconds worth of data")
     PrintN("=========================================================================")
      
     Repeat 
          String$ = Inkey()
     Until String$ <> ""      
     
     If FSOUND_Record_StartSample(samp1, #False) <= 0        ; It will RECORD into This sample For 5 seconds then stop
        PrintN("FSOUND_Record_StartSample() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        FSOUND_Close()
        CloseFMOD()        
        Delay(1000)        
        CloseConsole()
        End
     EndIf       

     Repeat 
        String$ = Inkey()
        ConsoleLocate(0,24)
        Print("Recording position = " + Str(FSOUND_Record_GetPosition()))
        Delay(50)
     Until ( FSOUND_Record_GetPosition() >= #RECORDLEN Or String$ <> "")
     FSOUND_Record_Stop()                                   ; it already stopped anyway 
     
     PrintN("")
     PrintN("=========================================================================")
     PrintN("Press a key to play back recorded data")
     PrintN("=========================================================================")
     
     Repeat 
          String$ = Inkey()
     Until String$ <> "" 
     
     channel = FSOUND_PlaySound(#FSOUND_FREE, samp1)
     PrintN("Playing back sound...")

     Repeat
        String$ = Inkey()
        ConsoleLocate(0,24)        
        Print("Playback position = " + Str(FSOUND_GetCurrentPosition(channel)))
        Delay(50)
      Until (  FSOUND_IsPlaying(channel) = #False Or String$ <> "" )
      
      ; SAVED To 
      SaveToWav(samp1)
      PrintN("")
      PrintN("Saved To record.wav!")
      
     ; REALTIME FULL DUPLEX RECORD / PLAYBACK!     
     
     PrintN("")
     PrintN("=========================================================================")
     PrintN("Press a key To do some full duplex realtime recording!")
     PrintN("(with reverb For mmx users)")
     PrintN("=========================================================================")

     Repeat 
          String$ = Inkey()
     Until String$ <> ""   
     
     FSOUND_Sample_SetMode(samp1, #FSOUND_LOOP_NORMAL)    	; make it a looping sample
     
     If FSOUND_Record_StartSample(samp1, #True) <= 0        ; start recording And make it loop also 
        PrintN("FSOUND_Init() Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        FSOUND_Close()
        CloseFMOD()        
        Delay(1000)
        CloseConsole()
        End
     EndIf     


     ; Let the record cursor move forward a little bit first before we try To play it
     ; (the position jumps in blocks, so any non 0 value will mean 1 block has been recorded)
     
     Repeat 
         Delay(1)
     Until FSOUND_Record_GetPosition() > 0
     
     If #ENABLEREVERB = 1 
          SetupReverb()
     EndIf

     channel = FSOUND_PlaySound(#FSOUND_FREE, samp1)         ; play the sound
     originalfreq = FSOUND_GetFrequency(channel)
     
     smoothedvu.f = 0.0
     Repeat
          String$ = Inkey()
          
          playpos = FSOUND_GetCurrentPosition(channel)
          recordpos = FSOUND_Record_GetPosition()
          
          ; NOTE : As the recording And playback frequencies arent guarranteed To be exactly in 
          ; sync, we have To adjust the playback frequency To keep the 2 cursors just enough 
          ; apart not To overlap. (And sound corrupted)
          ; This code tries To keep it inside a reasonable size window just behind the record
          ; cursor. ie [........|play window|<-delay->|<-Record cursor.............]           
          
          
          ;  Dont do this code If either of the cursors just wrapped          
          If (playpos > oldplaypos And recordpos > oldrecordpos) 
               diff = playpos - recordpos
               If (diff > -#RECORD_DELAY_SAMPLES)
                   FSOUND_SetFrequency(channel, originalfreq - 1000)  ; slow it down
               ElseIf (diff < -(#RECORD_DELAY_SAMPLES * 2))
                   FSOUND_SetFrequency(channel, originalfreq + 1000)  ; speed it up
               Else
                   FSOUND_SetFrequency(channel, originalfreq);	
               EndIf
          EndIf
          
          oldplaypos = playpos
          oldrecordpos = recordpos
          
          ; Print some info And a VU meter (vu is smoothed)          
          
          FSOUND_GetCurrentLevels(channel, @leftValue, @rightValue)
          vuval.f = (leftValue+rightValue) * 0.5
          vuval * 18.0
          
          If (vuval > smoothedvu)
              smoothedvu = vuval
          EndIf
          smoothedvu - #VUSPEED
          If (smoothedvu < 0)
              smoothedvu = 0
          EndIf
          vu.s = LSet$("=", Int(smoothedvu), "=")
       
          ConsoleLocate(0, 24)
          Print("Play=" + Str(playpos))
          ConsoleLocate(10, 24)
          Print(" Rec=" + Str(recordpos))
          ConsoleLocate(20, 24)
          Print(" (gap=" + Str(diff))
          ConsoleLocate(30, 24)
          Print(" freqchange=" + Str(FSOUND_GetFrequency(channel) - originalfreq))
          ConsoleLocate(45, 24)
          Print(" hz) VU:" + vu + " ")
          Delay(10)
          
     Until String$ <> ""
     
     FSOUND_StopSound(channel)
     FSOUND_Record_Stop()     

     If #ENABLEREVERB = 1
          CloseReverb()
     EndIf
        
     ; CLEANUP And SHUTDOWN
     FSOUND_Sample_Free(samp1)    
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
EndDataSection      
; jaPBe Version=1.4.4.25
; Build=17
; Manual do Advanced
; FirstLine=683
; CursorPosition=473
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\record\record.exe
; DontSaveDeclare
; EOF