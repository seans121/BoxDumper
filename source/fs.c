#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include "BoxDumper.h"

Result _srvGetServiceHandle(Handle* out, const char* name)
{
  Result rc = 0;

  u32* cmdbuf = getThreadCommandBuffer();
  cmdbuf[0] = 0x50100;
  strcpy((char*) &cmdbuf[1], name);
  cmdbuf[3] = strlen(name);
  cmdbuf[4] = 0x0;

  if((rc = svcSendSyncRequest(*srvGetSessionHandle())))return rc;

  *out = cmdbuf[3];
  return cmdbuf[1];
}

Result loadFile(char* path, void* dst, FS_archive* archive, Handle* fsHandle, u64 maxSize, u32 *bytesRead)
{
    // must malloc first! (and memset, if you'd like)
    if(!path || !dst || !archive)return -1;

    u64 size;
    Result ret;
    Handle fileHandle;

    ret=FSUSER_OpenFile(fsHandle, &fileHandle, *archive, FS_makePath(PATH_CHAR, path), FS_OPEN_READ, FS_ATTRIBUTE_NONE);
    if(ret!=0)return ret;

    ret=FSFILE_GetSize(fileHandle, &size);
    if(ret!=0)goto loadFileExit;
    if(size>maxSize){ret=-2; goto loadFileExit;}

    ret=FSFILE_Read(fileHandle, bytesRead, 0x0, dst, size);
    if(ret!=0)goto loadFileExit;
    if(*bytesRead<size){ret=-3; goto loadFileExit;}

    loadFileExit:
    FSFILE_Close(fileHandle);
    return ret;
}

Result FSUSER_ControlArchive(Handle handle, FS_archive archive)
{
  u32* cmdbuf=getThreadCommandBuffer();

  u32 b1 = 0, b2 = 0;

  cmdbuf[0]=0x080d0144;
  cmdbuf[1]=archive.handleLow;
  cmdbuf[2]=archive.handleHigh;
  cmdbuf[3]=0x0;
  cmdbuf[4]=0x1; //buffer1 size
  cmdbuf[5]=0x1; //buffer1 size
  cmdbuf[6]=0x1a;
  cmdbuf[7]=(u32)&b1;
  cmdbuf[8]=0x1c;
  cmdbuf[9]=(u32)&b2;

  Result ret=0;
  if((ret=svcSendSyncRequest(handle)))return ret;

  return cmdbuf[1];
}


s32	filesysInit(Handle *sd, Handle *save, FS_archive *sdarch, FS_archive *savearch)
{
  Result ret;

  printf("  Getting SD Card handle\n");
  ret = srvGetServiceHandle(sd, "fs:USER");
  if (ret) return ret;

  printf("  Opening SD Card archive\n");
  *sdarch = (FS_archive){0x00000009, (FS_path){PATH_EMPTY, 1, (u8*)""}, 0, 0};
  ret = FSUSER_OpenArchive(sd, sdarch);
  if (ret) return ret;

  printf("  Getting save handle\n");
  ret = _srvGetServiceHandle(save, "fs:USER");
  if (ret) return ret;

  printf("  Initializing save handle\n");
  ret = FSUSER_Initialize(save);
  if (ret) return ret;

  printf("  Opening save archive\n");
  *savearch = (FS_archive){0x4, (FS_path){PATH_EMPTY, 0, NULL}, 0, 0};
  ret = FSUSER_OpenArchive(save, savearch);
  return ret;

}

s32 	filesysExit(Handle *sd, Handle *save, FS_archive *sdarch, FS_archive *savearch)
{
  FSUSER_CloseArchive(save, savearch);
  FSUSER_CloseArchive(sd, sdarch);
  svcCloseHandle(*save);
  svcCloseHandle(*sd);
  return 0;
}

