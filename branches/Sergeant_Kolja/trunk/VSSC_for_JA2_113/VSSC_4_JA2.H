/*! \file VSSC_4_JA2.H
 *  \brief Very Simple Syslog Client for Jagged Alliance. Header. This file currently (Rev 700)
 *   only contains the rich numer of classic debug macro definitions, with their original code mapping
 *
 *  This File is the Interface for
 *  - Classic JA2 Debug / Trace Macros & Functions
 *
 *  It reports to
 *  - Syslog UDP port 514 (as defined in RFC 3164) (Debug + Release)
 *  - OutputDebugString (MS), but only if DEBUG or _DEBUG is set
 *
 *  \author Sergeant Kolja (Initial Author)<br>
 */


/* revision 0: old style JA2 macros, not changed yet. */



/*! \todo VSSC_4_JA2.H is currently only a helper/container file
 *  - this file shall later resolve into VSSC.H
 *  - because of some C++ templates, this file has currently not an C-only-Interface :-(
 *    but the power of templates is not really used inside the code,
 *    so we can junk it in the near future.
 */


/*! \defgroup Logmacros Macros mapped to classic JA2 LogFunctions
 * @{
 */


#ifdef __cplusplus
   extern "C" {
#endif


/*----------------------------------------------------------------------------------*/
#if defined( DEBUG ) || defined( _DEBUG ) || defined( DEBUG_ ) || defined( SGP_DEBUG )
/*----------------------------------------------------------------------------------*/


  /*! \brief Assert() Macro. It is fired, if the given expresion (ass) evaluates to FALSE.
   *
   * Assert, _assert(), ASSERT() and similiar Macros shall be used more frequently,
   * in RELEASE code they condense to nothing, resulting in no slower or bigger code.
   * But in DEBUG Code (Testing!) they usually talk about the failed assumption,
   * the location (file, line) and then abort() the process.
   * The more Asserts we use, the more failures we will locate before giving the 
   * code productive state. But assert is <b>no</b> replacement for some runtime checks
   * on function arguments. Go and find out the best way!
   * This Maco is called f.i. in Font.cpp as "Assert(iFont >= 0);"
   *
   * \todo Assert, AssertMsg are not mapped to VSSC_Log()
   */
  #define Assert(ass)              (ass) ? _Null() : _FailMessage( NULL, __LINE__, __FILE__ )

  /*! \brief AssertMsg() Macro. It is fired, if the given expresion (ass) evaluates to FALSE. 
   * Plus, it reports a string (msg)
   *
   * This is a slightly powerful variant of Assert()
   */
  #define AssertMsg(ass,msg)       (ass) ? _Null() : _FailMessage( msg , __LINE__, __FILE__ )

  /*! \brief DebugBreakpoint() Macro. Breaks the Program while running in IDE.
   *
   * currently, it is NOT used anymore in JA2 code.
   *
   * \todo IIRC, there is already a relative Command in MS SDK, called DebugBreak().
   * May be, we change later to it. Or we remove this one completely.
   */
  #define DebugBreakpoint()    __asm { int 3 }


  /*! \brief DbgMessage() Macro. Gives out a message - sometimes built from 
   *   sprintf + a level and a source ID
   *
   * - DBG_LEVEL_X is defined in TopicOps.h
   * - TOPIC_XXXXX is defined in TopicIDs.h and some in TopicOps.h. But Attention!
   *         it is not a list of constant int defines, but a list of global integers
   *         which are cleared by ClearAllDebugTopics() and set / reset by
   *         DbgTopicRegistration() (or macro variants of it). This makes
   *         Debug some flexible: every Topic can dynamically be registered.
   * - srcid : a dereferencable integer with a number describing where the message comes from
   * - lvl   : a debug level: 0 internal use only,
   *                        1 basic stuff, very important
   *                        2 even often interesting stuff
   *                        3 very detailed things.
   * msg   : an ascii zero terminated C string
   * 
   * typical calls are:
   * \code
     DbgMessage( TOPIC_BUTTON_HANDLER, DBG_LEVEL_0, "CreateCheckBoxButton: Can't load button image");
     DbgMessage( TOPIC_BUTTON_HANDLER, DBG_LEVEL_0, String("No button pictures selected for %s",filename));
     DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_2, "No extension given, using default" );
   * \endcode
   */
  #define DbgMessage( srcid, lvl, msg)     DbgMessageReal( (UINT16)(srcid), (UINT8)(TOPIC_MESSAGE), (UINT8)(lvl), (CHAR8 *)(msg) )


  /*! \brief FastDebugMsg() Macro
   * Gives out a message - sometimes built from sprintf - completed by line number and source file name
   *
   * msg   : an ascii zero terminated C string
   * typical calls are:
   * \code
     FastDebugMsg("Initializing Memory Manager");
     FastDebugMsg(String("BuildFileDirectory: CreateStack Failed for the filename stack"));
     FastDebugMsg(String("DirectoryExists: ERROR - GetFileAttributes failed, error #%d on file %s", uiLastError, pcDirectory));
     ErrorMsg("SMK ERROR: Can't open the SMK file");
   * \endcode
   */
  #define FastDebugMsg(msg)             _DebugMessage( (UINT8 *)(msg), (UINT32)(__LINE__), (UINT8 *)(__FILE__) )
  /*! \brief FastDebugMsg() Macro
   * Gives out a message - sometimes built from sprintf - completed by line number and source file name
   *
   * same as FastDebugMsg(), but only used in JA2 cinematics.cpp
   */
  #define ErrorMsg(msg)                 _DebugMessage( (UINT8 *)(msg), (UINT32)(__LINE__), (UINT8 *)(__FILE__) )


  /*! \brief RegisterDebugTopic() Macro
   *
   * The Debug System has the potential of only debugging 'registered' messages.
   * But it not used in the complex way it is designed.
   * - TOPIC_XXXXX: is defined in TopicIDs.h and some in TopicOps.h OBS: see 
   *      same description in DbgMessage(), too!
   * - srcid: a dereferencable integer with a number describing where the message 
   *         comes from. before the call, this number is not set.
   *         on return, the number was assigned dynamically
   * - msg: an ascii zero terminated C string
   * 
   * \bug this (srcid) asignment system could be (/have been) used to prefix every log 
   * message with the string of (msg) given here. But actually, the JA2 doesn't
   * use this. :-(
   * JA2 dynamically registers/deregisters a lot of TOPICs. But this is also
   * somehow out of function: DbgMessageReal() ignores the gfDebugTopics[] Array
   * currently and gives ALL messages, regardless of Registering.
   * MAYBE we reactivate this for the syslog-system...
   *
   * typical calls are:
   * \code
     RegisterDebugTopic(TOPIC_BUTTON_HANDLER,"Button System & Button Image Manager");
     RegisterDebugTopic(TOPIC_ORDLIST_CONTAINERS, "Ordered List Container");
   * \endcode
   */
  #define RegisterDebugTopic(srcid, msg)      DbgTopicRegistration( (UINT8)TOPIC_REGISTER, (UINT16 *)(&(srcid)), (CHAR8 *)(msg) )

  /*! \brief RegisterJA2DebugTopic() Macro (only another Name of the RegisterDebugTopic() function)
   *
   * see also: RegisterDebugTopic()
   *
   * typical calls are:
   * \code
     RegisterJA2DebugTopic( TOPIC_JA2OPPLIST, "Reg" );
     RegisterJA2DebugTopic( TOPIC_JA2, "Reg JA2 Debug" );
   * \endcode
   */
  #define RegisterJA2DebugTopic(srcid, msg)   DbgTopicRegistration( (UINT8)TOPIC_REGISTER, (UINT16 *)(&(srcid)), (CHAR8 *)(msg) )

  /*! \brief UnRegisterDebugTopic() Macro
   *
   * UnRegisterDebugTopic frees the former allocated Topic.
   */
  #define UnRegisterDebugTopic(a, b)    DbgTopicRegistration( (UINT8)TOPIC_UNREGISTER, (UINT16 *)(&(a)), (CHAR8 *)(b) )



  /*! \brief ClearAllDebugTopics() Macro
   *
   * Frees the whole debug System from Topics. 
   * it is only done once in InitializeGame() at start of the program
   * typical calls are:
   * \code
     ClearAllDebugTopics();
   * \endcode
   *
   * \todo the place where ClearAllDebugTopics() is called or ClearAllDebugTopics() itself 
   * is the best place for VSS_open() call.
   */
  #define ClearAllDebugTopics( )        DbgClearAllTopics( )


  /*! \brief undocumented yet
   *
   * a C++ template function to implement macros with different types
   */
  template <typename type1, typename type2>
  extern void _FailMessage(type1 pString, UINT32 uiLineNum, type2 pSourceFile );

  /*! \brief undocumented yet
   *
   * a C++ template function to implement macros with different types
   */
  template <typename type4>
  extern void DbgMessageReal(UINT16 uiTopicId, UINT8 uiCommand, UINT8 uiDebugLevel, type4 strMessage);

  /*! \brief undocumented yet
   *
   * a C style function to implement ordinary macros with well known types
   */
  extern void    _DebugMessage(UINT8 *pSourceFile, UINT32 uiLineNum, UINT8 *pString);
  /*! \brief undocumented yet
   *
   * a C style function to implement ordinary macros with well known types
   */
  extern void    DbgTopicRegistration( UINT8 ubCmd, UINT16 *usTopicID, CHAR8 *zMessage );
  /*! \brief undocumented yet
   *
   * a C style function to implement ordinary macros with well known types
   */
  extern void    DbgClearAllTopics( void );
  /*! \brief undocumented yet
   *
   * a C style function to implement ordinary macros with well known types
   */
  extern BOOLEAN DbgSetDebugLevel(UINT16 TopicId, UINT8 uiDebugLevel);
  /*! \brief undocumented yet
   *
   * a C style function to implement ordinary macros with well known types
   */
  extern void    DbgFailedAssertion( BOOLEAN fExpression, char *szFile, int nLine );

/*----------------------------------------------------------------------------------*/
#else /*defined( _DEBUG ) || defined( DEBUG )*/
/*----------------------------------------------------------------------------------*/

  #define Assert(a)                        ((void *)0)
  #define AssertMsg(a,b)                   ((void *)0)
  #define DebugBreakpoint()                ((void *)0)
  #define DbgMessage(a, b, c)              ((void *)0)
  #define FastDebugMsg(a)                  ((void *)0)
  #define ErrorMsg(a)                      ((void *)0)
  #define RegisterDebugTopic(a, b)         ((void *)0)
  #define RegisterJA2DebugTopic(a, b)      ((void *)0)
  #define UnRegisterDebugTopic(a, b)       ((void *)0)
  #define ClearAllDebugTopics( )           ((void *)0)

/*----------------------------------------------------------------------------------*/
#endif /*defined( _DEBUG ) || defined( DEBUG )*/
/*----------------------------------------------------------------------------------*/



#ifdef __cplusplus
   }
#endif


/* @} */


