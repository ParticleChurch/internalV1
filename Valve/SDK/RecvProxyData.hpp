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