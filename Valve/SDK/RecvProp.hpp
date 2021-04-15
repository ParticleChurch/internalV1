#pragma once
struct RecvProp
{
	char* m_pVarName;
	int m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void* m_pExtraData;
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;
	RecvVarProxyFn m_ProxyFn;
	void* m_DataTableProxyFn;
	RecvTable* m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char* m_pParentArrayPropName;
};

/*
#pragma once
class DVariant
{
public:
    union
    {
        float   m_Float;        //0x00
        int    m_Int;          //0x04
        char* m_pString;      //0x08
        void* m_pData;        //0x0C
        float   m_Vector[3];  //0x10
        __int64 m_Int64;        //0x1C
    };//Size=0x24

    int m_Type; //0x24

};//Size=0x28

class CRecvProxyData
{
public:
    const RecvProp* m_pRecvProp; //0x00
    DVariant        m_Value;     //0x04
    int         m_iElement;  //0x2C
    int         m_iObjectId; //0x30
};

class RecvTable
{
public:
    RecvProp* m_pProps;         //0x00
    int   m_nProps;         //0x04
    void* m_pDecoder;       //0x08
    char* m_pNetTableName;  //0x0C
private:
    char      m_pad0x10[0x2]; //0x10
};//Size=0x12

typedef void (*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);

class RecvProp
{
public:
    char* m_pVarName;             //0x00
    int        m_RecvType;             //0x04
    int        m_Flags;                //0x08
    int        m_StringBufferSize;     //0x0C
    bool           m_bIsInsideArray;       //0x10
    const void* m_pExtraData;           //0x14
    RecvProp* m_pArrayProp;           //0x18
    void* m_pArrayLengthProxy;    //0x1C
    RecvVarProxyFn m_ProxyFn;              //0x20
    void* m_pDataTableProxyFn;    //0x24
    RecvTable* m_pDataTable;           //0x28
    int        m_Offset;               //0x2C
    int        m_ElementStride;        //0x30
    int        m_nElements;            //0x34
    const char* m_pParentArrayPropName; //0x38
};//Size=0x3C
*/