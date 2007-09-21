include( $$quote( "$$BASEDIR/marsyasConfig.pri" ) )

CONFIG *= qt   # Qt4Apps need Qt

###############################################
# 			  Destination dirs
###############################################
CONFIG(release, debug|release) {
  message(Building with release support.)
  DESTDIR = $$quote( "$$BASEDIR/bin/release" )
}
CONFIG(debug, debug|release) {
  message(Building with debug support.)
  DESTDIR = $$quote( "$$BASEDIR/bin/debug" )
}

INCLUDEPATH += $$quote( "$$BASEDIR/src/marsyasqt" )

CONFIG(release, debug|release){
	win32-msvc2005:LIBS 	+= 	marsyas.lib marsyasqt.lib
	!win32-msvc2005:LIBS += 	-lmarsyas -lmarsyasqt
	LIBPATH += $$quote( \"$$BASEDIR/lib/release\" )
	PRE_TARGETDEPS += $$quote( \"$$BASEDIR/lib/release/marsyas.lib\" )
	PRE_TARGETDEPS += $$quote( \"$$BASEDIR/lib/release/marsyasqt.lib\" )
}
CONFIG(debug, debug|release){
	CONFIGURE	+= console
	win32-msvc2005:LIBS 	+= 	marsyas.lib marsyasqt.lib
	!win32-msvc2005:LIBS += 	-lmarsyas -lmarsyasqt
	LIBPATH += $$quote( \"$$BASEDIR/lib/debug\" )
	PRE_TARGETDEPS += $$quote( \"$$BASEDIR/lib/debug/marsyas.lib\" )
	PRE_TARGETDEPS += $$quote( \"$$BASEDIR/lib/debug/marsyasqt.lib\" )
}


