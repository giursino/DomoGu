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

#ifndef KNXMESSAGE_H
#define KNXMESSAGE_H

#include <vector>
#include <cstdint>
#include <string>

class KnxAddr
{
public:
    KnxAddr(const int hex_addr): m_addr(hex_addr) {}

    KnxAddr(const int main, const int sub, const int line):
        m_addr (((main & 0x0F)<< (8+4)) +
               ((sub & 0x0F) << (8)) +
               (line & 0xFF)) {}

    void set_value(int hex_addr) {m_addr = hex_addr;}

    int get_value() {return m_addr;}

    int get_main() {return ((m_addr & 0xF000) >> (8+4));}
    int get_sub() {return ((m_addr & 0x0F00) >> (8));}
    int get_line() {return (m_addr & 0x00FF);}

private:
    std::uint16_t m_addr;
};


class KnxMessage
{
public:
    KnxMessage();
    KnxMessage(const std::vector<std::uint8_t> message);

    bool set_raw(const std::vector<std::uint8_t> message);
    bool get_raw(std::vector<std::uint8_t> &message) const;

    std::string get_string() const;

    int get_src() const;
    int get_dest() const;
    int get_npci() const;
    int get_npci_payload() const;
    int get_tpci() const;
    int get_tpci_payload() const;
    int get_apci() const;
    int get_apci_payload() const;

    bool set_ctrl_field(const std::uint8_t value);
    bool set_src(const KnxAddr addr);
    bool set_dest(const KnxAddr addr);
    bool set_npci(const std::uint8_t value);
    bool set_tpci(const std::uint8_t value);
    bool set_apci(const std::uint16_t value);
    bool set_apci_payload(const long long int value);

private:
    std::vector<std::uint8_t> m_message;

};

#endif // KNXMESSAGE_H
