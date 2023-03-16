#include <mast1c0re.hpp>
#include "downloader/Downloader.hpp"

#define SERVER_PORT 9045
#define DOWNLOAD_GAME_CHUNK_SIZE 0xF800
#define DOWNLOAD_CONFIG_CHUNK_SIZE 0x20

void main()
{
    // Set pad light to purple
    PS::PadSetLightBar(150, 0, 255, 255);

    // Show "PS2 Game Loader" notification
    PS::notificationWithIcon("cxml://psnotification/tex_morpheus_trophy_platinum", "PS2 Game Loader (Network)");

    // Attempt to connect to debug server
    // PS::Debug.connect(IP(192, 168, 0, 7), 9023);
    PS::Debug.printf("---------- Load PS2 Game (Network) ----------\n");

    // Set paths
    const char* gameFilepath = "/av_contents/content_tmp/disc01.iso";
    const char* configFilepath = "/av_contents/content_tmp/SCUS-97129_cli.conf";
    bool hasConfig = false;

    // Download ISO if doesn't exist
    if (!PS::Filesystem::exists(gameFilepath))
    {
        // Download ISO
        if (!Downloader::download(gameFilepath, "game", SERVER_PORT, DOWNLOAD_CONFIG_CHUNK_SIZE))
        {
            PS::notification("Failed to download ISO");
            PS::Debug.printf("Failed to download ISO\n");

            // Disconnect from debug server
            PS::Debug.disconnect();
            return;
        }
    }

    // Open a new dialog to ask for the config file
    if (PS::Sce::MsgDialogUserMessage::show("Do you want to load a config file?", PS::Sce::MsgDialog::ButtonType::YESNO))
    {
        if (!PS::Filesystem::exists(configFilepath))
        {
            // Download config
            if (!Downloader::download(configFilepath, "config", SERVER_PORT, DOWNLOAD_GAME_CHUNK_SIZE))
            {
                // Failed to download config
                PS::notification("Failed to download config");
                PS::Debug.printf("Failed to download config\n");

                // Disconnect from debug server
                PS::Debug.disconnect();
                return;
            }

            hasConfig = true;
        }
    }

    // Mount & Load iso
    PS::Debug.printf("Mounting...\n");
    PS::MountDiscWithFilepath("./../av_contents/content_tmp/disc01.iso");

    // Get game code from mounted file
    char* gameCode = PS::GetMountedGameCode();
    if (PS2::strlen(gameCode) == 10)
    {
        // Convert name from "SCUS-97129" -> "cdrom0:\\SCUS_971.29;1"
        char* ps2Path = PS2::gameCodeToPath(gameCode);

        // Load configuration file
        if (hasConfig)
        {   
            PS::Debug.printf("Processing config %s\n", configFilepath);
            PS::ProcessConfigFile("./../av_contents/content_tmp/SCUS-97129_cli.conf");
        }

        // Disconnect from debug server
        PS::Debug.printf("Loading \"%s\"...\n", ps2Path);

        // Disconnect from debug server
        PS::Debug.disconnect();

        // Restore corruption
        PS::Breakout::restore();

        // Execute mounted iso
        PS2::LoadExecPS2(ps2Path, 0, NULL);
        return;
    }

    PS::notification("Unexpected game code \"%s\"!", gameCode);
    PS::Debug.printf("Unexpected game code (%s) length of %i, expecting %i\n", gameCode, PS2::strlen(gameCode), 10);

    // Disconnect from debug server
    PS::Debug.disconnect();
}
