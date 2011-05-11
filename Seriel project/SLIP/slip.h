#ifndef SLIP_H
#define SLIP_H
/** \file slip.h*/
/** Implements SLIP(Seriel Line Internet Protocol) transfer over serial communication(RS-232).
 * 			For info: http://tools.ietf.org/html/rfc1055
 *
 *			Notes:
 * 			<ul>
 * 				<li> Can send a maximum of DATA_MAX characters at a time
 * 			</ul>
 *	\author NSRD
 * 	\date 29/04 -11
 */
 #include <ezV24/ezV24.h> // Used for serial connection

// Global constants
#define DATA_MAX 1050

#ifdef __cplusplus
    extern "C" {
#endif

// Functions
/**	\brief Makes connection over "/dev/ttyS1".
*	\pre Another serial connection is not open.(Remember to close after using this connection)
* 	\post A connection is made(Returns '1' else '0').
*/
int SLIPConnect();

/**	\brief Closes connection.
*	\pre A serial connection is open.
* 	\post The connection is closed.
*/
void SLIPClose();

/**	\brief Sends a package.
*	\pre A connection is open. Data contains minimum 'n' number of bytes. 'n' is less than or equal to DATA_MAX.
*	\post 'n' number of bytes from 'data' is sent.
*/
void sendPacket(char* data, size_t n);

/** \brief Receives a package.
*	\pre A connection is open. Data can contain atleast DATA_MAX.
*	\post 'data' now contains the data from the package and the number of bytes received is returned.(0 if error stream or timeout)
*/
size_t receivePackage(char data[]);

#ifdef __cplusplus
    }
#endif

#endif