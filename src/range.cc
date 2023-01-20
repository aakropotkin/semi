/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#include <regex>
#include <sstream>
#include <stdexcept>
#include <list>
#include <vector>

#include "comparator.hh"
#include "regexes.hh"
#include "range.hh"

/* -------------------------------------------------------------------------- */

namespace semi {

/* -------------------------------------------------------------------------- */

  /* Helpers */

    bool
  isNullSet( const Comparator & comp  )
  {
    return comp.value == "<0.0.0-0";
  }

    bool
  isAny( const Comparator & comp )
  {
    return comp.value == "";
  }


/* -------------------------------------------------------------------------- */

  Range::Range( const std::string range, bool includePrerelease, bool loose )
  {
    this->raw               = range;
    this->includePrerelease = includePrerelease;
    this->loose             = loose;

    static const std::regex delim( "\\|\\|" );
    std::vector<std::vector<Comparator>> statements;
    std::sregex_token_iterator iter( range.begin(), range.end(), delim, -1 );
    std::sregex_token_iterator end;
    for ( ; iter != end; ++iter )
      {
        std::vector<Comparator> statement = this->parseRange( *iter );
        if ( ! statement.empty() )
          {
            statements.emplace_back( statement );
          }
      }

    if ( statements.empty() )
      {
        throw std::invalid_argument( "Invalid SemVer Range: '" + range + "'" );
      }

    if ( 1 < statements.size() )
      {
        const auto f = statements[0];
        std::vector<std::vector<Comparator>> keep;
        std::copy_if(
          statements.begin(),
          statements.end(),
          std::back_inserter( keep ),
          []( const std::vector<Comparator> & s ) {
            return ! isNullSet( s[0] );
          }
        );
        if ( keep.empty() )
          {
            this->set = { f };
          }
        else if ( 1 < keep.size() )
          {
            for ( size_t i = 0; i < keep.size(); ++i )
              {
                if ( ( keep[i].size() == 1 ) && isAny( keep[i][0] ) )
                  {
                    keep = { keep[i] };
                    break;
                  }
              }
            this->set = keep;
          }
        else
          {
            this->set = keep;
          }
      }
    else
      {
        this->set = statements;
      }

    this->range = "";
    this->format();
  }


  Range::Range( const Range & range, bool includePrerelease, bool loose )
  {
    this->includePrerelease = includePrerelease;
    this->loose             = loose;
    this->raw               = range.raw;
    if ( ( range.loose == loose ) &&
         ( range.includePrerelease == includePrerelease )
       )
      {
        this->set   = range.set;
        this->range = range.range;
      }
    else
      {
        // TODO: this is a literal copy of the other constructor and we should
        // call it instead to DRY.
        static const std::regex delim( "\\|\\|" );
        std::vector<std::vector<Comparator>> statements;
        std::sregex_token_iterator iter(
          range.raw.begin(), range.raw.end(), delim, -1
        );
        std::sregex_token_iterator end;
        for ( ; iter != end; ++iter )
          {
            std::vector<Comparator> statement = this->parseRange( *iter );
            if ( ! statement.empty() )
              {
                  statements.emplace_back( statement );
              }
          }

        if ( statements.empty() )
          {
            throw std::invalid_argument(
              "Invalid SemVer Range: '" + range.raw + "'"
            );
          }

        if ( 1 < statements.size() )
          {
            const auto f = statements[0];
            std::vector<std::vector<Comparator>> keep;
            std::copy_if(
              statements.begin(),
              statements.end(),
              std::back_inserter( keep ),
              []( const std::vector<Comparator> & s ) {
                return ! isNullSet( s[0] );
              }
            );
            if ( keep.empty() )
              {
                this->set = { f };
              }
            else if ( 1 < keep.size() )
              {
                for ( size_t i = 0; i < keep.size(); ++i )
                  {
                    if ( ( keep[i].size() == 1 ) && isAny( keep[i][0] ) )
                      {
                        keep = { keep[i] };
                        break;
                      }
                  }
                this->set = keep;
              }
            else
              {
                this->set = keep;
              }
          }
        else
          {
            this->set = statements;
          }

        this->range = "";
        this->format();
      }
  }


  Range::Range( const Comparator & range, bool includePrerelease, bool loose )
  {
    this->includePrerelease = includePrerelease;
    this->loose             = loose;
    this->raw               = range.value;
    this->set               = { { range } };
    this->range             = "";
    this->format();
  }


/* -------------------------------------------------------------------------- */

    std::vector<Comparator>
  Range::parseRange( const std::string range )
  {
    return {};  // FIXME
  }


/* -------------------------------------------------------------------------- */

    /* Comparators */

    bool
  Range::intersects( const Range & other ) const
  {
    return false; // FIXME
  }


    bool
  Range::test(
    const std::string comp, bool includePrerelease, bool loose
  ) const
  {
    return false; // FIXME
  }


    bool
  Range::test( const SemVer & semver, bool includePrerelease, bool loose ) const
  {
    return false; // FIXME
  }


/* -------------------------------------------------------------------------- */

    std::string
  Range::format()
  {
    return this->range; // FIXME
  }


    std::string
  Range::toString() const
  {
    return this->range;
  }


/* -------------------------------------------------------------------------- */

}  /* End Namespace `semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
