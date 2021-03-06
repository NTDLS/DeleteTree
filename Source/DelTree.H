#ifndef _DirIndexing_H
#define _DirIndexing_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _TAG_OPTIONS{
	bool bRemoveRoot;
	bool bTopLevelOnly;
	bool bNoPrompt;
	bool bSilent;
	char sPath[MAX_PATH * 2];
} OPTIONS, *LPOPTIONS;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DeleteTreeEx(LPOPTIONS lpOpt, const char *sDir);
bool DeleteTree(LPOPTIONS lpOpt);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
