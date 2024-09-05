#pragma once

class shell
{
public:
    shell();
    ~shell();
    void exec(const std::string& command);

private:
    HANDLE h_child_std_in_read = NULL;
    HANDLE h_child_std_in_write = NULL;
    HANDLE h_child_std_out_read = NULL;
    HANDLE h_child_std_out_write = NULL;
    PROCESS_INFORMATION pi_proc_info;

    void close_handles();
};
