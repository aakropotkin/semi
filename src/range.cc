/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#include <regex>
#include <sstream>
#include <stdexcept>

#include "regexes.hh"
#include "range.hh"

/* -------------------------------------------------------------------------- */

namespace semi {

/* -------------------------------------------------------------------------- */

  Range::Range( const std::string range, bool includePrerelease, bool loose )
  {
    this->raw               = range;
    this->includePrerelease = includePrerelease;
    this->loose             = loose;
    this->set               = this->parseRange( range );
    this->range             = "";
    this->format();
  }


  Range::Range( const Comparator & range, bool includePrerelease, bool loose )
  {
    // FIXME
    this->includePrerelease = includePrerelease;
    this->loose             = loose;
    this->range             = "";
    this->format();
  }


  Range::Range( const Range & range, bool includePrerelease, bool loose )
  {
    // FIXME
    this->includePrerelease = includePrerelease;
    this->loose             = loose;
    this->range             = "";
    this->format();
  }


/* -------------------------------------------------------------------------- */

    std::vector<std::vector<std::string>>
  Range::parseRange( const std::string range )
  {
    return {};  // FIXME
  }


/* -------------------------------------------------------------------------- */

    /* Comparators */

    bool
  Range::intersects( const Range & other ) const
  {
    return false; // FIXME
  }


    bool
  Range::test(
    const std::string comp, bool includePrerelease, bool loose
  ) const
  {
    return false; // FIXME
  }


    bool
  Range::test( const SemVer & semver, bool includePrerelease, bool loose ) const
  {
    return false; // FIXME
  }


/* -------------------------------------------------------------------------- */

    std::string
  Range::format()
  {
    return this->range;
  }


    std::string
  Range::toString() const
  {
    return this->range;
  }


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
