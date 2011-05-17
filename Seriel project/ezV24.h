/* -*- Mode: C -*-
 *
 * $Id: ezV24.h,v 1.7 2011/04/12 07:10:51 jdesch Exp $
 * -----------------------------------------------------------------------
 * Copyright  (c) 2001,02  Joerg Desch <jd@die-deschs.de>
 * -----------------------------------------------------------------------
 * PROJECT: ezV24 -- easy RS232/V24 access
 * MODULE.: EZV24.H:
 * AUTHOR.: Joerg Desch <jdesch@users.sourceforge.net>
 * -----------------------------------------------------------------------
 * DESCRIPTION:
 *
 * -----------------------------------------------------------------------
 * COMPILER-FLAGS:
 *
 * -----------------------------------------------------------------------
 */

/** @file ezV24.h
 *  @brief Reference of the ezV24 API
 *
 */

#ifndef __EZV24_H__
#define __EZV24_H__ 1


#include <stdlib.h>			     /* we need size_t */

#ifdef __cplusplus
extern "C" {
#endif



/*+=========================================================================+*/
/*|                      CONSTANT AND MACRO DEFINITIONS                     |*/
/*`========================================================================='*/

/*  common constant declarations
 */
#define V24_SZ_PORTNAME 25		     /* max. length of the port name */


/** \brief Error code of the library
 */
enum __EZV24_ERROR_CODES
{
    /// no errors, all went fine.
    V24_E_OK=0,
    /// `general' illegal parameter.
    V24_E_ILLPARM,
    /// illegal baudrate.
    V24_E_ILLBAUD,
    /// illegal datasize.
    V24_E_ILLDATASZ,
    /// illegal parity.
    V24_E_ILLPARITY,

    /** illegal handle. The parameter specifying the <em>port handle</em> is
     * bad. Make sure unused handles are set to <tt>NULL</tt>.
     */
    V24_E_ILLHANDLE,
    /// illegal timeout value.
    V24_E_ILLTIMEOUT,
    /// creation of the lock file failed.
    V24_E_CREATE_LOCK,
    /// lock file can't be opened.
    V24_E_OPEN_LOCK,
    /// unlink of the lock file failed.
    V24_E_KILL_LOCK,
    /// can't write to the lock file.
    V24_E_WRITE_LOCK,

    /** foreign lock file exist. This means that another process already has
     * locked the port.
     */
    V24_E_LOCK_EXIST,
    /// not enough memory.
    V24_E_NOMEM,
    /// pointer is <tt>NULL</tt>.
    V24_E_NULL_POINTER,
    /// <tt>open</tt> failed.
    V24_E_OPEN,
    /// <tt>read</tt> failed.
    V24_E_READ,
    /// <tt>write</tt> failed.
    V24_E_WRITE,
    /// library is not initialized.
    V24_E_NOT_INIT,

    /** no <tt>proc</tt> file system. We can't open the virtual file <tt>/proc/tty/...</tt>
     */
    V24_E_NO_PROC_FILE,
    /// function not implemented.
    V24_E_NOT_IMPLEMENTED,
    /// timeout waiting for data.
    V24_E_TIMEOUT,

    /** debugging. Error codes greater than this value are debug messages.
     */
    V24_E_DBG_MESSAGES=100,

    /** stale lock file overwritten. The PID stored in the lock file doesn't
     * exist, so we can overwrite it.
     */
    V24_E_DBG_STALE_LOCK
};

/** \brief Valid Baudrates
 * @see v24SetParameters
 */
enum __EZV24_BAUDRATE
{
    V24_B0=0,				///< used to drop DTR
    V24_B50,				///< 50 baud
    V24_B75,				///< 75 baud
    V24_B110,				///< 110 baud
    V24_B134,				///< 134.5 baud
    V24_B150,				///< 150 baud
    V24_B200,				///< 200 baud
    V24_B300,				///< 300 baud
    V24_B600,				///< 600 baud
    V24_B1200,				///< 1200 baud
    V24_B1800,				///< 1800 baud
    V24_B2400,				///< 2400 baud
    V24_B4800,				///< 4800 baud
    V24_B9600,				///< 9600 baud
    V24_B19200,				///< 19200 baud
    V24_B38400,				///< 38400 baud
    V24_B57600,				///< 57,600 baud
    V24_B115200,			///< 115,200 baud
    V24_NUM_BAUDRATES			///< the number of entries
};

/**  \brief Valid datasizes
 * @see v24SetParameters
 */
enum __EZV24_DATASIZE
{
    V24_5BIT=0,				     ///< 5 bits
    V24_6BIT,				     ///< 6 bits
    V24_7BIT,				     ///< 7 bits
    V24_8BIT,				     ///< 8 bits
    V24_NUM_DATASIZES			     ///< number of datasize values
};


/**  \brief Valid parity settings
 * @see v24SetParameters
 */
enum __EZV24_PARITY_FLAGS
{
    V24_NONE=0,				     ///< disable parity bit
    V24_EVEN,				     ///< even parity
    V24_ODD,				     ///< odd parity
    V24_IGNORE				     ///< use parity but do not test it
};


/**  \brief Valid open flags
 * @see v24OpenPort
 */
enum __EZV24_OPEN_FLAGS
{
    V24_STANDARD    = 0x0000,		     ///< just empty
    V24_LOCK        = 0x0001,		     ///< lock the port
    V24_NO_DELAY    = 0x0002,		     ///< no wait on DCD while opening the port
    V24_RTS_CTS     = 0x0004,		     ///< use hardware handshakes
    V24_XON_XOFF    = 0x0008,		     ///< use software handshakes
    V24_DROP_DTR    = 0x0010,		     ///< drop DTR on close the port
    V24_NON_BLOCK   = 0x0020,		     ///< non blocking read
    V24_SYNC        = 0x0040,                ///< synchronous writes, i.e. not using buffers
    V24_DEBUG_ON    = 0x8000		     ///< enable stderr messages
};


/**  \brief Valid status flags
 */
enum __EZV24_STATUS_FLAGS
{
    V24_DSR_LOW=0,                           ///< DSR state is down
    V24_DSR_HIGH,                            ///< DSR state is up
    V24_CTS_LOW,                             ///< CTS state is down
    V24_CTS_HIGH,                            ///< CTS state is up
    V24_DSRCTS_UNKNOWN                       ///< just in case the function returns abruptly
};



/*+=========================================================================+*/
/*|                             TYPEDECLARATIONS                            |*/
/*`========================================================================='*/

/** \brief internal data structure of the port handle.
 *
 * <b>Internal use only:</b> this structure shouldn't be used outside the library!
 */
struct __EZV24_PORT_HANDLE
{
	int fd;				     ///< the file handle
	int Errno;			     ///< error code of last op.
	int Locked;			     ///< Bool if port is locked
	int lock_fd;			     ///< handle of the lockfile or <tt>(-1)</tt>
	int Initialized;		     ///< Bool if port is configured
	char PortName[V24_SZ_PORTNAME+1];    ///< name of the port
	unsigned int OpenFlags;		     ///< current open flags
	int Baudrate;			     ///< current baudrate
	int Datasize;			     ///< current datasize
	int Parity;			     ///< and current parity
	int TimeoutValue;		     ///< timeout in 1/10 sec
};

typedef struct __EZV24_PORT_HANDLE v24_port_t;

// typedef void (*v24_error_handler_t)
//     (const v24_port_t *port, const int Errno, const char* caller);


/*+=========================================================================+*/
/*|                            PUBLIC VARIABLES                             |*/
/*`========================================================================='*/

/*+=========================================================================+*/
/*|                     PROTOTYPES OF GLOBAL FUNCTIONS                      |*/
/*`========================================================================='*/



/** Detect installed serial devices. The goal of this function is to detect
 * which serial devices are installed. Therefore the <tt>/proc</tt> file system is
 * used. The content of the virtual file <tt>/proc/tty/driver/serial</tt> must be
 * present. It must hold a specially formatted line for each device (see
 * below). A device is recognized as `installed' if this line doesn't contain
 * the word ``unknown'' but contain the ``tx:''.
 *
 * The line format of the virtual file should look like this:
 *
 * \verbatim
   0: uart:16550A port:3F8 irq:4 tx:0 rx:0
   1: uart:16550A port:2F8 irq:3 tx:0 rx:0
   2: uart:unknown port:3E8 irq:4
   \endverbatim
 *
 * The result of the detection is stored in a <tt>unsigned long</tt> variable. For
 * every port found, the corresponding bit is set. The above sample shows us
 * that port0 (<tt>ttyS0</tt>) and port1 (<tt>ttyS1</tt>) are present. So the bit0
 * and bit1 of the variable referenced by <tt>BitMask</tt> are set.
 *
 * If the function fails to detect the installed ports, a <tt>-1</tt> is returned. If
 * all went fine, the number of detected ports is returned.
 *
 * <b>Note:</b> this function can be used without an opened device!
 *
 * @param BitMask    pointer to a <tt>unsigned long</tt> variable.
 * @return (int) number of detected ports or <tt>-1</tt>.
 */
int v24CountPorts ( unsigned long* BitMask );


/** Build a valid port name. This function can be used to build the platform
 * dependent name of the serial device used by \ref v24OpenPort. The exact format
 * of the name is explained at \ref v24OpenPort.
 *
 * The parameter <tt>PortNo</tt> is the number of the port. The parameter <tt>PortName</tt>
 * is a pointer to a string buffer where the built name is stored.
 * <b>Important:</b> the caller must ensure that the string buffer
 * referenced by <tt>PortName</tt> can hold <tt>V24_SZ_PORTNAME</tt> characters.
 *
 * To ease the use, the function returns the pointer <tt>PortName</tt>.
 *
 * <b>Note:</b> this function can be used without an opened device!
 *
 * @param PortNo    number of the port.
 * @param PortName  resulting name for <tt>v24OpenPort</tt>.
 * @return (const char*) copy of the pointer <tt>PortName</tt>.
 */
const char* v24PortName ( int PortNo, char* PortName );



/** This function opens a serial device for reading and writing. A pointer to a
 * special <em>handle</em> is returned. This handle must be passed to all further
 * functions. If the function fails to open the port, a value of <tt>NULL</tt> is
 * returned to indicate the error.
 *
 * The portname is a string which represents the serial device. This
 * representation is platform dependent. The following table shows the naming
 * on several different operating systems. To avoid the usage of these names,
 * you can use \ref v24PortName to build the name for you.
 *
 * <table>
 *  <tr><th><b>System</b></th>  <th><b>port #1</b></th> <th><b>port #2</b></th> </tr>
 *  <tr><td>Linux               <td>/dev/ttyS0          <td>/dev/ttyS1  </tr>
 *  <tr><td>NetBSD              <td>/dev/tty00          <td>/dev/tty01  </tr>
 *  <tr><td>IRIX                <td>/dev/ttyf1          <td>/dev/ttyf2  </tr>
 *  <tr><td>HP-UX               <td>/dev/tty1p0         <td>/dev/tty2p0 </tr>
 *  <tr><td>Solaris/SunOS       <td>/dev/ttya           <td>/dev/ttyb   </tr>
 *  <tr><td>Digital UNIX        <td>/dev/tty01          <td>/dev/tty02  </tr>
 *  <tr><td>Cygwin              <td>/dev/ttyS0          <td>/dev/ttyS1  </tr>
 *  <tr><td>Windows             <td>com1:               <td>com2: </tr>
 * </table>
 *
 * The <em>open flags</em> are used to specify a special behaviour of the
 * library. The following flag can be OR'ed together.
 *
 * <dl>
 *   <dt><tt>V24_STANDARD</tt></dt>	<dd>just empty. The defaults are used.</dd>
 *   <dt><tt>V24_LOCK</tt></dt> 	<dd>lock the port. Therefore a lock file is
 *                              	created. After the port is closed, the lock
 *                              	file is removed automagically.</dd>
 *   <dt><tt>V24_NO_DELAY</tt></dt>	<dd>don't wait on DCD while opening the port.</dd>
 *   <dt><tt>V24_RTS_CTS</tt></dt> 	<dd>use RTS/CTS hardware handshake.</dd>
 *   <dt><tt>V24_XON_XOFF</tt></dt>	<dd>use Xon/Xoff software handshake.</dd>
 *   <dt><tt>V24_DROP_DTR</tt></dt>	<dd>drop DTR after closing the port.</dd>
 *   <dt><tt>V24_NON_BLOCK</tt></dt>	<dd>non blocking read. Using this flag, the read
 *                              	access is not delayed. Without this, read will
 *                              	wait until a character is received or the
 *                              	timeout time is reached. Note that in non
 *                              	blocking mode, there is no time error!</dd>
 *   <dt><tt>V24_SYNC</tt></dt> 	<dd>do all writes synchronously, i.e., do not use
 *                              	any buffering for writes.</dd>
 *   <dt><tt>V24_DEBUG_ON</tt></dt>	<dd>enable stderr messages.</dd>
 * </dl>
 *
 * Some more word about the lock files: the handling of the lock file
 * (hopefully) accords to the specification from
 * http://www.pathname.com/fhs/2.0/fhs-5.5.html.
 *
 * Device lock files, such as the serial device lock files that were originally
 * found in either <tt>/usr/spool/locks</tt> or <tt>/usr/spool/uucp</tt>, must now be stored
 * in <tt>/var/lock</tt>. The naming convention which must be used is <tt>LCK..</tt> followed
 * by the base name of the device.  For example, to lock <tt>/dev/ttyS0</tt> the file
 * <tt>LCK..ttyS0</tt> would be created.
 *
 * The format used for device lock files must be the HDB UUCP lock file
 * format. The HDB format is to store the process identifier (PID) as a ten
 * byte ASCII decimal number, with a trailing newline. For example, if process
 * 1230 holds a lock file, it would contain the eleven characters: space,
 * space, space, space, space, space, one, two, three, zero, and newline.
 *
 * @param PortName    the name of the device.
 * @param OpenFlags   the OR'ed <em>open flags</em>.
 * @return (v24_port_t*) pointer to a handle of the opened device.
 * @see v24ClosePort, v24SetParameters, v24SetTimeouts
 */
v24_port_t* v24OpenPort ( const char* PortName, unsigned int OpenFlags );


/** This function closes a previously opened device. If a lock file is used,
 * this file will be removed. As a result, the <tt>V24_*</tt> error code is
 * returned. Possible errors are <tt>V24_E_ILLHANDLE</tt> and <tt>V24_E_KILL_LOCK</tt>.
 * If all went fine, <tt>V24_E_OK</tt> is returned.
 *
 * @param port pointer to handle of the opened port.
 * @return (int) the <tt>V24_*</tt> error code.
 * @see v24OpenPort
 */
int v24ClosePort ( v24_port_t *port );


/** After a serial device is opened, the user have to setup the communication
 * parameter. The parameter <tt>port</tt> references the handle of the opened
 * device. With <tt>Baudrate</tt>, the speed can be set as bitrate (bits per
 * second). Use the <tt>__EZV24_BAUDRATE</tt> values as parameter. The size of a
 * transmitted data byte can be passed as <tt>__EZV24_DATASIZE</tt> value to
 * <tt>Datasize</tt>. The last parameter <tt>Parity</tt> defines the parity checking. The
 * possible values are defined in <tt>__EZV24_PARITY_FLAGS</tt>.
 *
 * The number of stop bits is set to <tt>1</tt>. If you need <tt>2</tt> stop bits, you have
 * to call \ref v24SetStopbits.
 *
 * The values returned are: <tt>V24_E_ILLHANDLE</tt>, <tt>V24_E_ILLBAUD</tt>,
 * <tt>V24_E_ILLDATASZ</tt>, <tt>V24_E_ILLPARITY</tt> and <tt>V24_E_OK</tt>.
 *
 * @param port pointer to handle of the opened port.
 * @param Baudrate a <tt>V24_*</tt> (\ref __EZV24_BAUDRATE) constant representing the <em>baudrate</em>.
 * @param Datasize a <tt>V24_*</tt> (\ref __EZV24_DATASIZE) constant representing the size of a data byte.
 * @param Parity a <tt>V24_*</tt> (\ref __EZV24_PARITY_FLAGS) constant representing the parity check mode.
 * @return (int) the <tt>V24_*</tt> error code.
 */
int v24SetParameters ( v24_port_t *port, int Baudrate, int Datasize, int Parity );


/** Setup the number of stop bits. This function should be called directly
 * after \ref v24SetParameters. The parameter <tt>Stops</tt> have to be set to the
 * correct number of stopbits. Valid values are <tt>1</tt> and <tt>2</tt>.
 *
 * The values returned are: <tt>V24_E_OK</tt>, <tt>V24_E_ILLHANDLE</tt>,
 * <tt>V24_E_ILLPARM</tt>,  <tt>V24_E_NOT_INIT</tt>.
 *
 * @param port   pointer to handle of the opened port.
 * @param Stops  the number of stop bits.
 * @return (int) the <tt>V24_*</tt> error code.
 * @see v24SetParameters
 */
int v24SetStopbits ( v24_port_t *port, int Stops );



/** All read functions may use a timeout mechanism while waiting for
 * characters. If this time limit is exceeded, the function abort reading. The
 * timeout mechanism is only active if the <em>open flag</em> <tt>V24_NON_BLOCK</tt>
 * isn't specified.
 *
 * The timeout duration is specified by the parameter <tt>TenthOfSeconds</tt>. This
 * value is given as a multiple of a tenth of a second.
 *
 * The values returned are: <tt>V24_E_OK</tt>,
 * <tt>V24_E_ILLHANDLE</tt>,
 * <tt>V24_E_ILLTIMEOUT</tt>,
 * <tt>V24_E_NOT_INIT</tt>.
 *
 * @param port             pointer to handle of the opened port.
 * @param TenthOfSeconds   the timeout duration.
 * @return (int) the <tt>V24_*</tt> error code.
 */
int v24SetTimeouts ( v24_port_t *port, int TenthOfSeconds );


/** The function tries to read a single character from the opened device. To do
 * this, the function \ref v24Read is used. If we have got some data, the
 * character is returned as integer value. The caller have to cast it to the
 * data type he need.
 *
 * In case of an error, the function returns <tt>-1</tt>. The caller has to use
 * \ref v24QueryErrno  to get the exact error code. Contrarily to \ref v24Read,
 * a timeout is reported as error!
 *
 * Possible error code are <tt>V24_OK</tt>, <tt>V24_E_NULL_POINTER</tt>,
 * <tt>V24_E_ILLHANDLE</tt>, <tt>V24_E_TIMEOUT</tt> or <tt>V24_READ</tt>.
 *
 * @param port pointer to handle of the opened port.
 * @return (int) the character read or <tt>-1</tt>.
 * @see v24QueryErrno, v24Read.
 */
int v24Getc ( v24_port_t *port );


/** This function simply sends one character. Nothing more and nothing less.
 *
 * The values returned are: <tt>V24_E_OK</tt>, <tt>V24_E_ILLHANDLE</tt>.
 *
 * @param port     pointer to handle of the opened port.
 * @param TheData  the character to be sent.
 * @return (int) the <tt>V24_*</tt> error code.
 */
int v24Putc ( v24_port_t *port, unsigned char TheData );


/** <tt>v24Read</tt> is the basic function to get one or more received data bytes out
 * of the receive queue. If the queue is empty, the behaviour of the function
 * depends on the used <em>open flags</em>. Without any open flags, the
 * function will wait for the reception of a character. The wait time is limited
 * to the given <em>timeout time</em>. If this limit is exceeded, the function
 * aborts waiting. If nothing is read, the returned value is <tt>0</tt>, and the error
 * code is set to <tt>V24_E_TIMEOUT</tt>. At this level it's no real error, so there
 * is no error report for the debug output (see <tt>V24_DEBUG_ON</tt> in
 * \ref v24OpenPort).
 *
 * If the flag <tt>V24_NON_BLOCK</tt> is used, a empty receive queue leads
 * to an error. The fuctions doesnt wait for the reception of characters.
 *
 * The parameter <tt>Buffer</tt> references a buffer that should hold the received
 * characters. The parameter <tt>Len</tt> is the number of characters to read.
 *
 * The function returns the number of character read. An error is indicated by
 * a return value of <tt>-1</tt>. Use \ref v24QueryErrno to get the exact error code.
 *
 * Possible error code are <tt>V24_OK</tt>, <tt>V24_E_NULL_POINTER</tt>,
 * <tt>V24_E_ILLHANDLE</tt>, <tt>V24_E_TIMEOUT</tt> or <tt>V24_READ</tt>.
 *
 * <b>Note:</b> the caller has to ensure that <tt>Buffer</tt> can hold the
 * <tt>Len</tt> characters.
 *
 * @param port    pointer to handle of the opened port.
 * @param Buffer  pointer to buffer to hold the data.
 * @param Len     number of characters to receive.
 * @return (int) number of received characters or <tt>-1</tt>.
 * @see v24QueryErrno, v24Getc, v24Gets.
 */
int v24Read ( v24_port_t *port, unsigned char* Buffer, size_t Len );


/** Send a buffer. This function sends all <tt>Len</tt> characters of the array
 * referenced by <tt>Buffer</tt>. The number of sent bytes is returned. If an error
 * occurs, a <tt>-1</tt> is returned. Use <tt>v24QueryErrno</tt> to retrieve
 * the error code.
 *
 * Possible error code are <tt>V24_OK</tt>, <tt>V24_E_NULL_POINTER</tt>,
 * <tt>V24_E_ILLHANDLE</tt> or <tt>V24_WRITE</tt>.
 *
 * @param port    pointer to handle of the opened port.
 * @param Buffer  pointer to buffer.
 * @param Len     number of characters to send.
 * @return  (int) number of sent characters or <tt>-1</tt>.
 * @see v24QueryErrno, v24Putc, v24Puts.
 */
int v24Write ( v24_port_t *port, const unsigned char* Buffer, size_t Len  );


/** Receive a string. Unlike <tt>v24Read</tt>, which tries to read a fixed number of
 * character, <tt>v24Gets</tt> read characters unless <tt>BuffSize</tt> characters are
 * fetched, or a <em>end of line</em> (<tt>\\n</tt>) is received. At the end of the game,
 * the number of characters read is returned. If an error occurs, a <tt>-1</tt> is
 * returned. The characters are stored in <tt>Buffer</tt>. The function ensures a
 * <tt>0</tt> terminated string!
 *
 * Possible error code are <tt>V24_OK</tt>, <tt>V24_E_NULL_POINTER</tt>,
 * <tt>V24_E_ILLPARM</tt>, <tt>V24_E_ILLHANDLE</tt> or <tt>V24_WRITE</tt>.
 * To get this error code, use \ref v24QueryErrno.
 *
 * A string should at least have 1 character and the terminating <tt>0</tt>. The
 * parameter <tt>BuffSize</tt> must therefore be greater than 1! If not, the operation
 * results in a <tt>V24_E_ILLPARM</tt> error. To read a single character, we have
 * \ref v24Getc.
 *
 * <b>Implementation note:</b> the current release of the library defines the
 * <em>end of a string</em> as a hard coded constant! Search the internal headers
 * <tt>EZV24_END_OF_STRING</tt>.
 *
 * @param port      pointer to handle of the opened port.
 * @param Buffer    pointer to buffer.
 * @param BuffSize  the maximum size of the buffer.
 * @return (int) number of fetched characters or <tt>-1</tt>.
 * @see v24QueryErrno, v24Getc, v24Read.
 */
int v24Gets ( v24_port_t *port, char* Buffer, size_t BuffSize );


/** Send a string. This function simply sends all characters of the ASCIIZ
 * string. A single <tt>\\n</tt> is not expanded to <tt>\\n\\r</tt>!
 * The function returns the number of characters sent, or a <tt>-1</tt> if
 * something fails. Use \ref v24QueryErrno to retrieve the error code.
 *
 * Possible error code are <tt>V24_OK</tt>, <tt>V24_E_NULL_POINTER</tt>,
 * <tt>V24_E_ILLHANDLE</tt> or <tt>V24_WRITE</tt>.
 *
 * <b>Implementation note:</b> the current release of the library defines the
 * <em>end of a string</em> as a hard coded constant! Search the internal headers
 * <tt>EZV24_END_OF_STRING</tt>.
 *
 * @param port    pointer to handle of the opened port.
 * @param Buffer  a pointer to an ASCIIZ string.
 * @return (int) number of characters sent.
 * @see v24QueryErrno, v24Putc, v24Write.
 */
int v24Puts ( v24_port_t *port, const char* Buffer );


/** If implemented by the operating system, this function returns the number of
 * character waiting in the receive-queue. If the function isn't available or
 * if an error happens, a <tt>-1</tt> is returned. Use \ref v24QueryErrno to
 * retrieve the error code.
 *
 * Possible error code are <tt>V24_OK</tt>, <tt>V24_E_NOT_IMPLEMENTED</tt> and
 * <tt>V24_E_ILLHANDLE</tt>.
 *
 * @param port pointer to handle of the opened port.
 * @return (int) number of character in the receive-queue.
 * @see v24QueryErrno.
 */
int v24HaveData ( v24_port_t *port );


/** All remaining characters in the receive-queue are removed.
 *
 * @param port pointer to handle of the opened port.
 * @return (int) the <tt>V24_*</tt> error code.
 */
int v24FlushRxQueue ( v24_port_t *port );


/** All remaining characters in the transmit-queue are removed.
 *
 * @param port pointer to handle of the opened port.
 * @return (int) the <tt>V24_*</tt> error code.
 */
int v24FlushTxQueue ( v24_port_t *port );


/** Set the state of the DTR line according to the parameter <tt>NewState</tt>. If
 * <tt>NewsState</tt> is <tt>0</tt> the DTR signal is unset, otherwise it is set.
 *
 * @param port pointer to handle of the opened port.
 * @param NewState the new state to set DTR to.
 * @return (int) the <tt>V24_*</tt> error code.
 */
int v24SetDTR ( v24_port_t *port, int NewState );


/** Set the state of the RTS line according to the parameter <tt>NewState</tt>. If
 * <tt>NewsState</tt> is <tt>0</tt> the RTS signal is unset, otherwise it is set.
 * This is only allowed if the port doesn't use <em>hardware handshaking</em>. This means
 * \ref v24OpenPort isn't called with <tt>V24_RTS_CTS</tt>. If hardware handshaking
 * is used while calling this function, it aborts and returns <tt>V24_E_ILLPARM</tt>
 * as error.
 *
 * The values returned are: <tt>V24_E_OK</tt>, <tt>V24_E_ILLHANDLE</tt>,
 * <tt>V24_E_ILLPARM</tt>, <tt>V24_E_NOT_IMPLEMENTED</tt>.
 *
 * @param port      pointer to handle of the opened port.
 * @param NewState  the new state to set DTR to.
 * @return (int) the <tt>V24_*</tt> error code.
 */
int v24SetRTS (v24_port_t *port, int NewState );


/** Get the state of the DSR line and store it on the parameter <tt>CurrState</tt>. If
 * <tt>CurrState</tt> is  <tt>0</tt> the DSR signal is unset, otherwise it is set.
 *
 * @param port pointer to handle of the opened port.
 * @param CurrState pointer to the current state of DSR (see <tt>V24_DSR_</tt> constants). If
 * there is a problem getting <tt>CurrState</tt>, it will be set to <tt>V24_DSRCTS_UNKNOWN</tt>.
 * @return (int) the <tt>V24_*</tt> error code.
 */
int v24GetDSR ( v24_port_t *port, int *CurrState );


/** Get the state of the CTS line and store it on the parameter <tt>CurrState</tt>. If
 * <tt>CurrState</tt> is <tt>0</tt> the CTS signal is unset, otherwise it is set. This will
 * only happen if the port uses <em>hardware handshaking</em>. This means
 * \ref v24OpenPort is called with <tt>V24_RTS_CTS</tt>. If no hardware handshaking
 * is used while calling this function, it aborts and returns <tt>V24_E_ILLPARM</tt>
 * as error.
 *
 * The values returned are: <tt>V24_E_OK</tt>, <tt>V24_E_ILLHANDLE</tt>,
 * <tt>V24_E_ILLPARM</tt>, <tt>V24_E_NOT_IMPLEMENTED</tt>.
 *
 * @param port pointer to handle of the opened port.
 * @param CurrState pointer to the current state of CTS (see <tt>V24_CTS_</tt>
 * constants). If there is a problem getting <tt>CurrState</tt>, it will be set to
 * <tt>V24_DSRCTS_UNKNOWN</tt>.
 * @return (int) the <tt>V24_*</tt> error code.
 */
int v24GetCTS (v24_port_t *port, int *CurrState );


/** This function returns the platform dependent name of the currently opened
 * device.
 *
 * @param port pointer to handle of the opened port.
 * @return (const char*) the name of the device.
 */
const char* v24QueryPortName ( v24_port_t *port );


/** In some cases it may be necessary to know the file handle returned by
 * the internal <tt>open</tt> call. To retrieve the current file handle for an
 * opened port, use this function.
 *
 * @param port pointer to handle of the opened port.
 * @return (int) internal used file handle.
 */
int v24QueryFileHandle ( v24_port_t *port );


/** If possible, all functions return the result of the operation as return
 * code. Some functions don't do this. Therefore, the last error code could be
 * retrieved with this function.
 *
 * For example: <tt>v24Read</tt> reports an error by returning a
 * <tt>-1</tt>. To get the exact error code, the application can call
 * \ref v24QueryErrno.
 *
 * @param port pointer to handle of the opened port.
 * @return the last error code as <tt>V24_*</tt> constant.
 */
int v24QueryErrno ( v24_port_t *port );



#ifdef __cplusplus
};
#endif

#endif
/* ==[End of file]========================================================== */
