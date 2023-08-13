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

  inline static bool
isNullSet( const Comparator & comp  )
{
  return comp.value == "<0.0.0-0";
}

  inline static bool
isAny( const Comparator & comp )
{
  return comp.value.empty();
}


/* -------------------------------------------------------------------------- */

  void
Range::splitStatements()
{
  static const std::regex delim( "\\s*\\|\\|\\s*" );

  std::vector<std::vector<Comparator>> statements;

  std::sregex_token_iterator iter( this->raw.begin()
                                 , this->raw.end()
                                 , delim, -1
                                 );
  std::sregex_token_iterator end;

  for ( ; iter != end; ++iter )
    {
      std::vector<Comparator> statement = this->parseRange( iter->str() );
      if ( ! statement.empty() )
        {
          statements.emplace_back( std::move( statement ) );
        }
    }

  if ( statements.empty() )
    {
      throw std::invalid_argument(
        "Invalid SemVer Range: '" + this->raw + "'"
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

}


/* -------------------------------------------------------------------------- */

Range::Range( std::string_view range, bool includePrerelease, bool loose )
  : raw( range )
  , range()
  , set()
  , includePrerelease( includePrerelease )
  , loose( loose )
{
  /* Split range string into "statements" ( sub-ranges ) */
  this->splitStatements();
  /* Set `this->range' */
  this->format();
}


Range::Range( const Range & range, bool includePrerelease, bool loose )
  : raw( range.raw )
  , range()
  , set()
  , includePrerelease( includePrerelease )
  , loose( loose )
{
  if ( ( range.loose == loose ) &&
       ( range.includePrerelease == includePrerelease )
     )
    {
      this->set   = range.set;
      this->range = range.range;
    }
  else
    {
      /* Split range string into "statements" ( sub-ranges ) */
      this->splitStatements();
      /* Set `this->range' */
      this->format();
    }
}


Range::Range( const Comparator & range, bool includePrerelease, bool loose )
  : raw( range.value )
  , range()
  , set( { { range } } )
  , includePrerelease( includePrerelease )
  , loose( loose )
{
  /* Set `this->range' */
  this->format();
}


/* -------------------------------------------------------------------------- */

/**
 * Parse a range part with no `||` delimiters.
 * @param range The range part to parse.
 * @return A collection of parsed Comparators logically delimited by `&&`s.
 */
  std::vector<Comparator>
Range::parseRange( std::string_view range )
{
  const std::regex hyphen_re(
    this->loose ? re::HYPHENRANGELOOSE : re::HYPHENRANGE
  );
  /* `1.2.3 - 1.2.4` => `>=1.2.3 <=1.2.4` */
  // FIXME: they use regex replacement but that's kind of wasteful.
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
Range::test( std::string_view comp, bool includePrerelease, bool loose ) const
{
  return false; // FIXME
}


  bool
Range::test( const SemVer & semver, bool includePrerelease, bool loose ) const
{
  return false; // FIXME
}


/* -------------------------------------------------------------------------- */

  const std::string &
Range::format()
{
  std::stringstream ss;
  /* Join "||" and "&&" statements */
  for ( auto i = this->set.cbegin(); i != this->set.cend(); )
    {
      for ( auto j = i->cbegin(); j != i->cend(); )
        {
          ss << j->toString();
          if ( ( ++j ) != i->cend() )
            {
              ss << " ";
            }
        }
      if ( ( ++i ) != this->set.cend() )
        {
          ss << "||";
        }
    }
  this->range = ss.str();
  return this->range;
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
