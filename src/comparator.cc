/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#include <regex>
#include <sstream>
#include <stdexcept>

#include "comparator.hh"
#include "regexes.hh"
#include "semver.hh"
#include "range.hh"

/* -------------------------------------------------------------------------- */

namespace semi {

/* -------------------------------------------------------------------------- */

  static const SemVer ANY = SemVer();


/* -------------------------------------------------------------------------- */

    static bool
  cmp( const SemVer & a, std::string_view op, const SemVer & b, bool loose )
  {
    if ( op == "===" )
      {
        return a.version == b.version;
      }
    if ( op == "!==" )
      {
        return a.version != b.version;
      }

    const SemVer _a =
      a.loose == loose ? a : SemVer( a.version, a.includePrerelease, loose );
    const SemVer _b =
      a.loose == loose ? b : SemVer( b.version, b.includePrerelease, loose );

    if ( ( op == "" ) || ( op == "=" ) || ( op == "==" ) )
      {
        return _a.compare( _b ) == 0;
      }
    if ( op == "!=" )
      {
        return _a.compare( _b ) != 0;
      }
    if ( op == ">" )
      {
        return _a.compare( _b ) > 0;
      }
    if ( op == ">=" )
      {
        return _a.compare( _b ) >= 0;
      }
    if ( op == "<" )
      {
        return _a.compare( _b ) < 0;
      }
    if ( op == "<=" )
      {
        return _a.compare( _b ) <= 0;
      }

    throw std::invalid_argument(
      "Invalid operator: '" + std::string( op ) + "'"
    );
    /* Unreachable */
    return false;
  }


/* -------------------------------------------------------------------------- */

  Comparator::Comparator( std::string_view comp
                        , bool             includePrerelease
                        , bool             loose
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

  Comparator::Comparator( std::string_view   op
                        , SemVer           & semver
                        , bool               includePrerelease
                        , bool               loose
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
  Comparator::parseComparator( std::string_view comp )
  {
    const std::regex pattern(
      this->loose ? re::COMPARATORLOOSE : re::COMPARATOR, std::regex::ECMAScript
    );
    std::string _comp( comp );
    std::smatch match;
    if ( std::regex_match( _comp, match, pattern ) )
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
              SemVer( match[2].str(), this->includePrerelease, this->loose );
          }
        else
          {
            this->semver = ANY;
          }
      }
    else
      {
        throw std::invalid_argument(
          "Invalid comparator version: '" + _comp + "'"
        );
      }
  }


/* -------------------------------------------------------------------------- */

    /* Comparators */

    bool
  Comparator::intersects( const Comparator & other
                        ,       bool         includePrerelease
                        ,       bool         loose
                        ) const
  {
    if ( this->op == "" )
      {
        if ( this->value == "" )
          {
            return true;
          }
        const Range r = Range( other.value, includePrerelease, loose );
        return r.test( this->value );
      }
    else if ( other.op == "" )
      {
        if ( other.value == "" )
          {
            return true;
          }
        const Range r = Range( this->value, includePrerelease, loose );
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

    const bool oppositeDirectionsLessThan =
      cmp( this->semver, "<", other.semver, loose ) &&
      ( ( this->op == ">=" ) || ( this->op == ">" ) ) &&
      ( ( other.op == "<=" ) || ( other.op == "<" ) );

    const bool oppositeDirectionsGreaterThan =
      cmp( this->semver, ">", other.semver, loose ) &&
      ( ( this->op == "<=" ) || ( this->op == "<" ) ) &&
      ( ( other.op == ">=" ) || ( other.op == ">" ) );

    return
      sameDirectionIncreasing || sameDirectionDecreasing ||
      ( sameSemVer && differentDirectionsInclusive ) ||
      oppositeDirectionsLessThan || oppositeDirectionsGreaterThan;
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

    bool rsl = false;

    try {
      rsl = cmp( this->semver, this->op, version, this->loose );
    } catch( std::invalid_argument e ) {}

    return rsl;
  }

    bool
  Comparator::test( std::string_view version ) const
  {
    const SemVer o = SemVer( version, this->includePrerelease, this->loose );
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
