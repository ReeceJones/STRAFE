#pragma once
#include <vector>
#include <iostream>
#include "Structs.h"
#include "Recv.h"
#include "IBaseClientDll.h"
#include "Interfaces.h"

class CNetvarManager
{
public:
	inline void Init() //stores all of the tables in client class, might need to make recursive
	{
		tables.clear();
		ClientClass* clientClass = Interfaces::Client->GetAllClasses();
		if (clientClass)
		{
			while (clientClass)
			{
				tables.push_back(clientClass->m_pRecvTable);
				clientClass = clientClass->m_pNext;
			}
		}
	}

	inline int GetOffset(const char* tableName, const char* propName)
	{
		for (int x = 0; x < tables.size(); x++)
		{
			if (!tables.empty())
			{
				RecvTable *table = tables[x];
				if (table)
				{ //b_inmainlist?
					if (!strcmp(table->m_pNetTableName, tableName))
					{
						/**/ // works fine
						RecvProp** prop = 0;
						return GetPropB(table, propName, prop); //not passing correct table through function?
																/**/
					}
				}
			}
		}
		return 0;
	}

	inline bool HookProp(const char *tableName, const char *propName, RecvVarProxyFn function)
	{
		RecvProp *recvProp = 0;
		RecvTable* r = GetTable(tableName);
		if (!r || r == nullptr)
			return false;

		int p = GetPropB(r, propName, &recvProp);
		if (!p)
			return false;
		if (!recvProp)
			return false;

		recvProp->m_ProxyFn = function;

		return true;
	}
private:
	std::vector<RecvTable*> tables;
	RecvTable* GetTable(const char* tableName)
	{
		for (int z = 0; z < tables.size(); z++)
		{
			RecvTable *table = tables[z];
			if (table)
			{ //b_inmainlist?
				if (!strcmp(table->m_pNetTableName, tableName))
				{
					return table;
				}
				for (int x = 0; x < table->m_nProps; ++x)
				{
					RecvProp* childProp = &table->m_pProps[x];
					GetTable(childProp->m_pDataTable->m_pNetTableName);
				}
			}
		}

	}

	int GetPropB(RecvTable *table, const char *propName, RecvProp **prop = 0)
	{
		int extraOffset = 0;
		for (int i = 0; i < table->m_nProps; ++i)
		{
			//printf("%i\n", i);
			RecvProp *recvProp = &table->m_pProps[i];
			RecvTable *child = recvProp->m_pDataTable; //something with child is returning null //cannot get datatable?
			RecvProp **prop = 0;

			if (child && (child->m_nProps > 0))
			{
				int tempOffset = GetPropB(child, propName);
				if (tempOffset)
				{
					extraOffset += (recvProp->m_Offset + tempOffset);
				}
			}

			if (_stricmp(recvProp->m_pVarName, propName))
				continue;

			if (prop)
				*prop = recvProp;

			return (recvProp->m_Offset + extraOffset);
		}

		return extraOffset;
	}
}; 
extern CNetvarManager* NetvarManager;