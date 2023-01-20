
/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#include <regex>
#include <sstream>

#include "comparator.hh"
#include "regexes.hh"
#include "semver.hh"
#include "range.hh"

/* -------------------------------------------------------------------------- */

namespace semi {

/* -------------------------------------------------------------------------- */

  static const SemVer ANY = SemVer();


/* -------------------------------------------------------------------------- */

  Comparator::Comparator(
    const std::string comp,
          bool        includePrerelease,
          bool        loose
  )
  {
    this->includePrerelease = includePrerelease;
    this->loose             = loose;
    parseComparator( comp );
    if ( this->semver.compare( ANY ) == 0 )
      {
        this->value = "";
      }
    else
      {
        this->value = this->op + this->semver.version;
      }
  }

  Comparator::Comparator(
    const std::string op,
          SemVer      semver,
          bool        includePrerelease,
          bool        loose
  )
  {
    this->op                = op;
    this->semver            = semver;
    this->includePrerelease = includePrerelease;
    this->loose             = loose;
    if ( this->semver.compare( ANY ) == 0 )
      {
        this->value = "";
      }
    else
      {
        this->value = this->op + this->semver.version;
      }
  }


/* -------------------------------------------------------------------------- */

    void
  Comparator::parseComparator( const std::string comp )
  {
    std::regex pattern(
      this->loose ? re::COMPARATORLOOSE : re::COMPARATOR, std::regex::ECMAScript
    );
    std::smatch match;
    if ( std::regex_match( comp, match, pattern ) )
      {
        if ( match[1].matched )
          {
            if ( match[1] == "=" )
              {
                this->op = "";
              }
            else
              {
                this->op = match[1];
              }
          }
        else
          {
            this->op = "";
          }

        if ( match[2].matched )
          {
            this->semver =
              SemVer( match[2], this->includePrerelease, this->loose );
          }
        else
          {
            this->semver = ANY;
          }
      }
    else
      {
        throw std::invalid_argument(
          "Invalid comparator version: '" + comp + "'"
        );
      }
  }


/* -------------------------------------------------------------------------- */

    /* Comparators */

    bool
  Comparator::intersects(
    const Comparator & other,
          bool         includePrerelease,
          bool         loose
  ) const
  {
    if ( this->op == "" )
      {
        if ( this->value == "" )
          {
            return true;
          }
        Range r = Range( other.value, includePrerelease, loose );
        return r.test( this->value );
      }
    else if ( other.op == "" )
      {
        if ( other.value == "" )
          {
            return true;
          }
        Range r = Range( this->value, includePrerelease, loose );
        return r.test( other.semver );
      }

    const bool sameDirectionIncreasing =
      ( ( this->op == ">=" ) || ( this->op == ">" ) ) &&
      ( ( other.op == ">=" ) || ( other.op == ">" ) );

    const bool sameDirectionDecreasing =
      ( ( this->op == "<=" ) || ( this->op == "<" ) ) &&
      ( ( other.op == "<=" ) || ( other.op == "<" ) );

    const bool sameSemVer = this->semver.version == other.semver.version;

    const bool differentDirectionsInclusive =
      ( ( this->op == ">=" ) || ( this->op == "<=" ) ) &&
      ( ( other.op == ">=" ) || ( other.op == "<=" ) );

    // const bool oppositeDirectionsLessThan = FIXME `cmp'
    // const bool oppositeDirectionsGreaterThan = FIXME `cmp'

    // FIXME `cmp'
    return
      sameDirectionIncreasing || sameDirectionDecreasing ||
      ( sameSemVer && differentDirectionsInclusive );
  }


    bool
  Comparator::test( const SemVer & version ) const
  {
    if ( ( this->semver.compare( ANY ) == 0 ) ||
         ( version.compare( ANY ) == 0 )
       )
      {
        return true;
      }
    // FIXME `cmp'
    return this->semver.compare( version ) == 0;
  }

    bool
  Comparator::test( const std::string version ) const
  {
    SemVer o = SemVer( version, this->includePrerelease, this->loose );
    return this->test( o );
  }


/* -------------------------------------------------------------------------- */

    /* Serializers */

    std::string
  Comparator::toString() const
  {
    return this->value;
  }


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
