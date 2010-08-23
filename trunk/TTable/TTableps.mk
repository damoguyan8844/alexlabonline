
TTableps.dll: dlldata.obj TTable_p.obj TTable_i.obj
	link /dll /out:TTableps.dll /def:TTableps.def /entry:DllMain dlldata.obj TTable_p.obj TTable_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del TTableps.dll
	@del TTableps.lib
	@del TTableps.exp
	@del dlldata.obj
	@del TTable_p.obj
	@del TTable_i.obj
