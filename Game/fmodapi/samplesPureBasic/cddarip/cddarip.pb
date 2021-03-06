;===============================================================================================
; CDDA.EXE
; Copyright (c), Firelight Technologies Pty, Ltd, 1999-2004.
;
; Use CDDA streaming To rip a CD track To a wav file
;=============================================================================================== 
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions

XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

Structure RIFF_HEADER
  RIFF.l
  riffBlockSize.l
  riffBlockType.l
EndStructure
m_structRIFF.RIFF_HEADER

Structure WAVE_HEADER
  dataBlockType.l
  dataBlockSize.l
EndStructure
m_structWAVEHdr.WAVE_HEADER

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
m_structWAVEFmt.WAVE_FORMAT

#FILE_HANDLE    = 1

Global last_openstate.l, this_openstate.l
Global firsttime.b
Global stream_ended.b
Global byteswritten.l

Procedure.l endcallback(*stream.l, *buff.l, len.l, param.l)
  stream_ended = #True
  ProcedureReturn #True
EndProcedure

Procedure DSP_RawWriteCallback(originalbuffer.l, newbuffer.l, length.l, param.l)
  If (stream_ended = #False)
    WriteData(newbuffer, length << 2)
    byteswritten + (length << 2)
  EndIf

  ProcedureReturn newbuffer
EndProcedure

  OpenConsole()
  ClearConsole()
  ConsoleTitle("PureBasic - CDDA Rip Example")
   
  ; INSTANCIATE FMOD DLL
  LoadFMOD()
  
  stream_ended.b = #False
  byteswritten.l = 0

  m_structWAVEFmt\wFormatTag          = 1  
  m_structWAVEFmt\wBitsPerSample      = 16
  m_structWAVEFmt\nChannels           = 2
  m_structWAVEFmt\nSamplesPerSec      = 44100
  m_structWAVEFmt\nBlockAlign         = m_structWAVEFmt\nChannels * m_structWAVEFmt\wBitsPerSample / 8
  m_structWAVEFmt\nAvgBytesPerSec     = m_structWAVEFmt\nSamplesPerSec * m_structWAVEFmt\nBlockAlign
  m_structWAVEFmt\wfBlockType         = $20746D66
  m_structWAVEFmt\wfBlockSize         = 16
  
  m_structRIFF\RIFF                   = $46464952
  m_structRIFF\riffBlockSize          = 0
  m_structRIFF\riffBlockType          = $45564157
  
  m_structWAVEHdr\dataBlockType       = $61746164
  m_structWAVEHdr\dataBlockSize       = 0
  
  
  track_num.l    = - 1
  start_time     = timeGetTime_()
  drive_letter.s = ProgramParameter()
  track_num      = Val(ProgramParameter())
  If (drive_letter = "" Or track_num = 0)
    PrintN("Usage:   cddarip <driveletter> <tracknumber starting from 1>")
    PrintN("Example: cddarip d 2")
    Delay(2000)
    CloseConsole()
    End
  EndIf
  
  If ( (Asc(drive_letter) < 65 And Asc(drive_letter) > 95) Or (Asc(drive_letter) < 97 And Asc(drive_letter) > 122) )
    PrintN("ERROR: Invalid drive letter")
    CloseConsole()
    Delay(2000)
    End
  EndIf
  drive_letter + ":"
  
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
  
  FSOUND_SetOutput(#FSOUND_OUTPUT_NOSOUND_NONREALTIME)
  
  ; INITIALIZE    
  If (FSOUND_Init(44100, 32, 0) <= 0)
    PrintN("FSOUND_Init() Error!")
    PrintN(FMOD_ErrorString(FSOUND_GetError()))
    FSOUND_Close()
    Delay(2000)    
    CloseConsole()
    End
  EndIf  
    
  retVal = CreateFile(#FILE_HANDLE, "dump.wav") 
  If (retVal = 0)
    PrintN("ERROR: Couldn't open dump.wav for writing")
    FSOUND_Close()
    Delay(2000)    
    CloseConsole()
    End
  EndIf
  
  ; Before we've even written the headers for the wav out, seek to the offset the raw data will start from.
  FileSeek(SizeOf(RIFF_HEADER) + SizeOf(WAVE_HEADER) + SizeOf(WAVE_FORMAT))

  ; Create a DSP callback which will capture the mixed Data And write it To disk
  rawwrite_dsp = FSOUND_DSP_Create(@DSP_RawWriteCallback(), #FSOUND_DSP_DEFAULTPRIORITY_USER, 0)
  FSOUND_DSP_SetActive(rawwrite_dsp, #True)

  ; LOAD SONG    
  stream = FSOUND_Stream_Open(@drive_letter, #FSOUND_NORMAL, 0, 0)
  If (stream <= 0)
    PrintN("ERROR: Couldn't create CDDA stream.")
    FSOUND_DSP_SetActive(rawwrite_dsp, #False)
    FSOUND_DSP_Free(rawwrite_dsp)
    FSOUND_Close()
    Delay(2000)
    CloseConsole()
    End
  EndIf
  
  If ((track_num < 1) Or ((track_num - 1) >= FSOUND_Stream_GetNumSubStreams(stream)))
    PrintN("ERROR: Invalid track number.")
    FSOUND_Stream_Close(stream)
    FSOUND_DSP_SetActive(rawwrite_dsp, #False)
    FSOUND_DSP_Free(rawwrite_dsp)
    FSOUND_Close()
    Delay(2000)
    CloseConsole()
    End
  EndIf  
  
  FSOUND_Stream_SetEndCallback(stream, @endcallback(), 0)
  FSOUND_Stream_SetSubStream(stream, track_num - 1)
  FSOUND_Stream_Play(#FSOUND_FREE, stream)
  
  PrintN("Ripping " + drive_letter + " track " + Str(track_num) + " (" + RSet$(Str(FSOUND_Stream_GetLengthMs(stream) / 1000 / 60), 2, "0") + ":" + RSet$(Str((FSOUND_Stream_GetLengthMs(stream) / 1000)%60), 2, "0") + ")" )

  s.s      = Space$(56)
Repeat
  keys.s = Left(Inkey(),1) 
  
  curtime.f = FSOUND_Stream_GetTime(stream)
  curlength.f = FSOUND_Stream_GetLengthMs(stream)
  read_percent.l = (curtime / curlength) * 100.0
  s = LSet$("=",(read_percent >> 1) + (read_percent & 1), "=")
  s + LSet(" ", (100 - read_percent) >> 1)
  ConsoleLocate(0, 2)
  Print("|" + s + "| " + Str(read_percent) + "% ")  
  
  FSOUND_Update()
Until (Asc(keys) = 27 Or (stream_ended = #True))

  FSOUND_Stream_Close(stream)
  FSOUND_DSP_SetActive(rawwrite_dsp, #False)
  FSOUND_DSP_Free(rawwrite_dsp)

  ; Now finalize the wav file by seeking To the start And putting in the headers.
  m_structRIFF\riffBlockSize = byteswritten + 44 - 8
  m_structWAVEHdr\dataBlockSize = byteswritten
  
  FileSeek(0)
  ; Write the WAV Header
  WriteData(m_structRIFF,    SizeOf(RIFF_HEADER))
  WriteData(m_structWAVEFmt, SizeOf(WAVE_FORMAT))
  WriteData(m_structWAVEHdr, SizeOf(WAVE_HEADER))
  
  WriteData(m_structRIFF\riffBlockSize,    4)        
  FileSeek(41)
  WriteData(m_structWAVEHdr\dataBlockSize, 4)        
  
  ; And close the output file
  CloseFile(#FILE_HANDLE)
  
  FSOUND_Close()
  CloseFMOD()
  
  elapsed_time = timeGetTime_() - start_time
  ConsoleLocate(0, 3)
  PrintN("")
  PrintN("Elapsed time: " + RSet$(Str(elapsed_time / 1000 / 60), 2, "0") + ":" + RSet$(Str((elapsed_time / 1000)%60), 2, "0"))
  Delay(2000)

  CloseConsole()
  End

 
; jaPBe Version=1.4.4.25
; Build=77
; Language=0x0000 Language Neutral
; FirstLine=177
; CursorPosition=213
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\cddarip\cddarip.exe
; DontSaveDeclare
; EOF