#include "aopch.h"
#include "shell.h"

void shell::exec(const std::string& command)
{
    std::string full_command = command + "\r\nCMD_EXECUTION_COMPLETED\r\n";
    DWORD bytes_written;
    WriteFile(h_child_std_in_write, full_command.c_str(), full_command.length(), &bytes_written, NULL);

    char buffer[4096];
    DWORD bytes_read;
    std::string output;
    bool command_completed = false;

    while (!command_completed)
    {
        if (!PeekNamedPipe(h_child_std_out_read, NULL, 0, NULL, &bytes_read, NULL) || bytes_read == 0)
        {
            Sleep(100); // Wait a bit if no data is available
            continue;
        }

        if (ReadFile(h_child_std_out_read, buffer, sizeof(buffer) - 1, &bytes_read, NULL))
        {
            buffer[bytes_read] = 0;
            output += buffer;

            // Check if the command has completed
            if (output.find("CMD_EXECUTION_COMPLETED") != std::string::npos)
                command_completed = true;
        }
    }

    // Remove the command echo, the completion marker, and the prompts
    size_t start = output.find(command);
    if (start != std::string::npos)
    {
        start = output.find('\n', start);
        if (start != std::string::npos)
            output = output.substr(start + 1);
    }

    size_t end = output.find("CMD_EXECUTION_COMPLETED");
    if (end != std::string::npos)
        output = output.substr(0, end);

    // Trim trailing whitespace and newlines
    end = output.find_last_not_of("\r\n");
    if (end != std::string::npos)
        output = output.substr(0, end + 1);

    std::cout << output << std::endl;
}

void shell::close_handles()
{
    if (h_child_std_in_read)
        CloseHandle(h_child_std_in_read);

    if (h_child_std_in_write)
        CloseHandle(h_child_std_in_write);

    if (h_child_std_out_read)
        CloseHandle(h_child_std_out_read);

    if (h_child_std_out_write)
        CloseHandle(h_child_std_out_write);

    if (pi_proc_info.hProcess)
        CloseHandle(pi_proc_info.hProcess);

    if (pi_proc_info.hThread)
        CloseHandle(pi_proc_info.hThread);
}

shell::shell()
{
    SECURITY_ATTRIBUTES sa_attr;
    sa_attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa_attr.bInheritHandle = TRUE;
    sa_attr.lpSecurityDescriptor = NULL;

    CreatePipe(&h_child_std_out_read, &h_child_std_out_write, &sa_attr, 0);
    SetHandleInformation(h_child_std_out_read, HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&h_child_std_in_read, &h_child_std_in_write, &sa_attr, 0);
    SetHandleInformation(h_child_std_in_write, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA siStartInfo;
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFOA));
    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.hStdError = h_child_std_out_write;
    siStartInfo.hStdOutput = h_child_std_out_write;
    siStartInfo.hStdInput = h_child_std_in_read;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    std::string cmd_line = "cmd.exe /K @echo off";
    std::vector<char> cmd_line_buffer(cmd_line.begin(), cmd_line.end());
    cmd_line_buffer.push_back(0);

    CreateProcessA(NULL, cmd_line_buffer.data(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &siStartInfo, &pi_proc_info);

    // Clear initial output
    char buffer[4096];
    DWORD bytes_read;
    while (PeekNamedPipe(h_child_std_out_read, NULL, 0, NULL, &bytes_read, NULL) && bytes_read > 0)
        ReadFile(h_child_std_out_read, buffer, sizeof(buffer), &bytes_read, NULL);
}

shell::~shell()
{
    close_handles();
}
