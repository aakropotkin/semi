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

    std::string version;
    std::string raw;
    std::optional<unsigned int> major;
    std::optional<unsigned int> minor;
    std::optional<unsigned int> patch;
    std::vector<std::string> prerelease;
    std::vector<std::string> build;

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
     */
    bool loose;
    /**
     * Right to Left vs. Left to Right.
     * Causes rightmost versions to be used.
     * E.g. "4.2.0.1" => "2.0.1"
     */
    bool rtl;

    SemVer(
      const std::string version,
      bool includePrerelease = false,
      bool loose             = false,
      bool rtl               = false
    );

    std::string format();
    std::string toString();
    char compare( const SemVer & other );
    char compareMain( const SemVer & other );
    char comparePre( const SemVer & other );
    char compareBuild( const SemVer & other );

    SemVer inc( const std::string release, const std::string identifier );

  };


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
