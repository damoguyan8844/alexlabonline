/******************************************************************************
* $Id: SpreadExcelRTDImpl.cpp 24 2008-02-20 17:21:19Z jim.kleckner $
*
*   File:   SpreadExcelRTDImpl.cpp
*
*   Date:   February 5, 2001
*
*   Description:   This file contains the definition of a simple real-time-data 
*                  server for Excel.
*
*   Modifications:
*
*     Morphed into a spread toolkit data provider.
*
 * Copyright 2008, James E. Kleckner
 *  Based on example published by Microsoft without any copyright notices.
 See: http://support.microsoft.com/kb/327215

  This file is part of spread-excel.

  spread-excel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  spread-excel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with spread-excel.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/
#include "SpreadExcelRTDImpl.h"
#include <stdio.h>
#define RTDSERVERTRACE(x) OutputDebugString(x)
#ifdef   NDEBUG
#define RTDSERVERTRACEDEBUG(x)
#else  //NDEBUG
#define RTDSERVERTRACEDEBUG(x) RTDSERVERTRACE(x)
#endif //NDEBUG
LONG g_cOb = 0;	//global count of the number of objects created.

//Constructor
SpreadExcelRTD::SpreadExcelRTD(IUnknown* pUnkOuter)
{
    RTDSERVERTRACE("SpreadExcelRTD\n");
    m_refCount = 0;
    m_pTypeInfoInterface = NULL;
    m_dwDataThread = -1;
    RTDSERVERTRACE("SpreadExcelRTD: InitializeCriticalSection.\n");
    InitializeCriticalSection( & m_criticalSection );
    m_pTopicMap   = new SpreadExcelRTD_TopicMap();
    m_pServerMap  = new SpreadExcelRTD_ServerMap();
    m_pChangedSet = new SpreadExcelRTD_ChangedSet();
   
    //Get the TypeInfo for this object
    LoadTypeInfo(&m_pTypeInfoInterface, IID_IRtdServer, 0x0);

    //Set up the aggregation
    if (pUnkOuter != NULL)
    {
        m_pOuterUnknown = pUnkOuter;
    }
    else
    {
        m_pOuterUnknown = reinterpret_cast<IUnknown*>
        (static_cast<INonDelegatingUnknown*>(this));   
    }

    //Increment the object count so the server knows not to unload
    InterlockedIncrement( &g_cOb );
}

//Destructor
SpreadExcelRTD::~SpreadExcelRTD()
{
    RTDSERVERTRACE("~SpreadExcelRTD\n");

    //Make sure we kill the data thread
    if (m_dwDataThread != -1)
    {
        RTDSERVERTRACE("~SpreadExcelRTD: Shutdown data thread.\n");
        PostThreadMessage( m_dwDataThread, WM_COMMAND, WM_SILENTTERMINATE, 0 );
    }
    g_spreadExcelRTD_instance = NULL;

    // Use critical section one last time to make sure that the timers are shut down
    //  before we destroy our maps.
    RTDSERVERTRACE("~SpreadExcelRTD: EnterCriticalSection.\n");
    EnterCriticalSection( & m_criticalSection );

    //Clean up the type information
    if (m_pTypeInfoInterface != NULL)
    {
        m_pTypeInfoInterface->Release();
        m_pTypeInfoInterface = NULL;
    }

    if (m_pChangedSet != NULL)
    {
        delete m_pChangedSet;
        m_pChangedSet = NULL;
    }

    if (m_pServerMap != NULL)
    {
        delete m_pServerMap;
        m_pServerMap = NULL;
    }

    //Clean up the topic list last, since it has important string keys.
    if (m_pTopicMap != NULL)
    {
        delete m_pTopicMap;
        m_pTopicMap = NULL;
    }

    LeaveCriticalSection( & m_criticalSection );
    RTDSERVERTRACE("~SpreadExcelRTD: LeaveCriticalSection.\n");
    RTDSERVERTRACE("~SpreadExcelRTD: DeleteCriticalSection.\n");
    DeleteCriticalSection( & m_criticalSection );

    //Decrement the object count
    InterlockedDecrement( &g_cOb );
}

/******************************************************************************
*   LoadTypeInfo -- Gets the type information of an object's interface from the 
*   type library.  Returns S_OK if successful.
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::LoadTypeInfo(ITypeInfo** pptinfo, REFCLSID clsid,
                                        LCID lcid)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::LoadTypeInfo\n");
   
    HRESULT hr;
    LPTYPELIB ptlib = NULL;
    LPTYPEINFO ptinfo = NULL;
    *pptinfo = NULL;
   
    // First try to load the type info from a registered type library
    hr = LoadRegTypeLib(LIBID_RTDServerLib, 1, 0, lcid, &ptlib);
    if (FAILED(hr))
    {
        RTDSERVERTRACE("Warning: TypeLib not registered.\n");

        //if the libary is not registered, try loading from a file
        hr = LoadTypeLib(L"SpreadExcelRTD.dll", &ptlib);
        if (FAILED(hr))
        {

            //can't get the type information
            RTDSERVERTRACE("Warning: TypeLib couldn't be loaded.\n");
            return hr;
        }
    }
   
    // Get type information for interface of the object.
    hr = ptlib->GetTypeInfoOfGuid(clsid, &ptinfo);
    if (FAILED(hr))
    {
        ptlib->Release();
        return hr;
    }
    ptlib->Release();
    *pptinfo = ptinfo;
    return S_OK;
}

/******************************************************************************
*   IUnknown Interfaces -- All COM objects must implement, either 
*  directly or indirectly, the IUnknown interface.
******************************************************************************/

