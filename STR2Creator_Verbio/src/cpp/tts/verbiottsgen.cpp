#include "src\include\verbiottsgen.h"

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "../../libverbio/VerbioTTS.h"

#define NUM_THREADS	12
#define TN_DISC		116

char *text_sp = "La cotización del dólar, el lunes 17 de septiembre de 2001, quedó fijada en 0.92 euros.";
char *text_ca = "La cotització del dòlar, el dilluns 17 de setembre de 2001, va quedar fixada en 3.92 euros.";
char *text_en = "This is a sample of the quality of the SAPI english text-to-speech engine for telephone applications.";


VerbioTTSGen::VerbioTTSGen(){}

void VerbioTTSGen::myTTS_DisconnectFunction(const char *pFailedIP)
{
    if (!pFailedIP)
        return;

    printf("Disconnection event received form %s. Trying to find another server...\n", pFailedIP);

    EngineTTS->Close();

    while (EngineTTS->Open() < 0);

    EngineTTS->Reconnect(pFailedIP);

    printf("Connection restablished in server %s\n", EngineTTS->GetServerIP());
}

typedef struct
{
    VerbioTTSResource*	TTSResource;

} RESOURCE_HANDLES;

RESOURCE_HANDLES	hResource[NUM_THREADS];

int VerbioTTSGen::verbioTtsAudioGeneration()
{
    HANDLE                      hThread[NUM_THREADS];
    unsigned long		dwThreadId, dwProdVer;
    int				i, numThreads = 0;

    VerbioTTSResource*		EngineTTSResource[NUM_THREADS];
    const VerbioTTSLicense*	LicenseTTS;
    VerbioSpeakerInfo*		SpeakerInfo;
    DWORD WINAPI ThreadProc(LPVOID);


    EngineTTS = getVerbioTTS();

    EngineTTS->SetDefaultServerIP("127.0.0.1");
    EngineTTS->AddBackUpServerIP("192.168.0.6");
    EngineTTS->SetNetTimeout(10);

    for (i=0; i<EngineTTS->GetNumberOfAvailableLngs(); i++)
        printf("Available Language: %s\n", EngineTTS->GetLanguage(i));
    EngineTTS->SetDefaultLanguage(VERBIO_LANG_SPANISH);
    EngineTTS->AddLanguage(VERBIO_LANG_CATALAN);

    //EngineTTS->SetCallBackServerDisconnect(this->myTTS_DisconnectFunction);

    if (EngineTTS->Open() < 0 ) {
        printf("Unable to find a TTS Engine\n");
        delete EngineTTS;
        return -1;
    }
    printf("Started TTSServer %s in %s language as default\n\n", EngineTTS->GetServerIP() ? EngineTTS->GetServerIP() : "localhost", EngineTTS->GetDefaultLanguage());

    EngineTTS->GetVersion(&dwProdVer);
    printf("VERBIO SAMPLE - TTS. VERBIO %d.%02d\n", HIWORD(dwProdVer), LOWORD(dwProdVer));
    printf("------------------\n");

    LicenseTTS = EngineTTS->GetLicenseInfo();
    printf("Spanish resources: %d\n", LicenseTTS->GetAvailableResources(VERBIO_LANG_SPANISH));
    printf("Catalan resources: %d\n", LicenseTTS->GetAvailableResources(VERBIO_LANG_CATALAN));
    if (LicenseTTS->IsEvaluation()) printf("Evaluation\n");
    if (LicenseTTS->IsLite()) printf("Lite\n");
    printf("\n");

    printf("Available speakers: %d\n\n", EngineTTS->GetNumberOfAvailableSpks());
    for (i=0; i<EngineTTS->GetNumberOfAvailableSpks(); i++) {
        if (!(SpeakerInfo = EngineTTS->GetSpeakerInfo(i)))
            continue;

        printf("Identifier: %s\n", SpeakerInfo->GetIdent());
        printf("Name: %s\n", SpeakerInfo->GetName());
        printf("Gender: %s\n", SpeakerInfo->GetGender());
        printf("Age: %s\n", SpeakerInfo->GetAge());
        printf("Language: %s\n\n", SpeakerInfo->GetLanguage());
    }

    for (i=0; i<NUM_THREADS; i++)
    {
        hThread[i] = 0;

        if (!(EngineTTSResource[i] = EngineTTS->GetVerbioTTSResource())) continue;
        hResource[i].TTSResource = EngineTTSResource[i];

        hThread[i] = CreateThread(NULL, 8096, ThreadProc, (LPVOID) i, CREATE_SUSPENDED, &dwThreadId);
        SetThreadPriority(hThread[i], THREAD_PRIORITY_NORMAL);

        numThreads++;
    }

    for (i=0; i<NUM_THREADS; i++) {
        if (hThread[i])
            ResumeThread(hThread[i]);
        Sleep(500);
    }

    WaitForMultipleObjects(numThreads, hThread, TRUE, INFINITE);

    for (i=0; i<NUM_THREADS; i++) {
        if (hThread[i])
            CloseHandle(hThread[i]);
    }

    EngineTTS->Close();
    delete EngineTTS;

    printf("Press any key to close\n");
    _getch();

    return 0;
}

