/*
Copyright 2018 giursino

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/
#include <CppUTest/TestHarness.h>
#include <iostream>
#include "KnxMessage.h"

TEST_GROUP(KnxMessage)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};


TEST(KnxMessage, addr)
{
    KnxAddr in(0x110F);
    CHECK(in.get_value()==0x110F);

    in.set_value(0xAABB);
    CHECK(in.get_value()==0xAABB);

    in.set_value(10);
    CHECK(in.get_value()==0x000A);

    in.set_value(0x1508);
    CHECK(in.get_main()==1);
    CHECK(in.get_sub()==5);
    CHECK(in.get_line()==8);

    KnxAddr knxaddr(1,5,10);
    CHECK(knxaddr.get_value()==0x150A);
    CHECK(knxaddr.get_main()==1);
    CHECK(knxaddr.get_sub()==5);
    CHECK(knxaddr.get_line()==10);
    CHECK(knxaddr.get_line()==0x0A);
}



TEST(KnxMessage, get_string_empty)
{
    KnxMessage in;

    CHECK(in.get_string() == "");
}

TEST(KnxMessage, get_string_not_empty)
{
    KnxMessage in({0x11, 0x22, 0x33});

    CHECK(in.get_string() == "0x11 0x22 0x33 ");
}

TEST(KnxMessage, get_string_not_hex)
{
    KnxMessage in({0, 1, 2, 10, 255});

    CHECK(in.get_string() == "0x00 0x01 0x02 0x0A 0xFF ");
}

TEST(KnxMessage, get_raw_empty)
{
    KnxMessage in;
    std::vector<std::uint8_t> out;

    CHECK(in.get_raw(out));
    CHECK(out.size()==0);
}

TEST(KnxMessage, get_raw_not_empty)
{
    KnxMessage in({0x11, 0x22, 0x33});
    std::vector<std::uint8_t> out;

    CHECK(in.get_raw(out));
    CHECK(out.size()==3);
    CHECK(out[0]==0x11);
    CHECK(out[1]==0x22);
    CHECK(out[2]==0x33);
}

TEST(KnxMessage, set_raw)
{
    KnxMessage in;
    std::vector<std::uint8_t> out;

    CHECK(in.get_raw(out));
    CHECK(out.size()==0);

    CHECK(in.set_raw({0x11, 0x22, 0x33}));
    CHECK(in.get_raw(out));
    CHECK(out.size()==3);
    CHECK(out[0]==0x11);
    CHECK(out[1]==0x22);
    CHECK(out[2]==0x33);

    out.push_back(0x44);
    CHECK(in.set_raw(out));
    CHECK(in.get_raw(out));
    CHECK(out.size()==4);
    CHECK(out[0]==0x11);
    CHECK(out[1]==0x22);
    CHECK(out[2]==0x33);
    CHECK(out[3]==0x44);
}

TEST(KnxMessage, get_src)
{
    KnxMessage in;
    KnxAddr out;

    CHECK(in.get_src(out)==false);

    in.set_raw({0xBC, 0x11, 0x0F, 0x0D, 0xB9, 0xE1, 0x00, 0x81});
    CHECK(in.get_src(out));
    CHECK(out.get_value()==0x110F);
}

TEST(KnxMessage, get_dest)
{
    KnxMessage in;
    KnxAddr out;

    CHECK(in.get_dest(out)==false);

    in.set_raw({0xBC, 0x11, 0x0F, 0x0D, 0xB9, 0xE1, 0x00, 0x81});
    CHECK(in.get_dest(out));
    CHECK(out.get_value()==0x0DB9);
}


