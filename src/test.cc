/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#include "semver.hh"
#include <iostream>

using namespace semi;

/* -------------------------------------------------------------------------- */

  static bool
semver_class()
{
  return
    ( SemVer( "1.0.0" ).compare( SemVer( "2.0.0" ) ) < 0 )  &&
    ( 0 < SemVer( "2.0.0" ).compare( SemVer( "1.0.0" ) ) )  &&
    ( SemVer( "1.0.0" ).compare( SemVer( "1.0.0" ) ) == 0 ) &&
    ( SemVer( "1.0.0" ).toString() == "1.0.0" )
  ;
}


/* -------------------------------------------------------------------------- */

  int
main( int argc, char * argv[], char ** envp )
{
  if ( ! semver_class() )
    {
      return 1;
    }
  return 0;
}


/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
