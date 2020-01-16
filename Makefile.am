AM_CPPFLAGS = -Wall
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = bootstrap

# Define binaries
noinst_LIBRARIES = librhr.a
bin_PROGRAMS = unnamedblocks
lib_LTLIBRARIES = libVinMod.la
# RHR
librhr_a_SOURCES = RHR/include/RHR/stacking/Plane.cpp RHR/include/RHR/stacking/Stack.cpp RHR/include/RHR/stacking/Block.cpp RHR/include/RHR/stacking/args/Argument.cpp RHR/include/RHR/registries/ButtonRegistry.cpp RHR/include/RHR/registries/ShaderRegistry.cpp RHR/include/RHR/registries/BlockRegistry.cpp RHR/include/RHR/ui/Button.cpp RHR/include/RHR/ui/TypingSystem.cpp RHR/include/RHR/modding/ModData.cpp RHR/include/RHR/RHR.cpp RHR/include/RHR/Global.cpp RHR/include/RHR/handlers/ProjectHandler.cpp RHR/include/RHR/handlers/Logger.cpp RHR/include/RHR/handlers/MessageHandler.cpp RHR/include/RHR/handlers/runtime/RuntimeHandler.cpp RHR/include/RHR/handlers/runtime/ObjectHandler.cpp RHR/include/RHR/handlers/runtime/ByteHandler.cpp RHR/include/RHR/handlers/runtime/VariableHandler.cpp RHR/include/RHR/handlers/runtime/ThreadHandler.cpp 
librhr_a_CPPFLAGS = -fPIC -Wall -IRHR/include -IRHR/include/RHR $(SFML_CFLAGS)
librhr_a_LDFLAGS = -fPIC
# VinMod
libVinMod_la_SOURCES = VanillaMods/Mod.cpp 
libVinMod_la_CPPFLAGS = -Wall -IRHR/include -IRHR/include/RHR $(SFML_CFLAGS)
libVinMod_la_LIBADD = librhr.a -lpthread $(SFML_LIBS) -ldl

# UnnamedBlocks
unnamedblocks_SOURCES = UnnamedBlocks/src/ModLoader.cpp UnnamedBlocks/src/main.cpp 
unnamedblocks_CPPFLAGS = -IUnnamedBlocks/src -IRHR/include -IRHR/include/RHR $(SFML_CFLAGS) $(GLEW_CFLAGS) $(BOOST_CPPFLAGS)
unnamedblocks_LDADD = librhr.a $(BOOST_FILESYSTEM_LIB) -lpthread $(SFML_LIBS) $(GLEW_LIBS) $(BOOST_LDFLAGS) -lX11 -ldl