/******************************************************************************
*  QueryInterface -- Determines if this component supports the 
*  requested interface, places a pointer to that interface in ppvObj if it's 
*  available, and returns S_OK.  If not, sets ppvObj to NULL and returns 
*  E_NOINTERFACE.
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::QueryInterface(REFIID riid, void ** ppvObj)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::QueryInterface\n");

    //defer to the outer unknown
    return m_pOuterUnknown->QueryInterface( riid, ppvObj );
}

/******************************************************************************
*  AddRef() -- In order to allow an object to delete itself when 
*  it is no longer needed, it is necessary to maintain a count of all 
*  references to this object.  When a new reference is created, this function 
*  increments the count.
******************************************************************************/
STDMETHODIMP_(ULONG) SpreadExcelRTD::AddRef()
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::AddRef\n");
   
    //defer to the outer unknown
    return m_pOuterUnknown->AddRef();
}

/******************************************************************************
*  Release() -- When a reference to this object is removed, this 
*  function decrements the reference count.  If the reference count is 0, then 
*  this function deletes this object and returns 0.
******************************************************************************/
STDMETHODIMP_(ULONG) SpreadExcelRTD::Release()
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::Release\n");

    //defer to the outer unknown
    return m_pOuterUnknown->Release();
}

/******************************************************************************
*   INonDelegatingUnknown Interfaces -- All COM objects must implement, either 
*  directly or indirectly, the IUnknown interface.
******************************************************************************/

/******************************************************************************
*  NonDelegatingQueryInterface -- Determines if this component supports the 
*  requested interface, places a pointer to that interface in ppvObj if it's 
*  available, and returns S_OK.  If not, sets ppvObj to NULL and returns 
*  E_NOINTERFACE.
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::NonDelegatingQueryInterface(REFIID riid, 
                                                     void ** ppvObj)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::NonDelegatingQueryInterface->");
   
    if (riid == IID_IUnknown)
    {
        RTDSERVERTRACE("IUnknown\n");
        *ppvObj = static_cast<INonDelegatingUnknown*>(this);
    }

    else if (riid == IID_IDispatch)
    {
        RTDSERVERTRACE("IDispatch\n");
        *ppvObj = static_cast<IDispatch*>(this);
    }
   
    else if (riid == IID_IRtdServer)
    {
        RTDSERVERTRACE("IRtdServer\n");
        *ppvObj = static_cast<IRtdServer*>(this);
    }
   
    else
    {
        static char buffer[80];
        LPOLESTR clsidString = NULL;
        StringFromCLSID( riid, &clsidString );
        sprintf_s( buffer, sizeof(buffer), "Unsupported Interface -- %S\n", clsidString );
        RTDSERVERTRACE( buffer );
        *ppvObj = NULL;
        return E_NOINTERFACE;
    }
   
    static_cast<IUnknown*>(*ppvObj)->AddRef();
    return S_OK;
}

/******************************************************************************
*  NonDelegatingAddRef() -- In order to allow an object to delete itself when 
*  it is no longer needed, it is necessary to maintain a count of all 
*  references to this object.  When a new reference is created, this function 
*  increments the count.
******************************************************************************/
STDMETHODIMP_(ULONG) SpreadExcelRTD::NonDelegatingAddRef()
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::NonDelegatingAddRef\n");
   
    return ++m_refCount;
}

/******************************************************************************
*  NonDelegatingRelease() -- When a reference to this object is removed, this 
*  function decrements the reference count.  If the reference count is 0, then 
*  this function deletes this object and returns 0.
******************************************************************************/
STDMETHODIMP_(ULONG) SpreadExcelRTD::NonDelegatingRelease()
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::NonDelegatingRelease: ");
    char txt[16];
    sprintf_s(txt, sizeof(txt), "%d\n", --m_refCount);
   
    if (m_refCount == 0)
    {
        delete this;
        return 0;
    }
    return m_refCount;
}

/******************************************************************************
*   IDispatch Interface -- This interface allows this class to be used as an
*   automation server, allowing its functions to be called by other COM
*   objects
******************************************************************************/

