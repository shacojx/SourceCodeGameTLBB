Here, you can find the PureBasic version of FMOD.
Note that this is the first version and has two known limitations :

- FSOUND_GetVersion() works but with a *tricks* (thanx to FroggerProgger)
- FSOUND_GetCPUUsage() doesn't work properly (due to PureBasic float limitations ?)

All the other functions seems to works well.
Examples and PureBasic port by Saïd Bougribate aka KarLKoX.
For bugs/suggestion reports, mail to KarLKoX@ifrance.com or surf to the fmod's forum.
And now, you can use FroggerProgger's PureBasic library, it fixes the FSOUND_GetVersion() and FSOUND_GetCPUUsage()
limitation. Read the readme text file on how to use it.

Note : if you want to use the library, remove the XInclude at the beginning of each example.