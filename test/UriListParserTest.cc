#include "UriListParser.h"

#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>

#include <cppunit/extensions/HelperMacros.h>

#include "Exception.h"
#include "Util.h"
#include "prefs.h"
#include "OptionHandler.h"

namespace aria2 {

class UriListParserTest : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(UriListParserTest);
  CPPUNIT_TEST(testHasNext);
  CPPUNIT_TEST_SUITE_END();
private:
  std::string list2String(const std::deque<std::string>& src);
public:
  void setUp() {
  }

  void testHasNext();
};


CPPUNIT_TEST_SUITE_REGISTRATION( UriListParserTest );

std::string UriListParserTest::list2String(const std::deque<std::string>& src)
{
  std::ostringstream strm;
  std::copy(src.begin(), src.end(), std::ostream_iterator<std::string>(strm, " "));
  return util::trim(strm.str());
}

void UriListParserTest::testHasNext()
{
  std::ifstream in("filelist1.txt", std::ios::binary);

  UriListParser flp(in);

  std::deque<std::string> uris;
  Option reqOp;

  CPPUNIT_ASSERT(flp.hasNext());

  flp.parseNext(uris, reqOp);
  CPPUNIT_ASSERT_EQUAL
    (std::string("http://localhost/index.html http://localhost2/index.html"),
     list2String(uris));

  uris.clear();
  reqOp.clear();

  CPPUNIT_ASSERT(flp.hasNext());

  flp.parseNext(uris, reqOp);
  CPPUNIT_ASSERT_EQUAL(std::string("ftp://localhost/aria2.tar.bz2"),
		       list2String(uris));
  CPPUNIT_ASSERT_EQUAL(std::string("/tmp"), reqOp.get(PREF_DIR));
  CPPUNIT_ASSERT_EQUAL(std::string("chunky_chocolate"), reqOp.get(PREF_OUT));

  uris.clear();
  reqOp.clear();

  CPPUNIT_ASSERT(!flp.hasNext());

  flp.parseNext(uris, reqOp);
  CPPUNIT_ASSERT_EQUAL(std::string(""), list2String(uris));

  CPPUNIT_ASSERT(!flp.hasNext());
}

} // namespace aria2