/******************************************************************************
*   GetTypeInfoCount -- This function determines if the class supports type 
*   information interfaces or not.  It places 1 in iTInfo if the class supports
*   type information and 0 if it doesn't.
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::GetTypeInfoCount(UINT *iTInfo)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::GetTypeInfoCount\n");
   
    *iTInfo = 0;
    return S_OK;
}

/******************************************************************************
*   GetTypeInfo -- Returns the type information for the class.  For classes 
*   that don't support type information, this function returns E_NOTIMPL;
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::GetTypeInfo(UINT iTInfo, LCID lcid, 
                                       ITypeInfo **ppTInfo)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::GetTypeInfo\n");

    return E_NOTIMPL;
}

/******************************************************************************
*   GetIDsOfNames -- Takes an array of strings and returns an array of DISPID's
*   which corespond to the methods or properties indicated.  If the name is not 
*   recognized, returns DISP_E_UNKNOWNNAME.
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::GetIDsOfNames(REFIID riid,  
                                         OLECHAR **rgszNames, 
                                         UINT cNames,  LCID lcid,
                                         DISPID *rgDispId)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::GetIDsOfNames -- ");
   
    HRESULT hr = E_FAIL;
   
    //Validate arguments
    if (riid != IID_NULL)
    {
        return E_INVALIDARG;
    }
   
    //this API call gets the DISPID's from the type information
    if (m_pTypeInfoInterface != NULL)
    {
        hr = m_pTypeInfoInterface->GetIDsOfNames(rgszNames, cNames, rgDispId);
    }
   
    //DispGetIDsOfNames may have failed, so pass back its return value.
    return hr;
}

/******************************************************************************
*   Invoke -- Takes a dispid and uses it to call another of this class's 
*   methods.  Returns S_OK if the call was successful.
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
                                  WORD wFlags, DISPPARAMS* pDispParams,
                                  VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
                                  UINT* puArgErr)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::Invoke\n");

    HRESULT hr = DISP_E_PARAMNOTFOUND;
   
    //Validate arguments
    if ((riid != IID_NULL))
    {
        return E_INVALIDARG;
    }

    hr = m_pTypeInfoInterface->Invoke((IRtdServer*)this, dispIdMember, wFlags, 
        pDispParams, pVarResult, pExcepInfo, puArgErr);     

    return S_OK;
}

////////////////////// Begin JEK modifiable code
// This hack variable is a cheap way to let the other thread get the server instance.
//  It really should be marshalled via CoMarshalInterThreadInterfaceInStream()
SpreadExcelRTD* g_spreadExcelRTD_instance = NULL;

/******************************************************************************
*  ServerStart -- The ServerStart method is called immediately after a 
*  real-time data server is instantiated.
*  Parameters: CallbackObject -- interface pointer the SpreadExcelRTD uses to 
*                                indicate new data is available.
*              pfRes -- set to positive value to indicate success.  0 or 
*                       negative value indicates failure.
*  Returns: S_OK
*           E_POINTER
*           E_FAIL
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::ServerStart(IRTDUpdateEvent *CallbackObject,
                                    long *pfRes)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::ServerStart\n");
    HRESULT hr = S_OK;

    //Check the arguments first
    if ((CallbackObject == NULL) || (pfRes == NULL))
    {
        hr = E_POINTER;
    }

    //if the data thread has already been launched, return an error
    else if (m_dwDataThread != -1)
    {
        hr = E_FAIL;
        *pfRes = -1;
    }
    
    //Try to launch the data thread
    else
    {
        //Marshal the interface to the new thread
        IStream* pMarshalStream = NULL;
        hr = CoMarshalInterThreadInterfaceInStream( IID_IRTDUpdateEvent, 
         CallbackObject, &pMarshalStream );
        if (SUCCEEDED(hr)) 
        {
            g_spreadExcelRTD_instance = this;
            CreateThread( NULL, 0, RealTimeDataThread, (void*)pMarshalStream, 0, 
             &m_dwDataThread );
            *pfRes = m_dwDataThread;
        }
    }

    return hr;
}

/******************************************************************************
*  ConnectData -- Adds new topics from a real-time data server. The ConnectData
*  method is called when a file is opened that contains real-time data 
*  functions or when a user types in a new formula which contains the RTD 
*  function.
*  Parameters: TopicID -- value assigned by Excel to identify the topic
*              Strings -- safe array containing the strings identifying the 
*                         data to be served.
*              GetNewValues -- BOOLEAN indicating whether to retrieve new 
*                              values or not.
*              pvarOut -- initial value of the topic
*  Returns: S_OK
*           E_POINTER
*           E_FAIL
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::ConnectData(long TopicID,
                                    SAFEARRAY **Strings,
                                    VARIANT_BOOL *GetNewValues,
                                    VARIANT *pvarOut)
{
    WCHAR valBuffer[1024];
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::ConnectData\n");
    // See: http://msdn2.microsoft.com/en-us/library/ms682608(VS.85).aspx
    RTDSERVERTRACE("SpreadExcelRTD::ConnectData: EnterCriticalSection.\n");
    EnterCriticalSection( & m_criticalSection );
    RTDSERVERTRACE("SpreadExcelRTD::ConnectData: EnterCriticalSection succeeded.\n");
    HRESULT hr = S_OK;

    //Check the arguments first
    if (pvarOut == NULL) 
    {
        hr = E_POINTER;
    }
    else
    {
        //wip: JEK: Validate topic strings.
        //wip: JEK: Obey GetNewValues flag.
        //Add the topic ID to the list
        // Iterate over Strings and construct a concatenated topic name.
        //  This code is hideous!!!
        char tmpServerString[MAX_GROUP_NAME + 1];
        char tmpTopicString[MAX_GROUP_NAME + 1];
        char tmpTopicStringConcat[MAX_GROUP_NAME + 1];
        tmpTopicString[0]       = tmpTopicString[MAX_GROUP_NAME]       = '\0';
        tmpTopicStringConcat[0] = tmpTopicStringConcat[MAX_GROUP_NAME] = '\0';
        VARTYPE vt;
        hr = SafeArrayGetVartype(*Strings, &vt);
        sprintf_s(tmpTopicString, sizeof(tmpTopicString), "0x%x", vt);
        RTDSERVERTRACE("SpreadExcelRTD::ConnectData: vartype:");
        RTDSERVERTRACE(tmpTopicString);
        RTDSERVERTRACE("\n");
        if (FAILED(hr)) 
        {
            RTDSERVERTRACE("SpreadExcelRTD::ConnectData: can't get vartype\n");
        }
        // wip: JEK: Should check types that Strings has strings...
        else if (vt != VT_VARIANT)
        {
            // Should have one-dimensional array of variants
            RTDSERVERTRACE("SpreadExcelRTD::ConnectData: not a variant array\n");
            hr = E_INVALIDARG;
        }
        else if ( (*Strings)->cDims != 1)
        {
            // Should have one-dimensional array of variants
            RTDSERVERTRACE("SpreadExcelRTD::ConnectData: cDims not 1\n");
            hr = E_INVALIDARG;
        }
        else if ( (*Strings)->rgsabound[0].cElements < 2 || (*Strings)->rgsabound[0].lLbound != 0 )
        {
            // Should have at least two strings and the bound should start at 0.
            RTDSERVERTRACE("SpreadExcelRTD::ConnectData: bounds not good\n");
            hr = E_INVALIDARG;
        }
        else
        {
            //RTDSERVERTRACE("SpreadExcelRTD::ConnectData: has a variant array\n");
            VARIANT varValue;
            VariantInit( &varValue );
            long size = (*Strings)->rgsabound[0].cElements;
            if ( size < 2 )
            {
                RTDSERVERTRACE("SpreadExcelRTD::ConnectData: size is < 2\n");
                hr = E_INVALIDARG;
            }
            else for (long i = 0; i < size; i++)
            {
                hr = SafeArrayGetElement(*Strings, &i, &varValue);
                if (FAILED(hr)) 
                {
                    RTDSERVERTRACE("SpreadExcelRTD::ConnectData: Failed SafeArrayGetElement\n");
                    break;
                }
                else if (varValue.vt != VT_BSTR)
                {
                    RTDSERVERTRACE("SpreadExcelRTD::ConnectData: variant is not a BSTR\n");
                    hr = E_INVALIDARG;
                    break;
                }
                size_t numConverted;
                wcstombs_s(&numConverted, tmpTopicString, sizeof(tmpTopicString), varValue.bstrVal, _TRUNCATE);
                if (i == 0)
                {
                    RTDSERVERTRACE("SpreadExcelRTD::ConnectData: ServerName: ");
                    RTDSERVERTRACE(tmpTopicString);
                    RTDSERVERTRACE("\n");
                    // First string is the server name.
                    strcpy_s(tmpServerString, sizeof(tmpServerString), tmpTopicString);
                }
                else
                {
                    RTDSERVERTRACE("SpreadExcelRTD::ConnectData: TopicName: ");
                    RTDSERVERTRACE(tmpTopicString);
                    RTDSERVERTRACE("\n");
                    // Subsequent strings make up the topic name
                    strncat_s(tmpTopicStringConcat, sizeof(tmpTopicStringConcat), tmpTopicString, _TRUNCATE);
                }
            }
            ::VariantClear(& varValue);
        }
        if (SUCCEEDED(hr))
        {
            if ( m_pTopicMap->find(TopicID) != m_pTopicMap->end() )
            {
                RTDSERVERTRACE("SpreadExcelRTD::ConnectData: Found existing topic with same name!!\n");
                hr = E_FAIL;
            }
            else
            {
                // Create the topic node.  Note that the std::strings will all be rooted there.
                SpreadExcelRTDTopicNodePtr topicNode(
                    new SpreadExcelRTDTopicNode(
                        TopicID,
                        (const char*)tmpServerString,
                        (const char*)tmpTopicStringConcat,
                        ""
                    )
                );
                (*m_pTopicMap)[TopicID] = topicNode;

                // Lookup server and create it if not there.
                SpreadExcelRTD_ServerMap::iterator sit = m_pServerMap->find(topicNode->m_serverName);
                SpreadExcelRTDServerNodePtr serverNode;
                if ( sit == m_pServerMap->end() )
                {
                    SpreadExcelRTDServerNodePtr newServerNode(new SpreadExcelRTDServerNode(topicNode->m_serverName));
                    (*m_pServerMap)[topicNode->m_serverName] = newServerNode;
                    serverNode = newServerNode;
                    //
                    // Debug test of success...
#ifndef  NDEBUG
                    sit = m_pServerMap->find(topicNode->m_serverName);
                    if (strcmp(topicNode->m_serverName.c_str(), sit->first.c_str()) != 0 )
                    {
                        RTDSERVERTRACE("SpreadExcelRTD::ConnectData: Failed to insert server node!!\n");
                    }
#endif //NDEBUG
                }
                else
                {
                    serverNode = sit->second;
                }
                // Now add group
                (*(serverNode->m_pGroupMap))[topicNode->m_topicName] = TopicID;
#ifndef NDEBUG
                assert( ((*(serverNode->m_pGroupMap))[topicNode->m_topicName]) == TopicID );
#endif
                if ( (! serverNode->m_isConnected) )
                {
                    if ( serverNode->m_serverName != "sysTime" )
                    {
                        hr = E_FAIL;
                    }
                }
                else
                {

                    // Join the "topic"/"group"
                    int ret;
                    ret = SP_join(serverNode->m_mbox, topicNode->m_topicName.c_str());
                    if (ret < 0)
                    {
                        hr = E_FAIL;
                        RTDSERVERTRACE("SpreadExcelRTD::ConnectData: Failed to SP_join group: ");
                        RTDSERVERTRACE(topicNode->m_topicName.c_str());
                        RTDSERVERTRACE("\n");
                    }
                    else
                    {
                        RTDSERVERTRACEDEBUG("SpreadExcelRTD::ConnectData: Succeeded to SP_join group: ");
                        RTDSERVERTRACEDEBUG(topicNode->m_topicName.c_str());
                        RTDSERVERTRACEDEBUG("\n");
                    }

                }
                if (SUCCEEDED(hr))
                {
                    //
                    // Now return the data value for the topic.
                    const char* ccp = topicNode->m_dataValue.c_str();
                    size_t numConverted;
                    mbstowcs_s(&numConverted, valBuffer, 1024, ccp, _TRUNCATE);
                    VariantInit( pvarOut );
                    pvarOut->vt = VT_BSTR;
                    pvarOut->bstrVal = SysAllocString( valBuffer );
                }
            }
        }
    }
    LeaveCriticalSection( & m_criticalSection );
    RTDSERVERTRACE("SpreadExcelRTD::ConnectData: LeaveCriticalSection.\n");
    return hr;
}

/******************************************************************************
*  RefreshData -- This method is called by Microsoft Excel to get new data. 
*  This method call only takes place after being notified by the real-time 
*  data server that there is new data.
*  Parameters: TopicCount -- filled with the count of topics in the safearray
*              parrayOut -- two-dimensional safearray.  First dimension 
*                           contains the list of topic IDs.  Second dimension 
*                           contains the values of those topics.
*  Returns: S_OK
*           E_POINTER
*           E_FAIL
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::RefreshData( long *TopicCount,
                                    SAFEARRAY **parrayOut)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::RefreshData\n");
    HRESULT hr = S_OK;
    RTDSERVERTRACE("SpreadExcelRTD::RefreshData: EnterCriticalSection.\n");
    EnterCriticalSection( & m_criticalSection );
    RTDSERVERTRACE("SpreadExcelRTD::RefreshData: EnterCriticalSection succeeded.\n");

    //Check the arguments first
    if ((TopicCount == NULL) || (parrayOut == NULL) || (*parrayOut != NULL))
    {
        hr = E_POINTER;
        RTDSERVERTRACE("   Bad pointer\n");
    }
   
    else
    {
        //Set the TopicCount
        *TopicCount = m_pChangedSet->size();

        SAFEARRAYBOUND bounds[2];
        WCHAR valBuffer[1024];
        VARIANT value;
        long index[2];

        //Build the safe-array values we want to insert
        bounds[0].cElements = 2;
        bounds[0].lLbound = 0;
        bounds[1].cElements = m_pChangedSet->size();
        bounds[1].lLbound = 0;
        *parrayOut = SafeArrayCreate(VT_VARIANT, 2, bounds);
      
        // Restrict the elements that are updated to only those that have changed.
        int size = m_pChangedSet->size(); 
#ifndef NDEBUG
        RTDSERVERTRACEDEBUG("SpreadExcelRTD::RefreshData: Number of changed topics: ");
        char txt[16];
        sprintf_s(txt, sizeof(txt), "%d\n", size);
        RTDSERVERTRACEDEBUG(txt);
#endif
        SpreadExcelRTD_ChangedSet::iterator cit = m_pChangedSet->begin();
        for (int i = 0; i < size; i++)
        { 
            size_t numConverted;
            long topicId = *cit;
            SpreadExcelRTD_TopicMap::iterator tit = m_pTopicMap->find(topicId);
            if ( tit != m_pTopicMap->end())
            {
                SpreadExcelRTDTopicNodePtr& topicNode = tit->second;
                RTDSERVERTRACE("SpreadExcelRTD::RefreshData: topic: ");
                RTDSERVERTRACE(topicNode->m_topicName.c_str());
                RTDSERVERTRACE("\n");
                
                //Construct the return value
                const char* ccp = topicNode->m_dataValue.c_str();
                mbstowcs_s(&numConverted, valBuffer, 1024, ccp, _TRUNCATE);
            }
            else
            {
                RTDSERVERTRACE("SpreadExcelRTD::RefreshData: serious error - can't find topic: ");
                char txt[16];
                sprintf_s(txt, sizeof(txt), "%l\n", topicId);
                RTDSERVERTRACE(txt);
                mbstowcs_s(&numConverted, valBuffer, 1024, "<error>", _TRUNCATE);
            }
            index[0] = 0;
            index[1] = i;
            
            VariantInit(&value);
            value.vt = VT_I4;
            value.lVal = topicId;
            SafeArrayPutElement( *parrayOut, index, &value);
            ::VariantClear(& value);
            
            index[0] = 1;
            index[1] = i;
            
            VariantInit(&value);
            value.vt = VT_BSTR;
            //JEK: Not clearing the variant is a source of memory leak.
            // See: http://www.wiredbox.net/Forum/Thread211395_Memory_leak_in_RTD_RefreshData_implementation.aspx
            value.bstrVal = SysAllocString( valBuffer );
            SafeArrayPutElement( *parrayOut, index, &value);
            //JEK: Freeing the string seems to get rid of the memory leak.
            //JEK: This means that it is probably acceptable to just use valBuffer.
            // could also just use: SysFreeString( value.bstrVal );
            ::VariantClear(& value);
            cit++;
        }
        // Clear out the changed set.
        m_pChangedSet->clear();
    }
    LeaveCriticalSection( & m_criticalSection );
    RTDSERVERTRACE("SpreadExcelRTD::RefreshData: LeaveCriticalSection.\n");
    return hr;
}

/******************************************************************************
*  DisconnectData -- Notifies the RTD server application that a topic is no
*  longer in use.
*  Parameters: TopicID -- the topic that is no longer in use.
*  Returns:
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::DisconnectData( long TopicID)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::DisconnectData\n");
    RTDSERVERTRACE("SpreadExcelRTD::DisconnectData: EnterCriticalSection.\n");
    EnterCriticalSection( & m_criticalSection );
    RTDSERVERTRACE("SpreadExcelRTD::DisconnectData: EnterCriticalSection succeeded.\n");
    HRESULT hr = S_OK;

    //Search for the topic id and remove it
    SpreadExcelRTD_TopicMap::iterator tit = m_pTopicMap->find(TopicID);
    if ( (tit) != m_pTopicMap->end())
    {
        SpreadExcelRTDTopicNodePtr& topicNode = tit->second;
        const std::string serverName = topicNode->m_serverName;
        const std::string topicName  = topicNode->m_topicName;
        SpreadExcelRTDServerNodePtr serverNode;
        SpreadExcelRTD_ServerMap::iterator sit = m_pServerMap->find(serverName);
        if ( (sit) != m_pServerMap->end())
        {
            serverNode = sit->second;
            SpreadExcelRTD_GroupMap* pGroupMap = serverNode->m_pGroupMap;
            SpreadExcelRTD_GroupMap::iterator git = pGroupMap->find(topicName);
            if ( (git) != pGroupMap->end())
            {
                //JEK: Notify spread of leaving the group.
                if (sit->second->m_isConnected)
                {
                    int ret;
                    ret = SP_leave(sit->second->m_mbox, topicName.c_str());
                    if (ret < 0)
                    {
                        RTDSERVERTRACE("SpreadExcelRTD: Error in SP_leave from server: ");
                        RTDSERVERTRACE(serverName.c_str());
                        RTDSERVERTRACE(" group: ");
                        RTDSERVERTRACE(topicName.c_str());
                        RTDSERVERTRACE("\n");
                    }
                    else
                    {
                        RTDSERVERTRACE("SpreadExcelRTD: Succeed in SP_leave from server: ");
                        RTDSERVERTRACE(serverName.c_str());
                        RTDSERVERTRACE(" group: ");
                        RTDSERVERTRACE(topicName.c_str());
                        RTDSERVERTRACE("\n");
                    }
                }
                else
                {
                    RTDSERVERTRACE("SpreadExcelRTD: SP_leave not done because disconnected from server: ");
                    RTDSERVERTRACE(serverName.c_str());
                    RTDSERVERTRACE(" group: ");
                    RTDSERVERTRACE(topicName.c_str());
                    RTDSERVERTRACE("\n");
                }
                // Now erase the group:
                pGroupMap->erase(topicName);
                // wip: JEK: Could close/erase the server if no more groups present.
            }
            else
            {
                RTDSERVERTRACE("SpreadExcelRTD: Couldn't find group to disconnect: ");
                RTDSERVERTRACE(topicName.c_str());
                RTDSERVERTRACE(" from server: ");
                RTDSERVERTRACE(serverName.c_str());
                RTDSERVERTRACE("\n");
            }
        }
        else
        {
            RTDSERVERTRACE("SpreadExcelRTD: Couldn't find server to disconnect: ");
            RTDSERVERTRACE(serverName.c_str());
            RTDSERVERTRACE("\n");
        }
        m_pTopicMap->erase(TopicID);
    }

    LeaveCriticalSection( & m_criticalSection );
    RTDSERVERTRACE("SpreadExcelRTD::DisconnectData: LeaveCriticalSection.\n");
    return hr;
}

/******************************************************************************
*  CheckForMessages -- Called from data thread on fast timer to check the
*   spread servers for messages.  Needs to protect critical sections.
*  Parameters: None
*  Returns: S_OK     if some changes.
*           S_FALSE  if no changes.
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::CheckForMessages(void)
{
    //tracing purposes only
    RTDSERVERTRACEDEBUG("SpreadExcelRTD::CheckForMessages\n");
    HRESULT hr = S_FALSE; // Mark return result as not having changes.

    // Protect this section from changes in topology of topics.
    //  If the try fails, the next timer event will come along later.
    if (TryEnterCriticalSection(& m_criticalSection) == 0 )
    {
        RTDSERVERTRACE("SpreadExcelRTD::CheckForMessages: TryEnterCriticalSection failed.\n");
        return E_FAIL;
    }
    RTDSERVERTRACEDEBUG("SpreadExcelRTD::CheckForMessages: TryEnterCriticalSection succeeded.\n");

    //
    // Iterate over servers and peek for messages
    SpreadExcelRTDServerNodePtr serverNode;
    SpreadExcelRTD_ServerMap::iterator sit = m_pServerMap->begin();
    for (; (sit) != m_pServerMap->end(); ++sit)
    {
        serverNode = sit->second;
        if (serverNode->m_isConnected)
        {
            int ret;
            while ( (ret = SP_poll(serverNode->m_mbox)) > 0 )
            {
                // Drain the messages.
                // This code adapted from SpreadModule for Python.
                /* CAUTION:  initializing svc_type is critical.  It's not clear from
                 * the docs, but this is an input as well as an output parameter.
                 * We didn't initialize it before, and very rarely the DROP_RECV flag
                 * would end up getting set in it.  That in turn has miserable
                 * consequences, and consequences only visible if a buffer (data or
                 * group) is too small for the msg being received (so it goes crazy
                 * at the worst possible times).
                 */
                SpreadExcelRTD_GroupMap* pGroupMap = serverNode->m_pGroupMap;

                service svc_type;
                int num_groups, endian, size;
                int16 msg_type;

