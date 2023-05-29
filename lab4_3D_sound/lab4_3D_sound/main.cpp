#include <iostream>
#include <conio.h>
#include <TCHAR.h>

#include "inc\fmod.hpp"
#include "inc\fmod_errors.h"

#pragma comment(lib, "fmodex_vc.lib")

int _tmain(int argc, _TCHAR* argv[])
{
    FMOD_RESULT result;                                                         // FMOD 4
    FMOD::System* systemF;
    result = FMOD::System_Create(&systemF);
    result = systemF->init(16, FMOD_INIT_NORMAL, 0);

    FMOD::Sound* sound;                                                         // sound
    FMOD::Channel* channel;                                                     // sound channel

    result = systemF->createSound("sound.mp3", FMOD_3D, 0, &sound);             // creating sound
    result = systemF->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);     // play sound

    std::cout << "Press any button to set3DListenerAttributes.\n\n";
    _getch();

    FMOD_VECTOR listenerpos = { 0, 0.0f, 0.0f };
    FMOD_VECTOR velocity = { 10.0f, 0.0f, 0.0f };
    FMOD_VECTOR forward = { 2.0f, 9.0f, 0.0f };
    FMOD_VECTOR up = { 0.0f, 0.0f, 15.0f };

    result = systemF->set3DListenerAttributes(0, &listenerpos, &velocity, &forward, &up); //3д звук
    result = systemF->update();

    std::cout << "Press any button to createDSPByType ECHO.\n\n";
    _getch();

    FMOD::DSP* dspECHO;
    result = systemF->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspECHO);            //создание эффектов: ЭХО

    channel->addDSP(dspECHO, 0);
    dspECHO->setParameter(0, 50.0f);

    std::cout << "Press any button to change volume.\n\n";
    _getch();

    channel->setVolume(0.2); 
    systemF->update();

    std::cout << "Press any button to end.\n";
    _getch();

    return 0;

}
