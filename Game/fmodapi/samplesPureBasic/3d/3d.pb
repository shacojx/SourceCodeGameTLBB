;===============================================================================================
; 3D.EXE
; Copyright (c), Firelight Technologies Pty, Ltd, 1999-2003.
;
; This test shows EAX, DS3D And Software all being used together And the simple commands needed
; To set up some 3D audio.
; This application also displays the use of FSOUND_GetDriverCaps To get information on the 
; 3D capabilities of the selected driver
;===============================================================================================
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions

    
XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

; Windows = 1; Linux = 2
#OS = 1
#INTERFACE_UPDATETIME = 50        ; 50ms update For interface

Global pos.SoundVector, vel.SoundVector, listenerpos.SoundVector, lastpos.SoundVector
t.f = 0.0
props.FSOUND_REVERB_PROPERTIES 


Procedure Close(samp1.l, samp2.l, samp3.l)

  ; you dont need To free samples If you let fsound's sample manager look after samples, as 
  ; it will free them all For you.
  FSOUND_Sample_Free(samp1)
  FSOUND_Sample_Free(samp2)
  FSOUND_Sample_Free(samp3)
  
  FSOUND_Close()
  
EndProcedure


     OpenConsole()
     ClearConsole()
     ConsoleTitle ("PureBasic - FMOD 3D Example")
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
     
     PrintN("---------------------------------------------------------") 
     PrintN("Output Type")
     PrintN("---------------------------------------------------------")
     
