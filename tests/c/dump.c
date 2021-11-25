#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

void dump(uint64_t x) {
    char buf[20];
    size_t buf_size = 1;
    buf[sizeof(buf) - buf_size] = '\n';
    do {
        buf[sizeof(buf) - buf_size - 1] = x % 10 + '0';
        buf_size++;
        x /= 10;
    } while (x);

    write(1, &buf[sizeof(buf) - buf_size], buf_size);
}

int main() {
    dump(12);
    return 0;
}
