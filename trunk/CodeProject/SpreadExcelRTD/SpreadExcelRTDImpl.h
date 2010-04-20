/******************************************************************************
* $Id: SpreadExcelRTDImpl.h 24 2008-02-20 17:21:19Z jim.kleckner $
*
*   File:   SpreadExcelRTDImpl.h
*
*   Date:   February 5, 2001
*
*   Description:   This file contains the declaration of a simple real-time-data 
*                  server for Excel.
*
*   Modifications:
*
*     Morphed into a spread toolkit data provider.
*
 * Copyright 2008, James E. Kleckner
 *  Based on example published by Microsoft without any copyright notices.

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
#include "comdef.h"
#include "sp.h"
#include "RTDDataThread.h"
//////////////////////////////// begin map usage
// SpreadExcelRTDImpl.cpp
#include <string>
#include <map>
#include <set>

/*
See this discussion for an excellent description of the brain-dead
way that std collections work:
  http://www.velocityreviews.com/forums/t285031-p2-stdmap-question.html
You have to implement reference-counted handles for the mapping objects.

The following counted_ptr implementation was taken from here:
 http://ootips.org/yonat/4dev/smart-pointers.html
*/
/*
 * counted_ptr - simple reference counted pointer.
 *
 * The is a non-intrusive implementation that allocates an additional
 * int and pointer for every counted object.
 */

#ifndef COUNTED_PTR_H
#define COUNTED_PTR_H

/* For ANSI-challenged compilers, you may want to #define
 * NO_MEMBER_TEMPLATES or explicit */
#define NO_MEMBER_TEMPLATES 1

template <class X> class counted_ptr
{
public:
    typedef X element_type;

    explicit counted_ptr(X* p = 0) // allocate a new counter
        : itsCounter(0) {if (p) itsCounter = new counter(p);}
    ~counted_ptr()
        {release();}
    counted_ptr(const counted_ptr& r) throw()
        {acquire(r.itsCounter);}
    counted_ptr& operator=(const counted_ptr& r)
    {
        if (this != &r) {
            release();
            acquire(r.itsCounter);
        }
        return *this;
    }

#ifndef NO_MEMBER_TEMPLATES
    template <class Y> friend class counted_ptr<Y>;
    template <class Y> counted_ptr(const counted_ptr<Y>& r) throw()
        {acquire(r.itsCounter);}
    template <class Y> counted_ptr& operator=(const counted_ptr<Y>& r)
    {
        if (this != &r) {
            release();
            acquire(r.itsCounter);
        }
        return *this;
    }
#endif // NO_MEMBER_TEMPLATES

    X& operator*()  const throw()   {return *itsCounter->ptr;}
    X* operator->() const throw()   {return itsCounter->ptr;}
    X* get()        const throw()   {return itsCounter ? itsCounter->ptr : 0;}
    bool unique()   const throw()
        {return (itsCounter ? itsCounter->count == 1 : true);}

private:

    struct counter {
        counter(X* p = 0, unsigned c = 1) : ptr(p), count(c) {}
        X*          ptr;
        unsigned    count;
    }* itsCounter;

    void acquire(counter* c) throw()
    { // increment the count
        itsCounter = c;
        if (c) ++c->count;
    }

    void release()
    { // decrement the count, delete if it is 0
        if (itsCounter) {
            if (--itsCounter->count == 0) {
                delete itsCounter->ptr;
                delete itsCounter;
            }
            itsCounter = 0;
        }
    }
};

#endif // COUNTED_PTR_H


typedef class SpreadExcelRTDTopicNode {
public:
    long        m_topicId;
    // Note that these std::string values are rooted and used in various maps.
    //  Delete the topic nodes last...
    std::string m_serverName;
    std::string m_topicName;
    std::string m_dataValue;
    SpreadExcelRTDTopicNode(long tid, const std::string& csn, const std::string& ctn, const std::string& cdv)
        : m_topicId(tid), m_serverName(csn), m_topicName(ctn), m_dataValue(cdv)
    { }
    /*
    SpreadExcelRTDTopicNode()
    {
        m_serverName = "";
        m_topicName  = "";
        m_dataValue  = "";
    }
    SpreadExcelRTDTopicNode(const SpreadExcelRTDTopicNode &other) : m_serverName(other.m_serverName), m_topicName(other.m_topicName), m_dataValue(other.m_dataValue)
    {
    }
    */
    ~SpreadExcelRTDTopicNode()
    { }
} SpreadExcelRTDTopicNode;

typedef counted_ptr<SpreadExcelRTDTopicNode>  SpreadExcelRTDTopicNodePtr;