DWORD WINAPI ThreadProc(LPVOID _ch)
{
    int					i, chdev, ch = (int) _ch;
    RESOURCE_HANDLES*	hResources = &(hResource[ch]);
    FILE*				file;
    char				str[256], filename[64], channel[32];
    HSAPI				hSapi;

    sprintf(channel, "dxxxB%1dC%1d", (ch/4)+1, (ch%4)+1);

    VerbioTTSResource*	EngineTTSResource = (VerbioTTSResource*) hResources->TTSResource;
    if (!EngineTTSResource)
        return -1;

    EngineTTSResource->AssignAudioChannel(chdev);


    while (1) {
        printf("Channel %d -> Waiting call...\n", ch);

        printf("Channel %d -> Call answered\n", ch);

        if (EngineTTSResource->WaitForLicense(1000) < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->SetLanguage(VERBIO_LANG_SPANISH) < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }
        else
            printf("Current TTS language: %s\n", EngineTTSResource->GetLanguage());

        if (EngineTTSResource->SetSpeakerName("laura") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        printf("Current speaker information\n");
        printf("Name: %s\n", (EngineTTSResource->GetSpeakerInfo())->GetName());
        printf("Gender: %s\n", (EngineTTSResource->GetSpeakerInfo())->GetGender());
        printf("Age: %s\n", (EngineTTSResource->GetSpeakerInfo())->GetAge());

        if(EngineTTSResource->PlayStr(text_sp, VERBIO_AUDIO_ALAW, "laura.esa") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }
        //        if (ATDX_TERMMSK(chdev) & (TM_LCOFF | TM_TONE))

        if (EngineTTSResource->SetSpeakerGender("male") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        printf("Current speaker information\n");
        printf("Name: %s\n", (EngineTTSResource->GetSpeakerInfo())->GetName());
        printf("Gender: %s\n", (EngineTTSResource->GetSpeakerInfo())->GetGender());
        printf("Age: %s\n", (EngineTTSResource->GetSpeakerInfo())->GetAge());

        if(EngineTTSResource->PlayStr(text_sp, VERBIO_AUDIO_MULAW, "carlos.esu") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->SetSpeakerName("laura") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->SetAbbreviations(VERBIO_LANG_SPANISH, "abr_es.txt") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->SetDictionary(VERBIO_LANG_SPANISH, "dic_es.txt") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->PlayStr(text_sp, VERBIO_AUDIO_MULAW, "laura.esu") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        EngineTTSResource->SetRate(EngineTTSResource->GetRate() / 2);
        EngineTTSResource->SetPitch(EngineTTSResource->GetPitch() / 2);
        EngineTTSResource->SetVolume(EngineTTSResource->GetVolume() / 2);

        sprintf(filename, "Record%d.esu", ch);
        if (!(file = fopen(filename, "wb")))
        { printf("Error opening file for writing\n");  }

        if (EngineTTSResource->PlayStrOpen(text_sp, VERBIO_AUDIO_MULAW) < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        do {
            i = EngineTTSResource->PlayStrRead(str, 255);
            i = fwrite(str, 1, i, file);
        } while (i == 255);

        fclose(file);

        if (EngineTTSResource->PlayStrClose() < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->SetLanguage(VERBIO_LANG_CATALAN) < 0 ||	EngineTTSResource->SetSpeakerName("marta") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->PlayStr(text_ca) < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->SetLanguage(VERBIO_LANG_CATALAN) < 0 ||	EngineTTSResource->SetSpeakerName("pau") < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if(EngineTTSResource->PlayStr(text_ca) < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (!(hSapi = EngineTTSResource->SAPIOpen(LANG_ENGLISH, SUBLANG_NEUTRAL)))
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->SAPISelect(hSapi) < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->PlayStr(text_en) < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        if (EngineTTSResource->SAPIClose(hSapi) < 0)
        { printf("%s\n", EngineTTSResource->GetError());  }

        EngineTTSResource->ReleaseLicense();

        return 0;
    }
}
