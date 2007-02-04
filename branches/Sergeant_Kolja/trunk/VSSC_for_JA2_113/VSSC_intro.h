/*! \file VSSC_intro.h
 *  \brief This is an overview of VSSC
 *
 *
 */


/*! \mainpage VSSC, a Very Simple Syslog Client
 * <br>
 * Purpose:<br>
 * Writing debug, informational, warning and error information into screen, 
 * file or network in a standardized manner.<br>
 * <br>
 * \author Sergeant Kolja (Initial Author)<br>
 * <br>
 * Additional Authors:<br>
 * - yet none<br>
 * <br>
 *
 * \note 
 * This documentation is currently not finished yet.<br>
 * <br>
 * Copyright: (c) 2006, SergeantKolja(at)yahoo.de (aka Modem-Man). 
 * The Copyright is hereby claimed for the whole Work.
 * The term 'Work' declares the sum of all files this package consist of, 
 * and means as well the source code, the documentation as well as the 
 * binary distributed files like .LIB or .DLL<br>
 * For more Information of the License and Copyright, please read far below.
 *
 * 
 * \attention This project is currently not mature. 
 * Please do not use it, except for improving it!<br>
 *
 *
 * \warning Do <b>not</b> distribute this Work, or parts of it 
 * without my file license.txt
 *
 *
 * \todo This Document shall also be generated in the following formats:
 *  - PDF
 *  - CHM
 *
 * regarding the PDF, I have currently no knowledge how to do it from DoxyGen 
 * under Windows OS. The CHM formatted document will comming soon.
 *
 *
 * \bug There are shurely a lot of bugs! If you found one, try to catch (fix) it. 
 * Any bug reports are highly appreciated!
 *
 *
 * \section intro_sec Introduction
 * 'syslog' is an alias for 'system logging' and has has the task to write 
 * debug, info, warning and critical informations to some collector which 
 * usually filters and displays this messages. Some collectors are also able 
 * to alert an administrator by mail, sound or SMS.<br>
 * <br>
 * WikiPedia says about syslog:<br>
 * Syslog was developed in the 1980s by Eric Allman as part of the Sendmail 
 * project, and was initially used solely for Sendmail. It proved so valuable, 
 * however, that other applications began using it as well. Syslog has since 
 * become the standard logging solution on Unix and Linux systems. [...] Until 
 * recently, Syslog functioned as a de facto standard, without any authoritative 
 * published specification, and many implementations existed [...]. In an effort 
 * to improve its security, the Internet Engineering Task Force implemented a 
 * working group. In 2001, the status quo was documented in RFC 3164
 * (http://tools.ietf.org/html/rfc3164).<br>
 * <br>
 * This project does NOT implement the collector but the sender. There are a lot 
 * of free and nice collectors out in the world. Each unix/linux machine has 
 * usually a running syslogd. On Windows, You can use f.i. "KiWisyslog" 
 * (http://www.kiwisyslog.com). This is a very neat and useful implementation. 
 * Please read theyr license before using. Another good usable Syslogd for
 * Win32 systems is 3Deamon from Cisco. Altough it is no longer under development 
 * and the initial developer seems to be vanished, it is still part of 
 * cisco/linksys WLAN router service tools and - IIRC, free for non-commercial use.
 * <br>
 * <br>
 *
 *
 * There are two parts behind todays syslog systems.<br>
 * - 1st) a proprietary, high speed Operating System call for writing log messages.
 * - 2nd) a public and well documented UDP/IP transport mechanism to transport 
 * that messages between different computer systems
 * <br>
 * since UDP ist able to be used locally too, the 1st part can be substituted by 
 * the 2nd, too. The only thing that must be done is giving 127.0.0.1 (localhost) 
 * as the destination address (and not to bind the collector on external IP address
 * only).
 * <br>
 * By doing this, everybody can implement syslog on a device with UDP plus localhost 
 * IP infrastructure. There aren't any system calls needed, the code will be less 
 * performant but also less complex and much more portable. 
 *
 * \section simple_sec Rough Usage
 * Using VSSC is simple: 
 * - link the LIB or C-Source with your programm
 * - call one of the open() functions
 * - call one of the write() functions or -macros as you wish
 * - watch the output at a collector/viewer
 * - call the close() function
 * 
 *
 * \section install_sec Installation
 *
 * \subsection srclink Linking VSSC.C on the developers machine
 * Three things must be done:<br>
 * - including at least 'vssc.h' file inside your source
 * - inserting 'vssc.c' into list of project files
 * - adding 'VSSC' define to Your global project definitions
 *
 * Copy VSSC.C and VSSC*.H onto Your system. You may want to put them into the same 
 * directories where other .C and .H files reside (this is not my cup of tea). 
 * This makes it very easy, because no extra path has to be specified.
 * You can also put hem directly into the source path of Your current project, 
 * or if You really like this project, You can copy it into a separate directory.<br>
 * <br>
 * However, there are two types of projects:<br>
 * - IDE (integrated develpment environment) or Studio controlled projects
 * - MAKEFILE controlled projects
 *
 *
 * \subsubsection srclinking_ide inside IDE / Studio
 * on most IDEs/Studios You have to add the *.c files in a separate (lefthanded) 
 * window. Add VSSC.C here, too. Now You have to add some statements like
 *  \code
    #include <VSSC.H>
 *  \endcode
 * <br>or<br>
 *  \code
    #include "VSSC.H"
 *  \endcode
 * <br>
 * to Your code. You can use the first form only if you added the path to any 
 * "global additional header search" dialogue.<br>
 * On MSVC projects as well as on other ones, there is often one global header 
 * file, named like stdafx.h, allincludes.h or so. Of course it is suggestet 
 * to put VSSC also here. But beware! if You are mixing C and C++, precompiled 
 * headers will not longer work. Switch them off or change them to stop before 
 * VSSC.<br>
 * Finally , You have to specify 'VSSC'. On most IDEs or Studios there is an 
 * dialogue for it. You'll probably recognize it, when searching for DEBUG, 
 * NODEBUG, __WIN32__ or so. Add VSSC to it.<br>
 * Done.<br>
 *
 * \subsubsection srclinking_make inside MAKEFILE
 * inside most MAKEFILES, You have a statement like 
 * \code
   INCLUDEPATH   = $(PROJECT_PATH);$(COMPILER)\INC
   GLOBALDEFINES = $(IF_DEBUG);USE_STLPORT;USE_WHATEVER=1;CONSOLEDEFAULT="\\\\.\\COM2:"
 * \endcode
 * add VSS, so You'll get somewhat like
 * \code
   INCLUDEPATH   = $(PROJECT_PATH);$(COMPILER)\INC;\opensource\vssc
   GLOBALDEFINES = $(IF_DEBUG);USE_STLPORT;USE_WHATEVER=1;CONSOLEDEFAULT="\\\\.\\COM2:";VSSC
 * \endcode
 * If not given in that Way, You have to search for '-I' for the Include and -D 
 * for the Defines.<br>
 * Now search the compiler call (often a list of other .c/.cpp files is given). 
 * Add VSSC.LIB after all other already given c/c++ files. Some MAKEFILES doing 
 * implicit rules, compiling all *.c inside a ruled path. If You added VSSC.C 
 * inside Your local source path, it is already okay, if it has its own 
 * directory, You may have to add a new rule for VSSCs path.<br>
 * The exact syntax depends on the MAKE You're using (MS, GNU, Borland, DMAKE).<br>
 * Now You have to add some statements like
 *  \code
    #include <VSSC.H>
 *  \endcode
 * <br>or<br>
 *  \code
    #include "VSSC.H"
 *  \endcode
 * <br>
 * to Your code. You can use the first form, because You added the path to Your -I 
 * derective.<br>
 * On MSVC projects as well as on other ones, there is often one global header file 
 * named like stdafx.h, allincludes.h or so. Of course it is suggestet to put VSSC 
 * also here. But beware! if You are mixing C and C++, precompiled headers will 
 * not longer work. Switch them off or change them to stop before VSSC.<br>
 * Done.<br>
 *
 *
 * \subsection liblink Linking with VSSC.LIB on the developers machine
 * Three things must be done:<br>
 * - including at least 'vssc.h' file inside your source
 * - inserting 'vssc.lib' into linker options
 * - adding 'VSSC' define to Your global project definitions
 *
 * Copy VSSC.LIB and VSSC*.H onto Your system. You may want to put them into 
 * the same directories where other .LIB and .H files reside (this is not my cup of tea).
 * This makes it very easy, because no extra path has to be specified.
 * You can also put hem directly into the source path of Your current project,
 * or, if You like this project, You can copy it into a separate directory.<br>
 * <br>
 * However, there are two types of projects:<br>
 * - IDE (integrated develpment environment) or Studio controlled projects
 * - MAKEFILE controlled projects
 *
 * \subsubsection liblinking_ide inside IDE / Studio
 * on most IDEs/Studios You have wheter to specify "additional global 
 * include/library search path" or You have to give the name of the lib 
 * including the full/relative path. You can leave the path empty, if the lib 
 * is in the local source path of Your own source as well as if You wrote the 
 * path to the LIB inside the earlier mentioned additional search for libs... thing.
 * In any way, you have to say inside some "Project Settings" dialogue that 
 * You'll use '[path\]VSSC.LIB'.<br><br>
 * Now You have to add some statements like
 *  \code
    #include <VSSC.H>
 *  \endcode
 * <br>or<br>
 *  \code
    #include "VSSC.H"
 *  \endcode
 * <br>
 * to Your code. You can use the first form only if you added the path to Your global additional header search.<br>
 * On MSVC projects as well as on other ones, there is often one global header file like stdafx.h, allincludes.h or so.
 * Of course it is suggestet to put VSSC also here. But beware! if You are mixing C and C++, precompiled headers will 
 * not longer work. Switch them off or change them to stop before VSSC.<br>
 * Finally , You have to specify 'VSSC'. On most IDEs or Studios there is an dialogue for it. You'll probably recognize
 * it, when searching for DEBUG, NODEBUG, __WIN32__ or so. Add VSSC to it.<br>
 * Done.<br>
 *
 * \subsubsection liblinking_make inside MAKEFILE
 * inside most MAKEFILES, You have a statement like 
 * \code
   INCLUDEPATH   = $(PROJECT_PATH);$(COMPILER)\INC
   LIBSEARCHPATH = $(PROJECT_PATH);$(COMPILER)\LIB
   GLOBALDEFINES = $(IF_DEBUG);USE_STLPORT;USE_WHATEVER=1;CONSOLEDEFAULT="\\\\.\\COM2:"
 * \endcode
 * add VSS, so You'll get somewhat like
 * \code
   INCLUDEPATH   = $(PROJECT_PATH);$(COMPILER)\INC;\opensource\vssc
   LIBSEARCHPATH = $(PROJECT_PATH);$(COMPILER)\LIB;\opensource\vssc
   GLOBALDEFINES = $(IF_DEBUG);USE_STLPORT;USE_WHATEVER=1;CONSOLEDEFAULT="\\\\.\\COM2:";VSSC
 * \endcode
 * If not given in that Way, You have to search for '-I' for the Include, '-L' for the Libraries and -D for the Defines.<br>
 * Now search the linker call (often a symbol LINK or a temporary response file is given). Add VSSC.LIB after all other already 
 * given LIB files. <br>
 * The exact syntax depends on the MAKE You're using (MS, GNU, Borland, DMAKE).<br>
 * Now You have to add some statements like
 *  \code
    #include <VSSC.H>
 *  \endcode
 * <br>or<br>
 *  \code
    #include "VSSC.H"
 *  \endcode
 * <br>
 * to Your code. You can use the first form, because You added the path to Your -I derective.<br>
 * On MSVC projects as well as on other ones, there is often one global header file like stdafx.h, allincludes.h or so.
 * Of course it is suggestet to put VSSC also here. But beware! if You are mixing C and C++, precompiled headers will 
 * not longer work. Switch them off or change them to stop before VSSC.<br>
 * Done.<br>
 *
 *
 * \subsection custompc at the customers machine
 * If You don't using a VSSC.DLL, there is nothing to do at the customers 
 * machine until You like to see Your Log messages.
 * At this point, it depends how You called VSSC_open(). Without arguments 
 * or with "localhost:514", you'll need a syslog relay or a syslog 
 * collector deamon & viewer, depending on <i>where</i> You want to see the 
 * messages. The relay will send them to a next computer, while the local 
 * deamon send it to a file and/or views it on the screen.<br>
 * Some days, You only want to see all Messages only while Co-Sessioning 
 * (VNC,RDP) - use a local collector + viewer plus set the Level
 * temporarily to a high value. All other days, You probably only want to be 
 * alerted if Your programm gone jerk. Install a local collector / deamon with 
 * alerting capabilities and set the Level inside Your program to 'only 
 * Critical + Errors'.<br>
 * At some hopefully not so far day, Your programm runs that perfect, You want 
 * to distribute it on thousands of other machines and do not want to get any 
 * alert from them. Thus, simply do not run any syslogd on that machines! You 
 * should additionally set the local Log Level to Zero (=Silent). Because You 
 * don't know if Your customer has alreday a running syslogd for other 
 * purposes ;-)
 * <br>
 *
 *
 * \subsection widechar_printf Character Set for the Messages
 * because syslog messages are related to TELNET Standard, all textual 
 * messages have to be represented as 8 bit 'netascii'. 'Netascii' means the 7 
 * least significant bits (characters 0x20 to 0x7F) of the Ascii charset, 
 * transported in 8-bit-octets. Thats why, we have only <b>char*</b> arguments 
 * and no <b>WCHAR*</b> functions at all.<br>
 * <br>
 * May be, we will extend this in the future for <b>WCHAR</B> and macros for 
 * <b>TCHAR</b> messages, too. It is also <b>possible</b> that I will decide 
 * to support MIME-quoted UTF-8 output for both char and WCHAR input in the 
 * future. But until that time, umlauts, accents and special characters 
 * outside of the 7 bit charset are  <b>prohibed</b>
 *
 *
 * \section license_copyright License and Copyright
 * Author: Sergeant Kolja (SergeantKolja(at)yahoo.de)<br>
 * Date  : first created on 01. November 2006<br>
 * <br>
 * License Model:<br>
 * This is not public domain but copyrighted work. Everybody can use and 
 * distribute it at no costs if he accepts the license agreement. If this 
 * Work is sometimes released to the public, (and while You read this text, 
 * it must have been released), it is protected by LGPL (Lesser Gnu Public 
 * License, see http://www.gnu.org/licenses/lgpl.html). If You want to use 
 * this Work, You have to agree the license, otherwise You have no 
 * permission to use this Work or parts of it.<br>
 * <br>
 * Only the Initial Author has the right to set any state of this project 
 * (additional) under another license. All releases of this project will 
 * (of course) keep staing under the license under which they were originally 
 * published. Only the one that was added to the new license is under both
 * licenses and its later improvements will be under the new license only.
 * <br>
 * <br>
 * Since this work was intensionally created to be used inside JA2.113, this 
 * work is licensed in a way that it can be distributed together with 
 * "Jagged Alliance (R)(TM) 2" Source Code (see: "SFI-SCLA" and "JA2 Open 
 * source temporary source exchange" in the File "JA2-source-license.txt" 
 * inside the JA2 Source packet).
 * Including this work, or parts of it, or derived material (source or binary) 
 * in some JA2 code does not put this work under the "SFI-SCLA" license. 
 * Both licenses will coexist and will protect their own code only.
 * <br>
 * <br>
 * 
 *
 * \remarks
 * Disclaimer:<br>
 * This work has been done in my sparetime, outside of my company and without 
 * any tools of my employer.<br>
 * This work has no relation (but the know how) to my professional written 
 * syslog routines. This Work also has no relation to my accidently also "VSSC" 
 * called, employee written, Very Simple Syslog Server (which should better 
 * be named 'VSSS')
 *
 *
 *
 */
