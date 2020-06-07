#include "Common.h"
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
    int ed = mkfifo(CL_TO_SR_PIPE, 0666);

    if (ed == -1) {
        throw_error(2, "Impossivel criar FIFO do cliente para servidor");
        return 1;
    }

    ed = mkfifo(SR_TO_CL_PIPE, 0666);

    if (ed == -1) {
        throw_error(2, "Impossivel criar FIFO do servidor para o cliente");
        return 1;
    }

    return 0;
}
