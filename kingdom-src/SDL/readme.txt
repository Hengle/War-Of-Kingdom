��sdl��������sdlԴ��������ת��Ϊ����ҪԴ���

1���޸�SDLmain���п⣨Debug��Release��һ�����ã�
C/C++
  Code Generation      Runtime Library 
           Debug    ��Multi-threaded Debug DLL(/MDd)��ΪMulti-threaded Debug(/MTd)
           Release    ��Multi-threaded DLL(/MD)��ΪMulti-threaded(/MT)

2��SDL��������ɺ�������ļ����Ƶ���ӦĿ¼��Debug��Releaseһ�����ã�
Build Events
  Post-Build Event     
     Command Line���룺
copy $(TargetDir)\$(TargetName).lib ..\..\..\SDL-dev-framework\SDL-1.3.x\lib\
copy $(TargetDir)\$(TargetName).dll ..\..\..\SDL-dev-framework\dll\

3��SDLmain��������ɺ�������ļ����Ƶ���ӦĿ¼��Debug��Releaseһ�����ã�
copy $(TargetDir)\$(TargetName).lib ..\..\..\SDL-dev-framework\SDL-1.3.x\lib\

4��Ϊ���ԣ�����ص��԰�dllֱ�Ӹ��Ƶ�kingdom��ִ��Ŀ¼�¡�
Build Events
  Post-Build Event
    Command Line���룺 
copy $(SolutionDir)\..\..\..\SDL\SDL-1.3.0-5538\VisualC\SDL\Debug\SDL.dll $(TargetDir)\SDL.dll
copy $(SolutionDir)\..\..\..\SDL\SDL_ttf-2.0.10\VisualC\Debug\SDL_ttf.dll $(TargetDir)\SDL_ttf.dll
copy $(SolutionDir)\..\..\..\SDL\SDL_mixer-1.2.11\VisualC\Debug\SDL_mixer.dll $(TargetDir)\SDL_mixer.dll

5��SDL_image�������±���