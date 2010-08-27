
ZTableps.dll: dlldata.obj ZTable_p.obj ZTable_i.obj
	link /dll /out:ZTableps.dll /def:ZTableps.def /entry:DllMain dlldata.obj ZTable_p.obj ZTable_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ZTableps.dll
	@del ZTableps.lib
	@del ZTableps.exp
	@del dlldata.obj
	@del ZTable_p.obj
	@del ZTable_i.obj
