#ifndef _ENTRY_CPP
#define _ENTRY_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <Stdio.H>
#include <Stdlib.H>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../Resources/Resource.H"

#include "Entry.H"
#include "DelTree.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HINSTANCE ghAppInstance = NULL;

char gsAppCopyRight[64];
char gsFileVersion[32];
char gsTitleCaption[64];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool AskUser(char *sPrompt)
{
	char sOpt[255];

	printf("%s\n[Y/N]? :", sPrompt);

	if(gets_s(sOpt, sizeof(sOpt)))
	{
		if(_strcmpi(sOpt, "Y") == 0 || _strcmpi(sOpt, "Yes") == 0)
		{
			return true;
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintSyntax(void)
{
	printf("Syntax: \n");
	printf("\t DelTree [Path] [Options]\n\n");
	printf("Options: \n");
	printf("\t /S : Silent. Suppresses deletion status messages.\n");
	printf("\t /T : Delete top level only. Does not delete sub-directories or their files.\n");
	printf("\t /E : Delete all files and sub-directories but leave the root directory in place.\n");
	printf("\t /Y : Suppresses prompting to confirm file deletion.\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParseCmdLineOptions(int iArgs, char *sArgs[], LPOPTIONS lpOpt)
{
	int iArg = 2;

	lpOpt->bRemoveRoot = true;
	lpOpt->bTopLevelOnly = false;
	lpOpt->bNoPrompt = false;
	lpOpt->bSilent = false;

	if(iArgs < 2)
	{
		PrintSyntax();
		return false;
	}

	if(iArgs > 1)
	{
		if(_strcmpi(sArgs[1], "/?") == 0)
		{
			PrintSyntax();
			return false;
		}
	}

	while(iArg < iArgs)
	{
		if(_strcmpi(sArgs[iArg], "/?") == 0)
		{
			PrintSyntax();
			return false;
		}
		else if (_strcmpi(sArgs[iArg], "/S") == 0)
		{
			lpOpt->bSilent = true;
		}
		else if(_strcmpi(sArgs[iArg], "/E") == 0)
		{
			lpOpt->bRemoveRoot = false;
		}
		else if(_strcmpi(sArgs[iArg], "/T") == 0)
		{
			lpOpt->bTopLevelOnly = true;
		}
		else if(_strcmpi(sArgs[iArg], "/Y") == 0)
		{
			lpOpt->bNoPrompt = true;
		}
		else{
			printf("[%s] is not a valid parameter\n", sArgs[iArg]);
			return false;
		}
		iArg++;
	}

	strcpy_s(lpOpt->sPath, sizeof(lpOpt->sPath), sArgs[1]);

	if(strcmp(lpOpt->sPath, ".") == 0 || strcmp(lpOpt->sPath, "..") == 0)
	{
		printf("You cannot specify \".\" or \"..\" as the directory name or path.\n");
		return false;
	}

	return iArgs > 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int iArgs, char *sArgs[])
{
    LoadString(ghAppInstance, IDS_APPCOPYRIGHT, gsAppCopyRight, sizeof(gsAppCopyRight));
    LoadString(ghAppInstance, IDS_FILEVERSION, gsFileVersion, sizeof(gsFileVersion));
    LoadString(ghAppInstance, IDS_TITLECAPTION, gsTitleCaption, sizeof(gsTitleCaption));

	OPTIONS Opt;

	if(!ParseCmdLineOptions(iArgs, sArgs, &Opt))
	{
		return 2;
	}

	if(!Opt.bNoPrompt)
	{
		char sDirPrompt[5120];
		sprintf_s(sDirPrompt, sizeof(sDirPrompt),
			"Are you sure you want to completely remove [%s]", Opt.sPath);

		if(!AskUser(sDirPrompt))
		{
			return 0;
		}
	}

	if(DeleteTree(&Opt))
	{
		return 0;
	}
	else{
		return 1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

