/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#pragma once

#include <string>
#include <vector>
#include <optional>

/* -------------------------------------------------------------------------- */

namespace semi {

/* -------------------------------------------------------------------------- */

  struct SemVer {

/* -------------------------------------------------------------------------- */

    /* Data Members */

    std::string                 version;
    std::string                 raw;
    std::optional<unsigned int> major;
    std::optional<unsigned int> minor;
    std::optional<unsigned int> patch;
    std::vector<std::string>    prerelease;
    std::vector<std::string>    build;

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

    /**
     * Right to Left vs. Left to Right.
     * Causes rightmost versions to be used.
     * E.g. "4.2.0.1" => "2.0.1"
     */
    bool rtl;


/* -------------------------------------------------------------------------- */

    /* Constructors */

    /**
     * Construct a version from a string.
     * The provided string must not omit any parts.
     * To parse a version string with missing parts you must use
     */
    SemVer(
      const std::string version,
            bool        includePrerelease = false,
            bool        loose             = false,
            bool        rtl               = false
    );

    SemVer(
      std::optional<unsigned int> major      = std::nullopt,
      std::optional<unsigned int> minor      = std::nullopt,
      std::optional<unsigned int> patch      = std::nullopt,
      std::vector<std::string>    prerelease = {},
      std::vector<std::string>    build      = {}
    );


/* -------------------------------------------------------------------------- */

    /* Serializers */

    std::string format();
    std::string toString() const;


/* -------------------------------------------------------------------------- */

    /* Comparators */

    char compare( const SemVer & other ) const;
    char compareMain( const SemVer & other ) const;
    char comparePre( const SemVer & other ) const;
    char compareBuild( const SemVer & other ) const;


/* -------------------------------------------------------------------------- */

    /* Misc. */

    // TODO
    //SemVer inc( const std::string release, const std::string identifier );


/* -------------------------------------------------------------------------- */

  };  /* End struct `SemVer' */


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
