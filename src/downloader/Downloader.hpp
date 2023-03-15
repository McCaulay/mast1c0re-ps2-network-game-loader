#pragma once

#include <types.hpp>
#include "mast1c0re.hpp"

#define MAGIC 0x0000EA6E
#define CONFIG_DOWNLOAD_CHUNK_SIZE 10
#define GAME_DOWNLOAD_CHUNK_SIZE 63488
#define DOWNLOAD_BAR_UPDATE_FREQUENCY 1500

class Downloader
{
public:
    static bool downloadGame(const char* gameFilepath, uint16_t port);
    static bool downloadConfig(const char* configFilepath, uint16_t port);
private:
    static void setProgress(PS::Sce::MsgDialogProgressBar dialog, size_t downloaded, size_t total);
};