$Id: README.txt 22 2008-02-20 06:31:08Z jim.kleckner $
This project uses the free version of Visual Studio 9.

To use the DLL with Excel, you need to register it.

Copy SpreadExcelRTD.dll somewhere on your executable path.

From a command line, run:
  regsvr32.exe SpreadExcelRTD.dll
to register it and:
  regsvr32.exe /U SpreadExcelRTD.dll
to unregister it.

Then open RTDServerSample.xls in Excel and you should see the test topics.

You can use the spreadsheet in ExcelSpreadPub/dlltestspreadpub.xls
to generate events.

