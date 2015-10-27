/**
 * @file greeterappserver_test.cpp
 *
 * Project Clearwater - IMS in the Cloud
 * Copyright (C) 2015  Metaswitch Networks Ltd
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version, along with the "Special Exception" for use of
 * the program along with SSL, set forth below. This program is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * The author can be reached by email at clearwater@metaswitch.com or by
 * post at Metaswitch Networks Ltd, 100 Church St, Enfield EN2 6BQ, UK
 *
 * Special Exception
 * Metaswitch Networks Ltd  grants you permission to copy, modify,
 * propagate, and distribute a work formed by combining OpenSSL with The
 * Software, or a work derivative of such a combination, even if such
 * copying, modification, propagation, or distribution would otherwise
 * violate the terms of the GPL. You must comply with the GPL in all
 * respects for all of the code used other than OpenSSL.
 * "OpenSSL" means OpenSSL toolkit software distributed by the OpenSSL
 * Project and licensed under the OpenSSL Licenses, or a work based on such
 * software and licensed under the OpenSSL Licenses.
 * "OpenSSL Licenses" means the OpenSSL License and Original SSLeay License
 * under which the OpenSSL Project distributes the OpenSSL toolkit software,
 * as those licenses appear in the file LICENSE-OPENSSL.
 */

#include <string>
#include "gtest/gtest.h"

#include "siptest.hpp"
#include "mockappserver.hpp"
#include "greeterappserver.h"
#include "stack.h"

using namespace std;
using testing::InSequence;
using testing::Return;

/// Fixture for GreeterAppServerTest.
class GreeterAppServerTest : public SipTest
{
public:
  static void SetUpTestCase()
  {
    SipTest::SetUpTestCase();
    _helper = new MockAppServerTsxHelper();
  }

  static void TearDownTestCase()
  {
    delete _helper; _helper = NULL;
    SipTest::TearDownTestCase();
  }

  GreeterAppServerTest() : SipTest()
  {
  }

  ~GreeterAppServerTest()
  {
  }

  static MockAppServerTsxHelper* _helper;
};

MockAppServerTsxHelper* GreeterAppServerTest::_helper = NULL;

namespace GreeterAS
{
class Message
{
public:
  string _method;
  string _toscheme;
  string _status;
  string _from;
  string _fromdomain;
  string _to;
  string _todomain;
  string _route;
  string _extra;

  Message() :
    _method("INVITE"),
    _toscheme("sip"),
    _status("200 OK"),
    _from("6505551000"),
    _fromdomain("homedomain"),
    _to("6505551234"),
    _todomain("homedomain"),
    _route(""),
    _extra("")
  {
  }

  string get_request();
};
}

string GreeterAS::Message::get_request()
{
  char buf[16384];

  // The remote target.
  string target = string(_toscheme).append(":").append(_to);
  if (!_todomain.empty())
  {
    target.append("@").append(_todomain);
  }

  int n = snprintf(buf, sizeof(buf),
                   "%1$s %4$s SIP/2.0\r\n"
                   "Via: SIP/2.0/TCP 10.114.61.213;branch=z9hG4bK0123456789abcdef\r\n"
                   "From: Alice <sip:%2$s@%3$s>;tag=10.114.61.213+1+8c8b232a+5fb751cf\r\n"
                   "To: Bob <%4$s>\r\n"
                   "%5$s"
                   "%6$s"
                   "Max-Forwards: 68\r\n"
                   "Call-ID: 0gQAAC8WAAACBAAALxYAAAL8P3UbW8l4mT8YBkKGRKc5SOHaJ1gMRqsUOO4ohntC@10.114.61.213\r\n"
                   "CSeq: 16567 %1$s\r\n"
                   "User-Agent: Accession 2.0.0.0\r\n"
                   "Allow: PRACK, INVITE, ACK, BYE, CANCEL, UPDATE, SUBSCRIBE, NOTIFY, REFER, MESSAGE, OPTIONS\r\n"
                   "Content-Length: 0\r\n\r\n",
                   /*  1 */ _method.c_str(),
                   /*  2 */ _from.c_str(),
                   /*  3 */ _fromdomain.c_str(),
                   /*  4 */ target.c_str(),
                   /*  5 */ _route.empty() ? "" : string(_route).append("\r\n").c_str(),
                   /*  6 */ _extra.empty() ? "" : string(_extra).append("\r\n").c_str()
    );

  EXPECT_LT(n, (int)sizeof(buf));

  string ret(buf, n);
  return ret;
}

using GreeterAS::Message;

// Test creation and destruction of the GreeterAppServer objects
TEST_F(GreeterAppServerTest, CreateGreeterAppServer)
{
  // Create a GreeterAppServer object
  std::string home_domain = "home.domain";
  GreeterAppServer* mas = new GreeterAppServer("greeter");

  // Create the app server with an INVITE.
  Message msg;
  msg._method = "INVITE";
  pjsip_msg* req = parse_msg(msg.get_request());
  GreeterAppServerTsx*mast = (GreeterAppServerTsx*)mas->get_app_tsx(_helper, req);
  EXPECT_TRUE(mast != NULL);
  delete mast; mast = NULL;

  delete mas; mas = NULL;
}

// Test the mainline case for an incoming call
TEST_F(GreeterAppServerTest, MainlineIncomingTest)
{
  GreeterAppServerTsx as_tsx(_helper);
  Message msg;
  msg._method = "INVITE";
  pjsip_msg* req = parse_msg(msg.get_request());
  {
    InSequence seq;
    EXPECT_CALL(*_helper, get_pool(req)).WillOnce(Return(stack_data.pool));
    EXPECT_CALL(*_helper, send_request(req)).WillOnce(Return(0));
  }
  as_tsx.on_initial_request(req);
}