#if 1
typedef long SpreadExcelRTDGroupNode;
#else
typedef class SpreadExcelRTDGroupNode {
public:
    long   m_topicId;
    //
    SpreadExcelRTDGroupNode()
    {
        m_topicId = -1;
    }
    ~SpreadExcelRTDGroupNode()
    {
        m_topicId = -1;
    }
} SpreadExcelRTDGroupNode;
#endif

typedef std::map<const std::string, SpreadExcelRTDGroupNode>  SpreadExcelRTD_GroupMap;

typedef class SpreadExcelRTDServerNode {
public:
    std::string              m_serverName;
    std::string              m_privateGroup;
    mailbox                  m_mbox;
    SpreadExcelRTD_GroupMap* m_pGroupMap;
    int                      m_isConnected;
    //
    SpreadExcelRTDServerNode(const std::string serverName);
    virtual ~SpreadExcelRTDServerNode();
    void connect(const std::string serverName);
    void disconnect();
} SpreadExcelRTDServerNode;

typedef counted_ptr<SpreadExcelRTDServerNode>  SpreadExcelRTDServerNodePtr;


typedef std::map<long, SpreadExcelRTDTopicNodePtr>               SpreadExcelRTD_TopicMap;
typedef std::map<const std::string, SpreadExcelRTDServerNodePtr> SpreadExcelRTD_ServerMap;

typedef std::set<long>  SpreadExcelRTD_ChangedSet;

extern LONG g_cOb; //global count of the number of objects created.

struct INonDelegatingUnknown
{
   /***** INonDelegatingUnknown Methods *****/
   virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, 
      void ** ppvObj) = 0;      
   virtual STDMETHODIMP_(ULONG) NonDelegatingAddRef() = 0;
   virtual STDMETHODIMP_(ULONG) NonDelegatingRelease() = 0;
};      

class SpreadExcelRTD : public INonDelegatingUnknown,
public IRtdServer
{
private:
   int         m_refCount;
   IUnknown*   m_pOuterUnknown;
   ITypeInfo*  m_pTypeInfoInterface;
   DWORD       m_dwDataThread;
   // Begin JEK-added members:
   CRITICAL_SECTION           m_criticalSection;
   SpreadExcelRTD_TopicMap*   m_pTopicMap;
   SpreadExcelRTD_ServerMap*  m_pServerMap;
   SpreadExcelRTD_ChangedSet* m_pChangedSet;

public:
   //Constructor
   SpreadExcelRTD(IUnknown* pUnkOuter);
   //Destructor
   ~SpreadExcelRTD();
   
   STDMETHODIMP LoadTypeInfo(ITypeInfo** pptinfo, REFCLSID clsid,
      LCID lcid);

   /***** IUnknown Methods *****/
   STDMETHODIMP QueryInterface(REFIID riid, void ** ppvObj);
   STDMETHODIMP_(ULONG) AddRef();
   STDMETHODIMP_(ULONG) Release();
   
   /***** INonDelegatingUnknown Methods *****/
   STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, 
      void ** ppvObj);      
   STDMETHODIMP_(ULONG) NonDelegatingAddRef();
   STDMETHODIMP_(ULONG) NonDelegatingRelease();
   
   /***** IDispatch Methods *****/
   STDMETHODIMP GetTypeInfoCount(UINT *iTInfo);
   STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, 
      ITypeInfo **ppTInfo);
   STDMETHODIMP GetIDsOfNames(REFIID riid,  
      OLECHAR **rgszNames, 
      UINT cNames,  LCID lcid,
      DISPID *rgDispId);
   STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
      WORD wFlags, DISPPARAMS* pDispParams,
      VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
      UINT* puArgErr);
   
   /***** ISpreadExcelRTD Methods *****/
   STDMETHODIMP ServerStart( 
      IRTDUpdateEvent *CallbackObject,
      long *pfRes);
   
   STDMETHODIMP ConnectData( 
      long TopicID,
      SAFEARRAY * *Strings,
      VARIANT_BOOL *GetNewValues,
      VARIANT *pvarOut);
   
   STDMETHODIMP RefreshData( 
      long *TopicCount,
      SAFEARRAY * *parrayOut);
   
   STDMETHODIMP DisconnectData( 
      long TopicID);
   
   STDMETHODIMP Heartbeat( 
      long *pfRes);
   
   STDMETHODIMP ServerTerminate( void);   
   /***** Callbacks Methods for data thread *****/
   STDMETHODIMP CheckForMessages(void);   
   STDMETHODIMP ProcessTimeTopics(void);   
};

extern SpreadExcelRTD* g_spreadExcelRTD_instance;