CompilerSelect #OS
     CompilerCase 1
      PrintN("1 - Direct Sound ")
      PrintN("2 - Windows Multimedia Waveout")
      PrintN("3 - ASIO")
     
     CompilerCase 2
      PrintN("1 - OSS - Open Sound System")
      PrintN("2 - ESD - Enlightment Sound Daemon")
      PrintN("3 - ALSA 0.9 - Advance Linux Sound Architecture")
      
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
 
    ;Select DRIVER
    
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
          caps = 0
          FSOUND_GetDriverCaps(i, @caps)
          
          PrintN(Str(i + 1) + " - " + PeekS(FSOUND_GetDriverName(i)) )
          If (caps & #FSOUND_CAPS_HARDWARE)
            PrintN("  * Driver supports hardware 3D sound!")
          EndIf
          If (caps & #FSOUND_CAPS_EAX2)
            PrintN("  * Driver supports EAX 2 reverb!")
          EndIf
          If (caps & #FSOUND_CAPS_EAX3)
            PrintN("  * Driver supports EAX 3 reverb!")
          EndIf
     Next i
     PrintN("---------------------------------------------------------")
    
     Repeat     
      driver.s = Left(Inkey(),1)          
      If Asc(driver) = 27
        FSOUND_Close()
        CloseFMOD()        
        Delay(1000)
        CloseConsole()
        End
      EndIf
      Delay(1)
     Until ( Val(driver) > 0 ) And ( Val(driver) <= FSOUND_GetNumDrivers() )
          
     FSOUND_SetDriver(Val(driver) - 1)                    ; Select sound card (0 = default)
      
     FSOUND_GetDriverCaps(FSOUND_GetDriver(), @caps)

     PrintN("---------------------------------------------------------")
     PrintN("Driver capabilities")
     PrintN("---------------------------------------------------------")
     If (caps <= 0)
        PrintN("- This driver will support software mode only.")
        PrintN("It does not properly support 3D sound hardware.")
     EndIf
     If (caps & #FSOUND_CAPS_HARDWARE)
        PrintN("- Driver supports hardware 3D sound!")
     EndIf
     If (caps & #FSOUND_CAPS_EAX2)
        PrintN("- Driver supports EAX 2 reverb!")
     EndIf
     If (caps & #FSOUND_CAPS_EAX3)
        PrintN("- Driver supports EAX 3 reverb!")
        PrintN("---------------------------------------------------------")
     EndIf
     
    ; INITIALIZE      
    
     If FSOUND_Init(44100, 32, 0) <= 0
        PrintN("FSOUND_Init() Error!")
        PrintN(Str(FMOD_ErrorString(FSOUND_GetError())))
        FSOUND_Close()
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
     
     ; 3D MONO
     
      soundsDir.s = Mid(currentDir$, 1, position) + "media\drumloop.wav"
      samp1 = FSOUND_Sample_Load(#FSOUND_FREE, @soundsDir, #FSOUND_HW3D, 0, 0)
      If samp1 <= 0
        PrintN("FSOUND_Sample_Load() samp1 Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        Close(samp1, samp2, samp3)
        CloseFMOD()
        Delay(1000)
        CloseConsole()
        End
      EndIf   
      
     ; increasing mindistnace makes it louder in 3d space
     FSOUND_Sample_SetMinMaxDistance(samp1, 4.0, 10000.0)
     FSOUND_Sample_SetMode(samp1, #FSOUND_LOOP_NORMAL)
     
     ; 3D MONO
     soundsDir = Mid(currentDir$, 1, position) + "media\jaguar.wav"     
     samp2 = FSOUND_Sample_Load(#FSOUND_UNMANAGED, @soundsDir, #FSOUND_HW3D, 0, 0)
     If samp2 <= 0
        PrintN("FSOUND_Sample_Load() samp2 Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        Close(samp1, samp2, samp3)
        CloseFMOD()        
        Delay(2000)
        CloseConsole()
        End
     EndIf   
     
     ; increasing mindistnace makes it louder in 3d space
     FSOUND_Sample_SetMinMaxDistance(samp2, 4.0, 10000.0)
     FSOUND_Sample_SetMode(samp2, #FSOUND_LOOP_NORMAL)
     
     ; 2D STEREO
     soundsDir = Mid(currentDir$, 1, position) + "media\chimes.wav"     
     samp3 = FSOUND_Sample_Load(#FSOUND_UNMANAGED, @soundsDir, #FSOUND_HW2D, 0, 0)
     If samp3 <= 0
        PrintN("FSOUND_Sample_Load() samp3 Error!")
        PrintN(FMOD_ErrorString(FSOUND_GetError()))
        Close(samp1, samp2, samp3)
        CloseFMOD()        
        Delay(2000)
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
     
     Print("FSOUND Driver        : ")
     PrintN(PeekS(FSOUND_GetDriverName(FSOUND_GetDriver()))) 
     FSOUND_GetNumHWChannels(@num2d, @num3d.l, @total)
     PrintN("Hardware 2D channels : " + Str(PeekL(@num2d)) )    
     PrintN("Hardware 3D channels : " + Str(PeekL(@num3d)) )
     
     PrintN("=========================================================================")
     PrintN("Press 1        Pause/Unpause 16bit 3D sound at any time")
     PrintN("      2        Pause/Unpause 8bit 3D sound at any time")
     PrintN("      3        Play 16bit STEREO 2D sound at any time")
     PrintN("      4        Change to EAX Reverb mode CONCERTHALL (DirectSound/SBLive only)")
     PrintN("      5        Change to EAX Reverb mode SEWERPIPE (DirectSound/SBLive only)")
     PrintN("      6        Change to EAX Reverb mode PSYCHOTIC (DirectSound/SBLive only)")
     PrintN("      <        Move listener left (in still mode)")
     PrintN("      >        Move listener right (in still mode)")
     PrintN("      SPACE Stop/Start listener automatic movement")
     PrintN("      ESC   Quit")
     PrintN("=========================================================================")

     ; PLAY 2 LOOPING SOUNDS

     pos\x = -10.0 : pos\y = -0.0 : pos\z = 0.0
     vel\x = 0     : vel\y = 0    : vel\z = 0
     
     channel1 = FSOUND_PlaySoundEx(#FSOUND_FREE, samp1, #Null, #True)
     FSOUND_3D_SetAttributes(channel1, @pos, @vel)
     FSOUND_SetPaused(channel1, #False)
     
     pos\x = 15.0  : pos\y = -0.0 : pos\z = -0.0
     vel\x = 0     : vel\y = 0    : vel\z = 0     

     channel2 = FSOUND_PlaySoundEx(#FSOUND_FREE, samp2, #Null, #True)
     FSOUND_3D_SetAttributes(channel2, @pos, @vel)
     FSOUND_SetVolume(channel2, 128)
     FSOUND_SetPaused(channel2, #False)
               
     ; MAIN LOOP
     listenerpos\x  = 0    : listenerpos\y = 0     : listenerpos\z = 0     
     lastpos\x      = 0    : lastpos\y     = 0     : lastpos\z   = 0     
     listenerflag   = 1
     
     Repeat
        keys.s = Left(Inkey(),1)
       
        If keys = "1"
            FSOUND_SetPaused(channel1, FSOUND_GetPaused(channel1) ! 1)         ; inverse bit logic
        
        ElseIf keys = "2"
            FSOUND_SetPaused(channel2, FSOUND_GetPaused(channel2) ! 1)         ; inverse bit logic
             
        ElseIf keys = "3"
            FSOUND_PlaySoundEx(#FSOUND_FREE, samp3, #Null, #False)
     
        ElseIf keys = "4"
            SetReverbEnvironnement(#FSOUND_PRESET_CONCERTHALL, @props)
            FSOUND_Reverb_SetProperties(@props)
        
        ElseIf keys = "5"
            SetReverbEnvironnement(#FSOUND_PRESET_SEWERPIPE, @props)
            FSOUND_Reverb_SetProperties(@props)
        
        ElseIf keys = "6"
            SetReverbEnvironnement(#FSOUND_PRESET_PSYCHOTIC, @props)
            FSOUND_Reverb_SetProperties(@props)
        
        ; = ' ' 
        ElseIf keys = " "
            listenerflag ! 1
        EndIf

        If (listnenerflag = 0)
          ; = '<'
          If keys = "<"
               listenerpos\x = listenerpos\x - 1.0
               If listenerpos\x < -35
                    listenerpos\x = -35
               EndIf
          ElseIf keys = ">"
               listenerpos\x = listenerpos\x + 1.0
               If listenerpos\x > 30
                    listenerpos\x = 30
               EndIf
          EndIf
        EndIf

        If listenerflag 
          listenerpos\x = Sin(t*0.05) * 33.0
        EndIf

        ; ********* NOTE ******* Read Next COMMENT!!!!!
        ; vel = how far we moved last FRAME (m/f), then time compensate it To SECONDS (m/s).
        vel\x = (listenerpos\x-lastpos\x) * (1000 / #INTERFACE_UPDATETIME)
        vel\y = (listenerpos\y-lastpos\y) * (1000 / #INTERFACE_UPDATETIME)
        vel\z = (listenerpos\z-lastpos\z) * (1000 / #INTERFACE_UPDATETIME)

        ; // store pos For Next time
        lastpos\x = listenerpos\x
        lastpos\y = listenerpos\y
        lastpos\z = listenerpos\z

        ret = FSOUND_3D_Listener_SetAttributes(@listenerpos, @vel, 0, 0, 1.0, 0, 1.0, 0)

        t + (30 * (1.0 / #INTERFACE_UPDATETIME))  ; t is just a time value .. it increments in 30m/s steps in this example
        ConsoleLocate(0,45)
        Print("|.......................<1>......................<2>....................|")
        ConsoleLocate(listenerpos\x+35,45)
        Print("L")

        FSOUND_Update()
        Delay(#INTERFACE_UPDATETIME-1)                ; -1 for print etc

     Until Asc(keys) = 27

     PrintN("")
     
     ; CLEANUP And SHUTDOWN
     Close(samp1, samp2, samp3)
     CloseFMOD()
     CloseConsole()
     End 
; jaPBe Version=1.4.4.25
; Build=46
; Language=0x0000 Language Neutral
; FirstLine=242
; CursorPosition=177
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\3d\3d.exe
; DontSaveDeclare
; EOF