#define DEFAULT_GROUPS_SIZE 10
#define DEFAULT_BUFFER_SIZE 10000
                char senderbuffer[MAX_GROUP_NAME];
                char groupbuffer[DEFAULT_GROUPS_SIZE][MAX_GROUP_NAME];
                char databuffer[DEFAULT_BUFFER_SIZE];

                int max_groups = DEFAULT_GROUPS_SIZE;
                char (*groups)[MAX_GROUP_NAME] = groupbuffer;

                int bufsize = DEFAULT_BUFFER_SIZE + 1;
                char *pbuffer = databuffer;

                // Perform the SP_receive loop
                for (;;)
                {
                    char *assertmsg = "internal error";

                    svc_type = 0;   /* initializing this is critical */
                    size = SP_receive(serverNode->m_mbox, &svc_type,
                              senderbuffer,
                              max_groups, &num_groups, groups,
                              &msg_type, &endian,
                              bufsize, pbuffer);

                    if (size >= 0)
                    {
                        if (num_groups < 0)
                        {
                            /* This isn't possible unless DROP_RECV is
                             * passed to SP_receive in svc_type.
                             */
                            assertmsg = "size >= 0 and num_groups < 0";
                            goto assert_error;
                        }
                        if (endian < 0)
                        {
                            /* This should never be possible. */
                            assertmsg = "size >= 0 and endian < 0";
                            goto assert_error;
                        }
                        // We added one byte to make the NUL possible
                        pbuffer[size] = '\0';
                        break;  /* This is the only normal loop exit. */
                    }
                    if (size == BUFFER_TOO_SHORT)
                    {
                        if (endian >= 0)
                        {
                            /* This isn't possible unless DROP_RECV is
                             * passed to SP_receive in svc_type.
                             */
                            assertmsg = "BUFFER_TOO_SHORT and endian >= 0";
                            goto assert_error;
                        }
                        bufsize = - endian;
                        pbuffer = (char*) malloc(bufsize + 1);
                        if (pbuffer == NULL)
                            goto error;
                        // Retry the read with the larger buffer.
                        continue;
                    }
                    if (size == GROUPS_TOO_SHORT)
                    {
                        /* If the data buffer and the group buffer are both
                         * too small, and DROP_RECV was not specified, then
                         * Jonathan Stanton said GROUPS_TOO_SHORT is returned.
                         * If both are too short and DROP_RECV is specified,
                         * then BUFFER_TOO_SHORT is returned.  "Backward
                         * compatibility" headaches.  For simplicity, we only
                         * deal with one "too short" condition per loop trip.
                         * When we loop back, SP_receive should tell us
                         * about the other (if another thread hasn't already
                         * grabbed the msg).
                         */
                        if (num_groups >= 0)
                        {
                            /* This should never be possible. */
                            assertmsg = "GROUPS_TOO_SHORT and num_groups >= 0";
                            goto assert_error;
                        }
                        max_groups = - num_groups;
                        if (groups != groupbuffer)
                            free(groups);
                        groups =  (char (*)[MAX_GROUP_NAME]) malloc(MAX_GROUP_NAME * max_groups);
                        if (groups == NULL)
                        {
                            goto error;
                        }
                        continue;
                    }
                    /* There's a real error we can't deal with (e.g., Spread
                     * got disconnected).
                     */
                    serverNode->disconnect();
                    goto error;
            assert_error:
            /*
                    PyErr_Format(PyExc_AssertionError,
                             "SP_receive: %s; "
                             "size=%d svc_type=%d num_groups=%d "
                             "msg_type=%d endian=%d",
                             assertmsg,
                             size, svc_type, num_groups, msg_type, endian);
            */
                    RTDSERVERTRACE("SpreadExcelRTD::CheckForMessages: assertion failed!!: ");
                    RTDSERVERTRACE(assertmsg);
                    RTDSERVERTRACE("\n");
                    goto error;
                }

                /* It's not clear from the SP_receive() man page what all the
                   possible categories of services types are possible. */

                RTDSERVERTRACEDEBUG("SpreadExcelRTD::CheckForMessages: sender: ");
                RTDSERVERTRACEDEBUG(senderbuffer);
                RTDSERVERTRACEDEBUG("\n");

                if (Is_regular_mess(svc_type))
                {
                    std::string groupName;
                    // Iterate over groups and send data to topics.
                    for ( int i = 0; i < num_groups; i++ )
                    {
                        groupName = groups[i];
                        // Look up group and set topic data.
                        SpreadExcelRTD_GroupMap::iterator git = pGroupMap->find(groupName);
                        if ( (git) != pGroupMap->end())
                        {
                            long topicId = git->second;
                            // Look up topicNode
                            SpreadExcelRTD_TopicMap::iterator tit = m_pTopicMap->find(topicId);
                            if ( (tit) != m_pTopicMap->end())
                            {
                                SpreadExcelRTDTopicNodePtr& topicNode = tit->second;
                                //wip: should check the string to be "reasonable".....
                                hr = S_OK; // Mark return result as having changes.
                                topicNode->m_dataValue = pbuffer;
                                m_pChangedSet->insert(topicNode->m_topicId);
                                RTDSERVERTRACEDEBUG("SpreadExcelRTD: CheckForMessages: from server: ");
                                RTDSERVERTRACEDEBUG(topicNode->m_serverName.c_str());
                                RTDSERVERTRACEDEBUG(" to group: ");
                                RTDSERVERTRACEDEBUG(topicNode->m_topicName.c_str());
                                RTDSERVERTRACEDEBUG(" data: '");
                                RTDSERVERTRACEDEBUG(pbuffer);
                                RTDSERVERTRACEDEBUG("'\n");
                            }
                            else
                            {
                                RTDSERVERTRACE("SpreadExcelRTD: CheckForMessages: Couldn't find topic: ");
                                RTDSERVERTRACE(groupName.c_str());
                                RTDSERVERTRACE("\n");
                            }
                        }
                        else
                        {
                            RTDSERVERTRACE("SpreadExcelRTD: CheckForMessages: Couldn't find group: ");
                            RTDSERVERTRACE(groupName.c_str());
                            RTDSERVERTRACE("\n");
                        }
                    }
                }
                // Ignore membership messages

              error:
                if (groups != groupbuffer)
                    free(groups);
                if (pbuffer != databuffer)
                    free(pbuffer);
            }
            if ( ret < 0 )
            {
                // Disconnect if there is an error.
                RTDSERVERTRACE("SpreadExcelRTD::CheckForMessages: error polling - disconnect.\n");
                serverNode->disconnect();
            }
        }
    }
    LeaveCriticalSection( & m_criticalSection );
    RTDSERVERTRACEDEBUG("SpreadExcelRTD::CheckForMessages: LeaveCriticalSection.\n");

    return hr;
}

