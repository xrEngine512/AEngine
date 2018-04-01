//
// Created by islam on 01.10.17.
//

#pragma once

#include <string>

#include <msgpack.hpp>

#include <OS.h>

namespace ASL {
    template <class T>
    void to_file_msg_pack(const std::string & file_name, const T & object) {
        msgpack::sbuffer buffer;

        msgpack::pack(buffer, object);

        auto file = OS::Files::open_file(file_name, "wb");

        fwrite(buffer.data(), buffer.size(), 1, file);

        fclose(file);
    }

    template <class T>
    void from_file_msg_pack(const std::string & file_name, T & object) {
        auto file = OS::Files::open_file(file_name, "rb");

        fseek(file, 0, SEEK_END);
        std::vector<char> buffer(static_cast<std::size_t>(ftell(file)));
        rewind(file);

        fread(buffer.data(), buffer.size(), 1, file);
        fclose(file);

        msgpack::unpack(buffer.data(), buffer.size()).get().convert(object);
    }
}
