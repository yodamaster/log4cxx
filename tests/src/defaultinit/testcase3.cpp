/*
 * Copyright 2003,2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <log4cxx/logmanager.h>
#include <log4cxx/logger.h>
#include "../insertwide.h"

using namespace log4cxx;

class TestCase3 : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(TestCase3);
      CPPUNIT_TEST(testProperties);
   CPPUNIT_TEST_SUITE_END();

public:
   void setUp()
   {
   }

   void tearDown()
   {
      LogManager::shutdown();
   }

   void testProperties()
   {
      LoggerPtr root = Logger::getRootLogger();
     LOG4CXX_DEBUG(root, "Hello, world");
      bool rootIsConfigured = !root->getAllAppenders().empty();
      CPPUNIT_ASSERT(rootIsConfigured);

      AppenderList list = root->getAllAppenders();
      AppenderPtr appender = list.front();
      CPPUNIT_ASSERT_EQUAL((LogString) LOG4CXX_STR("D3"), appender->getName());
   }

};

CPPUNIT_NS::Test* createTestCase3() {
   return TestCase3::suite();
}