/******************************************************************************
*  ProcessTimeTopics -- Called from data thread on slow timer to check for
*   a "sysTime" server to update.  Needs to protect critical sections.
*  Parameters: None
*  Returns: S_OK     if some changes.
*           S_FALSE  if no changes.
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::ProcessTimeTopics (void)
{
    //tracing purposes only
    RTDSERVERTRACEDEBUG("SpreadExcelRTD::ProcessTimeTopics\n");
    HRESULT hr = S_FALSE; // Mark return result as not having changes.

    // Protect this section from changes in topology of topics.
    //  If the try fails, the next timer event will come along later.
    if (TryEnterCriticalSection(& m_criticalSection) == 0 )
    {
        RTDSERVERTRACE("SpreadExcelRTD::ProcessTimeTopics: TryEnterCriticalSection failed.\n");
        return E_FAIL;
    }
    RTDSERVERTRACEDEBUG("SpreadExcelRTD::ProcessTimeTopics: TryEnterCriticalSection succeeded.\n");

    //
    // Iterate over servers and process sysTime one(s)
    SpreadExcelRTDServerNodePtr serverNode;
    SpreadExcelRTD_ServerMap::iterator sit = m_pServerMap->begin();
    for (; (sit) != m_pServerMap->end(); ++sit)
    {
        serverNode = sit->second;
        if (serverNode->m_serverName == "sysTime")
        {
            char valBuffer[1024];
            SYSTEMTIME sysTime;
            GetSystemTime( &sysTime );

            SpreadExcelRTD_GroupMap* pGroupMap = serverNode->m_pGroupMap;
            SpreadExcelRTD_GroupMap::iterator git = pGroupMap->begin();
            for (; git != pGroupMap->end(); ++git)
            {
                SpreadExcelRTD_TopicMap::iterator tit = m_pTopicMap->find(git->second);
                if ( (tit) != m_pTopicMap->end())
                {
                    SpreadExcelRTDTopicNodePtr& topicNode = tit->second;
                    //Build a value to return
                    sprintf_s( valBuffer, sizeof(valBuffer), "TopicID: %d | TopicName: %s | %02d:%02d:%02d",
                        topicNode->m_topicId,
                        topicNode->m_topicName.c_str(),
                        sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
                    topicNode->m_dataValue = valBuffer;
                    hr = S_OK; // Mark return result as having changes.
                    m_pChangedSet->insert(topicNode->m_topicId);
                    RTDSERVERTRACEDEBUG("processSysTimeTopic: ");
                    RTDSERVERTRACEDEBUG(valBuffer);
                    RTDSERVERTRACEDEBUG("\n");
                }
            }
        }
    }
    LeaveCriticalSection( & m_criticalSection );
    RTDSERVERTRACEDEBUG("SpreadExcelRTD::ProcessTimeTopics: LeaveCriticalSection.\n");

    return hr;
}

/******************************************************************************
*  Heartbeat -- Determines if the real-time data server is still active.
*  Parameters: pfRes -- filled with zero or negative number to indicate
*                       failure; positive number indicates success.
*  Returns: S_OK
*           E_POINTER
*           E_FAIL
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::Heartbeat(long *pfRes)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::Heartbeat\n");
    HRESULT hr = S_OK;

    //Let's reply with the ID of the data thread
    if (pfRes == NULL)
    {
        hr = E_POINTER;
    }
    else
    {
        *pfRes = m_dwDataThread;
    }

    return hr;
}

/******************************************************************************
*  ServerTerminate -- Terminates the connection to the real-time data server.
*  Parameters: none
*  Returns: S_OK
*           E_FAIL
******************************************************************************/
STDMETHODIMP SpreadExcelRTD::ServerTerminate(void)
{
    //tracing purposes only
    RTDSERVERTRACE("SpreadExcelRTD::ServerTerminate\n");
    HRESULT hr = S_OK;

    //Make sure we kill the data thread
    if (m_dwDataThread != -1)
    {
        PostThreadMessage( m_dwDataThread, WM_COMMAND, WM_TERMINATE, 0 );
    }
  //wip: JEK: Notify spread of disconnect (need to lock?)

    return hr;
}

