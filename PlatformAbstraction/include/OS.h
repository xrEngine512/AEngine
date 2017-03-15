//
// Created by islam on 12.02.17.
//

#pragma once

#include <experimental/string_view>
#include <string>
#include <vector>
#include <fstream>

namespace std {
    class thread;
}

namespace OS {
    namespace Files {
        FILE* open_file(const std::experimental::string_view& file_name, const std::experimental::string_view& mode);
        std::vector<std::string> list(const std::experimental::string_view& path, const std::experimental::string_view& filter);
    }
    namespace Threading {
        unsigned int get_core_count();
        void terminate_thread(std::thread&);
    }
    namespace Time {
        long long count_from();
        long long count_to(long long);
    }
    namespace Memory {
        void* aligned_alloc(size_t, size_t);
        void aligned_free(void*);
    }
}
