/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#pragma once

#include <regex>


/* -------------------------------------------------------------------------- */

namespace semi {
  /**
   * Regular Expressions can be used for tokenizing, validating, and parsing
   * SemVer version strings.
   */
  namespace re {

/* -------------------------------------------------------------------------- */

/** A single "0", or a non-zero digit followed by zero or more digits. */
static const std::string NUMERICIDENTIFIER      = "0|[1-9]\\d*";
static const std::string NUMERICIDENTIFIERLOOSE = "[0-9]+";

/**
 * Zero or more digits, followed by a letter or hyphen, and then zero or
 * more letters, digits, or hyphens.
 */
static const std::string NONNUMERICIDENTIFIER =
  "\\d*[a-zA-Z-][a-zA-Z0-9-]*";

/** Three dot-separated numeric identifiers. */
static const std::string MAINVERSION =
  "(" + NUMERICIDENTIFIER + ")\\." +
  "(" + NUMERICIDENTIFIER + ")\\." +
  "(" + NUMERICIDENTIFIER + ")";

static const std::string MAINVERSIONLOOSE =
  "(" + NUMERICIDENTIFIERLOOSE + ")\\." +
  "(" + NUMERICIDENTIFIERLOOSE + ")\\." +
  "(" + NUMERICIDENTIFIERLOOSE + ")";

/** A numeric identifier, or a non-numeric identifier. */
static const std::string PRERELEASEIDENTIFIER =
  "(?:" + NUMERICIDENTIFIER + "|" + NONNUMERICIDENTIFIER + ")";

static const std::string PRERELEASEIDENTIFIERLOOSE =
  "(?:" + NUMERICIDENTIFIERLOOSE + "|" + NONNUMERICIDENTIFIER + ")";

/**
 * Hyphen, followed by one or more dot-separated pre-release
 * version identifiers.
 */
static const std::string PRERELEASE =
  "(?:-(" + PRERELEASEIDENTIFIER + "(?:\\." + PRERELEASEIDENTIFIER + ")*))";

static const std::string PRERELEASELOOSE =
  "(?:-?(" + PRERELEASEIDENTIFIERLOOSE +
  "(?:\\." + PRERELEASEIDENTIFIERLOOSE + ")*))";

/** Any combination of digits, letters, or hyphens. */
static const std::string BUILDIDENTIFIER = "[0-9A-Za-z-]+";

/**
 * Plus sign, followed by one or more period-separated build
 * metadata identifiers.
 */
static const std::string BUILD =
  "(?:\\+(" + BUILDIDENTIFIER + "(?:\\." + BUILDIDENTIFIER + ")*))";

/**
 * A main version, followed optionally by a pre-release version and
 * build metadata.
 *
 * Note that the only major, minor, patch, and pre-release sections of
 * the version string are capturing groups.  The build metadata is not a
 * capturing group, because it should not ever be used in version
 * comparison.
 */
static const std::string FULLPLAIN =
  "v?" + MAINVERSION + PRERELEASE + "?" + BUILD + "?";

static const std::string FULL = "^" + FULLPLAIN + "$";

// like full, but allows v1.2.3 and =1.2.3, which people do sometimes.
// also, 1.0.0alpha1 (prerelease without the hyphen) which is pretty
// common in the npm registry.
static const std::string LOOSEPLAIN =
  "[v=\\s]*" + MAINVERSIONLOOSE + PRERELEASELOOSE + "?" + BUILD + "?";

static const std::string LOOSE = "^" + LOOSEPLAIN + "$";

static const std::string GTLT = "((?:<|>)?=?)";

/**
 * Something like "2.*" or "1.2.x".
 * Note that "x.x" is a valid xRange identifer, meaning "any version"
 * Only the first item is strictly required.
 */
static const std::string XRANGEIDENTIFIERLOOSE =
  NUMERICIDENTIFIERLOOSE + "|x|X|\\*";
static const std::string XRANGEIDENTIFIER = NUMERICIDENTIFIER + "|x|X|\\*";

static const std::string XRANGEPLAIN =
  "[v=\\s]*(" + XRANGEIDENTIFIER + ")" +
  "(?:\\.(" + XRANGEIDENTIFIER + ")" +
  "(?:\\.(" + XRANGEIDENTIFIER + ")" +
  "(?:" + PRERELEASE + ")?" + BUILD + "?" + ")?)?";

static const std::string XRANGEPLAINLOOSE =
  "[v=\\s]*(" + XRANGEIDENTIFIERLOOSE + ")" +
  "(?:\\.(" + XRANGEIDENTIFIERLOOSE + ")" +
  "(?:\\.(" + XRANGEIDENTIFIERLOOSE + ")" +
  "(?:" + PRERELEASELOOSE + ")?" + BUILD + "?" + ")?)?";

static const std::string XRANGE = "^" + GTLT + "\\s*" + XRANGEPLAIN + "$";
static const std::string XRANGELOOSE =
  "^" + GTLT + "\\s*" + XRANGEPLAINLOOSE + "$";

/** Extract anything that could conceivably be a part of a valid semver */
static const std::string COERCE(
  "(^|[^\\d])(\\d{1,16})"
  "(?:\\.(\\d{1,16}))?"
  "(?:\\.(\\d{1,16}))?"
  "(?:$|[^\\d])"
);
static const std::string COERCERTL = COERCE; // XXX: `g' flag

/** Tilde ranges. Meaning is "reasonably at or greater than". */
static const std::string LONETILDE = "(?:~>?)";

static const std::string TILDETRIM =
  "(\\s*)" + LONETILDE + "\\s+";  // XXX: `g' flag
static const std::string tildeTrimReplace = "$1~";

static const std::string TILDE = "^" + LONETILDE + XRANGEPLAIN + "$";
static const std::string TILDELOOSE =
  "^" + LONETILDE + XRANGEPLAINLOOSE + "$";

/** Caret ranges. Meaning is "at least and backwards compatible with" */
static const std::string LONECARET = "(?:\\^)";

static const std::string CARETTRIM =
  "(\\s*)" + LONECARET + "\\s+";  // XXX: `g' flag
static const std::string caretTrimReplace = "$1^";

static const std::string CARET = "^" + LONECARET + XRANGEPLAIN + "$";
static const std::string CARETLOOSE =
  "^" + LONECARET + XRANGEPLAINLOOSE + "$";

/** A simple gt/lt/eq thing, or just "" to indicate "any version" */
static const std::string COMPARATORLOOSE =
  "^" + GTLT + "\\s*(" + LOOSEPLAIN + ")$|^$";
static const std::string COMPARATOR =
  "^" + GTLT + "\\s*(" + FULLPLAIN + ")$|^$";

/**
 * An expression to strip any whitespace between the gtlt and the thing
 * it modifies, so that "> 1.2.3" ==> ">1.2.3"
 */
static const std::string COMPARATORTRIM =  // XXX: `g' flag
  "(\\s*)" + GTLT + "\\s*(" + LOOSEPLAIN + "|" + XRANGEPLAIN + ")";
static const std::string comparatorTrimReplace = "$1$2$3";

/**
 * Something like "1.2.3 - 1.2.4"
 * Note that these all use the loose form, because they'll be checked against
 * either the strict or loose comparator form later.
 */
static const std::string HYPHENRANGE =
  "^\\s*(" + XRANGEPLAIN + ")" +
  "\\s+-\\s+" +
  "(" + XRANGEPLAIN + ")" +
  "\\s*$";

static const std::string HYPHENRANGELOOSE =
  "^\\s*(" + XRANGEPLAINLOOSE + ")" +
  "\\s+-\\s+" +
  "(" + XRANGEPLAINLOOSE + ")" +
  "\\s*$";

/** Star ranges basically just allow anything at all. */
static const std::string STAR = "(<|>)?=?\\s*\\*";

/** >=0.0.0 is like a star. */
static const std::string GTE0 = "^\\s*>=\\s*0\\.0\\.0\\s*$";
static const std::string GTE0PRE = "^\\s*>=\\s*0\\.0\\.0-0\\s*$";


/* -------------------------------------------------------------------------- */

  }  /* End Namespace "semi::re' */
}  /* End Namespace "semi' */

/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
