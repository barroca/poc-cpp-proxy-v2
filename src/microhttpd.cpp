#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <boost/thread.hpp>
#include <boost/thread/tss.hpp>
#include <microhttpd.h>
#include "worker.hpp"

static boost::thread_specific_ptr<Worker> p_worker;

static int ahc_echo(void * _cls,
                    struct MHD_Connection * connection,
                    const char * url,
                    const char * method,
                    const char * version,
                    const char * upload_data,
                    size_t * upload_data_size,
                    void ** ptr) {
    if (p_worker.get() == NULL) {
        p_worker.reset(new Worker());
    }
    auto val = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, "q");
    struct MHD_Response * response;
    int ret;
    std::string result;
    if (val != NULL) {
        
        result = p_worker->work(val);

    }
    
    std::cout << "Searching for:" << val << std::endl;

    response = MHD_create_response_from_buffer(result.size(),
                                             (void*) result.c_str(),
                                             MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection,
                             MHD_HTTP_OK,
                             response);
    MHD_destroy_response(response);
    return ret;
}

int main(int argc, char ** argv) {
    struct MHD_Daemon * d;
    std::cout << "Init service" << std::endl;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        return 1;
    }
    
    
    
    d = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
                         atoi(argv[1]),
                         NULL,
                         NULL,
                         &ahc_echo,
                         NULL,
                         MHD_OPTION_THREAD_POOL_SIZE, 12,
                         MHD_OPTION_CONNECTION_LIMIT, 12,
                         MHD_OPTION_END);
    if (d == NULL) {
        return 1;
    }
    (void) getchar();
    MHD_stop_daemon(d);
    return 0;
}
