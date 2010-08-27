
ZCodeps.dll: dlldata.obj ZCode_p.obj ZCode_i.obj
	link /dll /out:ZCodeps.dll /def:ZCodeps.def /entry:DllMain dlldata.obj ZCode_p.obj ZCode_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ZCodeps.dll
	@del ZCodeps.lib
	@del ZCodeps.exp
	@del dlldata.obj
	@del ZCode_p.obj
	@del ZCode_i.obj
