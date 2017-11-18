#include "kernel/calls.h"

int user_read_proc(struct process *proc, addr_t addr, void *buf, size_t count) {
    char *cbuf = (char *) buf;
    size_t i = 0;
    while (i < count) {
        if (!mem_read(proc->cpu.mem, addr + i, &cbuf[i]))
            return 1;
        i++;
    }
    return 0;
}

int user_read(addr_t addr, void *buf, size_t count) {
    return user_read_proc(current, addr, buf, count);
}

int user_write_proc(struct process *proc, addr_t addr, const void *buf, size_t count) {
    const char *cbuf = (const char *) buf;
    size_t i = 0;
    while (i < count) {
        if (!mem_write(proc->cpu.mem, addr + i, &cbuf[i]))
            return 1;
        i++;
    }
    return 0;
}

int user_write(addr_t addr, const void *buf, size_t count) {
    return user_write_proc(current, addr, buf, count);
}

int user_read_string(addr_t addr, char *buf, size_t max) {
    if (addr == 0)
        return 1;
    size_t i = 0;
    while (i < max) {
        if (user_read(addr + i, &buf[i], sizeof(buf[i])))
            return 1;
        if (buf[i] == '\0')
            break;
        i++;
    }
    return 0;
}

int user_write_string(addr_t addr, const char *buf) {
    if (addr == 0)
        return 1;
    size_t i = 0;
    while (buf[i] != '\0') {
        if (user_write(addr + i, &buf[i], sizeof(buf[i])))
            return 1;
        i++;
    }
    if (user_write(addr + i, &buf[i], sizeof(buf[i])))
        return 1;
    return 0;
}