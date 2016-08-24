#ifndef USTR_PYSTR_H_
#define USTR_PYSTR_H_

#include <string>
#include <vector>

namespace ustr{
typedef std::string::size_type size_type;
const size_type npos = std::string::npos;

/* methods that return a modified string */
/* return with the first Upcase and other lowercase */
inline std::string capitalize (const std::string &);
/* return a string that is centerelized with the given fillchar */
inline std::string center (const std::string &, size_type width, const char fillchar = ' ');
/* return a string that is left(right) justed */
inline std::string ljust (const std::string &, size_type width, const char fillchar = ' ');
inline std::string rjust (const std::string &, size_type width, const char fillchar = ' ');

/* return a string with its tabs expand to the given tabsize */
inline std::string expandtabs (const std::string &, size_type tabsize = 8);

/* return a string with all of its characters lowered or uppered */
inline std::string lower (const std::string &);
inline std::string upper (const std::string &);
inline std::string swapcase (const std::string &);
/* return a string that the leading(tailing) characters are removed
 * on default the whitespaces are removed
 */
inline std::string lstrip (const std::string &, const std::string &chars = " ");
inline std::string rstrip (const std::string &, const std::string &chars = " ");
inline std::string strip (const std::string &, const std::string &chars = " ");
inline std::string replace (const std::string &, const std::string &old, const std::string &_new, size_type count = npos);
inline std::string zfill (const std::string &, size_type width);
/*methods that judges the string */

inline bool isalnum (const std::string &);
inline bool isalpha (const std::string &);
inline bool isdigit (const std::string &);
inline bool islower (const std::string &);
inline bool isspace (const std::string &);
inline bool istitle (const std::string &);
inline bool isupper (const std::string &);
inline bool endswith (const std::string &, const std::vector<std::string>&, size_type start = 0, size_type end = npos);
inline bool startswith (const std::string &, const std::vector<std::string>&, size_type start = 0, size_type end = npos);

/* methods for string search
 * generally return the index if success otherwise return std::string::npos
 */
inline size_type count (const std::string &, const std::string &, size_type start = 0, size_type end = npos);
inline size_type find (const std::string &, const std::string &, size_type start = 0, size_type end = npos);
inline size_type rfind (const std::string &, const std::string &, size_type start = 0, size_type end = npos);

/* string partition and join */
inline std::string join (const std::string &, const std::vector<std::string> &);
inline std::tuple<std::string, std::string, std::string>
partition (const std::string &, const std::string &);
inline std::tuple<std::string, std::string, std::string>
rpartition (const std::string &, const std::string &);
inline std::vector<std::string>
split (const std::string &, const std::string &seperator, int maxsplit = -1);
inline std::vector<std::string>
rsplit (const std::string &, const std::string &seperator, int maxsplit = -1);
inline std::vector<std::string>
splitlines (const std::string &, bool keepends = false);
}//namespace ustr
#endif