//
SpreadExcelRTDServerNode::SpreadExcelRTDServerNode(const std::string serverName)
{
    m_pGroupMap = new SpreadExcelRTD_GroupMap();
    this->connect(serverName);
}

//
void
SpreadExcelRTDServerNode::connect(const std::string serverName)
{
    int  ret;
    char private_group[MAX_GROUP_NAME + 2];
    m_mbox    = -1;
    m_isConnected = 0;
    m_privateGroup = "<Not connected>";
    m_serverName   = serverName;
    // Try to connect.
    if ( strcmp(m_serverName.c_str(), "sysTime") == 0 )
    {
        m_privateGroup = "<sysTime>";
        RTDSERVERTRACE("SpreadExcelRTDServerNode: sysTime server name\n");
    }
    else
    {
        //JEK: connect to spread server:
        ret = SP_connect(m_serverName.c_str(), NULL, 0, 0, &m_mbox, private_group);
        if ( ret < 0 )
        {
            m_mbox    = -1;
            RTDSERVERTRACE("SpreadExcelRTDServerNode: SP_connect failed to connect to server: ");
            RTDSERVERTRACE(m_serverName.c_str());
            RTDSERVERTRACE("\n");
        }
        else
        {
            m_isConnected = 1;
            m_privateGroup = private_group;
            RTDSERVERTRACE("SpreadExcelRTDServerNode: SP_connect succeeded to server: ");
            RTDSERVERTRACE(m_serverName.c_str());
            RTDSERVERTRACE(" as: ");
            RTDSERVERTRACE(private_group);
            RTDSERVERTRACE("\n");
        }
    }
}

