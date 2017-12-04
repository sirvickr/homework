#if ! defined (ORB_SERVER_H)
#define ORB_SERVER_H 1

#define HAVE_CONFIG_H 1

//#define ORB_ACETAO 1

#if defined (HAVE_CONFIG_H)
#include "config.h"
#endif

#if defined (ORB_MICO)
/*-------------------------------------------------------------------
 * M I C O   2 . 3 . *
 */

#include <CORBA.h>
#include <mico/CosNaming.h>
#include "sca/fs.h"

#elif defined (ORB_ACETAO)
/*-------------------------------------------------------------------
 * A C E  +  T A O   1 . 3
 */

#include <tao/corba.h>
#include <orbsvcs/CosNamingC.h>
#include <tao/PortableServer/PortableServer.h>
#include "fsS.h"
#include <assert.h>

#elif defined (ORB_ORBIX2000)
/*-------------------------------------------------------------------
 * O R B I X   2 0 0 0   2 . 0
 */

#include <omg/orb.hh>
#include <omg/CosNaming.hh>
#include "sca/fs.hh"
#include "sca/fsS.hh"

#elif defined (ORB_VISIBROKER)
/*-------------------------------------------------------------------
 * V I S I B R O K E R   5 . 2
 */

#include <orb.h>
#include <CosNaming_c.hh>
#include <PortableServerExt_c.hh>
#include "sca/fs_s.hh"
#include "sca/fs_c.hh"

#endif /* ORB_VISIBROBER */

#endif /* ORB_SERVER_H */
