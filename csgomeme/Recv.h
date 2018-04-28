#pragma once
struct RecvProp;
#pragma once
#define FMTFUNCTION( a, b )
int V_snprintf(char *pDest, int destLen, const char *pFormat, ...) FMTFUNCTION(3, 4);

#define Q_snprintf				V_snprintf
typedef __int64					int64;

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY, DPT_String,
	DPT_Array, DPT_DataTable,
#if 0 
	DPT_Quaternion,
#endif
	DPT_Int64,
	DPT_NUMSendPropTypes
} SendPropType;

class DVariant
{
public:
	DVariant() { m_Type = DPT_Float; }
	DVariant(float val) { m_Type = DPT_Float; m_Float = val; }

	const char *ToString()
	{
		static char text[128];

		switch (m_Type)
		{
		case DPT_Int:
			Q_snprintf(text, sizeof(text), "%i", (int)m_Int);
			break;
		case DPT_Float:
			Q_snprintf(text, sizeof(text), "%.3f", m_Float);
			break;
		case DPT_Vector:
			Q_snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f)",
				m_Vector[0], m_Vector[1], m_Vector[2]);
			break;
		case DPT_VectorXY:
			Q_snprintf(text, sizeof(text), "(%.3f,%.3f)",
				m_Vector[0], m_Vector[1]);
			break;
#if 0 
		case DPT_Quaternion:
			Q_snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f %.3f)",
				m_Vector[0], m_Vector[1], m_Vector[2], m_Vector[3]);
			break;
#endif
		case DPT_String:
			if (m_pString)
				return m_pString;
			else
				return "NULL";
			break;
		case DPT_Array:
			Q_snprintf(text, sizeof(text), "Array");
			break;
		case DPT_DataTable:
			Q_snprintf(text, sizeof(text), "DataTable");
			break;
		case DPT_Int64:
			Q_snprintf(text, sizeof(text), "%lld", m_Int64);
			break;
		default:
			Q_snprintf(text, sizeof(text), "DVariant type %i unknown", m_Type);
			break;
		}

		return text;
	}

	union
	{
		float	m_Float;
		long	m_Int;
		char	*m_pString;
		void	*m_pData;
#if 0 
		float	m_Vector[4];
#else
		float	m_Vector[3];
#endif
		int64	m_Int64;
	};
	SendPropType	m_Type;
};

class CRecvProxyData
{
public:
	const RecvProp	*m_pRecvProp;
	DVariant		m_Value;
	int				m_iElement;
	int				m_ObjectID;
};

typedef void(*RecvVarProxyFn)(const CRecvProxyData *pData, void *pStruct, void *pOut);