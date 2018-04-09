#ifndef _DirIndexing_CPP
#define _DirIndexing_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <Stdio.H>
#include <Stdlib.H>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../Resources/Resource.H"

#include "Entry.H"
#include "DelTree.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DeleteTree(LPOPTIONS lpOpt)
{
	bool bResult = false;

	if((bResult = DeleteTreeEx(lpOpt, lpOpt->sPath)) == true)
	{
		if(lpOpt->bRemoveRoot)
		{
			return(RemoveDirectory(lpOpt->sPath) == TRUE);
		}
		else{
			return true;
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DeleteTreeEx(LPOPTIONS lpOpt, const char *sDir)
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;

	char sPath[2048];

	sprintf_s(sPath, sizeof(sPath), "%s\\*.*", sDir);
	
	if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		if(!lpOpt->bSilent)
		{
			printf("Path not found: [%s]\n", sDir);
		}
		return false;
	}

	do
	{
		if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0)
		{
			sprintf_s(sPath, sizeof(sPath), "%s\\%s", sDir, fdFile.cFileName);

			SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);

			if(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(lpOpt->bTopLevelOnly == false)
				{
					if(!lpOpt->bSilent)
					{
						printf("%s\n", sPath);
					}
					if(!DeleteTreeEx(lpOpt, sPath))
					{
						return false;
					}

					RemoveDirectory(sPath);
				}
			}
			else{
				if(!lpOpt->bSilent)
				{
					printf("\t%s", fdFile.cFileName);
				}
			
				if(!DeleteFile(sPath))
				{
					if(!lpOpt->bSilent)
					{
						printf(" - Failed!");
					}
				}

				if(!lpOpt->bSilent)
				{
					printf("\n");
				}
			}
		}
	}
	while(FindNextFile(hFind, &fdFile));

	FindClose(hFind);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