//
void
SpreadExcelRTDServerNode::disconnect()
{
    // Disconnect if connected
    if ( m_isConnected )
    {
        m_isConnected = 0;
        int ret;
        ret = SP_disconnect( m_mbox );
        m_mbox = -1;
        if (ret < 0)
        {
            RTDSERVERTRACE("SpreadExcelRTDServerNode: SP_disconnect failed from server: ");
        }
        else
        {
            RTDSERVERTRACE("SpreadExcelRTDServerNode: SP_disconnect succeeded from server: ");
        }
        RTDSERVERTRACE(m_privateGroup.c_str());
        RTDSERVERTRACE("\n");
        m_privateGroup = "<Not connected>";
    }
    else if ( m_serverName == "sysTime" )
    {
        RTDSERVERTRACE("SpreadExcelRTDServerNode: Closing systime\n");
    }
    else
    {
        RTDSERVERTRACE("SpreadExcelRTDServerNode: Already disconnected from server!!!: ");
        RTDSERVERTRACE(m_serverName.c_str());
        RTDSERVERTRACE("\n");
    }
}

//
SpreadExcelRTDServerNode::~SpreadExcelRTDServerNode()
{
    RTDSERVERTRACE("~SpreadExcelRTDServerNode: ");
    RTDSERVERTRACE(m_serverName.c_str());
    RTDSERVERTRACE("\n");
    disconnect();
#if 0
    // Leak this for now while debugging
    if ( m_pGroupMap == NULL )
    {
        RTDSERVERTRACE("~SpreadExcelRTDServerNode: m_pGroupMap is NULL!!!\n");
    }
    else
    {
        delete m_pGroupMap;
    }
    disconnect();
#endif
    m_pGroupMap = NULL;
}

