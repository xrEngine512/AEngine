//
// Created by islam on 12.02.17.
//

#include <OS.h>

#include <ApproxSystemErrors.h>
#include <StringUtils.h>

#include <zconf.h>
#include <thread>
#include <stdlib.h>
#include <dirent.h>

using namespace std;
using namespace experimental;

namespace OS {
    namespace Files {
        FILE* open_file(const string_view& file_name, const string_view& mode) {
            return fopen(file_name.data(), mode.data());
        }

        vector<string> list(const string_view &path, const string_view &filter) {
            vector<string> files;

            DIR *dp;
            dirent *dirp;
            if((dp  = opendir(path.data())) == NULL) {
                throw approx_exception(string("Не удалось открыть путь ") + path.data() + " для обхода файлов.");
            }

            while ((dirp = readdir(dp)) != NULL) {
                if (ends_with(dirp->d_name, filter.data()))
                    files.push_back(string(dirp->d_name));
            }
            closedir(dp);
            return files;
        }

    }

    namespace Threading {
        unsigned int get_core_count() {
            auto core_count = static_cast<unsigned int>(sysconf(_SC_NPROCESSORS_ONLN));
            return core_count ? core_count : 4;
        }

        void terminate_thread(std::thread& thread) {
            pthread_cancel(thread.native_handle());
        }
    }

    namespace Time {
        long long count_from() {
            timespec time;
            clock_gettime(CLOCK_REALTIME, &time);
            return (1000000000LL * time.tv_sec + time.tv_nsec) / 1000000;
        }

        long long count_to(long long start) {
            return count_from() - start;
        }
    }

    namespace Memory {
        void* aligned_alloc(size_t size, size_t alignment) {
            void* aligned_memory = nullptr;
            posix_memalign(&aligned_memory, alignment, size);
            return aligned_memory;
        }

        void aligned_free(void* ptr) {
            free(ptr);
        }
    }
}
