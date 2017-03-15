//
// Created by islam on 12.02.17.
//

#include <OS.h>
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>
#include <malloc.h>

namespace OS {
    namespace Files {
        FILE* open_file(const string_view& file_name, const string_view& mode) {
            FILE* file;
            _wfopen_s(&file, file_name, mode);      // FIXME: convert to wide char
            return file;
        }
        vector<string> list(const string_view& path, const std::experimental::string_view& filter) {
            vector<string> names;
            WIN32_FIND_DATA fd;
            HANDLE hFind = FindFirstFile(search_path + filter, &fd);     // FIXME: convert to wide char
            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    // read all (real) files in current folder
                    // , delete '!' read other 2 default folder . and ..
                    if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                        names.push_back(fd.cFileName);
                    }
                } while (::FindNextFile(hFind, &fd));
                ::FindClose(hFind);
            }
            return names;
        }
    }

    namespace Threading {
        unsigned int get_core_count() {
            SYSTEM_INFO sysinfo;
            GetSystemInfo(&sysinfo);
            return sysinfo.dwNumberOfProcessors;
        }

        void terminate_thread(std::thread& thread) {
            TerminateThread(thread.native_handle(), 2);
        }
    }

    namespace Time {
        long long count_from() {
            LARGE_INTEGER t;
            QueryPerformanceCounter(&t);
            return t.QuadPart;
        }

        long long count_to(long long t_from){
            LARGE_INTEGER t, frequency;
            QueryPerformanceCounter(&t);

            QueryPerformanceFrequency(&frequency);
            return (t.QuadPart - t_from) * 1000.0 / frequency.QuadPart;
        }
    }

    namespace Memory {
        void* aligned_alloc(size_t size, size_t alignment) {
            return _aligned_malloc(size, alignment);
        }

        void aligned_free(void* ptr) {
            _aligned_free(ptr);
        }
    }
}
