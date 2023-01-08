/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#include "semver.hh"
#include "regexes.hh"

namespace semi {

/* -------------------------------------------------------------------------- */

  SemVer::SemVer(
    const std::string version,
    bool includePrerelease,
    bool loose,
    bool rtl
  )
  {
    this->includePrerelease = includePrerelease;
    this->loose             = loose;
    this->rtl               = rtl;

    std::regex pattern( loose ? re::LOOSE : re::FULL, std::regex::ECMAScript );
    std::smatch match;
    if ( std::regex_match( version, match, pattern ) )
      {
        this->raw   = version;
        this->major = std::stol( match[1] );
        this->minor = std::stol( match[2] );
        this->patch = std::stol( match[3] );
        if ( match[4].matched )
          {
            std::istringstream iss( match[4] );
            std::string part;
            auto itt = std::back_inserter( this->prerelease );
            while ( std::getline( iss, part, '.' ) )
              {
                *itt++ = part;
              }
          }
        else
          {
            this->prerelease = {};
          }
        if ( match[5].matched )
          {
            std::istringstream iss( match[5] );
            std::string part;
            auto itt = std::back_inserter( this->build );
            while ( std::getline( iss, part, '.' ) )
              {
                *itt++ = part;
              }
          }
        else
          {
            this->build = {};
          }
      }
    else
      {
        throw std::invalid_argument(
          "Invalid semantic version: '" + version + "'"
        );
      }
    this->version = "";
    this->format();
  }


/* -------------------------------------------------------------------------- */

    std::string
  SemVer::format()
  {
    char buf[256];
    if ( this->prerelease.empty() )
      {
        std::snprintf(
          buf, 256, "%u.%u.%u", this->major, this->minor, this->patch
        );
      }
    else
      {
        std::stringstream ss;

        for (
          auto i = this->prerelease.cbegin();
          i != this->prerelease.cend();
          i++
        )
        {
          ss << "." << *i;
        }
        std::snprintf(
          buf, 256, "%u.%u.%u-%s",
          this->major, this->minor, this->patch, ss.str().c_str()
        );
      }
    this->version = buf;
    return this->version;
  }


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
