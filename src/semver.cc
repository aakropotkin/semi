/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#include "semver.hh"
#include "regexes.hh"

namespace semi {

/* -------------------------------------------------------------------------- */

  static std::string
vptos( std::optional<unsigned int> & i )
{
  char buf[256];
  if ( i.has_value() )
    {
      std::snprintf( buf, 256, "%u", i.value() );
    }
  else
    {
      buf[0] = 'x';
      buf[1] = '\0';
    }
  return std::string( buf );
}


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
        if ( match[1].matched )
          {
            this->major = std::stol( match[1] );
          }
        else
          {
            this->major = std::nullopt;
          }

        if ( match[2].matched )
          {
            this->minor = std::stol( match[2] );
          }
        else
          {
            this->minor = std::nullopt;
          }

        if ( match[3].matched )
          {
            this->patch = std::stol( match[3] );
          }
        else
          {
            this->patch = std::nullopt;
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
          buf, 256, "%s.%s.%s",
          vptos( this->major ).c_str(),
          vptos( this->minor ).c_str(),
          vptos( this->patch ).c_str()
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
          buf, 256, "%s.%s.%s-%s",
          vptos( this->major ).c_str(),
          vptos( this->minor ).c_str(),
          vptos( this->patch ).c_str(),
          ss.str().c_str()
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

    if ( ! ( this->major.has_value() && o.major.has_value() ) )
      {
        return 0;
      }

    if ( this->major.value() != o.major.value() )
      {
        return this->major.value() - o.major.value();
      }

    if ( ! ( this->minor.has_value() && o.minor.has_value() ) )
      {
        return 0;
      }

    if ( this->minor.value() != o.minor.value() )
      {
        return this->minor.value() - o.minor.value();
      }

    if ( ! ( this->patch.has_value() && o.patch.has_value() ) )
      {
        return 0;
      }

    return this->patch.value() - o.patch.value();
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
