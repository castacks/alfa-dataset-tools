/*    error.h - source for processing errors
 *
 *    Carnegie Mellon University
 *    Authors: Mohammadreza Mousaei
 *    Contact: mmousaei@andrew.cmu.edu
 *
 */

#include "error.h"

void showErr(Error *err) {
    if( err ) {
        std::cout << BOLDRED << "error - " << Error_Message[*err] << ENDC << std::endl;
    }
}

void showWarn(Error *err) {
    if( err ) {
        std::cout << BOLDYEL << "warning - " << Error_Message[*err] << ENDC << std::endl;
    }
}
