###############################################################################
#####               This makefile generates ELWALC2.DLL                   #####
###############################################################################
!include $(WGDEV)\src\gcrules.mak

VIRDIR = .\GCVIRDAT

DLLLIB =            \
     $(DLLLIB)      \
     wgserver.lib   \
     galgsbl.lib

VIRFILES =          \
     GCVIRDAT\ELWALC2.VIR  \
     GCVIRDAT\ELWALC2U.VIR 

PARSEOPTS = $(PARSEOPTS) -DIS30

DEFILE = 

OBJFILES =          \
     alc2.obj       \
     alc2alc.obj    \
     alc2cmd.obj    \
     alc2com.obj    \
     alc2obr.obj    \
     alc2pot.obj    \
     alc2rom.obj    \
     alc2rsf.obj    \
     alc2tim.obj    \
     alc2utl.obj    \
     gamutils.obj   

elwalc2.dll:        \
     $(GCBUILD)     \
     $(OBJFILES)    \
     $(DLLLIB)      \
     $(DEFILE)      \
     $(VIRFILES)
     $(LINK) $(LIBCMD) @&&|
$(PRODLL) $(OBJFILES)
$(RUNDIR)\$&
$(RUNDIR)\$&
$(DLLLIB) $(LINKDLL)
$(DEFILE)
|
