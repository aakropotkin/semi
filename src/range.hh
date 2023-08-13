/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#pragma once

#include <string>
#include <vector>

#include "comparator.hh"
#include "semver.hh"

/* -------------------------------------------------------------------------- */

namespace semi {

/* -------------------------------------------------------------------------- */

struct Range {

/* -------------------------------------------------------------------------- */

  public:

    /* Data Members */

    /** Sum Type with comparator and range */
    std::string raw;

    /** Range  */
    std::string range;

    /**
     * Version ranges are a collection of subexpressions containing version
     * constraints as their terms.
     * Boolean "and"/"or" statements are represented as nested lists, such that
     * the outer list is "or" statements, containing "and" statements,
     * containing constraints.
     *   "a || b || c && d || e" -> [[a], [b], [c, d], [e]]
     */
    std::vector<std::vector<Comparator>> set;

    /**
     * Normally "max version" ranges will prefer lower versions if higher
     * versioned candidates are tagged with a pre-release suffix.
     * This option allows pre-release versions to satisfy ranges even if they
     * don't explicitly opt into the use of pre-release specifiers.
     */
    bool includePrerelease;

    /**
     * Interpret version ranges loosely.
     * This allows non-compliant ranges to be normalized without errors.
     * Specifically this allows "00.01.002" to be read as "0.1.2"
     */
    bool loose;


/* -------------------------------------------------------------------------- */

    Range( std::string_view range
         , bool             includePrerelease = false
         , bool             loose             = false
         );

    Range( const Range & range
         ,       bool    includePrerelease = false
         ,       bool    loose             = false
         );

    Range( const Comparator & range
         ,       bool         includePrerelease = false
         ,       bool         loose             = false
         );


/* -------------------------------------------------------------------------- */

    /* Comparators */

    bool intersects( const Range & other ) const;

    bool test( std::string_view comp
             , bool             includePrerelease = false
             , bool             loose             = false
             ) const;

    bool test( const SemVer & semver
             ,       bool     includePrerelease = false
             ,       bool     loose             = false
             ) const;


/* -------------------------------------------------------------------------- */

    /* Serializers */

    const std::string & format();

    std::string toString() const;


/* -------------------------------------------------------------------------- */

  /* Helpers */

  private:

    void splitStatements();

    std::vector<Comparator> parseRange( std::string_view range );


/* -------------------------------------------------------------------------- */

};  /* End struct `Range' */


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
