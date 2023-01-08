/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#pragma once

#include <string>
#include <vector>

/* -------------------------------------------------------------------------- */

namespace semi {

/* -------------------------------------------------------------------------- */

  struct SemVer {

    std::string version;
    std::string raw;
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
    std::vector<std::string> prerelease;
    std::vector<std::string> build;
    bool includePrerelease;
    bool loose;
    bool rtl;  /* Right to Left vs. Left to Right */

    SemVer(
      const std::string version,
    /**
     * Normally "max version" ranges will prefer lower versions if higher
     * versioned candidates are tagged with a pre-release suffix.
     * This option allows pre-release versions to satisfy ranges even if they
     * don't explicitly opt into the use of pre-release specifiers.
     */
      bool includePrerelease = false,
    /**
     * Interpret version ranges loosely.
     * This allows non-compliant ranges to be normalized without errors.
     */
      bool loose = false,
    /**
     * Causes rightmost versions to be used.
     * E.g. "4.2.0.1" => "2.0.1"
     */
      bool rtl = false
    );

    std::string format();
    std::string toString();
    bool compare( const SemVer & other );
    bool compareMain( const SemVer & other );
    bool comparePre( const SemVer & other );
    bool compareBuild( const SemVer & other );

    SemVer inc( const std::string release, unsigned short identifier );

  };


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
