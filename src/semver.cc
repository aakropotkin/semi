/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#include <sstream>
#include <algorithm>

#include "semver.hh"
#include "regexes.hh"

namespace semi {

/* -------------------------------------------------------------------------- */

  /**
   * Stringize a version part ( major, minor, or patch ).
   * Parts with a value are stringized as unsigned integers, while nullopts
   * ( undeclared/unset optionals ) are rendered as 'x'.
   */
    static std::string
  vptos( const std::optional<unsigned int> & i )
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

    const std::regex pattern(
      loose ? re::LOOSE : re::FULL, std::regex::ECMAScript
    );
    std::smatch match;

    if ( std::regex_match( version, match, pattern ) )
      {
        this->raw = version;
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

  SemVer::SemVer(
    std::optional<unsigned int> major,
    std::optional<unsigned int> minor,
    std::optional<unsigned int> patch,
    std::vector<std::string>    prerelease,
    std::vector<std::string>    build
  )
  {
    this->major             = major;
    this->minor             = minor;
    this->patch             = patch;
    this->prerelease        = prerelease;
    this->build             = build;
    this->rtl               = false;
    this->loose             = false;
    this->includePrerelease = ! prerelease.empty();
    this->version           = "";
    this->format();
    this->raw = this->version;
  }


/* -------------------------------------------------------------------------- */

  /**
   * Convert a semantic version parts to a string, updating the `version'
   * member on this record.
   * Build information is always omitted.
   */
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
        /* Join prerelease parts with dots. */
        for (
          auto i = this->prerelease.cbegin();
          i != this->prerelease.cend();
          ++i
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

  /**
   * Note that this does not update the `version' member before returning it.
   * This means that if you set version parts manually on a record you need to
   * call `format' first to render those updates.
   */
    std::string
  SemVer::toString() const
  {
    return this->version;
  }


/* -------------------------------------------------------------------------- */

    char
  SemVer::compare( const SemVer & other ) const
  {
    const SemVer o( other.version,
                    this->includePrerelease,
                    this->loose,
                    this->rtl
                  );

    if ( this->version == o.version )
      {
        return 0;
      }

    const char c = this->compareMain( other );

    if ( c != 0 )
      {
        return c;
      }

    return this->comparePre( other );
  }


/* -------------------------------------------------------------------------- */

    char
  SemVer::compareMain( const SemVer & other ) const
  {
    const SemVer o( other.version,
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
  SemVer::comparePre( const SemVer & other ) const
  {
    const SemVer o( other.version,
                    this->includePrerelease,
                    this->loose,
                    this->rtl
                  );

    if ( this->prerelease.empty() && o.prerelease.empty() )
      {
        return 0;
      }

    /**
     * Having a prerelease identifier implies being "younger".
     * So, if we don't have one and the other does, we are older.
     */
    if ( this->prerelease.empty() )
      {
        return 1;
      }

    if ( o.prerelease.empty() )
      {
        return -1;
      }

    const size_t la  = this->prerelease.size();
    const size_t lo  = o.prerelease.size();
    const size_t len = std::min( la, lo );

    for ( size_t i = 0; i < len; i++ )
      {
        if ( this->prerelease[i] < o.prerelease[i] )
          {
            return -1;
          }
        if ( o.prerelease[i] < this->prerelease[i] )
          {
            return 1;
          }
      }

    return la - lo;
  }


/* -------------------------------------------------------------------------- */

    char
  SemVer::compareBuild( const SemVer & other ) const
  {
    const SemVer o( other.version,
                    this->includePrerelease,
                    this->loose,
                    this->rtl
                  );

    if ( this->build.empty() && o.build.empty() )
      {
        return 0;
      }

    if ( this->build.empty() )
      {
        return 1;
      }

    if ( o.build.empty() )
      {
        return -1;
      }

    const size_t la  = this->build.size();
    const size_t lo  = o.build.size();
    const size_t len = std::min( la, lo );

    for ( size_t i = 0; i < len; i++ )
      {
        if ( this->build[i] < o.build[i] )
          {
            return -1;
          }
        if ( o.build[i] < this->build[i] )
          {
            return 1;
          }
      }

    return la - lo;
  }


/* -------------------------------------------------------------------------- */

  // TODO
  //  SemVer
  //SemVer::inc( const std::string release, const std::string identifier )
  //{
  //  SemVer o( *this );
  //  return o;
  //}


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
