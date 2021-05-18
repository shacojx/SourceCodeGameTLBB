s/\.\.\\\.\.\\\.\.\\OgreMain\\include/$(OGRE_HOME)\\include/i
s/\.\.\\\.\.\\\.\.\\Dependencies\\include\\ode/$(OGRE_HOME)\\include\\ode/i
s/\.\.\\\.\.\\\.\.\\Dependencies\\include//i
s/\.\.\\\.\.\\\.\.\\OgreMain\\lib\\\$(ConfigurationName)/$(OGRE_HOME)\\lib/i
s/\"\.\.\\\.\.\\\.\.\\Dependencies\\lib\\\$(ConfigurationName)\"//i
s/\.\.\\\.\.\\\.\.\\Dependencies\\lib\\Debug/$(OGRE_HOME)\\lib/i
s/\.\.\\\.\.\\\.\.\\Dependencies\\lib\\Release/$(OGRE_HOME)\\lib/i
s/\.\.\\\.\.\\\.\.\\OgreMain\\lib/$(OGRE_HOME)\\bin/i
s/\.\.\\\.\.\\\.\.\\Samples\\Common/$(OGRE_HOME)/i
s/\.\.\\obj/\.\.\\\.\.\\obj/i
s/\.\.\\lib/\.\.\\\.\.\\lib/g
s/\.\.\\\.\.\\\.\.\\OgreMain\\lib\\Debug/$(OGRE_HOME)\\lib/i
s/\.\.\\\.\.\\\.\.\\OgreMain\\lib\\Release/$(OGRE_HOME)\\lib/i
s/CommandLine=\"copy.*\"/CommandLine=\"copy $(OutDir)\\$(TargetFileName) $(OGRE_HOME)\\bin\\$(ConfigurationName)\\"/i
s/$(#STLPORT_DIR)\\stlport/$(OGRE_HOME)\\stlport\\stlport/i
s/$(#STLPORT_DIR)\\lib/$(OGRE_HOME)\\stlport\\lib/i
s/PostBuild_Cmds=copy \.\.\\\.\.\\lib\\Release.*/PostBuild_Cmds=copy \.\.\\lib\\Release\\\*\.dll $(OGRE_HOME)\\bin\\Release/i
s/PostBuild_Cmds=copy \.\.\\\.\.\\lib\\Debug.*/PostBuild_Cmds=copy \.\.\\lib\\Debug\\\*\.dll $(OGRE_HOME)\\bin\\Debug/i



