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

TEST(KnxMessage, dest_addr_type)
{
  KnxMessage in;

  CHECK(!in.is_destination_group_address());
  CHECK(!in.is_destination_broadcast_address());

  in.set_raw({0xB8, 0x10, 0xAA, 0x10, 0x01, 0x69, 0x03, 0xD7, 0x01, 0xC9, 0x40, 0x01, 0x04, 0x00, 0x00, 0x00, 0xDC});
  CHECK(!in.is_destination_group_address());
  CHECK(!in.is_destination_broadcast_address());

  in.set_raw({0xBC, 0x11, 0x0F, 0x0D, 0xB9, 0xE1, 0x00, 0x81});
  CHECK(in.is_destination_group_address());
  CHECK(!in.is_destination_broadcast_address());
}

TEST(KnxMessage, TP1_L2_frame)
{
    KnxMessage in;
    FrameType frame;
    Priority priority;
    std::uint8_t length;

    CHECK(!in.get_frame_type(frame));
    CHECK(!in.is_frame_repeated());
    CHECK(!in.get_priority(priority));
    CHECK(!in.get_length(length));

    in.set_raw({0x9C, 0x11, 0x0F, 0x0D, 0xB9, 0xE1, 0x00, 0x81});
    CHECK(!in.is_frame_repeated());
    CHECK(in.get_priority(priority));
    CHECK(priority==Priority::low);
    CHECK(in.get_length(length));
    CHECK(length==1);

    in.set_raw({0x98, 0x11, 0x0F, 0x0D, 0xB9, 0xE2, 0x00, 0x80, 0x01});
    CHECK(!in.is_frame_repeated());
    CHECK(in.get_priority(priority));
    CHECK(priority==Priority::urgent);
    CHECK(in.get_length(length));
    CHECK(length==2);

    in.set_raw({0x90, 0x11, 0x0F, 0x0D, 0xB9, 0xE3, 0x00, 0x80, 0x01, 0x02});
    CHECK(!in.is_frame_repeated());
    CHECK(in.get_priority(priority));
    CHECK(priority==Priority::system);
    CHECK(in.get_length(length));
    CHECK(length==3);

    in.set_raw({0x94, 0x11, 0x0F, 0x0D, 0xB9, 0xEF, 0x00, 0x81, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
    CHECK(!in.is_frame_repeated());
    CHECK(in.get_priority(priority));
    CHECK(priority==Priority::normal);
    CHECK(in.get_length(length));
    CHECK(length==15);


    in.set_raw({0xBC, 0x11, 0x0F, 0x0D, 0xB9, 0xE1, 0x00, 0x81});
    CHECK(in.is_frame_repeated());
    CHECK(in.get_frame_type(frame));
    CHECK(frame==FrameType::L_Data_Standard);
    CHECK(in.get_priority(priority));
    CHECK(priority==Priority::low);


    in.set_raw({0xCC});
    CHECK(!in.is_frame_repeated());
    CHECK(in.get_frame_type(frame));
    CHECK(frame==FrameType::ACK);
    CHECK(!in.get_priority(priority));
    CHECK(!in.get_length(length));

    in.set_raw({0x0C});
    CHECK(!in.is_frame_repeated());
    CHECK(in.get_frame_type(frame));
    CHECK(frame==FrameType::NAK);
    CHECK(!in.get_priority(priority));
    CHECK(!in.get_length(length));

    in.set_raw({0xC0});
    CHECK(!in.is_frame_repeated());
    CHECK(in.get_frame_type(frame));
    CHECK(frame==FrameType::BUSY);
    CHECK(!in.get_priority(priority));
    CHECK(!in.get_length(length));

    in.set_raw({0x00});
    CHECK(!in.is_frame_repeated());
    CHECK(in.get_frame_type(frame));
    CHECK(frame==FrameType::NAK_and_BUSY);
    CHECK(!in.get_priority(priority));
    CHECK(!in.get_length(length));
}

