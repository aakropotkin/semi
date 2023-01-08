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
        if ( match[2].matched )
          {
            this->minor = std::stol( match[2] );
          }
        else
          {
            this->minor = 0;
          }
        if ( match[3].matched )
          {
            this->patch = std::stol( match[3] );
          }
        else
          {
            this->patch = 0;
          }

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

    std::string
  SemVer::toString()
  {
    return this->version;
  }


/* -------------------------------------------------------------------------- */

    char
  SemVer::compare( const SemVer & other )
  {
    SemVer o( other.version,
              this->includePrerelease,
              this->loose,
              this->rtl
            );
    if ( this->version == o.version )
      {
        return 0;
      }
    char c = this->compareMain( other );
    if ( c != 0 )
      {
        return c;
      }
    return this->comparePre( other );
  }


/* -------------------------------------------------------------------------- */

    char
  SemVer::compareMain( const SemVer & other )
  {
    SemVer o( other.version,
              this->includePrerelease,
              this->loose,
              this->rtl
            );
    if ( this->major != o.major )
      {
        return this->major - o.major;
      }
    if ( this->minor != o.minor )
      {
        return this->minor - o.minor;
      }
    return this->patch - o.patch;
  }


/* -------------------------------------------------------------------------- */

    char
  SemVer::comparePre( const SemVer & other )
  {
    SemVer o( other.version,
              this->includePrerelease,
              this->loose,
              this->rtl
            );
    if ( this->prerelease.empty() && o.prerelease.empty() )
      {
        return 0;
      }
    if ( this->prerelease.empty() && ( ! o.prerelease.empty() ) )
      {
        return -1;
      }
    if ( ( ! this->prerelease.empty() ) && o.prerelease.empty() )
      {
        return 1;
      }
    if ( this->prerelease < o.prerelease )
      {
        return -1;
      }
    if ( this->prerelease == o.prerelease )
      {
        return 0;
      }
    return 1;
  }


/* -------------------------------------------------------------------------- */

    char
  SemVer::compareBuild( const SemVer & other )
  {
    SemVer o( other.version,
              this->includePrerelease,
              this->loose,
              this->rtl
            );
    if ( this->build.empty() && o.build.empty() )
      {
        return 0;
      }
    if ( this->build.empty() && ( ! o.build.empty() ) )
      {
        return -1;
      }
    if ( ( ! this->build.empty() ) && o.build.empty() )
      {
        return 1;
      }
    if ( this->build < o.build )
      {
        return -1;
      }
    if ( this->build == o.build )
      {
        return 0;
      }
    return 1;
  }


/* -------------------------------------------------------------------------- */

  // FIXME
    SemVer
  SemVer::inc( const std::string release, const std::string identifier )
  {
    SemVer o( *this );
    return o;
  }


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
