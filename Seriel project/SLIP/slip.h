#ifndef SLIP_H
#define SLIP_H
/** \file slip.h*/
/** \brief Implements SLIP(Seriel Line Internet Protocol) transfer over serial communication(RS-232).
 * 			Uses the standard: http://tools.ietf.org/html/rfc1055
 * 			Except: 
 * 			<ul>
 * 				<li> Uses one kind of byte for start and finish('A')
 * 				<li> Can send a maximum of DATA_MAX characters at a time(Can actually be more because of byte-stuffing)
 * 			</ul>
 *	\author NSRD
 * 	\date 27/04 -11
 */
 #include <ezV24/ezV24.h> // Used for serial connection

/** Global constants */
const DATA_MAX = 1050;

// Functions
/**	\brief Makes connection over "/dev/ttyS0";.
*	\pre Another serial connection is not open.(Remember to close after using this connection)
* 	\post A connection is made(Returns '1' else '0').
*/
int connect();
 
/**	\brief Closes connection.
*	\pre A serial connection is open.
* 	\post The connection is closed.
*/
void close();
 
/**	\brief Sends a package.
*	\pre A connection is open. Data contains minimum 'n' number of bytes. 'n' is less than or equal to DATA_MAX.	
*	\post 'n' number of bytes from 'data' is sent.
*/
void sendPacket(char* data, size_t n);
 
/** \brief Receives a package.
*	\pre A connection is open. Data can contain atleast DATA_MAX * 2.
*	\post 'data' now contains the data from the package and the number of bytes received is returned.(0 if error)
*/
size_t receivePackage(char data[]);

#endif