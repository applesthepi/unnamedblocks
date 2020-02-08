AM_CPPFLAGS = -Wall
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = bootstrap

# Define binaries
noinst_LIBRARIES = librhr.a
bin_PROGRAMS = unnamedblocks
lib_LTLIBRARIES = ModVin.la
# RHR
librhr_a_SOURCES = RHR/include/RHR/Global.cpp RHR/include/RHR/ui/TypingSystem.cpp RHR/include/RHR/ui/Button.cpp RHR/include/RHR/handlers/MessageHandler.cpp RHR/include/RHR/handlers/Logger.cpp RHR/include/RHR/handlers/ProjectHandler.cpp RHR/include/RHR/handlers/runtime/VariableHandler.cpp RHR/include/RHR/handlers/runtime/ObjectHandler.cpp RHR/include/RHR/handlers/runtime/ByteHandler.cpp RHR/include/RHR/handlers/runtime/ThreadHandler.cpp RHR/include/RHR/handlers/runtime/RuntimeHandler.cpp RHR/include/RHR/StatmentIf.cpp RHR/include/RHR/RHR.cpp RHR/include/RHR/modding/ModData.cpp RHR/include/RHR/registries/ShaderRegistry.cpp RHR/include/RHR/registries/ButtonRegistry.cpp RHR/include/RHR/registries/BlockRegistry.cpp RHR/include/RHR/stacking/args/Argument.cpp RHR/include/RHR/stacking/Block.cpp RHR/include/RHR/stacking/Plane.cpp RHR/include/RHR/stacking/Stack.cpp 
librhr_a_CPPFLAGS = -fPIC -std=c++17 -Wall -IRHR/include -IRHR/include/RHR $(SFML_CFLAGS)
librhr_a_LDFLAGS = -fPIC -std=c++17
# VinMod
ModVin_la_SOURCES = VanillaMods/ub-ModVin/Mod.cpp 
ModVin_la_CPPFLAGS = -Wall -std=c++17 -IRHR/include -IRHR/include/RHR $(SFML_CFLAGS)
ModVin_la_LIBADD = librhr.a -lpthread $(SFML_LIBS) -ldl
ModVin_la_LDFLAGS = -module -std=c++17

# UnnamedBlocks
unnamedblocks_SOURCES = UnnamedBlocks/src/ModLoader.cpp UnnamedBlocks/src/main.cpp 
unnamedblocks_CPPFLAGS = -std=c++17 -IUnnamedBlocks/src -IRHR/include -IRHR/include/RHR $(SFML_CFLAGS) $(GLEW_CFLAGS) $(BOOST_CPPFLAGS) $(X11_CFLAGS)
unnamedblocks_LDADD = librhr.a $(BOOST_FILESYSTEM_LIB) -lpthread $(SFML_LIBS) $(GLEW_LIBS) $(BOOST_LDFLAGS) $(X11_LIBS) -ldl
unnamedblocks_LDFLAGS = -std=c++17
