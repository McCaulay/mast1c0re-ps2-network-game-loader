#include "Downloader.hpp"

bool Downloader::download(const char* filepath, const char* fileType, uint16_t port, uint16_t chunkSize)
{
    // Create TCP server
    PS::TcpServer server = PS::TcpServer();
    if (!server.listen(port))
        return false;

    // Waiting for file dialog
    char waitingMessage[256];
    PS2::sprintf(waitingMessage, "Waiting for %s file...", fileType);

    PS::Sce::MsgDialog::Initialize();
    PS::Sce::MsgDialogUserMessage waitingDialog = PS::Sce::MsgDialogUserMessage(waitingMessage, PS::Sce::MsgDialog::ButtonType::NONE);
    waitingDialog.open();

    // Accept connection
    PS::Debug.printf("Waiting for client connection...\n");
    PS::TcpClient client = server.accept();

    // File download variables
    size_t filesize = 0;
    size_t offset = 0;
    size_t headerSize = 0;

    // Check if file is sent with filesize
    uint8_t magic[sizeof(uint32_t)];
    client.read(magic, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    if (*(uint32_t*)(magic) == MAGIC)
    {
        // Get filesize
        filesize = client.read<size_t>();
        offset += sizeof(size_t);
        PS::Debug.printf("Download file of size: %llu\n", filesize);

        headerSize = sizeof(uint32_t) + sizeof(size_t);
    }

    // Close waiting for file dialog
    waitingDialog.close();
    PS::Sce::MsgDialog::Terminate();

    // Show progress bar dialog
    char downloadingMessage[256];
    PS2::sprintf(downloadingMessage, "Downloading %s file...", fileType);

    PS::Sce::MsgDialog::Initialize();
    PS::Sce::MsgDialogProgressBar progressDialog = PS::Sce::MsgDialogProgressBar(downloadingMessage);
    PS::Sce::MsgDialogUserMessage staticDialog = PS::Sce::MsgDialogUserMessage(downloadingMessage, PS::Sce::MsgDialog::ButtonType::NONE);
    if (filesize != 0)
    {
        progressDialog.open();
        Downloader::setProgress(progressDialog, 0, filesize);
    }
    else
        staticDialog.open();

    // Write file to device
    PS::Debug.printf("Opening %s\n", filepath);
    int fd = PS::open(filepath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd > 0)
    {
        // Write first 4 bytes if it was not the magic value
        if (filesize == 0)
            PS::writeAll(fd, magic, 4);

        // Write file to disk from socket in chunks
        uint32_t updateBar = 0;
        char buffer[MAX_DOWNLOAD_CHUNK_SIZE];
        while (true)
        {
            size_t readCount = client.read(buffer, chunkSize);
            offset += readCount;

            size_t writeCount = PS::writeAll(fd, buffer, readCount);
            if (writeCount != readCount)
            {
                PS::notification("Failed to write file to disk!");
                PS::Debug.printf("Failed to write file, wrote %llu, expected to write %llu\n", writeCount, readCount);
                PS::close(fd);
                client.disconnect();
                server.disconnect();
                if (filesize != 0)
                {
                    progressDialog.setValue(100);
                    progressDialog.close();
                }
                else
                    staticDialog.close();
                PS::Sce::MsgDialog::Terminate();
                return false;
            }

            // End of download
            if (readCount != chunkSize)
            {
                PS::Debug.printf("Downloaded %lu bytes\n", offset);
                break;
            }

            // Update progress bar
            if (updateBar == DOWNLOAD_BAR_UPDATE_FREQUENCY)
            {
                if (filesize != 0)
                    Downloader::setProgress(progressDialog, offset - headerSize, filesize);
                updateBar = 0;
            }
            updateBar++;
        }
    }

    // Disconnect
    PS::Debug.printf("Closing connection...\n");
    PS::close(fd);
    client.disconnect();
    server.disconnect();

    if (filesize != 0)
    {
        progressDialog.setValue(100);
        progressDialog.close();
    }
    else
        staticDialog.close();
    PS::Sce::MsgDialog::Terminate();

    return true;
}

void Downloader::setProgress(PS::Sce::MsgDialogProgressBar dialog, size_t downloaded, size_t total)
{
    if (total == 0)
        return;

    // Calculate percentage without float/double
    uint64_t divident = downloaded * 100;
    uint64_t percentage = 0;

    while (divident >= total)
    {
        divident -= total;
        percentage++;
    }

    dialog.setValue(percentage);
}
