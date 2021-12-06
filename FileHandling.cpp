#include "pch.h"
#include <ppltasks.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace Windows::Storage;
using namespace Windows::UI::Popups;
using namespace Platform;
using namespace concurrency;

class LogHelper
{
public:
    StorageFile^ logFile;
    std::shared_ptr<spdlog::logger> logger;

    task<StorageFile^> CreateFile()
    {
        StorageFolder^ localFolder = ApplicationData::Current->LocalFolder;
        return create_task(localFolder->CreateFileAsync("data.log", CreationCollisionOption::ReplaceExisting));

        /*create_task(KnownFolders::GetFolderForUserAsync(nullptr, KnownFolderId::DocumentsLibrary)).then([this](StorageFolder^ docFolder)
        {
            return docFolder->CreateFileAsync("data.log", CreationCollisionOption::ReplaceExisting);
        }).then([this](task<StorageFile^> task)
        {
            try
            {
                StorageFile^ file = task.get();
                MessageDialog Dialog("File created with success!");
                Dialog.ShowAsync();
            }
            catch (Exception^ e)
            {
                MessageDialog Dialog("ERROR: While trying to create file\n " + e->Message);
                Dialog.ShowAsync();
            }
        });*/


        //// Create a sample file; replace if exists.
        //auto localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
        //Windows::Storage::StorageFolder storageFolder;
        //co_await storageFolder.CreateFileAsync(L"sample.txt", Windows::Storage::CreationCollisionOption::ReplaceExisting);
    }

    void SetLogFile(StorageFile^ _logFile)
    {
        logFile = _logFile;
    }

    void InitializeLogger()
    {
        try
        {
            std::wstring ws(logFile->Path->Data());
            std::string path(ws.begin(), ws.end());
            logger = spdlog::basic_logger_mt("main_file_logger", path);
            //logger->info("Created log file.");
            //logger->flush();
            //spdlog::register_logger(logger);
            spdlog::set_default_logger(logger);
            spdlog::flush_every(std::chrono::seconds(3));

            spdlog::info("Log File Generated with success.");
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::string err(ex.what());
            std::wstring w_str = std::wstring(err.begin(), err.end());
            const wchar_t* wchars = w_str.c_str();
            Platform::String^ errStr = ref new Platform::String(wchars);
            MessageDialog Dialog(errStr, "ERROR");
            Dialog.ShowAsync();
        }
    }
};