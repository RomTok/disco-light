import os

#######################################################################
# Command line options                                                #
#######################################################################
opts = Options()
opts.Add(BoolOption('debug',     'Build with debug symbols and without optimize', 0))
opts.Add(BoolOption('messages',  'Build with logfile support', 0))

env = Environment(ENV = os.environ, CPPPATH = '../../../inc')
Help(opts.GenerateHelpText(env))
Export('env')

#######################################################################
# Subdirectories                                                      #
#######################################################################
diskoLibs  = ["mmsinfo",
              "mmstools",
              "mmsconfig",
              "mmsgui",
              "mmsbase",
              "mmsinput",
              "mmsmedia",
              "mmscore",
              "mmssip"]
diskoTools = ["taff"]

#######################################################################
# Helper functions                                                    #
#######################################################################
def tryConfigCommand(context, cmd):
	ret = context.TryAction(cmd)[0]
	context.Result(ret)
	if ret:
		context.env.ParseConfig(cmd)
	return ret

def checkPKG(context, name):
	context.Message('Checking for %s (using pkg-config)... ' % name)
	return tryConfigCommand(context, 'pkg-config --libs --cflags \'%s\'' % name)

def checkConf(context, name):
	binary = '%s-config' % name.lower()
	context.Message('Checking for %s (using %s)... ' % (name, binary))
	configcall = '%s --libs --cflags' %binary
	return tryConfigCommand(context, configcall)

def checkSimpleLib(context, liblist, header = '', lang = 'c++', required = 1):
	for lib in liblist:
		ret = checkPKG(context, lib)
		if ret:
			return ret

		ret = checkConf(context, lib)
		if ret:
			return ret

		if len(header):
			ret = conf.CheckLibWithHeader(lib, header, lang)
		else:
			ret = conf.CheckLib(lib)

		if ret:
			if not lib in conf.env['LIBS']:
				conf.env.Append(LIBS = [lib])
			return ret

	if required:
		print 'required lib %s not found :(' % lib
		Exit(1)

	return False

#######################################################################
# Check dependencies                                                  #
#######################################################################
conf = Configure(env, 
				 custom_tests = {'checkConf': checkConf, 'checkPKG': checkPKG, 'checkSimpleLib': checkSimpleLib},
				 conf_dir = 'build/.sconf_temp',
				 log_file = 'build/config.log')
conf.checkSimpleLib(['directfb'],   'directfb/directfb.h')
conf.checkSimpleLib(['sigc++-2.0'], 'sigc++-2.0/sigc++/sigc++.h')
conf.checkSimpleLib(['libxml-2.0'], 'libxml2/libxml/parser.h')
conf.checkSimpleLib(['libpng'],     'libpng/png.h')
conf.checkSimpleLib(['libcurl'],    'curl/curl.h')
conf.checkSimpleLib(['libxine'],    'xine.h')
conf.checkSimpleLib(['alsa'],       'alsa/version.h')
if conf.checkSimpleLib(['sqlite3'],    'sqlite3.h', required = 0):
	env.Append(CCFLAGS = '-D__ENABLE_SQLITE__')
if conf.CheckCXXHeader('/usr/include/sql.h'):
	env.Append(CCFLAGS = '-D__ENABLE_FREETDS__', LIBS = 'odbc')
elif conf.CheckCXXHeader('/usr/local/include/sql.h'):
	env.Append(CCFLAGS = '-D__ENABLE_FREETDS__ -I/usr/local/include', LIBPATH = '/usr/local/lib', LIBS = 'odbc')
if not conf.checkSimpleLib(['openssl'],    'openssl/conf.h', required = 0):
	env.Append(CCFLAGS = '-D_NO_MMSCRYPT')
if not conf.checkSimpleLib(['libpj'],      'pjlib.h', required = 0):
	env.Append(CCFLAGS = '-D_NO_MMSSIP')
env = conf.Finish()

if not 'sqlite3' in env['LIBS'] and not 'odbc' in env['LIBS']:
	print 'No database backend found. Please install either sqlite3 or unix-odbc.'
	Exit(1)

#######################################################################
# Building disko                                                      #
#######################################################################
print "==================={ Building Disko }===================="

libList = ""
for libDir in diskoLibs:
        libList += 'build/libs/' + libDir + "/SConscript "

toolList = ""
for toolDir in diskoTools:
        toolList += 'build/tools/' + toolDir + "/SConscript "

BuildDir('build/libs', 'src', duplicate = 0)
SConscript(Split(libList), options = opts)

BuildDir('build/tools', 'tools', duplicate = 0)
SConscript(Split(toolList), options = opts)
