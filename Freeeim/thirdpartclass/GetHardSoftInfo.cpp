
////----------------------------------------------
// File : GetHardSoftInfo.cpp
//
// Content : for displaying the details of hard drives in 
//
// Author : 06/11/2000  Lynn McGuire  written with many contributions from others,
//                            IDE drives only under Windows NT/2K and 9X,
//                            maybe SCSI drives later
//
// Fix Bug : 06/06/2003 NieHuawen(nie173@sohu.com)
//
// Content : Replant to Class
//
//-------------------------------------------------
#include "stdafx.h"
#include "GetHardSoftInfo.h"

CGetMachineInfo::CGetMachineInfo(void)
{
	OSVERSIONINFO version;
	memset (&version, 0, sizeof (version));
	version.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);

	GetVersionEx (&version);
	if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)//nt
	{
     ReadPhysicalDriveInNT();
	}
	if(version.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)//9x
	{
		ReadDrivePortsInWin9X();
	}
}
BOOL CGetMachineInfo::ReturnInfo(int drive, DWORD diskdata [256])
{
	char string1 [1024];
   __int64 sectors = 0;
   __int64 bytes = 0;

      //  copy the hard drive serial number to the buffer
   strcpy (string1, ConvertToString (diskdata, 10, 19));
   if (0 == HardDriveSerialNumber [0] &&
            //  serial number must be alphanumeric
            //  (but there can be leading spaces on IBM drives)
       (isalnum (string1 [0]) || isalnum (string1 [19])))
      strcpy (HardDriveSerialNumber, string1);

//#ifdef PRINTING_TO_CONSOLE_ALLOWED

   switch (drive / 2)
   {
      case 0: str_HardDesk_Form ="Primary Controller";
              break;
      case 1: str_HardDesk_Form ="Secondary Controller";
              break;
      case 2: str_HardDesk_Form ="Tertiary Controller";
              break;
      case 3: str_HardDesk_Form ="Quaternary Controller";
              break;
   }
//MessageBox(NULL,str_HardDesk_Form,NULL,NULL);
   switch (drive % 2)
   {
      case 0: str_Controller ="Master drive";
              break;
      case 1: str_Controller ="Slave drive";
              break;
   }

   str_DN_Modol =CString(ConvertToString (diskdata, 27, 46));
  
   str_DN_Serial =CString(ConvertToString (diskdata, 10, 19));

   str_DN_ControllerRevision =CString(ConvertToString (diskdata, 23, 26));

   str_HardDeskBufferSize.Format("%u", diskdata [21] * 512);

   printf ("Drive Type________________________: ");
   if (diskdata [0] & 0x0080)
      str_HardDeskType ="Removable";
   else if (diskdata [0] & 0x0040)
      str_HardDeskType ="Fixed";
   else str_HardDeskType ="Unknown";
           
		//  calculate size based on 28 bit or 48 bit addressing
		//  48 bit addressing is reflected by bit 10 of word 83
	if (diskdata [83] & 0x400) 
		sectors = diskdata [103] * 65536I64 * 65536I64 * 65536I64 + 
					diskdata [102] * 65536I64 * 65536I64 + 
					diskdata [101] * 65536I64 + 
					diskdata [100];
	else
		sectors = diskdata [61] * 65536 + diskdata [60];
		//  there are 512 bytes in a sector
	bytes = sectors * 512;
	str_HardDeskSize.Format("%I64d",bytes);
return 1;

}
//conversion to char 
char *CGetMachineInfo::ConvertToString(DWORD diskdata [256], int firstIndex, int lastIndex)
{
static char string [1024];
   int index = 0;
   int position = 0;

      //  each integer has two characters stored in it backwards
   for (index = firstIndex; index <= lastIndex; index++)
   {
         //  get high byte for 1st character
      string [position] = (char) (diskdata [index] / 256);
      position++;

         //  get low byte for 2nd character
      string [position] = (char) (diskdata [index] % 256);
      position++;
   }
      //  end the string 
   string [position] = '\0';

      //  cut off the trailing blanks
   for (index = position - 1; index > 0 && ' ' == string [index]; index--)
      string [index] = '\0';
   return string;
}
//
 CGetMachineInfo::DoIDENTIFY(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
                 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
                 PDWORD lpcbBytesReturned)
{
 // Set up data structures for IDENTIFY command.
   pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
   pSCIP -> irDriveRegs.bFeaturesReg = 0;
   pSCIP -> irDriveRegs.bSectorCountReg = 1;
   pSCIP -> irDriveRegs.bSectorNumberReg = 1;
   pSCIP -> irDriveRegs.bCylLowReg = 0;
   pSCIP -> irDriveRegs.bCylHighReg = 0;

      // Compute the drive number.
   pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

      // The command can either be IDE identify or ATAPI identify.
   pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
   pSCIP -> bDriveNumber = bDriveNum;
   pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;

   return( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
               (LPVOID) pSCIP,
               sizeof(SENDCMDINPARAMS) - 1,
               (LPVOID) pSCOP,
               sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
               lpcbBytesReturned, NULL) );
}
//
int CGetMachineInfo::ReadPhysicalDriveInNT(void)
{
	int done = FALSE;
   int drive = 0;

   for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
   {
      HANDLE hPhysicalDriveIOCTL = 0;

         //  Try to get a handle to PhysicalDrive IOCTL, report failure
         //  and exit if can't.
      char driveName [256];

      sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);

         //  Windows NT, Windows 2000, must have admin rights
      hPhysicalDriveIOCTL = CreateFile (driveName,
                               GENERIC_READ | GENERIC_WRITE, 
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, 0, NULL);
      // if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
      //    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
      //            drive, GetLastError ());

      if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
      {
         GETVERSIONOUTPARAMS VersionParams;
         DWORD               cbBytesReturned = 0;

            // Get the version, etc of PhysicalDrive IOCTL
         memset ((void*) &VersionParams, 0, sizeof(VersionParams));

         if ( ! DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
                   NULL, 
                   0,
                   &VersionParams,
                   sizeof(VersionParams),
                   &cbBytesReturned, NULL) )
         {         
            // printf ("DFP_GET_VERSION failed for drive %d\n", i);
            // continue;
         }

            // If there is a IDE device at number "i" issue commands
            // to the device
         if (VersionParams.bIDEDeviceMap > 0)
         {
            BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
            SENDCMDINPARAMS  scip;
            //SENDCMDOUTPARAMS OutCmd;
			BYTE IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
			// Now, get the ID sector for all IDE devices in the system.
               // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
               // otherwise use the IDE_ATA_IDENTIFY command
            bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
                      IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

            memset (&scip, 0, sizeof(scip));
            memset (IdOutCmd, 0, sizeof(IdOutCmd));

            if ( DoIDENTIFY (hPhysicalDriveIOCTL, 
                       &scip, 
                       (PSENDCMDOUTPARAMS)&IdOutCmd, 
                       (BYTE) bIDCmd,
                       (BYTE) drive,
                       &cbBytesReturned))
            {
               DWORD diskdata [256];
               int ijk = 0;
               USHORT *pIdSector = (USHORT *)
                             ((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;

               for (ijk = 0; ijk < 256; ijk++)
                  diskdata [ijk] = pIdSector [ijk];

               ReturnInfo (drive, diskdata);

               done = TRUE;
            }
	    }

         CloseHandle (hPhysicalDriveIOCTL);
      }
   }

   return done;
}
//
int CGetMachineInfo::ReadDrivePortsInWin9X(void)
{
	int done = FALSE;
   HANDLE VxDHandle = 0;
   pt_IdeDInfo pOutBufVxD = 0;
   DWORD lpBytesReturned = 0;

		//  set the thread priority high so that we get exclusive access to the disk
SetPriorityClass (GetCurrentProcess (), REALTIME_PRIORITY_CLASS);
	



      // 1. Make an output buffer for the VxD
   rt_IdeDInfo info;
   pOutBufVxD = &info;

      // *****************
      // KLUDGE WARNING!!!
      // HAVE to zero out the buffer space for the IDE information!
      // If this is NOT done then garbage could be in the memory
      // locations indicating if a disk exists or not.
   ZeroMemory (&info, sizeof(info));

      // 1. Try to load the VxD
       //  must use the short file name path to open a VXD file
   //char StartupDirectory [2048];
   //char shortFileNamePath [2048];
   //char *p = NULL;
   //char vxd [2048];
      //  get the directory that the exe was started from
   //GetModuleFileName (hInst, (LPSTR) StartupDirectory, sizeof (StartupDirectory));
      //  cut the exe name from string
   //p = &(StartupDirectory [strlen (StartupDirectory) - 1]);
   //while (p >= StartupDirectory && *p && '\\' != *p) p--;
   //*p = '\0';   
   //GetShortPathName (StartupDirectory, shortFileNamePath, 2048);
   //sprintf (vxd, "\\\\.\\%s\\IDE21201.VXD", shortFileNamePath);
   //VxDHandle = CreateFile (vxd, 0, 0, 0,
   //               0, FILE_FLAG_DELETE_ON_CLOSE, 0);   
   VxDHandle = CreateFile ("\\\\.\\IDE21201.VXD", 0, 0, 0,
							0, FILE_FLAG_DELETE_ON_CLOSE, 0);

   if (VxDHandle != INVALID_HANDLE_VALUE)
   {
         // 2. Run VxD function
      DeviceIoControl (VxDHandle, m_cVxDFunctionIdesDInfo,
					0, 0, pOutBufVxD, sizeof(pt_IdeDInfo), &lpBytesReturned, 0);

         // 3. Unload VxD
      CloseHandle (VxDHandle);
   }
   else
		MessageBox (NULL, "ERROR: Could not open IDE21201.VXD file", 
					TITLE, MB_ICONSTOP);

      // 4. Translate and store data
   unsigned long int i = 0;
   for (i=0; i<8; i++)
   {
      if((pOutBufVxD->DiskExists[i]) && (pOutBufVxD->IDEExists[i/2]))
      {
			DWORD diskinfo [256];
			for (int j = 0; j < 256; j++) 
				diskinfo [j] = pOutBufVxD -> DisksRawInfo [i * 256 + j];

            // process the information for this buffer
		   ReturnInfo (i, diskinfo);
			done = TRUE;
      }
   }

		//  reset the thread priority back to normal
   // SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_NORMAL);
   SetPriorityClass (GetCurrentProcess (), NORMAL_PRIORITY_CLASS);

   return done;
}
//
int CGetMachineInfo::ReadIdeDriveAsScsiDriveInNT(void)
{
	int done = FALSE;
   int controller = 0;

   for (controller = 0; controller < 2; controller++)
   {
      HANDLE hScsiDriveIOCTL = 0;
      char   driveName [256];

         //  Try to get a handle to PhysicalDrive IOCTL, report failure
         //  and exit if can't.
      sprintf (driveName, "\\\\.\\Scsi%d:", controller);
         //  Windows NT, Windows 2000, any rights should do
      hScsiDriveIOCTL = CreateFile (driveName,
                               GENERIC_READ | GENERIC_WRITE, 
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, 0, NULL);
      // if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
      //    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
      //            controller, GetLastError ());

      if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
      {
         int drive = 0;

         for (drive = 0; drive < 2; drive++)
         {
            char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH];
            SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer;
            SENDCMDINPARAMS *pin =
                   (SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
            DWORD dummy;
   
            memset (buffer, 0, sizeof (buffer));
            p -> HeaderLength = sizeof (SRB_IO_CONTROL);
            p -> Timeout = 10000;
            p -> Length = SENDIDLENGTH;
            p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
            strncpy ((char *) p -> Signature, "SCSIDISK", 8);
  
            pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
            pin -> bDriveNumber = drive;

            if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
                                 buffer,
                                 sizeof (SRB_IO_CONTROL) +
                                         sizeof (SENDCMDINPARAMS) - 1,
                                 buffer,
                                 sizeof (SRB_IO_CONTROL) + SENDIDLENGTH,
                                 &dummy, NULL))
            {
               SENDCMDOUTPARAMS *pOut =
                    (SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
               IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer);
               if (pId -> sModelNumber [0])
               {
                  DWORD diskdata [256];
                  int ijk = 0;
                  USHORT *pIdSector = (USHORT *) pId;
          
                  for (ijk = 0; ijk < 256; ijk++)
                     diskdata [ijk] = pIdSector [ijk];

                  ReturnInfo (controller * 2 + drive, diskdata);

                  done = TRUE;
               }
            }
         }
         CloseHandle (hScsiDriveIOCTL);
      }
   }

   return done;
}
//
